#include <gccore.h>

#include <list>
#include <fat.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "fontmap.h"
#include "pacman.h"

#ifndef GEKKO
 #include <stdbool.h>
#else
 #include <gccore.h>
 #include <fat.h>
#endif
#include <wiiuse/wpad.h>
#include <SDL/SDL.h>

#define WINDOW_HEIGHT	480
#define WINDOW_WIDTH	640

enum
{
  DIR_NONE,
  DIR_UP,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_DOWN
};

static int		g_choose_file = 1;
static int		g_playloop = 1;
static int		g_score = 0;
static int		g_time = 0;
static int		g_pause = 0;
static int		g_count = 0;
static t_list		g_map;
static t_list		g_file;
static t_str		g_fname("/maps");
static t_str		g_prompt("$>");
static unsigned int	g_pos_p[3] = {0, 0, DIR_NONE};
static unsigned int	g_pos_f[3] = {0, 0, DIR_NONE};
static char		g_last_char = ' ';

void			read_dir()
{
  DIR*			pdir;
  struct dirent*	pent;
  struct stat 		statbuf;

  pdir = opendir(g_fname.c_str());
  if (!pdir)
    exit(EXIT_FAILURE);
  while ((pent = readdir(pdir)) != NULL)
    {
      stat(pent->d_name,&statbuf);
      g_file.push_back(pent->d_name);
    }
  closedir(pdir);
}

void			read_file(const char* path)
{
  unsigned int		y;
  t_file		f(path);
  t_str			line;
  t_str			new_line;

  if (!f.is_open())
    return ;
  g_map.clear();
  y = 0;
  g_count = 0;
  while (getline(f, line))
    {
      new_line.clear();
      for (unsigned int x = 0; x < line.size(); x += 1)
	{
	  switch (line[x])
	    {
	    case '\r':
	    case '\n':
	      break;
	    case '0':
	      new_line += '.';
	      g_count += 1;
	      break;
	    case '1':
	      new_line += '+';
	      break;
	    case 'P':
	      g_pos_p[0] = x;
	      g_pos_p[1] = y;
	      g_pos_p[2] = DIR_NONE;
	      new_line += 'P';
	      break;
	    case 'F':
	      g_pos_f[0] = x;
	      g_pos_f[1] = y;
	      g_pos_f[2] = DIR_NONE;
	      g_last_char = ' ';
	      new_line += 'F';
	      break;
	    default:
	      new_line += line[x];
	    }
	}
      g_map.push_back(new_line);
      y += 1;
    }
}

void			move_player()
{
  int			x = g_pos_p[0];
  int			y = g_pos_p[1];
  int			d = g_pos_p[2];
  int			ylen = g_map.size();
  int			xlen = 0;

  if (y >= ylen || x >= (xlen = g_map[y].size()))
    return;
  switch (d)
    {
    case DIR_UP:
      y = (ylen - 1 + y) % ylen;
      break;
    case DIR_LEFT:
      x = (xlen - 1 + x) % xlen;
      break;
    case DIR_RIGHT:
      x = (x + 1 + xlen) % xlen;
      break;
    case DIR_DOWN:
      y = (y + 1 + ylen) % ylen;
      break;
    }
  switch (g_map[y][x])
    {
    case 'F':
      g_choose_file = 1;
      g_fname = "/maps";
      break;
    case '.':
      g_score += 10;
    case ' ':
      g_map[y][x] = 'P';
      g_map[g_pos_p[1]][g_pos_p[0]] = ' ';
      g_pos_p[0] = x;
      g_pos_p[1] = y;
      break;
    default:
      g_pos_p[2] = DIR_NONE;
    }
}

int			get_ghost_dir(int x, int y, int xlen, int ylen)
{
  std::vector<int>	l;

  switch (g_map[(ylen - 1 + y) % ylen][x])
    {
    case 'P':
      return (DIR_UP);
    case ' ':
    case '.':
      l.push_back(DIR_UP);
    }
  switch (g_map[y][(xlen - 1 + x) % xlen])
    {
    case 'P':
      return (DIR_LEFT);
    case ' ':
    case '.':
      l.push_back(DIR_LEFT);
    }
  switch (g_map[y][(x + 1 + xlen) % xlen])
    {
    case 'P':
      return (DIR_RIGHT);
    case ' ':
    case '.':
      l.push_back(DIR_RIGHT);
    }
  switch (g_map[(y + 1 + ylen) % ylen][x])
    {
    case 'P':
      return (DIR_DOWN);
    case ' ':
    case '.':
      l.push_back(DIR_DOWN);
    }
  if (l.size() == 0)
    l.push_back(DIR_NONE);
  return (l[rand() % l.size()]);
}

void			move_ghost()
{
  int			x = g_pos_f[0];
  int			y = g_pos_f[1];
  int			d = g_pos_f[2];
  int			ylen = g_map.size();
  int			xlen = 0;

  if (y >= ylen || x >= (xlen = g_map[y].size()))
    return;
  d = get_ghost_dir(x, y, xlen, ylen);
  switch (d)
    {
    case DIR_UP:
      y = (ylen - 1 + y) % ylen;
      break;
    case DIR_LEFT:
      x = (xlen - 1 + x) % xlen;
      break;
    case DIR_RIGHT:
      x = (x + 1 + xlen) % xlen;
      break;
    case DIR_DOWN:
      y = (y + 1 + ylen) % ylen;
      break;
    }
  switch (g_map[y][x])
    {
    case 'P':
      g_choose_file = 1;
      g_fname = "/maps";
      break;
    case '.':
    case ' ':
      g_map[g_pos_f[1]][g_pos_f[0]] = g_last_char;
      g_last_char = g_map[y][x];
      g_map[y][x] = 'F';
      g_pos_f[0] = x;
      g_pos_f[1] = y;
      break;
    default:
      g_pos_f[2] = DIR_NONE;
    }
}

void			handle_event(SDL_Surface* screen, int y)
{
  static int		i = 0;
  static int		idx = 0;
  static t_str		name("NULL");
  static const int	lim = 60;
  u32			pressed;

  WPAD_ScanPads();
  pressed = WPAD_ButtonsUp(WPAD_CHAN_0); 
  if (pressed & WPAD_BUTTON_HOME)
    {
      exit(EXIT_SUCCESS);
    }
  else if (g_choose_file && g_file.size() > 0)
    {
      if (pressed & WPAD_BUTTON_LEFT) 
	{
	  idx = (g_file.size() + idx - 1) % g_file.size();
	  name = g_file[idx];
	}
      else if (pressed & WPAD_BUTTON_RIGHT)
	{
	  idx = (g_file.size() + idx + 1) % g_file.size();
	  name = g_file[idx];
	}
      else if (strcmp(name.c_str(), "NULL") != 0 && (pressed & WPAD_BUTTON_A))
	{
	  g_fname += "/";
	  g_fname += name;
	  read_file(g_fname.c_str());
	  g_choose_file = 0;
	  g_time = 0;
	  g_score = 0;
	}
      if (i < (lim / 2))
	fontmap_printf(23, y, screen, "<%s>", name.c_str());
      else
	fontmap_printf(23, y, screen, " %s ", name.c_str());
      i = (i + 1) % lim;
    }
  else if (pressed & WPAD_BUTTON_MINUS)
    {
      g_choose_file = 1;
      g_fname = "/maps";
    }
  else if (pressed & WPAD_BUTTON_PLUS)
    {
      g_pause ^= 1;
    }
  else if (pressed & WPAD_BUTTON_UP)
    {
      g_pos_p[2] = DIR_UP;
    }
  else if (pressed & WPAD_BUTTON_LEFT)
    {
      g_pos_p[2] = DIR_LEFT;
    }
  else if (pressed & WPAD_BUTTON_RIGHT)
    {
      g_pos_p[2] = DIR_RIGHT;
    }
  else if (pressed & WPAD_BUTTON_DOWN)
    {
      g_pos_p[2] = DIR_DOWN;
    }
  else
    {
      fontmap_printf(23, y, screen,
		     "PLAYER <%02u;%02u;%u> + GHOST <%02u;%02u;%u>",
		     g_pos_p[0], g_pos_p[1], g_pos_p[2],
		     g_pos_f[0], g_pos_f[1], g_pos_f[2]);
    }
}

void			play(SDL_Surface* screen)
{
  int			x;
  int			y;

  x = 10;
  while (g_playloop == 1)
    {
      y = 23;
      SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 5, 5, 5));
      fontmap_printf(x, y, screen,
		     "304-PACMAN <PAYET-S> + 1UP: 0 + "
                     "SCORE: %03d + TIME: %d", g_score, g_time);
      y += FONTMAP_SIZE * 2;
      fontmap_print(x, y, screen, g_prompt.c_str());
      y += FONTMAP_SIZE * 2;
      handle_event(screen, y);
      y += FONTMAP_SIZE * 2;
      for (auto line : g_map)
	{
	  fontmap_print(x, y, screen, line.c_str());
	  y += FONTMAP_SIZE;
	}
      if (g_choose_file == 1)
        g_prompt = "$> Choose a map.";
      else if (g_count <= (g_score / 10))
        g_prompt = "$> Congratulation!";
      else if (g_pause == 0)
	{
	  g_prompt = "$> GO!";
	  g_time += 1;
	  if ((g_time % 100) == 0)
	    {
	      move_player();
	      move_ghost();
	    }
	}
      else if (g_pause == 1)
	{
	  g_prompt = "$> Game is paused.";
	}
      SDL_Flip(screen);
    }
}

int			main(int argc, char** argv)
{
  SDL_Surface*		screen = NULL;

  if (!fatInitDefault())
    {
      fprintf(stderr, "fatInitDefault failure: terminating\n");
      exit(EXIT_FAILURE);
    }
  read_dir();
  srand(time(NULL));
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  fontmap_init();
  WPAD_Init();
  SDL_WM_SetCaption("304pacman", NULL);
  atexit(SDL_Quit);
  SDL_ShowCursor(SDL_DISABLE);
  screen = SDL_SetVideoMode(640, 480, 8, SDL_HWSURFACE);
  if (!screen)
    {
      fprintf(stderr, "Unable to set video: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  play(screen);
  SDL_Quit();
  return (EXIT_SUCCESS);
}
