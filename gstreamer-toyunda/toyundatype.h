
#ifndef H_TOYUNDA_TYPE_H
#define H_TOYUNDA_TYPE_H

#include <glib.h>

struct s_rgba_color
{
	gint red;
	gint blue;
	gint green;
	gint alpha;
};

typedef struct s_rgba_color rgba_color_t;

struct s_toyunda_sub {
	guint	start;
	guint	stop;
	gchar*	text;
	rgba_color_t	color1;
	rgba_color_t	color2;
	rgba_color_t	tmpcolor;

	gfloat	positionx;
	gfloat	positiony;
	gfloat	position2x;
	gfloat	position2y;
	gfloat	fadingpositionx;
	gfloat	fadingpositiony;

	gint	size;
	gint	size2;
	gint fadingsize;

	gchar*	image;
};

typedef struct s_toyunda_sub toyunda_sub_t;

#define STR_TOYUNDA_LOGO_NONE "none"
#define STR_TOYUNDA_LOGO_DEFAULT "toyunda.tga"
#define STR_TOYUNDA_FONT_DESCRIPTION "Bitstream Vera Sans Mono Bold"
#define INT_TOYUNDA_FONT_SIZE 30
#define INT_TOYUNDA_BASE_HEIGHT 600
#define INT_TOYUNDA_BASE_WIDTH 800

#endif
