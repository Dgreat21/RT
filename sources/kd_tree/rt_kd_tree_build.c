#include "rt.h"
#include "rt_kd_tree.h"

t_split		kd_split(t_aabb root_aabb, int axis, int split_num)
{
	t_split		split;

	split.s.min = root_aabb.min;
	split.s.min.s[axis] = root_aabb.min.s[axis] + ((root_aabb.max.s[axis] - root_aabb.min.s[axis]) / BUCKETS) * (float)split_num;
	split.s.max = root_aabb.max;
	split.s.max.s[axis] = split.s.min.s[axis];
	return (split);
}

bool		kd_is_obj_in_aabb(t_aabb root_aabb, t_aabb obj_aabb)
{
	if ((obj_aabb.min.x <= root_aabb.max.x
	&& obj_aabb.min.y <= root_aabb.max.y
	&& obj_aabb.min.z <= root_aabb.max.z
	&& obj_aabb.max.x >= root_aabb.min.x
	&& obj_aabb.max.y >= root_aabb.min.y
	&& obj_aabb.max.z >= root_aabb.min.z)
	||
	(obj_aabb.max.x >= root_aabb.min.x
	&& obj_aabb.max.y >= root_aabb.min.y
	&& obj_aabb.max.z >= root_aabb.min.z
	&& obj_aabb.min.x <= root_aabb.max.x
	&& obj_aabb.min.y <= root_aabb.max.y
	&& obj_aabb.min.z <= root_aabb.max.z))
		return true;
	return false;
}

t_aabb_objects		kd_get_objects_in_aabb(t_aabb aabb, t_aabb *all_aabbs, t_aabb_objects *prev_objects)
{
	t_aabb_objects	aabb_objects;
	int				i;

	aabb_objects.num = 0;
	aabb_objects.indices = rt_safe_malloc(sizeof(int) * prev_objects->num);
	i = 0;
	while (i < prev_objects->num)
	{
		if (kd_is_obj_in_aabb(aabb, all_aabbs[prev_objects->indices[i]]))
		{
			aabb_objects.indices[aabb_objects.num] = prev_objects->indices[i];
			aabb_objects.num++;
		}
		i++;
	}
	return aabb_objects;
}

float		kd_get_aabb_area(t_aabb aabb)
{
	return ((aabb.max.x - aabb.min.x)
	* (aabb.max.y - aabb.min.y)
	* (aabb.max.z - aabb.min.z));
}

float		kd_split_buckets_sah(t_aabb root_aabb,
						   t_aabb_objects *root_objects,
						   t_aabb *all_aabbs,
						   t_aabb *out_left_aabb,
						   t_aabb *out_right_aabb,
						   t_aabb_objects *out_left_objects,
						   t_aabb_objects *out_right_objects,
						   float *out_split,
						   int *out_split_axis)
{
	float			best_sah;
	int				axis;
	int				split_num;
	t_aabb_objects	left_objects;
	t_aabb_objects	right_objects;

	axis = 0;
	best_sah = INFINITY;
	while (axis < AXIS_NUM)
	{
		split_num = 1;
		while (split_num < BUCKETS)
		{
			t_split split = kd_split(root_aabb, axis, split_num);
			t_aabb	left_aabb = root_aabb;
			t_aabb	right_aabb = root_aabb;

			left_aabb.max = split.s.max;
			right_aabb.min = split.s.min;

			left_objects = kd_get_objects_in_aabb(left_aabb, all_aabbs, root_objects);
			right_objects = kd_get_objects_in_aabb(right_aabb, all_aabbs, root_objects);

			float	sah = EMPTY_COST
					+ (float)left_objects.num * kd_get_aabb_area(left_aabb)
					+ (float)right_objects.num * kd_get_aabb_area(right_aabb);

			if (sah < best_sah)
			{
				if (best_sah != INFINITY)
				{
					free((*out_left_objects).indices);
					free((*out_right_objects).indices);
				}
				best_sah = sah;
				*out_left_aabb = left_aabb;
				*out_right_aabb = right_aabb;
				*out_left_objects = left_objects;
				*out_right_objects = right_objects;
				*out_split = split.s.min.s[axis]; // split.s.min.s[axis] == split.s.max.s[axis]
				*out_split_axis = axis;
			}
			else
			{
				free(left_objects.indices);
				free(right_objects.indices);
			}
			split_num++;
		}
		axis++;
	}
	return best_sah;
}

void		build_kd_tree_recursive(t_kd_tree *tree,
							 t_aabb *all_aabbs,
							 int level,
							 int *index)
{
	tree->left = NULL;
	tree->right = NULL;
	tree->split = NOT_SET;
	tree->split_axis = NOT_SET;
	tree->left_index = NOT_SET;
	tree->right_index = NOT_SET;

	if (level >= KD_TREE_MAX_HEIGHT)
		return;

	t_aabb	left_aabb;
	t_aabb	right_aabb;

	t_aabb_objects	left_objects;
	t_aabb_objects	right_objects;

	float			split;
	int				split_axis;

	float	sah = kd_split_buckets_sah(tree->aabb, &tree->objects,
			all_aabbs, &left_aabb, &right_aabb, &left_objects, &right_objects,
			&split, &split_axis);
	if (sah > tree->sah)
	{
		free(left_objects.indices);
		free(right_objects.indices);
		return;
	}

	tree->split = split;
	tree->split_axis = split_axis;
	tree->objects.num = NOT_SET;
	free(tree->objects.indices);
	tree->left_index = ++(*index);
	tree->right_index = ++(*index);

	tree->left = rt_safe_malloc(sizeof(t_kd_tree));
	tree->left->objects = left_objects;
	tree->left->aabb = left_aabb;
	tree->left->sah = kd_get_aabb_area(left_aabb) * (float)left_objects.num;
	build_kd_tree_recursive(tree->left, all_aabbs, level + 1, index);

	tree->right = rt_safe_malloc(sizeof(t_kd_tree));
	tree->right->objects = right_objects;
	tree->right->aabb = right_aabb;
	tree->right->sah = kd_get_aabb_area(right_aabb) * (float) right_objects.num;
	build_kd_tree_recursive(tree->right, all_aabbs, level + 1, index);
}

t_aabb_objects	get_root_aabb_objects(int num_aabbs)
{
	t_aabb_objects	aabb_objects;
	int		i;

	i = 0;
	aabb_objects.num = num_aabbs;
	aabb_objects.indices = rt_safe_malloc(sizeof(int) * num_aabbs);
	while (i < num_aabbs)
	{
		aabb_objects.indices[i] = i;
		i++;
	}
	return (aabb_objects);
}

t_kd_tree	*build_kd_tree(t_aabb *all_aabbs, int num_aabbs)
{
	t_kd_tree	*root;
	int			index = 0;

	root = rt_safe_malloc(sizeof(t_kd_tree));
	root->aabb = get_root_aabb(all_aabbs, num_aabbs);
	root->sah = INFINITY;
	root->objects = get_root_aabb_objects(num_aabbs);
	build_kd_tree_recursive(root, all_aabbs, 0, &index);
	return (root);
}