/* GStreamer
 * Copyright (C) 2013 FIXME <fixme@example.com>
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
#include <gst/gst.h>
#include <glib/gstdio.h>
#include <string.h>
#include "gsttoyunda.h"
#include <gst/video/video.h>
#include "stb_image.c"

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

static GstPad *gst_toyunda_request_new_pad (GstElement * element,
    GstPadTemplate * templ, const gchar * name);
static void gst_toyunda_release_pad (GstElement * element, GstPad * pad);
static GstStateChangeReturn
gst_toyunda_change_state (GstElement * element, GstStateChange transition);
static gboolean gst_toyunda_send_event (GstElement * element, GstEvent * event);
static gboolean gst_toyunda_query (GstElement * element, GstQuery * query);

static GstCaps* gst_toyunda_sink_getcaps (GstPad *pad);
static gboolean gst_toyunda_sink_setcaps (GstPad *pad, GstCaps *caps, GstCaps *outcaps);
static GstPadLinkReturn gst_toyunda_sink_link (GstPad *pad, GstPad *peer);
static void gst_toyunda_sink_unlink (GstPad *pad);
static GstFlowReturn gst_toyunda_sink_bufferalloc (GstPad * pad,
    guint64 offset, guint size, GstCaps * caps, GstBuffer ** buffer);
static GstFlowReturn gst_toyunda_sink_chain (GstPad *pad, GstBuffer *buffer);
static gboolean gst_toyunda_sink_event (GstPad *pad, GstEvent *event);
static gboolean gst_toyunda_sink_query (GstPad *pad, GstQuery *query);


static GstCaps* gst_toyunda_src_getcaps (GstPad *pad);
static gboolean gst_toyunda_src_setcaps (GstPad *pad, GstCaps *caps);
static GstPadLinkReturn gst_toyunda_src_link (GstPad *pad, GstPad *peer);
static void gst_toyunda_src_unlink (GstPad *pad);
static gboolean gst_toyunda_src_event (GstPad *pad, GstEvent *event);
static gboolean gst_toyunda_src_query (GstPad *pad, GstQuery *query);


static gboolean gst_toyunda_parse_toyunda_subtitle(GstToyunda* toyunda);
static void	parse_toyunda_line(GSequence *, char *line);
static void	set_color_t_default(rgba_color_t *);
static void	print_toyunda_sub_t(toyunda_sub_t);
static void	read_line(FILE *file_stream, char** linetoret, int *size);
static gboolean	parse_digit(char* str, int pos, char* toret);
static void	parse_toyunda_options(char *str, int *pos, toyunda_sub_t **sub);
static int	parse_toyunda_option(char* str, int pos, toyunda_sub_t **sub);
static int	parse_toyunda_abgr_color(char *str, int pos, rgba_color_t *color);
static void	toyunda_sub_t_partial_cpy(toyunda_sub_t *dest, toyunda_sub_t *src);
static size_t 	mygetline(char **lineptr, size_t *n, FILE *stream);
static gint	toyunda_subtitle_compare(gpointer a, gpointer b, gpointer data);

static gboolean is_valid_rgba_color_t(rgba_color_t);

static GstBuffer*	gst_toyunda_get_image_data(GstToyunda *toyunda, gchar *image_file, uint *width, uint *height);

static void	gst_toyunda_select_subtitle(GstToyunda* toyunda, int framenb);
static void	gst_toyunda_cleanup_subtitles_seq(GSequence *seq);
static void	gst_toyunda_reset_subtitle_buffer_statut(GstToyunda *toyunda);

/* Render function */
static	void	gst_toyunda_draw_grid(GstToyunda *toyunda, GstBuffer *video_frame);
static	void	gst_toyunda_blend_subtitles(GstToyunda *toyunda, GstBuffer *video_frame);
static inline void gst_toyunda_unpremultiply (GstBuffer* buffer, uint width, uint height);

static	void	gst_toyunda_adjust_default_font_size(GstToyunda *toyunda);

enum
{
  PROP_0,
  PROP_SUBFILE
};

/* pad templates */

static GstStaticPadTemplate gst_toyunda_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_BGRx ";"
        GST_VIDEO_CAPS_RGB ";"
        GST_VIDEO_CAPS_BGR ";"
        GST_VIDEO_CAPS_RGBx ";"
        GST_VIDEO_CAPS_xRGB ";"
        GST_VIDEO_CAPS_xBGR ";"
        GST_VIDEO_CAPS_RGBA ";"
        GST_VIDEO_CAPS_BGRA ";"
        GST_VIDEO_CAPS_ARGB ";"
        GST_VIDEO_CAPS_ABGR ";"
        "video/x-surface;"
        GST_VIDEO_CAPS_YUV ("{I420, YV12, AYUV, YUY2, UYVY, v308, v210,"
            " v216, Y41B, Y42B, Y444, Y800, Y16, NV12, NV21, UYVP, A420,"
            " YUV9, IYU1}"))
    );

static GstStaticPadTemplate gst_toyunda_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_BGRx ";"
        GST_VIDEO_CAPS_RGB ";"
        GST_VIDEO_CAPS_BGR ";"
        GST_VIDEO_CAPS_RGBx ";"
        GST_VIDEO_CAPS_xRGB ";"
        GST_VIDEO_CAPS_xBGR ";"
        GST_VIDEO_CAPS_RGBA ";"
        GST_VIDEO_CAPS_BGRA ";"
        GST_VIDEO_CAPS_ARGB ";"
        GST_VIDEO_CAPS_ABGR ";"
        "video/x-surface;"
        GST_VIDEO_CAPS_YUV ("{I420, YV12, AYUV, YUY2, UYVY, v308, v210,"
            " v216, Y41B, Y42B, Y444, Y800, Y16, NV12, NV21, UYVP, A420,"
            " YUV9, IYU1}"))
    );


/* class initialization */

#define DEBUG_INIT(bla) \
  GST_DEBUG_CATEGORY_INIT (gst_toyunda_debug_category, "toyunda", 0, \
      "debug category for toyunda element");

GST_BOILERPLATE_FULL (GstToyunda, gst_toyunda, GstElement,
    GST_TYPE_ELEMENT, DEBUG_INIT);

static void
gst_toyunda_base_init (gpointer g_class)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_add_static_pad_template (element_class,
      &gst_toyunda_sink_template);
  gst_element_class_add_static_pad_template (element_class,
      &gst_toyunda_src_template);

  gst_element_class_set_details_simple (element_class, "Toyunda",
      "toyunda", "FIXME Description", "FIXME <fixme@example.com>");

}

static void
gst_toyunda_class_init (GstToyundaClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  gobject_class->set_property = gst_toyunda_set_property;
  gobject_class->get_property = gst_toyunda_get_property;
  gobject_class->dispose = gst_toyunda_dispose;
  gobject_class->finalize = gst_toyunda_finalize;
  element_class->change_state = GST_DEBUG_FUNCPTR (gst_toyunda_change_state);
  element_class->send_event = GST_DEBUG_FUNCPTR (gst_toyunda_send_event);
  element_class->query = GST_DEBUG_FUNCPTR (gst_toyunda_query);
  
  g_object_class_install_property (G_OBJECT_CLASS (klass), PROP_SUBFILE,
		         g_param_spec_string ("subfile", "Subfile",
			  "The subtitle file to parse",
			  "none", G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

}

static void
gst_toyunda_init (GstToyunda * toyunda, GstToyundaClass * toyunda_class)
{

  toyunda->sinkpad = gst_pad_new_from_static_template (&gst_toyunda_sink_template
      ,     
            "sink");
  gst_pad_set_getcaps_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_getcaps));
  gst_pad_set_setcaps_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_setcaps));
  gst_pad_set_link_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_link));
  gst_pad_set_unlink_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_unlink));
  gst_pad_set_bufferalloc_function(toyunda->sinkpad,
	    GST_DEBUG_FUNCPTR(gst_toyunda_sink_bufferalloc));
  gst_pad_set_chain_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_chain));
  gst_pad_set_event_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_event));
  gst_pad_set_query_function (toyunda->sinkpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_sink_query));
  gst_element_add_pad (GST_ELEMENT(toyunda), toyunda->sinkpad);



  toyunda->srcpad = gst_pad_new_from_static_template (&gst_toyunda_src_template
      ,     
            "src");
  gst_pad_set_getcaps_function (toyunda->srcpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_src_getcaps));
  gst_pad_set_setcaps_function (toyunda->srcpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_src_setcaps));
   gst_pad_set_link_function (toyunda->srcpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_src_link));
  gst_pad_set_unlink_function (toyunda->srcpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_src_unlink));
  gst_pad_set_event_function (toyunda->srcpad,
            GST_DEBUG_FUNCPTR(gst_toyunda_src_event));
  gst_pad_set_query_function (toyunda->srcpad,
		GST_DEBUG_FUNCPTR(gst_toyunda_src_query));
  gst_element_add_pad (GST_ELEMENT(toyunda), toyunda->srcpad);


  toyunda->subfile_parsed = FALSE;
  toyunda->subtitle_changed = FALSE;
  toyunda->current_sub_it = NULL;
  toyunda->subtitles = NULL;
  toyunda->current_subtitles = NULL;
  toyunda->hardware_surface = FALSE;
  
  toyunda->font_desc = STR_TOYUNDA_FONT_DESCRIPTION;
  toyunda->pango_context = pango_font_map_create_context(pango_cairo_font_map_get_default());
  toyunda->pango_layout = pango_layout_new(toyunda->pango_context);
  toyunda->pango_fontdesc = pango_font_description_from_string(toyunda->font_desc);
  pango_font_description_set_size(toyunda->pango_fontdesc, INT_TOYUNDA_FONT_SIZE * PANGO_SCALE);
  pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
}


gboolean
gst_toyunda_parse_toyunda_subtitle(GstToyunda* toyunda)
{
	char	*line;
	int	linesize;
	FILE* 	file_id;
	
	if (toyunda->subtitles != NULL)
	{
		gst_toyunda_cleanup_subtitles_seq(toyunda->subtitles);
		gst_toyunda_cleanup_subtitles_seq(toyunda->current_subtitles);	
	}
	toyunda->subtitles = g_sequence_new(NULL);
	file_id = g_fopen(toyunda->subfile, "r");
	if (file_id == NULL)
	{
		g_printf("Can't open file : %s\n", toyunda->subfile);
		return FALSE;
	}

	while(1)
	{
		read_line(file_id, &line, &linesize);
		if (linesize <= 0)
			break;
		if (linesize <= 5)
			continue;
		if (line[0] != '\0')
			parse_toyunda_line(toyunda->subtitles, line);
		g_free(line);
	}
	toyunda->subfile_parsed = TRUE;
	g_sequence_sort(toyunda->subtitles, toyunda_subtitle_compare, NULL);
	return TRUE;
}

static void	parse_toyunda_line(GSequence* subtitles, char *line)
{
	int	strpos = 0;
	char	*strtmp;
	int	strtmppos = 0;
	int	pipecpt = 0;
	
	toyunda_sub_t*	tmp_sub = NULL; /* In case of multiline */

	toyunda_sub_t*	new_sub = g_new(toyunda_sub_t, 1);
	set_color_t_default(&(new_sub->color1));
	set_color_t_default(&(new_sub->color2));
	set_color_t_default(&(new_sub->tmpcolor));
	strtmp = g_new(char, 255);
	strtmp[0] = '\0';
	new_sub->positionx = -1;
	new_sub->positiony = -1;
	new_sub->image = g_new(char, strlen(STR_TOYUNDA_LOGO_NONE) + 1);
	strcpy(new_sub->image, STR_TOYUNDA_LOGO_NONE);
	new_sub->size = -1;

	while (line[strpos] == ' ' && line[strpos] != '\0')
		strpos++;

	/* let start with the start and end of subtitle */
	if (line[strpos] == '{')
	{
		strpos++;
		if (parse_digit(line, strpos, strtmp) == TRUE)
		{
			strpos += strlen(strtmp);
			new_sub->start = (guint) g_ascii_strtod(strtmp, NULL);
		}
		else
		{
			g_printf("Expecting frame start number\n");
			return ;
		}
	}
	else
	{
		g_printf("Expecting {\n");
		return;
	}
	/* Now we are at the } */
	if (line[strpos] == '}')
		strpos++;
	else
	{
		g_printf("Expecting }\n");
		return;
	}
	/* ok now the stop number */
	if (line[strpos] == '{')
	{
		strpos++;
		if (parse_digit(line, strpos, strtmp) == TRUE)
		{
			strpos += strlen(strtmp);
			new_sub->stop = (guint) g_ascii_strtod(strtmp, NULL);
		}
		else
		{
			g_printf("Expecting frame stop number\n");
			return ;
		}
	}
	else
	{
		g_printf("Expecting {\n");
		return;
	}
	if (line[strpos] == '}')
		strpos++;
	else
	{
		g_printf("Expecting }\n");
		return;
	}

	/* OK easiest part done, now option or content */
	if (new_sub->start == 0 && new_sub->stop == 0)
	{
		g_free(new_sub->image);
		g_free(new_sub);
		return ;
	}
	/* Option */
	parse_toyunda_options(line, &strpos, &new_sub);
	/* Welcome to the pipe wtf */
pipehandle :
	pipecpt = 0;
	if (line[strpos] == '|')
	{
		while (line[strpos] == '|')
		{
			strpos++;
			pipecpt++;
		}
	}
	/* Option can be before and after pipe */
	parse_toyunda_options(line, &strpos, &new_sub);
	/* The content itself*/
	strtmppos = 0;
	strtmp[0] = '\0';
	while (line[strpos] != '\n' && line[strpos] != '\0' && line[strpos] != '|' && line != '\r')
	{
		strtmp[strtmppos] = line[strpos];
		if (line[strpos] == -1 && strcmp(new_sub->image, STR_TOYUNDA_LOGO_NONE) == 0)
		{
			g_free(new_sub->image);
			new_sub->image = g_new(char, strlen(STR_TOYUNDA_LOGO_DEFAULT) + 1);
			strcpy(new_sub->image, STR_TOYUNDA_LOGO_DEFAULT);
		}
		strpos++;
		strtmppos++;
	}
	strtmp[strtmppos] = '\0';
	new_sub->text = g_new(char, strtmppos + 1);
	strcpy(new_sub->text, strtmp);
	new_sub->text[strtmppos] = '\0';
	/* Position calculation, pipe have incidence*/
	new_sub->positiony =  ((float)1/(float)12) * (pipecpt);
	//print_toyunda_sub_t(*new_sub);
	g_sequence_append(subtitles, new_sub);
	/* we got a new line in the line*/
	if (line[strpos] == '|')
	{
		g_printf("Multiline :(\n");
		tmp_sub = g_new(toyunda_sub_t, 1);
		toyunda_sub_t_partial_cpy(tmp_sub, new_sub);
		new_sub = tmp_sub;
		new_sub->image = g_new(char, strlen(STR_TOYUNDA_LOGO_NONE));
		strcpy(new_sub->image, STR_TOYUNDA_LOGO_NONE);
		goto pipehandle;
	}
	
	g_free(strtmp);
}

static void	parse_toyunda_options(char *str, int *pos, toyunda_sub_t **sub)
{
	int	strtmppos = 0;

	while (str[*pos] == '{')
	{
		(*pos)++;
		if ((strtmppos = parse_toyunda_option(str, *pos, sub)) == 0)
		{
			g_printf("Expecting Option\n");
			return ;
		}
		else
			*pos += strtmppos;
		if (str[*pos] != '}')
		{
			g_printf("Expecting end of Option '}'\n");
			return ;
		}
		(*pos)++;
	}
}

/* return the lenght of option string */
/* option are like this : {optidentifier:value(:value)} */

static int	parse_toyunda_option(char* str, int pos, toyunda_sub_t **sub)
{
	int	toret;
	int	startpos = pos;
	char	strtmp[32];
	char	buff[255];
	int	tmp;

	/* c = color, o = pos, s = size */
	if (str[pos] == 'c')
	{
		if (str[pos + 1] == ':')
		{
			pos += 2;
			tmp = parse_toyunda_abgr_color(str, pos, &((*sub)->color1));
			if (tmp == 0)
			{
				g_printf("Expecting valid color ($AABBGGRR)\n");
				return 0;
			}
			pos += tmp;
			/* Color 2 */
			if (str[pos] == ':')
			{
				pos++;
				tmp = parse_toyunda_abgr_color(str, pos, &((*sub)->color2));
				if (tmp == 0)
				{
					g_printf("Expecting valid color ($AABBGGRR)\n");
					return 0;
				}
			pos += tmp;
			}
		}
		else
		{
			g_printf("Expecting option value\n");
			return 0;
		}
	} else
		if (str[pos] == 'o')
		{
			if (str[pos + 1]  == ':')
			{
				pos+= 2;
				if (parse_digit(str, pos, strtmp) == FALSE)
				{
					g_printf("Expecting position X value\n");
					return 0;
				}
				(*sub)->positionx = ((gint) g_ascii_strtod(strtmp, NULL)) / 800;
				pos += strlen(strtmp);
				if (str[pos] == ',')
				{
					pos++;
					if (parse_digit(str, pos, strtmp) == FALSE)
					{
						g_printf("Expecting position Y value\n");
						return 0;
					}
					(*sub)->positiony = ((gint) g_ascii_strtod(strtmp, NULL)) / 600;
					pos += strlen(strtmp);
				}
			}
			else
			{
				g_printf("Expecting option value\n");
				return 0;
			}
		} else
			if (str[pos] == 'b')
			{
				if (str[pos + 1] == ':')
				{
					pos += 2;
					int cpt = 0;
					while (str[pos] != '}')
					{
						buff[cpt] = str[pos];
						pos++;
						cpt++;
					}
					buff[cpt] = '\0';
					g_free((*sub)->image);
					(*sub)->image = g_new(gchar, strlen(buff) + 1);
					strcpy((*sub)->image, buff);
				}
			}
	return (pos - startpos);
}

static int	parse_toyunda_abgr_color(char *str, int pos, rgba_color_t *color)
{
	char	colorstr[32];
	char	tmpstr[3];
	int	cpos = 0;
	int 	offset = 0; /* use for handling alpha, because the format is (a)bgr*/

	if (str[pos] == '$')
		pos++;
	else
		return 0;
	while ((str[pos] <= '9' && str[pos] >= '0') || (str[pos] <= 'f' && str[pos] >= 'a')
		|| (str[pos] <= 'F' && str[pos] >= 'A'))
	{
		colorstr[cpos] = str[pos];
		pos++;
		cpos++;
	}
	colorstr[cpos] = '\0';
	if (strlen(colorstr) != 6 && strlen(colorstr) != 8)
	{
		g_printf("Invalide color format\n");
		return 0;
	}
	tmpstr[2] = '\0';
	/* Alpha */
	if (strlen(colorstr) == 8)
	{
		tmpstr[0] = colorstr[0];
		tmpstr[1] = colorstr[1];
		color->alpha = g_ascii_strtoll(tmpstr, NULL, 16);
		offset = 2;
	}
	else
		color->alpha = 0xFF;
	tmpstr[0] = colorstr[offset + 0];
	tmpstr[1] = colorstr[offset + 1];
	color->blue = g_ascii_strtoll(tmpstr, NULL, 16);
	tmpstr[0] = colorstr[offset + 2];
	tmpstr[1] = colorstr[offset + 3];
	color->green = g_ascii_strtoll(tmpstr, NULL, 16);
	tmpstr[0] = colorstr[offset + 4];
	tmpstr[1] = colorstr[offset + 5];
	color->red = g_ascii_strtoll(tmpstr, NULL, 16);
	return (cpos + 1);
}

static gboolean	parse_digit(char* str, int pos, char* toret)
{
	int	toretpos = 0;

	while (g_ascii_isdigit(str[pos]) == TRUE)
	{
		toret[toretpos] = str[pos];
		pos++;
		toretpos++;
	}
	toret[toretpos] = '\0';
	if (toretpos != 0)
		return TRUE;
	else
		return FALSE;
}

static void	print_toyunda_sub_t(toyunda_sub_t toysub)
{
	g_printf("Start : %d\n", toysub.start);
	g_printf("Stop  : %d\n", toysub.stop);
	g_printf("Color1 : r:%d, g:%d, b:%d, a:%d\n", toysub.color1.red, toysub.color1.green, toysub.color1.blue, toysub.color1.alpha);
	g_printf("Color2 : r:%d, g:%d, b:%d, a:%d\n", toysub.color2.red, toysub.color2.green, toysub.color2.blue, toysub.color2.alpha);
	g_printf("TMPColor  : r:%d, g:%d, b:%d, a:%d\n", toysub.tmpcolor.red, toysub.tmpcolor.green, toysub.tmpcolor.blue, toysub.tmpcolor.alpha);
	g_printf("positionx : %d (Float : %f)\n", (int)(toysub.positionx * 800), toysub.positionx);
	g_printf("positiony : %d (Float : %f)\n", (int) (toysub.positiony * 600), toysub.positiony);
	g_printf("Text : %s\n", toysub.text);
	g_printf("Image : %s\n", toysub.image);
	g_printf("Size : %d\n", toysub.size);
}


/* */
static void	read_line(FILE *file_stream, char** linetoret, int *size)
{
		*linetoret = NULL;
		size_t p = 0;
		*size = mygetline(linetoret, &p, file_stream);
}

/* don't copy image and text field */
static void	toyunda_sub_t_partial_cpy(toyunda_sub_t *dest, toyunda_sub_t *src)
{
	dest->color1.alpha = src->color1.alpha;
	dest->color1.red = src->color1.red;
	dest->color1.green = src->color1.green;
	dest->color1.blue = src->color1.blue;
	dest->color2.alpha = src->color2.alpha;
	dest->color2.red = src->color2.red;
	dest->color2.green = src->color2.green;
	dest->color2.blue = src->color2.blue;
	dest->positionx = src->positionx;
	dest->positiony = src->positiony;
	dest->start = src->start;
	dest->stop = src->stop;
	dest->size = src->size;
}

static gboolean is_valid_rgba_color_t(rgba_color_t col)
{
	if (col.red == -1)
		return FALSE;
	return TRUE;
}

gint toyunda_subtitle_compare(gpointer a, gpointer b, gpointer data)
{
	toyunda_sub_t*	t1 = (toyunda_sub_t*) a;
	toyunda_sub_t*	t2 = (toyunda_sub_t*) b;

	if (t1->start < t2->start)
		return -1;
	if (t1->start == t2->start)
		if (t1->stop < t2->stop)
			return -1;
		else
		{
			if (t1->positiony < t2->positiony)
				return -1;
			else
				return 1;
		}
	return 1;
}


static void	set_color_t_default(rgba_color_t *toset)
{
	toset->alpha = -1;
	toset->blue = -1;
	toset->green = -1;
	toset->red = -1;
}

/* This code is public domain -- Will Hartung 4/9/09 */
static size_t mygetline(char **lineptr, size_t *n, FILE *stream)
{
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

	if (lineptr == NULL) {
		return -1;
	}
	if (stream == NULL) {
		return -1;
	}
	if (n == NULL) {
		return -1;
	}
	bufptr = *lineptr;
	size = *n;

	c = fgetc(stream);
	if (c == EOF) {
		return -1;
	}
	if (bufptr == NULL) {
		bufptr = malloc(128);
		if (bufptr == NULL) {
			return -1;
		}
		size = 128;
	}
	p = bufptr;
	while(c != EOF) {
		if ((p - bufptr) > (size - 1)) {
			size = size + 128;
			bufptr = realloc(bufptr, size);
			if (bufptr == NULL) {
				return -1;
			}
		}
		*p++ = c;
		if (c == '\n') {
			break;
		}
		c = fgetc(stream);
	}

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}


void gst_toyunda_adjust_default_font_size(GstToyunda* toyunda)
{
	guint	new_size;

	new_size = (double)(((double) toyunda->video_height / INT_TOYUNDA_BASE_HEIGHT) * INT_TOYUNDA_FONT_SIZE) + 0.5;
	pango_font_description_set_size(toyunda->pango_fontdesc, new_size * PANGO_SCALE);
	pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
	g_printf("New size : %d -- Font desct : %s\n", new_size, pango_font_description_to_string(toyunda->pango_fontdesc));
}



static GstBuffer*	gst_toyunda_get_image_data(GstToyunda *toyunda, gchar *image_file, uint *width, uint *height)
{
	toyunda_image_t*	img;
	GSequenceIter*		it, *it_end;
	
	if (toyunda->images == NULL)
		toyunda->images = g_sequence_new(NULL);
	it = g_sequence_get_begin_iter(toyunda->images);
	it_end = g_sequence_get_end_iter(toyunda->images);
	/* Search for the image */
	
	while (it != it_end)
	{
		img = (toyunda_image_t*) g_sequence_get(it);
		if (strcmp(img->path, image_file) == 0)
		{
			*width = img->width;
			*height = img->height;
			return img->buffer;
		}
		it = g_sequence_iter_next(it);
	}
	/* Can't find the image, time to load and create the buffer */
	
	int x,y,n;
	unsigned char *data = stbi_load(image_file, &x, &y, &n, 4);
	if (data == NULL)
	{
		g_printf("Can't open file %s : %s\n", image_file, stbi_failure_reason());
		return NULL;
	}
	g_printf("Adding new image buffer for image : %s - widht : %d, height :%d\n", image_file, x, y);
	img = g_new(toyunda_image_t, 1);
	img->path = g_new(gchar, strlen(image_file) + 1);
	strcpy(img->path, image_file);
	img->width = x;
	img->height = y;
	*width = x;
	*height = y;
	/* the output format is RGBA, need bgra */
	uint cpt;
	char a, r, g, b;
	for (cpt = 0; cpt < x * y; cpt++)
	{
		r = data[cpt * 4];
		//g = data[cpt * 4 + 1];
		b = data[cpt * 4 + 2];
		//a = data[cpt * 4 + 3];
		data[cpt * 4] = b;
		//data[cpt *4 + 1] = r;
		data[cpt * 4 + 2] = r;
		//data[cpt * 4 + 3] = b;
	}
	img->buffer = gst_buffer_new_and_alloc(x * y * 4);
	memcpy(GST_BUFFER_DATA(img->buffer), data, x * y * 4);
	g_free(data);
	g_sequence_append(toyunda->images, img);
	return img->buffer;
}

void gst_toyunda_reset_subtitle_buffer_statut(GstToyunda* toyunda)
{
	GSequenceIter*	it;
	GSequenceIter*	it_end;
	toyunda_sub_and_buff_t* sub_buff;

	if (toyunda->current_subtitles != NULL && g_sequence_get_length(toyunda->current_subtitles) > 0)
	{
		it = g_sequence_get_begin_iter(toyunda->current_subtitles);
		it_end = g_sequence_get_end_iter(toyunda->current_subtitles);
		while (it != it_end)
		{
			sub_buff = (toyunda_sub_and_buff_t *) g_sequence_get(it);
			sub_buff->to_change = TRUE;
			it = g_sequence_iter_next(it);
		}
	}
}


/* set current_subtitle to reflect the frame number*/
/* current_sub_it is the last position in the total subtitles list,
 since the subtitle list is sorted, no need to begin from the start
 if a seek/reset event didn't occur */

static void	gst_toyunda_select_subtitle(GstToyunda *toyunda, int framenb)
{
	GSequenceIter*	it;
	GSequenceIter*	it_end;
	toyunda_sub_t*	sub;
	toyunda_sub_and_buff_t* sub_buff;
	
	toyunda->subtitle_changed = FALSE;
	if (toyunda->current_subtitles == NULL)
		toyunda->current_subtitles = g_sequence_new(NULL);
	if (toyunda->current_sub_it != NULL)
		it = toyunda->current_sub_it;
	else
		toyunda->current_sub_it = g_sequence_get_begin_iter(toyunda->subtitles);
	
	/* Add new subtitle to current according to framenb */
	it = toyunda->current_sub_it;
	it_end = g_sequence_get_end_iter(toyunda->subtitles);
	while (it != it_end)
	{
		sub = (toyunda_sub_t*) g_sequence_get(it);
		if (sub->start == framenb)
		{
			g_printf("ADD SUBTITLE : ");
			print_toyunda_sub_t(*sub);
			sub_buff = g_new(toyunda_sub_and_buff_t, 1);
			sub_buff->subtitle = sub;
			sub_buff->overlay_rect = NULL;
			sub_buff->to_change = TRUE;
			toyunda->subtitle_changed = TRUE;
			g_sequence_append(toyunda->current_subtitles, sub_buff);
		}
		if (sub->start > framenb)
			break;
		it = g_sequence_iter_next(it);
	}
	toyunda->current_sub_it = it;
	/* Remove subtitle */
	/*g_printf("==Current subtitle lenght : %d\n", g_sequence_get_length(toyunda->current_subtitles));*/
	it = g_sequence_get_begin_iter(toyunda->current_subtitles);
	it_end = g_sequence_get_end_iter(toyunda->current_subtitles);

	GSequenceIter* it_next;
	while (it != it_end)
	{
		it_next = g_sequence_iter_next(it);
		sub_buff = (toyunda_sub_and_buff_t*) g_sequence_get(it);
		sub = sub_buff->subtitle;
		if (sub->stop <= framenb)
		{
			g_printf("REMOVE SUBTITLE : ");
			print_toyunda_sub_t(*sub);
			g_sequence_remove(it);
			if (sub_buff->overlay_rect != NULL)
				gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
			g_free(sub_buff);
			it = it_next;
			toyunda->subtitle_changed = TRUE;
			continue;
		} else
		{
			if (is_valid_rgba_color_t(sub->color2) == TRUE)
			{
				float rap = (float) (framenb - sub->start) / (float) (sub->stop - sub->start);
			
				sub->tmpcolor.alpha = sub->color1.alpha + rap * (sub->color2.alpha - sub->color1.alpha);
				sub->tmpcolor.red = sub->color1.red + rap * (sub->color2.red - sub->color1.red);
				sub->tmpcolor.blue = sub->color1.blue + rap * (sub->color2.blue - sub->color1.blue);
				sub->tmpcolor.green = sub->color1.green + rap * (sub->color2.green - sub->color1.green);
				toyunda->subtitle_changed = TRUE;
				sub_buff->to_change = TRUE;
			}
		}
		it = g_sequence_iter_next(it);
	}
		
}


void
gst_toyunda_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstToyunda *toyunda = GST_TOYUNDA (object);

  switch (property_id) {
	  case PROP_SUBFILE:
		  toyunda->subfile = g_value_dup_string(value);
		  toyunda->subfile_parsed = FALSE;
		  if (toyunda->base_toyunda.current_state != NULL)
			  gst_toyunda_parse_toyunda_subtitle(toyunda);
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

  switch (property_id) {
	case PROP_SUBFILE:
		if (toyunda->subfile != NULL)
			g_free(toyunda->subfile);
		toyunda->subfile = g_value_dup_string(value);
	break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_toyunda_dispose (GObject * object)
{
  /* GstToyunda *toyunda = GST_TOYUNDA (object); */

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

void
gst_toyunda_finalize (GObject * object)
{
  /* GstToyunda *toyunda = GST_TOYUNDA (object); */

  /* clean up object here */

  G_OBJECT_CLASS (parent_class)->finalize (object);
}



static GstPad *
gst_toyunda_request_new_pad (GstElement * element, GstPadTemplate * templ,
    const gchar * name)
{

  return NULL;
}

static void
gst_toyunda_release_pad (GstElement * element, GstPad * pad)
{

}

static GstStateChangeReturn
gst_toyunda_change_state (GstElement * element, GstStateChange transition)
{
  GstToyunda *toyunda;
  GstStateChangeReturn ret;

  g_printf("TOYUNDA : Change state\n");
  g_return_val_if_fail (GST_IS_TOYUNDA (element), GST_STATE_CHANGE_FAILURE);
  toyunda = GST_TOYUNDA (element);

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
	    if (toyunda->subfile_parsed == FALSE && toyunda->subfile != NULL)
		    gst_toyunda_parse_toyunda_subtitle(toyunda);
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      break;
    default:
      break;
  }

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      break;
    default:
      break;
  }

  return ret;
}


static gboolean
gst_toyunda_send_event (GstElement * element, GstEvent * event)
{

  return TRUE;
}

static gboolean
gst_toyunda_query (GstElement * element, GstQuery * query)
{

  return FALSE;
}

static GstCaps*
gst_toyunda_sink_getcaps (GstPad *pad)
{
  GstToyunda *toyunda;
  GstCaps *caps;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "getcaps");

  caps = gst_caps_copy (gst_pad_get_pad_template_caps (pad));

  gst_object_unref (toyunda);
  return caps;
}

static gboolean
gst_toyunda_sink_setcaps (GstPad *pad, GstCaps *caps, GstCaps *outcaps)
{
  GstToyunda *toyunda;
  GstStructure *structure;
  gboolean ret = FALSE;
  const GValue *fps;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));
  

  GST_DEBUG_OBJECT(toyunda, "setcaps");

  if (!GST_PAD_IS_SINK (pad))
    return TRUE;

  g_return_val_if_fail (gst_caps_is_fixed (caps), FALSE);

  if (G_UNLIKELY (!toyunda))
    return FALSE;

  toyunda->video_width = 0;
  toyunda->video_height = 0;
  structure = gst_caps_get_structure (caps, 0);
  fps = gst_structure_get_value (structure, "framerate");

  if (fps
      && gst_video_format_parse_caps (caps, &toyunda->format, &toyunda->video_width,
          &toyunda->video_height)) {
    ret = gst_pad_set_caps (toyunda->srcpad, caps);
  }
  if (ret == TRUE)
  {
	structure = gst_caps_get_structure (caps, 0);
	if (gst_structure_has_name (structure, "video/x-surface"))
		toyunda->hardware_surface = TRUE;
	else
		toyunda->hardware_surface = FALSE;
  }
  
  toyunda->fps_n = gst_value_get_fraction_numerator (fps);
  toyunda->fps_d = gst_value_get_fraction_denominator (fps);
  g_printf("video format : %s\n", gst_structure_to_string(structure));
  g_printf("fps d,n : %f , %f\n", toyunda->fps_d, toyunda->fps_n);
  g_printf("height : %d, width : %d\n", toyunda->video_height, toyunda->video_width);
  
  gst_toyunda_adjust_default_font_size(toyunda);
  gst_toyunda_reset_subtitle_buffer_statut(toyunda);
  
  GST_DEBUG_OBJECT(toyunda, "setcaps");


  gst_object_unref (toyunda);
  return TRUE;
}

static gboolean
gst_toyunda_sink_acceptcaps (GstPad *pad, GstCaps *caps)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "acceptcaps");


  gst_object_unref (toyunda);
  return TRUE;
}

static void
gst_toyunda_sink_fixatecaps (GstPad *pad, GstCaps *caps)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "fixatecaps");


  gst_object_unref (toyunda);
}

static gboolean
gst_toyunda_sink_activate (GstPad *pad)
{
  GstToyunda *toyunda;
  gboolean ret;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "activate");

  if (gst_pad_check_pull_range (pad)) {
    GST_DEBUG_OBJECT (pad, "activating pull");
    ret = gst_pad_activate_pull (pad, TRUE);
  } else {
    GST_DEBUG_OBJECT (pad, "activating push");
    ret = gst_pad_activate_push (pad, TRUE);
  }

  gst_object_unref (toyunda);
  return ret;
}

static gboolean
gst_toyunda_sink_activatepush (GstPad *pad, gboolean active)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "activatepush");


  gst_object_unref (toyunda);
  return TRUE;
}

static gboolean
gst_toyunda_sink_activatepull (GstPad *pad, gboolean active)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "activatepull");


  gst_object_unref (toyunda);
  return TRUE;
}

static GstPadLinkReturn
gst_toyunda_sink_link (GstPad *pad, GstPad *peer)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "link");


  gst_object_unref (toyunda);
  return GST_PAD_LINK_OK;
}

static void
gst_toyunda_sink_unlink (GstPad *pad)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "unlink");


  gst_object_unref (toyunda);
}



void gst_toyunda_draw_grid(GstToyunda* toyunda, GstBuffer* video_frame)
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
	
		buff_data = GST_BUFFER_DATA(buff);
		/* draw line */
		for (cpt = 0; cpt < GST_BUFFER_SIZE(buff); cpt++)
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
		
		comprect = gst_video_overlay_rectangle_new_argb(buff, toyunda->video_width, toyunda->video_height,
			toyunda->video_width * 4, 0, 0, toyunda->video_width, toyunda->video_height, 
			GST_VIDEO_OVERLAY_FORMAT_FLAG_NONE);
		comp = gst_video_overlay_composition_new(comprect);
	}
	gst_video_overlay_composition_blend(comp, video_frame);
	/*gst_buffer_unref(buff);
	gst_video_overlay_composition_unref(comp);
	gst_video_overlay_rectangle_unref(comprect);*/
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

void gst_toyunda_create_subtitle_buffers(GstToyunda* toyunda)
{
	GSequenceIter		*it, *it_end;
	toyunda_sub_t*		sub;
	toyunda_sub_and_buff_t*	sub_buff;
	cairo_t*		cr;
	cairo_surface_t*	surface;
	PangoRectangle		ink_rect, logical_rect;
	GstBuffer		*buffer;
	rgba_color_t		color;
	uint			pos_x, pos_y;
	PangoAttrList		*pattrl;
	uint			old_size, new_size;
	char			*tmptext;
	
	it = g_sequence_get_begin_iter(toyunda->current_subtitles);
	it_end = g_sequence_get_end_iter(toyunda->current_subtitles);
	
	while (it != it_end)
	{
		new_size = 0;
		old_size = 0;
		sub_buff = (toyunda_sub_and_buff_t*) g_sequence_get(it);
		if (sub_buff->to_change == TRUE)
		{
			sub = sub_buff->subtitle;
			print_toyunda_sub_t(*sub);
			if (strcmp(sub->image, STR_TOYUNDA_LOGO_NONE) == 0)
				tmptext = sub->text;
			else
			{
				/* pango doesn't like the ÿ (-1) char) replace with something else */
				tmptext = g_new(char, strlen(sub->text) + 1);
				strcpy(tmptext, sub->text);
				uint cpt_tmp = 0;
				while (tmptext[cpt_tmp] != -1)
					cpt_tmp++;
				tmptext[cpt_tmp] = 'o';
			}
			/* Need this in both case of image or text */
			pango_layout_set_width(toyunda->pango_layout, -1);
			pango_layout_set_text(toyunda->pango_layout, tmptext, strlen(tmptext));
			pango_layout_get_pixel_extents (toyunda->pango_layout, &ink_rect, &logical_rect);
			/* if The text is too long */
			if (logical_rect.width > toyunda->video_width)
			{
				old_size = pango_font_description_get_size(toyunda->pango_fontdesc) / PANGO_SCALE;
				new_size = old_size * ((double)toyunda->video_width / logical_rect.width);
				g_printf("text too long, resize : lrectwidht : %d old_size : %d, new_size : %d\n", 
					 logical_rect.width, old_size, new_size);
				pango_font_description_set_size(toyunda->pango_fontdesc, new_size * PANGO_SCALE);
				pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
		
				pango_layout_set_width(toyunda->pango_layout, -1);
				pango_layout_set_text(toyunda->pango_layout, tmptext, strlen(tmptext));
				pango_layout_get_pixel_extents (toyunda->pango_layout, &ink_rect, &logical_rect);
			}
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
					color.green = 0x00;
					color.red = 0x00;
				}
				
				buffer = gst_buffer_new_and_alloc(logical_rect.height * logical_rect.width * 4);
				surface = cairo_image_surface_create_for_data(GST_BUFFER_DATA(buffer), CAIRO_FORMAT_ARGB32,
					logical_rect.width, logical_rect.height, logical_rect.width * 4);
				cr = cairo_create(surface);
				cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
				cairo_paint(cr);
				cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
				/* Outline */
				uint outline_size = (double)(pango_font_description_get_size(toyunda->pango_fontdesc) / PANGO_SCALE) / 15;
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
				gst_toyunda_unpremultiply(buffer, logical_rect.width, logical_rect.height);
				//plop_show_data(GST_BUFFER_DATA(buffer), logical_rect.width, logical_rect.height);
				if (sub->positionx == -1)
					pos_x = (toyunda->video_width - logical_rect.width) / 2;
				else
					pos_x = sub->positionx;
				if (sub_buff->overlay_rect != NULL)
					gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
				sub_buff->overlay_rect = gst_video_overlay_rectangle_new_argb(buffer, logical_rect.width,
					logical_rect.height, logical_rect.width * 4, pos_x, pos_y, logical_rect.width, logical_rect.height,
					GST_VIDEO_OVERLAY_FORMAT_FLAG_NONE);
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
				pos_x = (toyunda->video_width - logical_rect.width) / 2 + img_pos * new_rect.width;
				if (pos_x > new_rect.width / 2)
					pos_x -= new_rect.width / 2;
				g_printf("POSITION X SYL : %d\n", pos_x);
				buffer = gst_toyunda_get_image_data(toyunda, sub->image, &width, &height);
				if (buffer != NULL)
				{
					if (sub_buff->overlay_rect != NULL)
						gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
					sub_buff->overlay_rect = gst_video_overlay_rectangle_new_argb(buffer, width,
					height, width * 4, pos_x, pos_y, width * transform_ratio, height * transform_ratio,
					GST_VIDEO_OVERLAY_FORMAT_FLAG_NONE);
					sub_buff->to_change = FALSE;
				}
			}
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

void gst_toyunda_blend_subtitles(GstToyunda* toyunda, GstBuffer* video_frame)
{
	GSequenceIter	*it, *it_end;
	GstVideoOverlayComposition	*comp;
	GstVideoOverlayRectangle	*rect;
	toyunda_sub_and_buff_t		*sub_buff;
	
	it = g_sequence_get_begin_iter(toyunda->current_subtitles);
	it_end = g_sequence_get_end_iter(toyunda->current_subtitles);
	
	comp = NULL;
	video_frame = gst_buffer_make_writable(video_frame);
	
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
		if (toyunda->hardware_surface == TRUE)
			gst_video_buffer_set_overlay_composition(video_frame, comp);
		else
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
gst_toyunda_unpremultiply (GstBuffer* buffer, uint width, uint height)
{
  guint i, j;
  guint8 *pimage, *text_image = GST_BUFFER_DATA (buffer);

  for (i = 0; i < height; i++) {
    pimage = text_image + 4 * (i * width);
    for (j = 0; j < width; j++) {
      CAIRO_UNPREMULTIPLY (pimage[CAIRO_ARGB_A], &pimage[CAIRO_ARGB_R],
          &pimage[CAIRO_ARGB_G], &pimage[CAIRO_ARGB_B]);

      pimage += 4;
    }
  }
}

static GstFlowReturn
gst_toyunda_sink_chain (GstPad *pad, GstBuffer *buffer)
{
  GstToyunda *toyunda;
  GstFlowReturn ret;
  gboolean in_seg = FALSE;
  float framerate;
  int framenb;
  gint64 start, stop, clip_start = 0, clip_stop = 0;
  
  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));
  start = GST_BUFFER_TIMESTAMP (buffer);
  framerate = toyunda->fps_n / toyunda->fps_d;
  framenb = ((start / 1000000) * framerate + 100 )/ 1000;
  /*g_printf("Timestamp : %u ---- frame :", start);
  g_printf("%d\n", framenb);*/
  
  gst_toyunda_select_subtitle(toyunda, framenb);
  gst_toyunda_draw_grid(toyunda, buffer);
  if (toyunda->subtitle_changed == TRUE)
	gst_toyunda_create_subtitle_buffers(toyunda);
  toyunda->subtitle_changed = FALSE;
  gst_toyunda_blend_subtitles(toyunda, buffer);
	
  GST_DEBUG_OBJECT(toyunda, "chain");


  ret = gst_pad_push(toyunda->srcpad, buffer);
  gst_object_unref (toyunda);
  return ret;
}

static GstFlowReturn
gst_toyunda_sink_chainlist (GstPad *pad, GstBufferList *bufferlist)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "chainlist");


  gst_object_unref (toyunda);
  return GST_FLOW_OK;
}

static gboolean
gst_toyunda_sink_event (GstPad *pad, GstEvent *event)
{
  gboolean res;
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "event");


  switch (GST_EVENT_TYPE (event)) {
    default:
      res = gst_pad_event_default (pad, event);
      break;
  }

  gst_object_unref (toyunda);
  return res;
}

static gboolean
gst_toyunda_sink_query (GstPad *pad, GstQuery *query)
{
  gboolean res;
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "query");

  switch (GST_QUERY_TYPE(query)) {
    default:
      res = gst_pad_query_default (pad, query);
      break;
  }

  gst_object_unref (toyunda);
  return res;
}

static GstFlowReturn
gst_toyunda_sink_bufferalloc (GstPad *pad, guint64 offset, guint size,
    GstCaps *caps, GstBuffer **buf)
{
  GstToyunda *toyunda;
  GstFlowReturn ret = GST_FLOW_WRONG_STATE;
  GstPad *allocpad;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "bufferalloc");


  if (G_UNLIKELY (!toyunda))
    return GST_FLOW_WRONG_STATE;

  allocpad = toyunda->srcpad ? gst_object_ref (toyunda->srcpad) : NULL;

  if (allocpad) {
    ret = gst_pad_alloc_buffer (allocpad, offset, size, caps, buf);
    gst_object_unref (allocpad);
  }

  gst_object_unref (toyunda);
  return ret;
}

static GstIterator *
gst_toyunda_sink_iterintlink (GstPad *pad)
{
  GstToyunda *toyunda;
  GstIterator *iter;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "iterintlink");

  iter = gst_pad_iterate_internal_links_default (pad);

  gst_object_unref (toyunda);
  return iter;
}


static GstCaps*
gst_toyunda_src_getcaps (GstPad *pad)
{
  GstToyunda *toyunda;
  GstCaps *caps;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "getcaps");

  caps = gst_pad_peer_get_caps (toyunda->sinkpad);
  if (caps) {
    GstCaps *temp;
    const GstCaps *templ;

    //GST_DEBUG_OBJECT (pad, "peer caps  %" GST_PTR_FORMAT, caps);

    /* filtered against our padtemplate */
    templ = gst_pad_get_pad_template_caps (toyunda->sinkpad);
    //GST_DEBUG_OBJECT (pad, "our template  %" GST_PTR_FORMAT, templ);
    temp = gst_caps_intersect (caps, templ);
    //GST_DEBUG_OBJECT (pad, "intersected %" GST_PTR_FORMAT, temp);
    gst_caps_unref (caps);
    /* this is what we can do */
    caps = temp;
  } else {
    /* no peer, our padtemplate is enough then */
    caps = gst_caps_copy (gst_pad_get_pad_template_caps (pad));
  }
  
  gst_object_unref (toyunda);
  return caps;
}

static gboolean
gst_toyunda_src_setcaps (GstPad *pad, GstCaps *caps)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "setcaps");

  gst_object_unref (toyunda);
  return TRUE;
}

static gboolean
gst_toyunda_src_acceptcaps (GstPad *pad, GstCaps *caps)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "acceptcaps");


  gst_object_unref (toyunda);
  return TRUE;
}

static void
gst_toyunda_src_fixatecaps (GstPad *pad, GstCaps *caps)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "fixatecaps");


  gst_object_unref (toyunda);
}

static gboolean
gst_toyunda_src_activate (GstPad *pad)
{
  GstToyunda *toyunda;
  gboolean ret;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "activate");

  if (gst_pad_check_pull_range (pad)) {
    GST_DEBUG_OBJECT (pad, "activating pull");
    ret = gst_pad_activate_pull (pad, TRUE);
  } else {
    GST_DEBUG_OBJECT (pad, "activating push");
    ret = gst_pad_activate_push (pad, TRUE);
  }

  gst_object_unref (toyunda);
  return ret;
}

static gboolean
gst_toyunda_src_activatepush (GstPad *pad, gboolean active)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "activatepush");


  gst_object_unref (toyunda);
  return TRUE;
}

static gboolean
gst_toyunda_src_activatepull (GstPad *pad, gboolean active)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "activatepull");


  gst_object_unref (toyunda);
  return TRUE;
}

static GstPadLinkReturn
gst_toyunda_src_link (GstPad *pad, GstPad *peer)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "link");


  gst_object_unref (toyunda);
  return GST_PAD_LINK_OK;
}

static void
gst_toyunda_src_unlink (GstPad *pad)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "unlink");


  gst_object_unref (toyunda);
}

static GstFlowReturn
gst_toyunda_src_getrange (GstPad *pad, guint64 offset, guint length,
    GstBuffer **buffer)
{
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "getrange");


  gst_object_unref (toyunda);
  return GST_FLOW_OK;
}

static gboolean
gst_toyunda_src_event (GstPad *pad, GstEvent *event)
{
  gboolean res;
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "event");

  switch (GST_EVENT_TYPE (event)) {
    default:
      res = gst_pad_push_event(toyunda->sinkpad, event);
      break;
  }

  gst_object_unref (toyunda);
  return res;
}

static gboolean
gst_toyunda_src_query (GstPad *pad, GstQuery *query)
{
  gboolean res;
  GstToyunda *toyunda;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "query");

  switch (GST_QUERY_TYPE(query)) {
    default:
      res = gst_pad_query_default (pad, query);
      break;
  }

  gst_object_unref (toyunda);
  return res;
}

static GstIterator *
gst_toyunda_src_iterintlink (GstPad *pad)
{
  GstToyunda *toyunda;
  GstIterator *iter;

  toyunda = GST_TOYUNDA (gst_pad_get_parent (pad));

  GST_DEBUG_OBJECT(toyunda, "iterintlink");

  iter = gst_pad_iterate_internal_links_default (pad);

  gst_object_unref (toyunda);
  return iter;
}


static void	gst_toyunda_cleanup_subtitles_seq(GSequence *seq)
{
	/* TODO */
}


static gboolean
plugin_init (GstPlugin * plugin)
{

  return gst_element_register (plugin, "toyunda", GST_RANK_NONE,
      GST_TYPE_TOYUNDA);
}

#ifndef VERSION
#define VERSION "0.0.FIXME"
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
    "toyunda",
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

