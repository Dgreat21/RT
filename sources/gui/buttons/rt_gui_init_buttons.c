/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_gui_init_buttons.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgreat <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by dgreat            #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by dgreat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_gui.h"
#include "rt_colors.h"
#include "rt_window_params.h"

void		init_algo_buttons(void)
{
	SDL_Rect	rect;
	short		algo;

	algo = g_gui.render_algo;
	rect = (SDL_Rect){.x = 0, .y = GUI_TITLE, .h = 100, .w = WIN_GUI_WIDTH / 2};
	g_gui.obj[pt_btn] = (t_transform){ .rect = rect, .state = non_event,
			.text = PATH_TRACE_LABEL, .type = RENDER_BTN, .action = pt_btn,
			.callback = button_callback, .color = get_color_from_hex(COL_RED)};
	rect.x += WIN_GUI_WIDTH / 2;
	g_gui.obj[rt_btn] = (t_transform){ .rect = rect, .state = non_event,
			.text = RAY_TRACE_LABEL, .type = RENDER_BTN, .action = rt_btn,
			.callback = button_callback, .color = get_color_from_hex(COL_BLUE)};
	if (algo == pt_btn)
		g_gui.obj[pt_btn].state = click;
	else if (algo == rt_btn)
		g_gui.obj[rt_btn].state = click;
}

/*
**	On a small screen the tools window is shorter than WIN_HEIGHT, so the
**	option rows shrink to fit together with the screenshot button below them.
*/

void		init_gui_layout(void)
{
	int			gui_height;

	SDL_GetWindowSize(g_gui.win_tool, NULL, &gui_height);
	g_gui.option_row_h = (gui_height - OPTIONS_BLOCK) / (OPTION_ROWS + 1);
	if (g_gui.option_row_h > BTN_BIG_SIZE)
		g_gui.option_row_h = BTN_BIG_SIZE;
}

void		init_other_buttons(void)
{
	SDL_Rect	rect;
	int			gui_height;
	int			options_end;

	SDL_GetWindowSize(g_gui.win_tool, NULL, &gui_height);
	options_end = OPTIONS_BLOCK + OPTION_ROWS * OPTION_ROW_H;
	rect = (SDL_Rect){.x = 0, .y = gui_height - GUI_FOOTER,
			.h = 100, .w = WIN_GUI_WIDTH};
	if (rect.y < options_end)
		rect.y = options_end;
	if (rect.h > gui_height - rect.y)
		rect.h = gui_height - rect.y;
	g_gui.obj[scr_sbtn] = (t_transform){ .rect = rect,
			.state = non_event, .text = SCREENSHOT_LABEL, .action = scr_sbtn,
			.callback = button_callback, .type = SCREENSHOT,
			.color = get_color_from_hex(COL_GREEN)};
}
