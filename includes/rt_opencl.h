#ifndef RT_OPENCL_H
# define RT_OPENCL_H

#include "rt_host_structs.h"

void				rt_opencl_prepare_memory(t_rt *rt, t_rt_renderer *renderer);
void				rt_opencl_init(void);
void				rt_opencl_render(t_rt *rt);
void				rt_opencl_move_host_mem_to_kernel(
		t_rt_renderer *renderer,
		int max_memobj_num, ...);
char				*get_opencl_kernel_code_text(size_t *out_size);
void				opencl_clean_memobjs(t_rt_renderer *renderer);
void				rt_opencl_setup_image_buffer(t_rt_renderer *renderer);
void				rt_opencl_handle_error(
		const char *rt_err_str,
		int opencl_err_code);

void				rt_opencl_create_kernel(
		const char *compile_options,
		cl_kernel *out_kernel,
		cl_program *out_program);
t_rt_renderer		*rt_get_renderer(uint32_t flags);

#endif