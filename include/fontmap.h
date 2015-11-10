#ifndef FONTMAP_H_
# define FONTMAP_H_

# include <stdlib.h>
# include <stdio.h>

# include <SDL/SDL.h>
# include <SDL/SDL_image.h>

# define FONTMAP_SIZE	7

void			fontmap_init();
void			fontmap_print(int x, int y, SDL_Surface* destination,
				      const char *text);
int			fontmap_printf(int x, int y, SDL_Surface* destination,
				       const char *format, ...);

#endif /* !FONTMAP_H_ */
