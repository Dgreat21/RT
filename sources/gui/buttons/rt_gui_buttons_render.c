/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_gui_buttons_render.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgreat <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by dgreat            #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by dgreat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_gui.h"
#include "rt.h"

void	render_button(t_transform btn)
{
	SDL_Color	color;
	t_ttf		label;

	if (btn.state == click)
		color = get_color_from_hex(BTN_COLOR_CLICK);
	else if (btn.state == hover)
		color = get_color_from_hex(BTN_COLOR_HOVER);
	else if (btn.state == non_event)
		color = get_color_from_hex(BTN_COLOR_NONACTIVE);
	else
		color = btn.color;
	render_border(&btn, DEFAULT_BORDER, get_color_from_hex(GUI_BG));
	label = get_centered_label(g_gui.subtitle, btn.text, btn.rect);
	render_rect(g_gui.surface, &(btn.rect), color);
	SDL_BlitSurface(label.surface, NULL, g_gui.surface, &(label.rect));
	SDL_FreeSurface(label.surface);
}

void	render_button_with_params(t_transform btn, TTF_Font *font, int px)
{
	SDL_Color	color;

	if (btn.state == click)
		color = get_color_from_hex(TEXT_BOX_UNFOCUSED);
	else if (btn.state == hover)
		color = get_color_from_hex(BTN_COLOR_HOVER);
	else if (btn.state == non_event)
		color = get_color_from_hex(BTN_COLOR_NONACTIVE);
	else
		color = btn.color;
	if (!px)
		render_border(&btn, px, get_color_from_hex(GUI_BG));
	render_rect(g_gui.surface, &(btn.rect), color);
	render_text(font, btn.text, btn.rect);
}

void	auto_render_button(int i)
{
	if (g_gui.obj[i].type & PANEL)
		render_button_with_params(g_gui.obj[i], g_gui.body, 0);
	else if ((g_gui.obj[i].type & TEXT_BOX)
	&& g_gui.obj[i].state != hidden)
		render_text_box(g_gui.obj[i]);
	else if (g_gui.obj[i].state != hidden)
		render_button(g_gui.obj[i]);
}

void	render_all_buttons(void)
{
	int			i;

	i = 0;
	while (i < btn_count)
	{
		if (g_gui.obj[i].callback == NULL)
		{
			rt_raise_error(BTN_TROUBLE);
		}
		fix_states(i);
		auto_render_button(i++);
	}
	SDL_UpdateWindowSurface(g_gui.win_tool);
}
