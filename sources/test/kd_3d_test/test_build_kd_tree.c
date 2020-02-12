#include "test_header.h"
#include "rt_parsing.h"
#include "rt_kd_tree.h"

void	free_kd_tree(t_kd_tree *tree)
{
	if (!tree)
		return;
	free_kd_tree(tree->left);
	free_kd_tree(tree->right);
	free(tree->objects.indices);
	free(tree);
}

void	test_build_kd_tree(void)
{
	t_meshes		meshes;
	t_aabb			*obj_aabbs;
	t_kd_tree		*kd_tree;

	clock_t			start;
	clock_t			end;

	start = clock();
	rt_load_obj_files(&meshes);
	end = clock();
	printf(".obj loaded in [%f] sec.\n", (double)(end - start) / CLOCKS_PER_SEC);

	start = clock();
	obj_aabbs = rt_get_all_aabbs(&meshes);
	end = clock();
	printf("aabbs of polygons got in [%f] sec.\n", (double)(end - start) / CLOCKS_PER_SEC);

	start = clock();
	kd_tree = build_kd_tree(obj_aabbs, meshes.num_polygons);
	end = clock();
	printf("k-d tree built in [%f] sec.\n", (double)(end - start) / CLOCKS_PER_SEC);

//	print_kd_tree(kd_tree);

	export_aabbs(kd_tree);

	free_kd_tree(kd_tree);
}
