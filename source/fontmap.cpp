#include "fontmap.h"

static const char*	g_cfont_xpm[] =
{
  "301 7 2 1",
  " 	c #FF00FF",
  ".	c #FFFFFF",
  "  ...    ..    .....  ......   ... ......   .... ....... .....  .....   ...  ......   .... .....  ..............  .......   .. ......   ......   .. ..    ..   ....   .. ..... ......  ..... ......  ....   ........   ....   ....   ....   .. ..  .........  ....          ...            ...  ...          ",
  " .  ..  ...   ..   ..    ..   .... ..      ..    ..   ....   ....   .. .. .. ..   .. ..  ....  .. ..     ..      ..    ..   ..   ..       ....  ..  ..    ... ......  ....   ....   ....   ....   ....  ..    ..  ..   ....   ....   ..... ... ..  ..    ...  ....          ...           ...    ...  .......",
  "..   ..  ..       ...   ..   .. .. ...... ..         .. ..   ....   ....   ....   ....     ..   ....     ..     ..     ..   ..   ..       .... ..   ..    ........... ....   ....   ....   ....   ....        ..  ..   ....   .... . .. .....  ..  ..   ...   ....  ......  ...          ...      ... .......",
  "..   ..  ..     ....   .... ..  ..      ........    ..   .....  ........   ........ ..     ..   ........ ...... ..  ..........   ..       ......    ..    ................   ....   ....   ....  ... .....    ..  ..   ..... ..........  ...    ....   ...     ..   ......  ...    ...  ...        ..........",
  "..   ..  ..    ....       .........     ....   ..  ..   ..   ..     ...........   ....     ..   ....     ..     ..   ....   ..   ..  ..   .......   ..    .. . .... ......   ........ .. .........       ..   ..  ..   .. ..... ....... .....    ..   ...      ..           ...    ...   ...      ... .......",
  " ..  .   ..   ...    ..   ..    .. ..   ....   ..  ..   ..   ..    .. ..   ....   .. ..  ....  .. ..     ..      ..  ....   ..   ..  ..   .... ...  ..    ..   ....  .....   ....     ..  .. .. ... ..   ..   ..  ..   ..  ...  ... ...... ...   ..  ...                    ...           ...    ...  .......",
  "  ...  ...... ....... .....     ..  .....  .....   ..    .....  ....  ..   ........   .... .....  .........       .......   .. ...... ..... ..  ... ........   ....   .. ..... ..      .... ...  ... .....    ..   .....    .   ..   ....   ..   ..  .......   ..           ...            ...  ...          "
};

static char**		g_font_xpm = (char**)g_cfont_xpm;

static char		g_font_layout[] =
  {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', '!', '-', '|', '.',
    '<', '>', '+'
  };

static SDL_Surface*	g_surface_font = NULL;

void fontmap_init()
{
  g_surface_font = IMG_ReadXPMFromArray(g_font_xpm);
  SDL_SetColorKey(g_surface_font, SDL_SRCCOLORKEY,
		  SDL_MapRGB(g_surface_font->format, 255, 0, 255));
}

static size_t		find_layout(char character)
{
  unsigned int		i;

  if (character >= 'a' && character <= 'z')
    character -= 0x20;
  for(i = 0; i < sizeof(g_font_layout); i += 1)
    {
      if(character == g_font_layout[i])
	return (i);
    }
  return (-1);
}

void			fontmap_print(int x,int y, SDL_Surface* destination,
				      const char* text)
{
  int			show;
  SDL_Rect		rect_src;
  SDL_Rect		rect_dest;

  rect_dest.x = x;
  rect_dest.y = y;
  rect_dest.h = FONTMAP_SIZE;
  rect_dest.w = FONTMAP_SIZE;

  rect_src.x = 0;
  rect_src.y = 0;
  rect_src.h = FONTMAP_SIZE;
  rect_src.w = FONTMAP_SIZE;

  for (show = 0; text[show] != '\0'; show += 1)
    {
      int index = find_layout(text[show]);

      if (text[show] == '\n')
	{
	  rect_dest.y += FONTMAP_SIZE + 1;
	  rect_dest.x = x;
        }
      else if (text[show] == ' ' || index == -1)
	{
	  rect_dest.x += FONTMAP_SIZE + 1;
        }
      else
	{
	  rect_src.x = index * FONTMAP_SIZE;
	  SDL_BlitSurface(g_surface_font, &rect_src, destination, &rect_dest);
	  rect_dest.x += FONTMAP_SIZE + 1;
        }
    }
}

int			fontmap_printf(int x,int y, SDL_Surface* destination,
				       const char *format, ...)
{
  char			text[BUFSIZ];
  va_list		list;

  va_start(list,format);
  vsprintf(text,format,list);
  fontmap_print(x, y, destination, text);
  va_end(list);
  return (strlen(text));
}
