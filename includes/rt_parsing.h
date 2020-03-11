#ifndef RT_PARSING_H
# define RT_PARSING_H

void		rt_load_obj_file(const char *path_to_obj, t_meshes *out_meshes);
t_scene 	rt_parse_scene(const char *json_scene_file, uint32_t *renderer_flags);
void		parse_json_file(json_t *root, t_tmp *tmp, uint32_t *renderer_flags);
void		count_elements(t_scene *scene, t_tmp *tmp);
void		parse_object(t_tmp *tmp, const char *key, json_t *value, uint32_t *renderer_flags);
void		parse_array(t_tmp *tmp, const char *key, json_t *value, uint32_t *renderer_flags);
void		parse_variable(t_tmp *tmp, const char *key, json_t *value);
void 		parse_string(t_tmp *tmp, const char *key, json_t *value, uint32_t *renderer_flags);
int 		parse_texture(const char *name);
void 		parse_bool(t_tmp *tmp, const char *key, json_t *value, uint32_t *renderer_flags);
void 		init_textures_default(void);
void		init_tmp(t_tmp *tmp);
void		free_tmp(t_tmp *tmp);
int			ft_type_of_structure(const char *param, int structure_type);
int 		ft_type_of_array(int *type_of_element, const char *param, int structure_type);
void 		add_elements(t_scene *scene, t_tmp *tmp);
void 		check_duplicated(bool *checker, int number);
int			ft_add_texture_name_back(t_name **list, const char *data);
void 		add_directory(json_t *value);
void		rt_correct_scene(t_scene *scene);
void		check_object(t_tmp *tmp);

#endif
