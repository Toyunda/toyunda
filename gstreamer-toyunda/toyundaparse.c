#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <glib/gprintf.h>
#include <string.h>


struct color_s {
	gint red;
	gint blue;
	gint green;
	gint alpha;
};

typedef struct color_s color_t;

struct toyunda_sub_s {
	guint	start;
	guint	stop;
	gchar*	text;
	color_t	color1;
	color_t	color2;
	color_t	tmpcolor;
	gfloat	positionx;
	gfloat	positiony;
	gint	size;
	gchar*	image;

};

GSequence*	toyunda_subtitle;
gchar*	toyunda_logo_path = "toyunda.tga\0";
gchar*	toyunda_logo_none = "none\0";

typedef struct toyunda_sub_s toyunda_sub_t;

void	parse_toyunda_line(char *line);
void	sub_parse(char* file);
void	set_color_t_default(color_t *);
void	print_toyunda_sub_t(toyunda_sub_t);
void	read_line(FILE *file_stream, char** linetoret, int *size);
gboolean	parse_digit(char* str, int pos, char* toret);
int	parse_toyunda_option(char* str, int pos, toyunda_sub_t **sub);
int	parse_toyunda_abgr_color(char *str, int pos, color_t *color);


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



int	main(int ac, char *ag[])
{
	sub_parse(ag[1]);
	toyunda_subtitle = g_sequence_new(NULL);
	return 0;
}

void	sub_parse(char* file)
{
	char	*line;
	int	linesize;
	FILE* 	file_id = g_fopen(file, "r");

	if (file_id == NULL)
	{
		g_printf("Can't open file : %s\n", file);
		return ;
	}

	while(1)
	{
		read_line(file_id, &line, &linesize);
		if (linesize == -1)
			break;
		if (linesize <= 5)
			continue;
		parse_toyunda_line(line);
		g_free(line);
	}
}

void	parse_toyunda_line(char *line)
{
	int	strpos = 0;
	char	*strtmp;
	int	strtmppos = 0;
	int	pipecpt = 0;

	toyunda_sub_t*	new_sub = g_new(toyunda_sub_t, 1);
	set_color_t_default(&(new_sub->color1));
	set_color_t_default(&(new_sub->color2));
	set_color_t_default(&(new_sub->tmpcolor));
	strtmp = g_new(char, 255);
	new_sub->positionx = -1;
	new_sub->positiony = -1;
	new_sub->image = g_new(char, strlen(toyunda_logo_none) + 1);
	strcpy(new_sub->image, toyunda_logo_none);
	new_sub->size = -1;

	while (line[strpos] == ' ')
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

	/* Option */
	while (line[strpos] == '{')
	{
		strpos++;
		if ((strtmppos = parse_toyunda_option(line, strpos, &new_sub)) == 0)
		{
			g_printf("Expecting Option\n");
			return ;
		}
		else
			strpos += strtmppos;
		if (line[strpos] != '}')
		{
			g_printf("Expecting end of Option '}'\n");
			return ;
		}
		strpos++;
	}
	/* Welcome to the pipe wtf */
	if (line[strpos] == '|')
	{
		pipecpt++;
		while(line[strpos] == '|')
		{
			strpos++;
			pipecpt++;
		}
	}
	/* Option can be before and after pipe */
	while (line[strpos] == '{')
	{
		strpos++;
		if ((strtmppos = parse_toyunda_option(line, strpos, &new_sub)) == 0)
		{
			g_printf("Expecting Option\n");
			return ;
		}
		else
			strpos += strtmppos;
		if (line[strpos] != '}')
		{
			g_printf("Expecting end of Option '}'\n");
			return ;
		}
		strpos++;
	}

	/* The content itself*/
	strtmppos = 0;
	strtmp[0] = '\0';
	while (line[strpos] != '\n')
	{
		strtmp[strtmppos] = line[strpos];
		if (line[strpos] == -1)
		{
			g_free(new_sub->image);
			new_sub->image = g_new(char, strlen(toyunda_logo_path) + 1);
			strcpy(new_sub->image, toyunda_logo_path);
		}
		strpos++;
		strtmppos++;
	}
	strtmp[strtmppos] = '\0';
	new_sub->text = g_new(char, strlen(strtmp) + 1);
	strcpy(new_sub->text, strtmp);
	/* Position calculation, pipe have incidence*/
	new_sub->positiony =  ((float)1/(float)12) * (pipecpt - 1);
	print_toyunda_sub_t(*new_sub);
	g_free(strtmp);
}

/* return the lenght of option string */
/* option are like this : {optidentifier:value(:value)} */

int	parse_toyunda_option(char* str, int pos, toyunda_sub_t **sub)
{
	int	toret;
	int	startpos = pos;
	char	strtmp[32];
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
		}
	return (pos - startpos);
}

int	parse_toyunda_abgr_color(char *str, int pos, color_t *color)
{
	char	colorstr[32];
	char	tmpstr[3];
	int	cpos = 0;
	int 	offset = 0; /* use for handling alpha, because the format is (a)bgr*/

	if (str[pos] == '$')
		pos++;
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

gboolean	parse_digit(char* str, int pos, char* toret)
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

void	print_toyunda_sub_t(toyunda_sub_t toysub)
{
	g_printf("Start : %d\n", toysub.start);
	g_printf("Stop  : %d\n", toysub.stop);
	g_printf("Color1 : r:%d, g:%d, b:%d, a:%d\n", toysub.color1.red, toysub.color1.green, toysub.color1.blue, toysub.color1.alpha);
	g_printf("Color2 : r:%d, g:%d, b:%d, a:%d\n", toysub.color2.red, toysub.color2.green, toysub.color2.blue, toysub.color2.alpha);
	g_printf("positionx : %d (Float : %f)\n", (int)(toysub.positionx * 800), toysub.positionx);
	g_printf("positiony : %d (Float : %f)\n", (int) (toysub.positiony * 600), toysub.positiony);
	g_printf("Text : %s\n", toysub.text);
	g_printf("Image : %s\n", toysub.image);
	g_printf("Size : %d\n", toysub.size);
}


/* */
void	read_line(FILE *file_stream, char** linetoret, int *size)
{
		*linetoret = NULL;
		size_t p = 0;
		*size = mygetline(linetoret, &p, file_stream);
}


void	set_color_t_default(color_t *toset)
{
	toset->alpha = -1;
	toset->blue = -1;
	toset->green = -1;
	toset->red = -1;
}

