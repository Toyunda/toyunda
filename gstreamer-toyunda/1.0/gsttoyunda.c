/* GStreamer
 * Copyright (C) 2015 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gsttoyunda
 *
 * The toyunda element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v fakesrc ! toyunda ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>
#include "gsttoyunda.h"
#include <glib/gstdio.h>
#include <string.h>
#include "gsttoyunda.h"
#include <gst/video/video.h>

#if G_BYTE_ORDER == G_LITTLE_ENDIAN
# define CAIRO_ARGB_A 3
# define CAIRO_ARGB_R 2
# define CAIRO_ARGB_G 1
# define CAIRO_ARGB_B 0
#else
# define CAIRO_ARGB_A 0
# define CAIRO_ARGB_R 1
# define CAIRO_ARGB_G 2
# define CAIRO_ARGB_B 3
#endif

GST_DEBUG_CATEGORY_STATIC (gst_toyunda_debug_category);
#define GST_CAT_DEFAULT gst_toyunda_debug_category

/* prototypes */


static void gst_toyunda_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_toyunda_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_toyunda_dispose (GObject * object);
static void gst_toyunda_finalize (GObject * object);

static gboolean gst_toyunda_start (GstBaseTransform * trans);
static gboolean gst_toyunda_stop (GstBaseTransform * trans);
static gboolean gst_toyunda_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info);
static GstFlowReturn gst_toyunda_transform_frame (GstVideoFilter * filter,
    GstVideoFrame * inframe, GstVideoFrame * outframe);
static GstFlowReturn gst_toyunda_transform_frame_ip (GstVideoFilter * filter,
    GstVideoFrame * frame);

/* Render function */
static	void	gst_toyunda_draw_grid(GstToyunda *toyunda, GstVideoFrame *video_frame);
static	void	gst_toyunda_blend_subtitles(GstToyunda *toyunda, GstVideoFrame *video_frame);
static inline void gst_toyunda_unpremultiply (GstBuffer* buffer, guint width, guint height);

static void gst_toyunda_create_subtitle_buffers(GstToyunda* toyunda);
static void gst_toyunda_blend_subtitles(GstToyunda* toyunda, GstVideoFrame* video_frame);

#include "../gsttoyundaparse.c"
#include "gsttoyundamisc.c"

enum
{
  PROP_0,
  PROP_SUBFILE,
  PROP_TOYUNDA_LOGO,
  PROP_IMAGE_DIR,
  PROP_SUB_ENABLED
};

/* pad templates */

/* FIXME: add/remove formats you can handle */
#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE(GST_VIDEO_OVERLAY_COMPOSITION_BLEND_FORMATS)

/* FIXME: add/remove formats you can handle */
#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE(GST_VIDEO_OVERLAY_COMPOSITION_BLEND_FORMATS)


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstToyunda, gst_toyunda, GST_TYPE_VIDEO_FILTER,
  GST_DEBUG_CATEGORY_INIT (gst_toyunda_debug_category, "toyunda", 0,
  "debug category for toyunda element"));

static void
gst_toyunda_class_init (GstToyundaClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  GstVideoFilterClass *video_filter_class = GST_VIDEO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SRC_CAPS)));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SINK_CAPS)));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "toyunda", "Toyunda", "Toyunda Karaoke plugin", "Sylvain \"Skarsnik\" Colinet <scolinet@gmail.com>");

  gobject_class->set_property = gst_toyunda_set_property;
  gobject_class->get_property = gst_toyunda_get_property;
  gobject_class->dispose = gst_toyunda_dispose;
  gobject_class->finalize = gst_toyunda_finalize;
  base_transform_class->start = GST_DEBUG_FUNCPTR (gst_toyunda_start);
  base_transform_class->stop = GST_DEBUG_FUNCPTR (gst_toyunda_stop);
  video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_toyunda_set_info);
  /*video_filter_class->transform_frame = GST_DEBUG_FUNCPTR (gst_toyunda_transform_frame);*/
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_toyunda_transform_frame_ip);
  
  g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_SUBFILE,
		         g_param_spec_string ("subfile", "Subfile",
			  "The subtitle file to parse",
			  "none", G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_TOYUNDA_LOGO,
		         g_param_spec_string ("toyunda-logo", "Toyunda logo",
			  "The default toyunda logo image",
			  "toyunda.tga", G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_IMAGE_DIR,
		         g_param_spec_string ("image-base-dir", "The base directory for image",
			  "The base directory for image",
			  NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_SUB_ENABLED,
		         g_param_spec_boolean ("sub-enabled", "Enable or disable subtitle rendering",
			  "Enable or disable subtitle rendering",
			  TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

}

static void
gst_toyunda_init (GstToyunda *toyunda)
{
  GST_DEBUG_OBJECT (toyunda, "init");
  gst_toyunda_field_init(toyunda);
	toyunda->pango_context = pango_font_map_create_context(pango_cairo_font_map_get_default());
	toyunda->pango_layout = pango_layout_new(toyunda->pango_context);
	toyunda->pango_fontdesc = pango_font_description_from_string(toyunda->font_desc);
	pango_font_description_set_size(toyunda->pango_fontdesc, INT_TOYUNDA_FONT_SIZE * PANGO_SCALE);
	pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
}

void
gst_toyunda_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstToyunda *toyunda = GST_TOYUNDA (object);

  GST_DEBUG_OBJECT (toyunda, "set_property");

  switch (property_id) {
    case PROP_SUBFILE:
			if (toyunda->subfile != NULL)
				g_free(toyunda->subfile);
			toyunda->subfile = g_value_dup_string(value);
			toyunda->subfile_parsed = FALSE;
			break;
		case PROP_TOYUNDA_LOGO:
			if (toyunda->toyunda_logo != NULL)
				g_free(toyunda->toyunda_logo);
			toyunda->toyunda_logo = g_value_dup_string(value);
			g_printf("Set Toyunda Logo : %s\n", toyunda->toyunda_logo);
			break;
		case PROP_IMAGE_DIR:
			if (toyunda->images_base_path != NULL)
				g_free(toyunda->images_base_path);
			toyunda->images_base_path = g_value_dup_string(value);
			break;
		case PROP_SUB_ENABLED:
			toyunda->sub_enabled = g_value_get_boolean(value);
			break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_toyunda_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstToyunda *toyunda = GST_TOYUNDA (object);

  GST_DEBUG_OBJECT (toyunda, "get_property");

  switch (property_id) {
    case PROP_SUBFILE:
			g_value_set_string(value, toyunda->subfile);
			break;
		case PROP_TOYUNDA_LOGO:
			g_value_set_string(value, toyunda->toyunda_logo);
			break;
		case PROP_IMAGE_DIR:
			g_value_set_string(value, toyunda->images_base_path);
			break;
		case PROP_SUB_ENABLED:
			g_value_set_boolean(value, toyunda->sub_enabled);
			break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_toyunda_dispose (GObject * object)
{
  GstToyunda *toyunda = GST_TOYUNDA (object);

  GST_DEBUG_OBJECT (toyunda, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_toyunda_parent_class)->dispose (object);
}

void
gst_toyunda_finalize (GObject * object)
{
  GstToyunda *toyunda = GST_TOYUNDA (object);

  GST_DEBUG_OBJECT (toyunda, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_toyunda_parent_class)->finalize (object);
}

static gboolean
gst_toyunda_start (GstBaseTransform * trans)
{
  GstToyunda *toyunda = GST_TOYUNDA (trans);

  GST_DEBUG_OBJECT (toyunda, "start");
  
  if (toyunda->subfile_parsed == FALSE && toyunda->subfile != NULL) {
		gst_toyunda_parse_toyunda_subtitle(toyunda);
		GST_DEBUG_OBJECT (toyunda, "subtitle parsed succeffully, file : %s", toyunda->subfile);
  }

  return TRUE;
}

static gboolean
gst_toyunda_stop (GstBaseTransform * trans)
{
  GstToyunda *toyunda = GST_TOYUNDA (trans);

  GST_DEBUG_OBJECT (toyunda, "stop");
  
  // FIXME Free stuff probably?

  return TRUE;
}

static gboolean
gst_toyunda_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  GstToyunda *toyunda = GST_TOYUNDA (filter);

  GST_DEBUG_OBJECT (toyunda, "set_info");

  
  toyunda->video_width = in_info->width;
  toyunda->video_height = in_info->height;
  
  toyunda->fps_n = in_info->fps_n;
  toyunda->fps_d = in_info->fps_d;
  
  g_printf("video format : %s - %s\n", in_info->finfo->name, in_info->finfo->description);
  g_printf("fps d,n : %f , %f\n", toyunda->fps_d, toyunda->fps_n);
  g_printf("height : %d, width : %d\n", toyunda->video_height, toyunda->video_width);

  gst_toyunda_adjust_default_font_size(toyunda);
  toyunda->subtitle_changed = TRUE;
  gst_toyunda_reset_subtitle_buffer_statut(toyunda);

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_toyunda_transform_frame (GstVideoFilter * filter, GstVideoFrame * inframe,
    GstVideoFrame * outframe)
{
  GstToyunda *toyunda = GST_TOYUNDA (filter);

  GST_DEBUG_OBJECT (toyunda, "transform_frame");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_toyunda_transform_frame_ip (GstVideoFilter * filter, GstVideoFrame * frame)
{
  GstToyunda *toyunda = GST_TOYUNDA (filter);
  GstFlowReturn ret;
  gboolean in_seg = FALSE;
  float framerate;
  int framenb;
  gint64 start, stop, clip_start = 0, clip_stop = 0;

  //GST_DEBUG_OBJECT (toyunda, "transform_frame_ip");
  

	GST_OBJECT_LOCK(toyunda);
	start = GST_BUFFER_TIMESTAMP (frame->buffer);
	framerate = toyunda->fps_n / toyunda->fps_d;
	framenb = ((start / 1000000) * framerate + 100 )/ 1000;
	//g_printf("Timestamp : %u ---- frame :", start);
	//g_printf("%d\n", framenb);
	if (toyunda->subfile_parsed != NULL && toyunda->sub_enabled == TRUE)
	{
		//GST_DEBUG_OBJECT (toyunda, "Modif the frame");
		gst_toyunda_select_subtitle(toyunda, framenb);
		//gst_toyunda_draw_grid(toyunda, buffer);
		if (toyunda->subtitle_changed == TRUE)
			gst_toyunda_create_subtitle_buffers(toyunda);
		//g_printf("Before blending\n");
		toyunda->subtitle_changed = FALSE;
		gst_toyunda_blend_subtitles(toyunda, frame);
	}
	GST_OBJECT_UNLOCK(toyunda);
  return GST_FLOW_OK;
}

// ========================== Toyunda stuff ===============

void gst_toyunda_draw_grid(GstToyunda* toyunda, GstVideoFrame* video_frame)
{
	static GstVideoOverlayRectangle*	comprect = NULL;
	static GstVideoOverlayComposition*	comp = NULL;
	static GstBuffer*			buff = NULL;
	guint8*			buff_data;
	uint				cpt, cpt2;
	uint				line_offset, v_offset;

	line_offset = toyunda->video_width * 4;
	v_offset = toyunda->video_height / 12;
	if (buff == NULL)
	{
		buff = gst_buffer_new_and_alloc(4 * toyunda->video_height * toyunda->video_width);

		video_frame = gst_buffer_make_writable(video_frame);

		//buff_data = GST_BUFFER_DATA(buff);
		/* draw line */
		//for (cpt = 0; cpt < GST_BUFFER_SIZE(buff); cpt++)
			buff_data[cpt] = 0x00;
		for (cpt = 0; cpt < 12; cpt++)
		{
			for (cpt2 = 0; cpt2 < toyunda->video_width; cpt2++)
			{
				buff_data[line_offset * cpt * v_offset + cpt2 * 4] = 0xFF;
				buff_data[line_offset * cpt * v_offset + cpt2 * 4 + 1] = 0x00;
				buff_data[line_offset * cpt * v_offset + cpt2 * 4 + 2] = 0x00;
				buff_data[line_offset * cpt * v_offset + cpt2 * 4 + 3] = 0xFF;
			}
		}
		/*Draw a vertical centered line */
		for (cpt = 0; cpt < toyunda->video_height; cpt++)
		{
			buff_data[cpt * line_offset + line_offset / 2] = 0xFF;
			buff_data[cpt * line_offset + line_offset / 2 + 1] = 0x00;
			buff_data[cpt * line_offset + line_offset / 2 + 2] = 0x00;
			buff_data[cpt * line_offset + line_offset / 2 + 3] = 0xFF;
		}

		/*comprect = gst_video_overlay_rectangle_new_raw(buff, toyunda->video_width, toyunda->video_height,
			toyunda->video_width * 4, 0, 0, toyunda->video_width, toyunda->video_height,
			GST_VIDEO_OVERLAY_FORMAT_FLAG_NONE);*/
		comp = gst_video_overlay_composition_new(comprect);
	}
	gst_video_overlay_composition_blend(comp, video_frame);
}

static	void	plop_show_data(unsigned char *data, int width, int height)
{
	uint cpt = 0;

	g_printf("====================Show Buffer=================\n");
	for (cpt = 0; cpt < width * height * 4; cpt++)
	{
		g_printf("%02x", data[cpt]);
		if (cpt % (width * 4) == 0)
			g_printf("\n");
	}
	g_printf("=================================================\n");
}

static void gst_toyunda_create_subtitle_buffers(GstToyunda* toyunda)
{
	GSequenceIter		*it, *it_end;
	toyunda_sub_t*		sub;
	toyunda_sub_and_buff_t*	sub_buff;
	cairo_t*		cr;
	cairo_surface_t*	surface;
	PangoRectangle		ink_rect, logical_rect;
	GstBuffer		*buffer;
	rgba_color_t		color;
	gint			pos_x, pos_y;
	PangoAttrList		*pattrl;
	guint			old_size, new_size, actual_size;
	guint			cpt_tmp;
	guint			outline_size;
	char			*tmptext;
	gsize			a, b;
	GError			*g_err = NULL;
	gchar			*converted_text;

	it = g_sequence_get_begin_iter(toyunda->current_subtitles);
	it_end = g_sequence_get_end_iter(toyunda->current_subtitles);

	while (it != it_end)
	{
		new_size = 0;
		old_size = 0;
		sub_buff = (toyunda_sub_and_buff_t*) g_sequence_get(it);
		pos_x = 0;
		pos_y = 0;
		if (sub_buff->to_change == TRUE)
		{
			sub = sub_buff->subtitle;
			//print_toyunda_sub_t(*sub);
			/* we don't render subtitle that out the video */
			if ((sub->positionx > 1 && sub->position2x == -1) || (sub->position2x != -1 && sub->fadingpositionx > 1)
				|| (sub->positiony > 1 && sub->position2y == -1) || (sub->position2y != -1 && sub->fadingpositiony > 1)
			)
			{
				if (sub_buff->overlay_rect != NULL)
					gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
				sub_buff->overlay_rect = NULL;
				it = g_sequence_iter_next(it);
				continue;
			}
			if (strcmp(sub->image, STR_TOYUNDA_LOGO_NONE) == 0)
				tmptext = sub->text;
			else
			{
				/* pango doesn't like the ÿ (-1) char) replace with something else */
				tmptext = g_new(char, strlen(sub->text) + 1);
				strcpy(tmptext, sub->text);
				cpt_tmp = 0;
				while (tmptext[cpt_tmp] != -1)
					cpt_tmp++;
				tmptext[cpt_tmp] = 'o';
			}
			/* Convert to utf-8 */


			converted_text = g_convert(tmptext, -1, "UTF-8", "iso-8859-1", &a, &b, &g_err);
			if (converted_text == NULL)
			{
				g_printf("Error converting the text to utf-8 : %s\n", g_err->message);
				g_free(g_err);
				converted_text = tmptext;
			}
			old_size = pango_font_description_get_size(toyunda->pango_fontdesc) / PANGO_SCALE;
			if (sub->size != -1)
			{
				if (sub->size2 != -1)
					pango_font_description_set_size(toyunda->pango_fontdesc, sub->fadingsize * PANGO_SCALE);
				else
					pango_font_description_set_size(toyunda->pango_fontdesc, sub->size * PANGO_SCALE);
				pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
				new_size = 42;
			}
			/* Need this in both case of image or text */
			pango_layout_set_width(toyunda->pango_layout, -1);
			pango_layout_set_text(toyunda->pango_layout, converted_text, -1);
			pango_layout_get_pixel_extents (toyunda->pango_layout, &ink_rect, &logical_rect);

			/* if The text is too long and no position given */
			if (sub->positionx == -1)
			{
				while (logical_rect.width > (int) toyunda->video_width)
				{
					actual_size = pango_font_description_get_size(toyunda->pango_fontdesc) / PANGO_SCALE;
					new_size = actual_size * ((double)toyunda->video_width / logical_rect.width);
					pango_font_description_set_size(toyunda->pango_fontdesc, new_size * PANGO_SCALE);
					g_printf("text too long, resize : widht : %d lrectwidht : %d old_size : %d, new_size : %d real new_size : %d\n",
						toyunda->video_width, logical_rect.width, actual_size, new_size,
						pango_font_description_get_size(toyunda->pango_fontdesc) / PANGO_SCALE);
					pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
					pango_layout_set_width(toyunda->pango_layout, -1);
					pango_layout_set_text(toyunda->pango_layout, converted_text, -1);
					pango_layout_get_pixel_extents (toyunda->pango_layout, &ink_rect, &logical_rect);
				}
			}
			if (sub->position2y != -1)
				pos_y = sub->fadingpositiony * toyunda->video_height;
			else
				pos_y = sub->positiony * toyunda->video_height;
			/* Simple Text */
			if (strcmp(sub->image, STR_TOYUNDA_LOGO_NONE) == 0)
			{
				if (is_valid_rgba_color_t(sub->color1) == TRUE)
					if (is_valid_rgba_color_t(sub->color2) == TRUE)
						color = sub->tmpcolor;
					else
						color = sub->color1;
				else
				{
					color.alpha = 0xFF;
					color.blue = 0xFF;
					color.green = 0xFF;
					color.red = 0xFF;
				}
				buffer = gst_buffer_new();
				GstMemory* mem = gst_allocator_alloc(NULL, logical_rect.height * logical_rect.width * 4, NULL);
				GstMapInfo memmap;
				gst_memory_map(mem, &memmap, GST_MAP_WRITE);
				surface = cairo_image_surface_create_for_data(memmap.data, CAIRO_FORMAT_ARGB32,
					logical_rect.width, logical_rect.height, logical_rect.width * 4);
				cr = cairo_create(surface);
				cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
				cairo_paint(cr);
				cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
				/* Outline */
				outline_size = (double)(pango_font_description_get_size(toyunda->pango_fontdesc) / PANGO_SCALE) / 15;
				if (outline_size < 1)
					outline_size = 1;
				cairo_save (cr);
				cairo_set_source_rgba (cr, 0, 0, 0, color.alpha / 255.0);
				cairo_set_line_width (cr, outline_size);
				pango_cairo_layout_path (cr, toyunda->pango_layout);
				cairo_stroke (cr);
				cairo_restore (cr);
				/* Text */
				cairo_save (cr);
				cairo_set_source_rgba (cr, color.red / 255.0, color.green / 255.0, color.blue / 255.0, color.alpha / 255.0);
				pango_cairo_show_layout (cr, toyunda->pango_layout);
				cairo_restore (cr);
				cairo_destroy (cr);
				cairo_surface_destroy (surface);
				gst_memory_unmap(mem, &memmap);
				gst_buffer_insert_memory(buffer, -1, mem);
				//1.0 patch gst_toyunda_unpremultiply(buffer, logical_rect.width, logical_rect.height);
				
				//plop_show_data(GST_BUFFER_DATA(buffer), logical_rect.width, logical_rect.height);
				if (sub->positionx == -1)
					pos_x = (toyunda->video_width - logical_rect.width) / 2;
				else
					if (sub->position2x != -1)
						pos_x = sub->fadingpositionx * toyunda->video_width;
					else
						pos_x = sub->positionx * toyunda->video_width;
				if (sub_buff->overlay_rect != NULL)
					gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
				/*Can't create overlayrect with negative position FIXME 1.0 check how gst_overlay react*/
				if (pos_x >= 0 && pos_y >= 0)
				{
					//g_printf("text : %s -- Pos x : %d -- log.rec : %d -- ink.rec: %d\n", tmptext, pos_x, logical_rect.width, ink_rect.width);
					gst_buffer_add_video_meta (buffer, GST_VIDEO_FRAME_FLAG_NONE,
					    GST_VIDEO_OVERLAY_COMPOSITION_FORMAT_RGB,
					    logical_rect.width, logical_rect.height);
				
					sub_buff->overlay_rect = gst_video_overlay_rectangle_new_raw(buffer, pos_x, pos_y, logical_rect.width, logical_rect.height,
					GST_VIDEO_OVERLAY_FORMAT_FLAG_PREMULTIPLIED_ALPHA);
				}
				else
					sub_buff->overlay_rect = NULL;
				sub_buff->to_change = FALSE;
				gst_buffer_unref(buffer);
			}

			/* Toyunda logo, let the wtf begins */
			else
			{
				uint aprox_lettersize = logical_rect.width / strlen(sub->text);
				uint img_pos = 0;
				uint width, height;
				PangoRectangle new_rect;
				float transform_ratio = (float) toyunda->video_height / INT_TOYUNDA_BASE_HEIGHT * 0.8;
				//float transform_ratio = 2;
				pango_layout_set_text(toyunda->pango_layout, "o", -1);
				pango_layout_get_pixel_extents (toyunda->pango_layout, &ink_rect, &new_rect);
				g_free(tmptext);
				/* count the number of space before the ÿ */
				while (sub->text[img_pos] != -1)
					img_pos++;
				if (sub->positionx != -1 && sub->position2x == -1)
					pos_x = sub->positionx * toyunda->video_width + img_pos * new_rect.width;
				if (sub->position2x != -1)
					pos_x = sub->fadingpositionx * toyunda->video_width + img_pos * new_rect.width;
				if (sub->positionx == -1)
				{
					pos_x = (toyunda->video_width - logical_rect.width) / 2 + img_pos * new_rect.width;
					if ((int)pos_x > new_rect.width / 2)
						pos_x -= new_rect.width / 2;
				}
				color.alpha = 255;
				if (is_valid_rgba_color_t(sub->color1) == TRUE)
					if (is_valid_rgba_color_t(sub->color2) == TRUE)
						color = sub->tmpcolor;
					else
						color = sub->color1;
				//g_printf("POSITION X SYL : %d\n", pos_x);
				buffer = gst_toyunda_get_image_data(toyunda, gst_toyunda_get_image_path(toyunda, sub->image), &width, &height);
				if (buffer != NULL)
				{
					if (sub_buff->overlay_rect != NULL)
						gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
					if (pos_x >= 0 && pos_y >= 0)
					{
						sub_buff->overlay_rect = gst_video_overlay_rectangle_new_raw(buffer, pos_x, pos_y, width * transform_ratio, height * transform_ratio,
						GST_VIDEO_OVERLAY_FORMAT_FLAG_NONE);
						gst_video_overlay_rectangle_set_global_alpha(sub_buff->overlay_rect, color.alpha / 255.0);
						//g_printf("Alpha logo : %d -- %f\n", color.alpha, color.alpha / 255.0);
					}
					else
						sub_buff->overlay_rect = NULL;
					sub_buff->to_change = FALSE;
				}
			}
			if (converted_text != sub->text)
				g_free(converted_text);
			/* restaure font */
			if (new_size != 0)
			{
				pango_font_description_set_size(toyunda->pango_fontdesc, old_size * PANGO_SCALE);
				pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
			}
		}
		it = g_sequence_iter_next(it);
	}
}

static void gst_toyunda_blend_subtitles(GstToyunda* toyunda, GstVideoFrame* video_frame)
{
	GSequenceIter			*it, *it_end;
	GstVideoOverlayComposition	*comp;
	GstVideoOverlayRectangle	*rect;
	toyunda_sub_and_buff_t		*sub_buff;

	it = g_sequence_get_begin_iter(toyunda->current_subtitles);
	it_end = g_sequence_get_end_iter(toyunda->current_subtitles);

	comp = NULL;
	video_frame->buffer = gst_buffer_make_writable(video_frame->buffer);

	//g_printf("some NB buff : %d\n", g_sequence_get_length(toyunda->current_subtitles));
	while (it != it_end)
	{
		sub_buff = (toyunda_sub_and_buff_t*) g_sequence_get(it);
		rect = sub_buff->overlay_rect;
		if (rect != NULL)
		{
			if (comp == NULL)
				comp = gst_video_overlay_composition_new(rect);
			else
				gst_video_overlay_composition_add_rectangle(comp, rect);
		}
		it = g_sequence_iter_next(it);
	}
	if (comp != NULL)
	{
		/*if (toyunda->hardware_surface == TRUE)
			gst_video_buffer_set_overlay_composition(video_frame, comp);
		else*/
			gst_video_overlay_composition_blend(comp, video_frame);
		gst_video_overlay_composition_unref(comp);
	}
}


#define CAIRO_UNPREMULTIPLY(a,r,g,b) G_STMT_START { \
  *b = (a > 0) ? MIN ((*b * 255 + a / 2) / a, 255) : 0; \
  *g = (a > 0) ? MIN ((*g * 255 + a / 2) / a, 255) : 0; \
  *r = (a > 0) ? MIN ((*r * 255 + a / 2) / a, 255) : 0; \
} G_STMT_END

static inline void
gst_toyunda_unpremultiply (GstBuffer* buffer, guint width, guint height)
{
  guint i, j;
  guint8 *pimage, *text_image;// = GST_BUFFER_DATA (buffer);

  for (i = 0; i < height; i++) {
    pimage = text_image + 4 * (i * width);
    for (j = 0; j < width; j++) {
      CAIRO_UNPREMULTIPLY (pimage[CAIRO_ARGB_A], &pimage[CAIRO_ARGB_R],
          &pimage[CAIRO_ARGB_G], &pimage[CAIRO_ARGB_B]);

      pimage += 4;
    }
  }
}


static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "toyunda", GST_RANK_NONE,
      GST_TYPE_TOYUNDA);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.1"
#endif
#ifndef PACKAGE
#define PACKAGE "toyunda"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "toyunda"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    toyunda,
    "Toyunda Karaoke plugin",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

