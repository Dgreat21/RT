#include "test_header.h"

t_opencl	g_opencl;
t_sdl		g_sdl;
int			*g_img_data;
t_textures  g_textures;
cl_float3	g_img_data_float[WIN_WIDTH * WIN_HEIGHT];

int g_max_height;

int		main(int argc, char **argv)
{
	RT_UNUSED(argc);
	RT_UNUSED(argv);
	rt_sdl_init();
	test_kd_2d_tree_main();
//	test_build_kd_tree();
	return 0;
}
