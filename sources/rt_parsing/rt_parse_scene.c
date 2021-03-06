/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_parse_scene.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahorker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by ahorker           #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by ahorker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "rt_parsing.h"

void	parse_json_file(json_t *root, t_tmp *tmp, uint32_t *renderer_flags)
{
	void		*iter;
	const char	*key;
	json_t		*value;

	iter = json_object_iter(root);
	while (iter)
	{
		key = json_object_iter_key(iter);
		value = json_object_iter_value(iter);
		if (json_is_object(value))
			parse_object(tmp, key, value, renderer_flags);
		else if (json_is_array(value))
			parse_array(tmp, key, value, renderer_flags);
		else if (json_is_number(value))
			parse_variable(tmp, key, value);
		else if (json_is_string(value))
			parse_string(tmp, key, value, renderer_flags);
		else if (json_is_boolean(value))
			parse_boolean(tmp, key, value, renderer_flags);
		else
			rt_raise_error(ft_strjoin(ERR_PARS_WRONG_PRM, key));
		while (tmp->next != NULL)
			tmp = tmp->next;
		iter = json_object_iter_next(root, iter);
	}
}

t_scene	rt_parse_scene(const char *json_scene_file, uint32_t *renderer_flags)
{
	t_scene			scene;
	char			*text;
	t_tmp			*tmp;
	json_t			*root;
	json_error_t	error;

	*renderer_flags = RENDER_OBJECTS;
	tmp = rt_safe_malloc(sizeof(t_tmp));
	if (!(text = ft_readfile(open(json_scene_file, O_RDONLY), NULL, 10000)))
		rt_raise_error(ERR_INVALID_JSON_FILE);
	init_tmp(tmp);
	init_textures_default();
	root = json_loads(text, 0, &error);
	parse_json_file(root, tmp, renderer_flags);
	count_elements(&scene, tmp);
	add_elements(&scene, tmp);
	json_decref(root);
	free(text);
	return (scene);
}
