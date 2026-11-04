/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfoote <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by gfoote            #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by gfoote           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_opencl.h"
#include "rt_events.h"

/*
**	Only path tracing renders without events (it keeps accumulating samples),
**	so in the other modes wait for an event instead of spinning a CPU core.
*/

void		rt_loop(t_rt *rt)
{
	SDL_Event		events[MAX_EVENT_IN_LIST];
	int				event_num;
	bool			quit;

	event_num = 0;
	quit = false;
	while (!quit)
	{
		if (!rt_bit_isset(rt->render_options, RENDER_PATHTRACE))
			SDL_WaitEvent(NULL);
		while (event_num < MAX_EVENT_IN_LIST
			&& SDL_PollEvent(&events[event_num]))
		{
			quit |= events[event_num].type == SDL_QUIT
				|| (events[event_num].type == SDL_WINDOWEVENT
				&& events[event_num].window.event == SDL_WINDOWEVENT_CLOSE);
			event_num++;
		}
		if (event_num > 0)
		{
			handle_event(rt, events, event_num);
			event_num = 0;
		}
		if (rt_bit_isset(rt->render_options, RENDER_PATHTRACE))
			rt_render(rt, rt_opencl_render);
	}
}
