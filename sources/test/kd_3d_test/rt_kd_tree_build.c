#include "rt.h"
#include "rt_kd_tree.h"

void		rt_kd_unset_indices(int *indices)
{
	if (!indices)
		return;
	indices[0] = NOT_SET;
	indices[1] = NOT_SET;
	indices[2] = NOT_SET;
}

t_split		kd_split(t_aabb root_aabb, int axis, int split_num)
{
	t_split		split;

	split.s.min = root_aabb.bounds.min;
	split.s.min.s[axis] = ((root_aabb.bounds.min.s[axis] + root_aabb.bounds.min.s[axis]) / BUCKETS) * (float)split_num;
	split.s.max = root_aabb.bounds.max;
	split.s.max.s[axis] = split.s.min.s[axis]; // координата разделения та же, бокс выровнен по осям
	return (split);
}

bool		kd_is_obj_in_aabb(t_aabb root_aabb, t_aabb obj_aabb)
{
	if ((obj_aabb.bounds.min.x < root_aabb.bounds.max.x
	&& obj_aabb.bounds.min.y < root_aabb.bounds.max.y
	&& obj_aabb.bounds.min.z < root_aabb.bounds.max.z)
	|| (obj_aabb.bounds.max.x > root_aabb.bounds.min.x
	&& obj_aabb.bounds.max.y > root_aabb.bounds.min.y
	&&  obj_aabb.bounds.max.z > root_aabb.bounds.min.z))
		return true;
	return false;
}

int			kd_count_obj_in_aabb(t_aabb aabb, t_aabb *obj_aabbs, int num_aabbs, int out_indices[MAX_OBJ_IN_LEAF])
{
	int		obj_in_bounds;
	int		i;

	obj_in_bounds = 0;
	i = 0;
	rt_kd_unset_indices(out_indices);
	while (i < num_aabbs)
	{
		if (kd_is_obj_in_aabb(aabb, obj_aabbs[i]))
		{
			if (obj_in_bounds < MAX_OBJ_IN_LEAF && out_indices)
				out_indices[obj_in_bounds] = i; // тут i == индексу полигона
			obj_in_bounds++;
		}
		i++;
	}
	if (obj_in_bounds >= MAX_OBJ_IN_LEAF && out_indices)
		rt_kd_unset_indices(out_indices);
	return obj_in_bounds;
}

float		kd_get_aabb_area(t_aabb aabb)
{
	return (fabsf(aabb.bounds.min.x - aabb.bounds.max.x)
	* fabsf(aabb.bounds.min.y - aabb.bounds.max.y)
	* fabsf(aabb.bounds.min.z - aabb.bounds.max.z));
}

float		calc_sah(t_aabb *obj_aabbs, int num_aabbs, t_aabb left_bounds, t_aabb right_bounds)
{
	return EMPTY_COST // kd_2d_count_obj_in_aabb можно закешировать раньше (и не считать два раза)
	+ (float) kd_count_obj_in_aabb(left_bounds, obj_aabbs, num_aabbs, NULL) * kd_get_aabb_area(left_bounds)
	+ (float) kd_count_obj_in_aabb(right_bounds, obj_aabbs, num_aabbs, NULL) * kd_get_aabb_area(right_bounds);
}

float		kd_split_buckets_sah(t_aabb root_aabb, t_aabb *obj_aabbs, int num_aabbs, t_aabb *out_left_aabb, t_aabb *out_right_aabb)
{
	float	best_sah;
	int		axis;
	int		split_num;

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

			left_aabb.bounds.max = split.s.max;
			right_aabb.bounds.min = split.s.min;

			float	sah = calc_sah(obj_aabbs, num_aabbs, left_aabb, right_aabb);
			if (sah < best_sah)
			{
				best_sah = sah;
				*out_left_aabb = left_aabb;
				*out_right_aabb = right_aabb;
			}
			split_num++;
		}
		axis++;
	}
	return best_sah;
}

int			kd_find_matches(const int *left_indices, const int *right_indices)
{
	int		matches;
	int		i;
	int		j;

	i = 0;
	matches = 0;
	while(i < MAX_OBJ_IN_LEAF)
	{
		j = 0;
		while(j < MAX_OBJ_IN_LEAF)
		{
			if (left_indices[i] != NOT_SET && right_indices[i] != NOT_SET
				&& left_indices[i] == right_indices[j])
				matches++;
			j++;
		}
		i++;
	}
	return (matches);
}



void		build_kd_tree_recursive(t_kd_tree *tree, t_aabb *obj_aabbs, int num_aabbs, int level)
{
	tree->left = NULL;
	tree->right = NULL;

	if (level >= KD_MAX_HEIGHT || tree->obj_num <= MIN_OBJ_IN_LEAF)
		return;

	t_aabb	left_aabb;
	t_aabb	right_aabb;

	float	sah = kd_split_buckets_sah(tree->aabb, obj_aabbs, num_aabbs, &left_aabb, &right_aabb);
	if (sah / tree->sah > 0.8f)
		return;

	int		left_obj_indices[MAX_OBJ_IN_LEAF];
	int		right_obj_indices[MAX_OBJ_IN_LEAF];

	int		left_obj_num = kd_count_obj_in_aabb(tree->aabb, obj_aabbs, num_aabbs, left_obj_indices);
	int		right_obj_num = kd_count_obj_in_aabb(tree->aabb, obj_aabbs, num_aabbs, right_obj_indices);

	int		matches = kd_find_matches(left_obj_indices, right_obj_indices);

	if (((float) matches / (float) left_obj_num > 0.5f
	|| (float) matches / (float) right_obj_num > 0.5f))
		return;

	tree->left = rt_safe_malloc(sizeof(t_kd_tree));
	tree->left->obj_num = left_obj_num;
	tree->left->aabb = left_aabb;
	tree->left->sah = sah;
	ft_memcpy(tree->left->indices, left_obj_indices, sizeof(int) * MAX_OBJ_IN_LEAF);
	build_kd_tree_recursive(tree->left, obj_aabbs, num_aabbs, level + 1);

	tree->right = rt_safe_malloc(sizeof(t_kd_tree));
	tree->right->obj_num = right_obj_num;
	tree->right->aabb = right_aabb;
	tree->right->sah = sah;
	ft_memcpy(tree->right->indices, right_obj_indices, sizeof(int) * MAX_OBJ_IN_LEAF);
	build_kd_tree_recursive(tree->right, obj_aabbs, num_aabbs, level + 1);
}

t_kd_tree	*build_kd_tree(t_aabb *obj_aabbs, int num_aabbs)
{
	t_kd_tree	*root;

	root = rt_safe_malloc(sizeof(t_kd_tree));
	root->aabb = get_root_aabb(obj_aabbs, num_aabbs);
	root->sah = INFINITY;
	rt_kd_unset_indices(root->indices);
	root->obj_num = kd_count_obj_in_aabb(root->aabb, obj_aabbs, num_aabbs, root->indices);
	build_kd_tree_recursive(root, obj_aabbs, num_aabbs, 0);
	return (root);
}
