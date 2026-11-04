/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by ahorker           #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_gui.h"
#include "rt_window_params.h"

/*
**	WIN_WIDTH x WIN_HEIGHT (+ WIN_GUI_WIDTH) is larger than most laptop
**	screens (e.g. MacBooks are 1440..1728 points wide), which put the tools
**	window off-screen. The render resolution stays WIN_WIDTH x WIN_HEIGHT,
**	only the windows are shrunk (keeping the aspect ratio) to fit the display.
*/

static void	rt_fit_windows_to_display(SDL_Rect *main_win, SDL_Rect *gui_win)
{
	SDL_Rect	usable;
	float		scale;
	int			avail_h;

	*main_win = (SDL_Rect){.x = 0, .y = SDL_WINDOWPOS_CENTERED,
		.w = WIN_WIDTH * SCALE_WIDTH, .h = WIN_HEIGHT * SCALE_HEIGHT};
	*gui_win = (SDL_Rect){.x = main_win->w, .y = SDL_WINDOWPOS_CENTERED,
		.w = WIN_GUI_WIDTH * SCALE_WIDTH, .h = WIN_HEIGHT * SCALE_HEIGHT};
	if (SDL_GetDisplayUsableBounds(0, &usable) != 0)
		return ;
	avail_h = usable.h - WIN_TITLE_BAR_HEIGHT;
	scale = fminf((float)(usable.w - gui_win->w) / main_win->w,
			(float)avail_h / main_win->h);
	if (scale > 0.f && scale < 1.f)
	{
		main_win->w = (int)(main_win->w * scale);
		main_win->h = (int)(main_win->h * scale);
	}
	gui_win->h = gui_win->h < avail_h ? gui_win->h : avail_h;
	main_win->x = usable.x;
	gui_win->x = usable.x + main_win->w;
	main_win->y = usable.y + WIN_TITLE_BAR_HEIGHT;
	gui_win->y = main_win->y;
}

void		rt_sdl_init(void)
{
	SDL_Rect	main_win;
	SDL_Rect	gui_win;

	if (SDL_Init(SDL_INIT_EVERYTHING))
		rt_raise_error(ERR_SDL_INIT);
	rt_fit_windows_to_display(&main_win, &gui_win);
	if (!(g_sdl.win = SDL_CreateWindow(WIN_TITLE,
			main_win.x, main_win.y, main_win.w, main_win.h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)))
		rt_raise_error(ERR_SDL_WIN);
	if (!(g_gui.win_tool = SDL_CreateWindow(RT_GUI_NAME,
			gui_win.x, gui_win.y, gui_win.w, gui_win.h,
			SDL_WINDOW_OPENGL)))
		rt_raise_error(ERR_SDL_WIN);
	if (!(g_sdl.rend = SDL_CreateRenderer(g_sdl.win, -1,
			SDL_RENDERER_ACCELERATED)))
		rt_raise_error(ERR_SDL_RENDER);
	if (!(g_sdl.texture = SDL_CreateTexture(g_sdl.rend,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, WIN_WIDTH, WIN_HEIGHT)))
		rt_raise_error(ERR_SDL_TEXTURE_CREATE);
}
