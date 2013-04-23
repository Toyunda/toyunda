/* GStreamer
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

static void	gst_toyunda_cleanup_subtitles_seq(GSequence *seq);

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
	fclose(file_id);
	toyunda->subfile_parsed = TRUE;
	g_sequence_sort(toyunda->subtitles, (GCompareDataFunc) toyunda_subtitle_compare, NULL);
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
	new_sub->position2x = -1;
	new_sub->position2y = -1;
	new_sub->image = g_new(char, strlen(STR_TOYUNDA_LOGO_NONE) + 1);
	strcpy(new_sub->image, STR_TOYUNDA_LOGO_NONE);
	new_sub->size = -1;
	new_sub->size2 = -1;

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
		return;
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
	if (line[strpos] == '{')
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
	if (line[strpos] == '{')
		parse_toyunda_options(line, &strpos, &new_sub);
	/* The content itself*/
	strtmppos = 0;
	strtmp[0] = '\0';
	while (line[strpos] != '\n' && line[strpos] != '\0' && line[strpos] != '|' && line[strpos] != '\r')
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
	if (new_sub->positiony == -1)
		new_sub->positiony = ((float)1/(float)12) * (pipecpt);
	else
		new_sub->positiony += ((float)1/(float)12) * (pipecpt);
	if (new_sub->position2y != -1)
		new_sub->position2y += ((float)1/(float)12) * (pipecpt);
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
	int	startpos = pos;
	char	strtmp[32];
	char	buff[255];
	int	tmp;
	int	cpt;

	/* c = color, o = pos, s = size */
	if (str[pos] == 'c' || str[pos] == 'C')
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
					g_printf("Expecting valid color2 ($AABBGGRR)\n");
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
				(*sub)->positionx = (float) ((gint) g_ascii_strtod(strtmp, NULL)) / 800;
				pos += strlen(strtmp);
				if (str[pos] == ',')
				{
					pos++;
					if (parse_digit(str, pos, strtmp) == FALSE)
					{
						g_printf("Expecting position Y value\n");
						return 0;
					}
					(*sub)->positiony = (float)((gint) g_ascii_strtod(strtmp, NULL)) / 600;
					pos += strlen(strtmp);
				}
				if (str[pos] == ':')
				{
					pos++;
					if (parse_digit(str, pos, strtmp) == FALSE)
					{
						g_printf("Expecting position X2 value\n");
						return 0;
					}
					(*sub)->position2x = (float) ((gint) g_ascii_strtod(strtmp, NULL)) / 800;
					pos += strlen(strtmp);
					if (str[pos] == ',')
					{
						pos++;
							if (parse_digit(str, pos, strtmp) == FALSE)
						{
							g_printf("Expecting position Y2 value\n");
							return 0;
						}
						(*sub)->position2y = (float)((gint) g_ascii_strtod(strtmp, NULL)) / 600;
						pos += strlen(strtmp);
					}
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
			else
				if (str[pos] == 's')
				{
					if (str[pos + 1] == ':')
					{
						pos += 2;
						if (parse_digit(str, pos, strtmp) == FALSE)
						{
							g_printf("Expecting size value\n");
							return 0;
						}
						(*sub)->size = g_ascii_strtod(strtmp, NULL);
						pos += strlen(strtmp);
						if (str[pos] == ':')
						{
							pos++;
							if (parse_digit(str, pos, strtmp) == FALSE)
							{
								g_printf("Expecting second size value\n");
								return 0;
							}
							(*sub)->size2 = g_ascii_strtod(strtmp, NULL);
							pos += strlen(strtmp);
						}
					}
					else
					{
						g_printf("Expecting option value\n");
						return 0;
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
	int	dol = 0;

	if (str[pos] == '$')
	{
		pos++;
		dol = 1;
	}
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
	return (cpos + dol);
}

static gboolean	parse_digit(char* str, int pos, char* toret)
{
	int	toretpos = 0;

	if (str[pos] == '-')
	{
		toret[toretpos] = str[pos];
		pos++;
		toretpos++;
	}
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
	g_printf("Text : %s$\n", toysub.text);
	g_printf("Image : %s\n", toysub.image);
	g_printf("Size : %d\n", toysub.size);
}


/* */
static void	read_line(FILE *file_stream, char** linetoret, int *size)
{
		size_t p = 0;
		*linetoret = NULL;
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
	dest->position2x = src->position2x;
	dest->position2y = src->position2y;
	dest->start = src->start;
	dest->stop = src->stop;
	dest->size = src->size;
	dest->size2 = src->size2;
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
	{
		if (t1->stop < t2->stop)
			return -1;
		else
		{
			if (t1->positiony < t2->positiony)
				return -1;
			else
				return 1;
		}
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

