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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef _GST_TOYUNDA_H_
#define _GST_TOYUNDA_H_

#include <gst/base/gstbasetransform.h>
#include <gst/gst.h>
#include <gst/video/video.h>
#include "toyundatype.h"
#include <gst/video/video-overlay-composition.h>
#include <pango/pangocairo.h>


G_BEGIN_DECLS

#define GST_TYPE_TOYUNDA   (gst_toyunda_get_type())
#define GST_TOYUNDA(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_TOYUNDA,GstToyunda))
#define GST_TOYUNDA_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_TOYUNDA,GstToyundaClass))
#define GST_IS_TOYUNDA(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_TOYUNDA))
#define GST_IS_TOYUNDA_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_TOYUNDA))

typedef struct _GstToyunda GstToyunda;
typedef struct _GstToyundaClass GstToyundaClass;


typedef struct _toyunda_sub_and_buff 	toyunda_sub_and_buff_t;
typedef struct _toyunda_image		toyunda_image_t;

struct _toyunda_sub_and_buff
{
	toyunda_sub_t			*subtitle;
	GstVideoOverlayRectangle	*overlay_rect;
	gboolean			to_change;
};

struct	_toyunda_image
{
	gchar*		path;
	GstBuffer*	buffer;
	guint		width;
	guint		height;
};


struct _GstToyunda
{
	GstBaseTransform base_toyunda;

	GstPad *sinkpad;
	GstPad *srcpad;
	gfloat	fps_d;
	gfloat	fps_n;

	guint		video_width;
	guint		video_height;
	guint		window_width;
	guint		window_height;
	GstVideoFormat	format;

	PangoContext*		pango_context;
	PangoLayout*		pango_layout;
	PangoFontDescription*	pango_fontdesc;

	GSequence*	subtitles;
	GSequence*	current_subtitles;
	GSequence*	images;

	GSequenceIter* current_sub_it;

	gchar*	subfile;
	gchar*	font_desc;
	gchar*	toyunda_logo;
	gchar*	images_base_path;

	gboolean	subfile_parsed;
	gboolean	subtitle_changed;
	gboolean	hardware_surface;
	gboolean	sub_enabled;
};

struct _GstToyundaClass
{
  GstBaseTransformClass base_toyunda_class;
};

GType gst_toyunda_get_type (void);

G_END_DECLS

#endif
