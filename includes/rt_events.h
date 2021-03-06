/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_events.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sleonard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 13:23:43 by sleonard          #+#    #+#             */
/*   Updated: 2020/06/12 13:23:43 by sleonard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_EVENTS_H
# define RT_EVENTS_H

# define MAX_EVENT_IN_LIST 30

typedef enum	e_events
{
	EVENT_NOTHING = 0x0,
	EVENT_W = (1 << 0),
	EVENT_A = (1 << 1),
	EVENT_S = (1 << 2),
	EVENT_D = (1 << 3),
	EVENT_INFO = (1 << 4),
	EVENT_SPACE = (1 << 5),
	EVENT_LSHIFT = (1 << 6),
}				t_events;

bool		rt_handle_mouse_event(SDL_Event *event, t_rt *rt);
bool		rt_handle_key_event(SDL_Event *event, t_rt *rt);
bool		rt_camera_move(t_camera *camera, uint32_t events);
void		rt_handle_keypress2(SDL_Event *event, t_rt *rt);
void		sync_rt_and_gui(t_scene scene,
							uint64_t options,
							uint64_t states);

#endif
