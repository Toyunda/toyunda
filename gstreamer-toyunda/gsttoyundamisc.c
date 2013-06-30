/* GStreamer Toyunda
 * Copyright (C) 2013 Sylvain Colinet <scolinet@gmail.com>
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

#include "gsttoyunda.h"
#include "stb_image.c"

static GstBuffer*	gst_toyunda_get_image_data(GstToyunda *toyunda, gchar *image_file, uint *width, uint *height);

static void	gst_toyunda_select_subtitle(GstToyunda* toyunda, int framenb);
static void	gst_toyunda_cleanup_current_subtitles(GSequence *seq);
static void	gst_toyunda_cleanup_all_subtitles(GSequence *seq);

static void	gst_toyunda_reset_subtitle_buffer_statut(GstToyunda *toyunda);

static	void	gst_toyunda_adjust_default_font_size(GstToyunda *toyunda);
static	gchar*	gst_toyunda_get_image_path(GstToyunda* toyunda, gchar* image);
static	void	gst_toyunda_field_init(GstToyunda *toyunda);
gboolean	gst_toyunda_parse_toyunda_subtitle(GstToyunda* toyunda);


static void gst_toyunda_field_init(GstToyunda *toyunda)
{
	toyunda->subfile_parsed = FALSE;
	toyunda->subtitle_changed = FALSE;
	toyunda->current_sub_it = NULL;
	toyunda->subtitles = NULL;
	toyunda->current_subtitles = NULL;
	toyunda->hardware_surface = FALSE;
	toyunda->toyunda_logo = g_new(char, strlen(STR_TOYUNDA_LOGO_DEFAULT) + 1);
	toyunda->images_base_path = NULL;
	strcpy(toyunda->toyunda_logo, STR_TOYUNDA_LOGO_DEFAULT);
	toyunda->font_desc = STR_TOYUNDA_FONT_DESCRIPTION;
	toyunda->sub_enabled = TRUE;
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
	GSequenceIter* it_next;

	if (toyunda->subfile_parsed == FALSE)
		gst_toyunda_parse_toyunda_subtitle(toyunda);
	if (toyunda->current_subtitles == NULL)
	{
		toyunda->current_subtitles = g_sequence_new(NULL);
		//g_printf("creating new sequence\n");
	}
	if (toyunda->current_sub_it == NULL)
		toyunda->current_sub_it = g_sequence_get_begin_iter(toyunda->subtitles);
	
	/* Add new subtitle to current according to framenb */
	it = toyunda->current_sub_it;
	it_end = g_sequence_get_end_iter(toyunda->subtitles);
	while (it != it_end)
	{
		sub = (toyunda_sub_t*) g_sequence_get(it);
		if (sub->start == framenb || sub->start < framenb && sub->stop >= framenb)
		{
			//g_printf("ADD SUBTITLE : ");
			//print_toyunda_sub_t(*sub);
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
	//g_printf("==Current subtitle lenght : %d\n", g_sequence_get_length(toyunda->current_subtitles));
	it = g_sequence_get_begin_iter(toyunda->current_subtitles);
	it_end = g_sequence_get_end_iter(toyunda->current_subtitles);

	while (it != it_end)
	{
		it_next = g_sequence_iter_next(it);
		sub_buff = (toyunda_sub_and_buff_t*) g_sequence_get(it);
		sub = sub_buff->subtitle;
		if ((sub->stop <= framenb))
		{
			//g_printf("REMOVE SUBTITLE : ");
			//print_toyunda_sub_t(*sub);
			g_sequence_remove(it);
			if (sub_buff->overlay_rect != NULL)
				gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
			g_free(sub_buff);
			it = it_next;
			toyunda->subtitle_changed = TRUE;
			continue;
		} else
		{
#define SET_FADING_VAR(var, rapp, var1, var2) var = var1 +  rapp * (var2 - var1);
			if (is_valid_rgba_color_t(sub->color2) == TRUE)
			{
				float rap = (float) (framenb - sub->start) / (float) (sub->stop - sub->start);
			
				SET_FADING_VAR(sub->tmpcolor.alpha, rap, sub->color1.alpha, sub->color2.alpha)
				SET_FADING_VAR(sub->tmpcolor.red, rap, sub->color1.red, sub->color2.red)
				SET_FADING_VAR(sub->tmpcolor.blue, rap, sub->color1.blue, sub->color2.blue)
				SET_FADING_VAR(sub->tmpcolor.green, rap, sub->color1.green, sub->color2.green)
				toyunda->subtitle_changed = TRUE;
				sub_buff->to_change = TRUE;
			}
			if (sub->size2 != -1)
			{
				float rap = (float) (framenb - sub->start) / (float) (sub->stop - sub->start);
				SET_FADING_VAR(sub->fadingsize, rap, sub->size, sub->size2)
				toyunda->subtitle_changed = TRUE;
				sub_buff->to_change = TRUE;
			}
			if (sub->position2x != -1)
			{
				float rap = (float) (framenb - sub->start) / (float) (sub->stop - sub->start);
				SET_FADING_VAR(sub->fadingpositionx, rap, sub->positionx, sub->position2x)
				SET_FADING_VAR(sub->fadingpositiony, rap, sub->positiony, sub->position2y)
				toyunda->subtitle_changed = TRUE;
				sub_buff->to_change = TRUE;
			}
		}
		it = it_next;
	}
		
}



static GstBuffer*	gst_toyunda_get_image_data(GstToyunda *toyunda, gchar *image_file, uint *width, uint *height)
{
	toyunda_image_t*	img;
	GSequenceIter*		it, *it_end;
	int x,y,n;
	unsigned char *data;
	guint cpt;
	char a, r, g, b;
	
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
	
	data = stbi_load(image_file, &x, &y, &n, 4);
	if (data == NULL)
	{
		g_printf("Can't open file %s : %s\n", image_file, stbi_failure_reason());
		return NULL;
	}
	g_printf("Adding NNEW image buffer for image : %s - widht : %d, height :%d\n", image_file, x, y);
	img = g_new(toyunda_image_t, 1);
	img->path = g_new(gchar, strlen(image_file) + 1);
	strcpy(img->path, image_file);
	img->width = x;
	img->height = y;
	*width = x;
	*height = y;
	/* the output format is RGBA, need bgra */
	for (cpt = 0; cpt < (uint)( x * y); cpt++)
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


void gst_toyunda_adjust_default_font_size(GstToyunda* toyunda)
{
	guint	new_size;

	new_size = (double)(((double) toyunda->video_height / INT_TOYUNDA_BASE_HEIGHT) * INT_TOYUNDA_FONT_SIZE) + 0.5;
	pango_font_description_set_size(toyunda->pango_fontdesc, new_size * PANGO_SCALE);
	pango_layout_set_font_description(toyunda->pango_layout, toyunda->pango_fontdesc);
	g_printf("New size : %d -- Font desct : %s\n", new_size, pango_font_description_to_string(toyunda->pango_fontdesc));
}


static	gchar*	gst_toyunda_get_image_path(GstToyunda* toyunda, gchar* image)
{
	gchar	*toret;
	uint 	sizeM;

	if (strcmp(image, STR_TOYUNDA_LOGO_DEFAULT) == 0)
		return toyunda->toyunda_logo;
	else
	{
		if (toyunda->images_base_path == NULL)
		{
			g_printf("No image base path set, trying to fallback to .");
			sizeM = strlen("./") + 1 + strlen(image) + 1;
		} else
			sizeM = strlen(toyunda->images_base_path) + 1 + strlen(image) + 1;
		toret = g_new(char, sizeM);
		toret[0] = '\0';
		if (toyunda->images_base_path != NULL)
			strcpy(toret, toyunda->images_base_path);
		else
			strcpy(toret, "./");
		strcat(toret, "/");
		strcat(toret, image);
		return toret;
	}
}


static void	gst_toyunda_cleanup_current_subtitles(GSequence *seq)
{
	GSequenceIter* it = g_sequence_get_begin_iter(seq);
	GSequenceIter* it_end = g_sequence_get_end_iter(seq);
	toyunda_sub_and_buff_t* sub_buff;
	
	while (it != it_end)
	{
		sub_buff = g_sequence_get(it);
		if (sub_buff->overlay_rect != NULL)
			gst_video_overlay_rectangle_unref(sub_buff->overlay_rect);
		g_free(sub_buff);
		it = g_sequence_iter_next(it);
	}
}

static void	gst_toyunda_cleanup_all_subtitles(GSequence *seq)
{
	GSequenceIter* it = g_sequence_get_begin_iter(seq);
	GSequenceIter* it_end = g_sequence_get_end_iter(seq);
	toyunda_sub_t*	sub;
	while (it != it_end)
	{
		sub = g_sequence_get(it);
		if (sub->image != NULL)
			g_free(sub->image);
		g_free(sub);
		it = g_sequence_iter_next(it);
	}
}
