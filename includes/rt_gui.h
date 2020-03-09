
#ifndef RT_GUI_H
#define RT_GUI_H

# include <SDL.h>
# include <SDL_ttf.h>

# include <stdbool.h>

# include "rt_gui_defines.h"
# include "rt_gui_errors.h"
# include "rt_gui_structs.h"



typedef struct s_rt t_rt;
typedef struct s_sdl t_sdl;

void		init_gui(uint64_t algo);
bool		rt_handle_event_gui(SDL_Event *event, t_rt *rt);

/*
** fake HTML
*/

SDL_Rect	centered_label( SDL_Rect button, SDL_Surface *sur);
void		cut_rect(SDL_Rect *rect, int px);
void		render_border(t_transform *btn, int px, SDL_Color color);

/*
** Buttons init
*/

void		init_algo_buttons(void);
void		init_other_buttons(void);

/*
** Buttons render
*/

void		render_button(t_transform btn);
void		render_button_with_params(t_transform btn, TTF_Font *font, int px);
void		auto_render_button(int i);

/*
** Buttons utils
*/

bool		check_click(SDL_Event *event, SDL_Rect button);
bool		check_hover(SDL_Event *event, SDL_Rect button);
void		render_all_buttons(void);

/*
** Buttons actions
*/

void		change_render_algo(short algo, t_rt *rt);
void		create_screenshot_bmp(void);
void		create_screenshot_png(void);
void		create_screenshot_jpg(void);
bool		button_callback(t_transform *btn, SDL_Event *event, t_rt *rt);

/*
** Utils
*/

char 		*name_generator(char *ext);
SDL_Rect	get_window_data(void);
SDL_Color	get_color_from_hex(unsigned int hex);
SDL_Color	get_rgba_from_hex(unsigned int hex);

#endif //RT_GUI_H
