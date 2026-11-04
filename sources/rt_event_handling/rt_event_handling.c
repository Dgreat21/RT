/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_event_handling.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgreat <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by dgreat            #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by dgreat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_gui.h"
#include "rt_events.h"
#include "rt_opencl.h"
#include "rt_window_params.h"

/*
**	The tools window follows the render window, in one direction only.
**	Moving each window after the other one moved made a feedback loop on
**	macOS (SDL3 moves windows asynchronously and the system pushes the tools
**	window back on screen), so both windows kept jumping. The tools window is
**	also kept inside the display, so the system has no reason to move it.
*/

static void	rt_attach_gui_window(void)
{
	SDL_Rect	main_win;
	SDL_Rect	usable;
	int			gui_w;
	int			gui_x;

	SDL_GetWindowPosition(g_sdl.win, &main_win.x, &main_win.y);
	SDL_GetWindowSize(g_sdl.win, &main_win.w, &main_win.h);
	SDL_GetWindowSize(g_gui.win_tool, &gui_w, NULL);
	gui_x = main_win.x + main_win.w;
	if (SDL_GetDisplayUsableBounds(SDL_GetWindowDisplayIndex(g_sdl.win),
			&usable) == 0 && gui_x + gui_w > usable.x + usable.w)
		gui_x = usable.x + usable.w - gui_w;
	SDL_SetWindowPosition(g_gui.win_tool, gui_x, main_win.y);
}

bool		rt_handle_window_event(SDL_Event *event, t_rt *rt)
{
	SDL_Rect win;

	if (event->type != SDL_WINDOWEVENT)
		return (false);
	if (event->window.event == SDL_WINDOWEVENT_MOVED
		&& event->window.windowID == RT_WIN_ID)
		rt_attach_gui_window();
	if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED
		&& event->window.windowID == RT_WIN_ID)
	{
		rt_attach_gui_window();
		SDL_GetWindowSize(g_sdl.win, &win.w, &win.h);
		rt->scene.camera.viewport_width = (float)win.w / (float)win.h;
		rt->scene.camera.viewport_height = 1;
		return (true);
	}
	return (false);
}

void		rt_check_render_state(
		uint32_t *render_action,
		uint32_t *render_options,
		uint32_t render_state)
{
	(void)render_action;
	if (render_state & STATE_NO_MESH)
		rt_unset_bit(render_options, RENDER_MESH | RENDER_MESH_VTEXTURES
		| RENDER_BACKFACE_CULLING);
	if (render_state & STATE_NO_TEXTURES)
		rt_unset_bit(render_options, RENDER_TEXTURES);
	if (render_state & STATE_NO_SKYBOX)
		rt_unset_bit(render_options, RENDER_SKYBOX);
}

static bool	handle_all_events(t_rt *rt, SDL_Event *events, int events_num)
{
	bool	key_event_handled;
	bool	mouse_event_handled;
	bool	gui_event_handled;
	bool	window_event_handled;
	int		i;

	key_event_handled = false;
	mouse_event_handled = false;
	gui_event_handled = false;
	window_event_handled = false;
	i = 0;
	while (i < events_num)
	{
		key_event_handled |= rt_handle_key_event(&events[i], rt);
		mouse_event_handled |= rt_handle_mouse_event(&events[i], rt);
		gui_event_handled |= rt_handle_event_gui(&events[i], rt);
		window_event_handled |= rt_handle_window_event(&events[i], rt);
		i++;
	}
	return (key_event_handled | mouse_event_handled
	| gui_event_handled | window_event_handled);
}

void		handle_event(t_rt *rt, SDL_Event *events, int events_num)
{
	if (handle_all_events(rt, events, events_num) || rt->events)
	{
		rt_camera_move(&rt->scene.camera, rt->events);
		if (rt_camera_changed(&rt->scene.camera))
			rt->render_actions |= ACTION_CAMERA_CHANGED;
		rt_check_render_state(&rt->render_actions,
				&rt->render_options, rt->render_state);
		rt_render(rt, &rt_opencl_render);
	}
}
