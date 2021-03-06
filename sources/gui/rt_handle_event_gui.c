/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_handle_event_gui.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by sleonard          #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_gui.h"

bool		handle_movement(t_scene scene)
{
	bool res;

	res = false;
	res |= is_positions_changed(c_pos_x, scene.camera.pos.x);
	res |= is_positions_changed(c_pos_y, scene.camera.pos.y);
	res |= is_positions_changed(c_pos_z, scene.camera.pos.z);
	res |= is_positions_changed(c_angle_x, scene.camera.rotation.x);
	res |= is_positions_changed(c_angle_y, scene.camera.rotation.y);
	res |= is_positions_changed(c_angle_z, scene.camera.rotation.z);
	return (res);
}

bool		rt_handle_event_gui(SDL_Event *event, t_rt *rt)
{
	bool	event_handled;
	int		i;
	t_btn	now;
	t_btn	panel;

	event_handled = false;
	i = 0;
	now = g_gui.render_algo;
	panel = g_gui.panel;
	while (i < btn_count)
	{
		if (g_gui.obj[i].callback(&g_gui.obj[i], event, rt))
		{
			auto_render_button(i);
			event_handled = true;
		}
		i++;
	}
	if (now != g_gui.render_algo || panel != g_gui.panel ||
		handle_movement(rt->scene))
		render_all_buttons(rt->scene);
	SDL_UpdateWindowSurface(g_gui.win_tool);
	return (event_handled);
}
