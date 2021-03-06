void				closest_intersection(
		__global const t_scene *scene,
		__global const t_object *objects,
		__global const t_kd_info *kd_info,
		__global const t_kd_arr_tree *kd_tree,
		__global const int *kd_indices,
		__global const t_polygon *polygons,
		__global const float3 *vertices,
		__global const float3 *v_normals,
		t_ray *ray,
		t_rayhit *out_best_hit,
		int *out_closest_polygon_index,
		int *out_closest_obj_index)
{
	*out_closest_polygon_index = NOT_SET;
	*out_closest_obj_index = NOT_SET;
#ifdef RENDER_OBJECTS
		for (int i = 0; i < scene->obj_nbr; i++)
		{
			switch (objects[i].type)
			{
				case (SPHERE):
					if (objects[i].len == NOT_SET ?
							ray_sphere_intsect(ray, &objects[i], out_best_hit)
							: ray_sphere_intsect_cut(ray, &objects[i],
																out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (PLANE):
					if (ray_plane_intersect(ray, objects[i].center,
							objects[i].normal, out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (CONE):
					if (objects[i].len != NOT_SET ?
							ray_cone_intersect_cut(ray, &objects[i],
									out_best_hit)
							: ray_cone_intersect(ray, &objects[i],
									out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (CYLINDER):
					if (objects[i].len != NOT_SET ?
							ray_cylinder_intersect_cut(ray, &objects[i],
									out_best_hit)
							: ray_cylinder_intersect(ray, &objects[i],
									out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (TRIANGLE):
					if (ray_triangle_intersect_MT(ray, &objects[i],
																out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (PARABOLOID):
					if (objects[i].len != NOT_SET ?
							ray_paraboloid_intersect_cut(ray, &objects[i],
																out_best_hit)
							: ray_paraboloid_intersect(ray, &objects[i],
																out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (ELLIPSOID):
					if ((objects[i].len != NOT_SET) ?
							ray_ellipsoid_intersect_cut(ray, &objects[i],
																out_best_hit)
							: ray_ellipsoid_intersect(ray, &objects[i],
																out_best_hit))
						*out_closest_obj_index = i;
					break;
				case (AABB):
					if (ray_aabb_intersection(ray, &objects[i], out_best_hit))
						*out_closest_obj_index = i;
					break;
				default :
					{
						if (objects[i].raymarch_type != NOTHING)
						{
							if (objects[i].raymarch_type == SIMPLE
									|| objects[i].raymarch_type == UNION)
							{
								t_rayhit	c = *out_best_hit;
								if (ray_march(ray, &objects[i], &c))
								{
									*out_best_hit = c;
									*out_closest_obj_index = i;
									break ;
								}


								/**
								 *
								 * Проверка камеры внутри объекта
								 *
								 **/

								t_ray r_plus = (t_ray){.origin = ray->origin
										+ RAY_MARCH_MAX_DIST * ray->dir,
										.dir = -ray->dir,
										.energy = ray->energy};
								t_ray r_minus = (t_ray){.origin = ray->origin
										- RAY_MARCH_MAX_DIST * ray->dir,
										.dir = ray->dir,
										.energy = ray->energy};
								t_rayhit p = (t_rayhit){
										.distance = RAY_MARCH_MAX_DIST,
										.pos = ray->origin,
										.normal = -ray->dir};
								t_rayhit m = (t_rayhit){
										.distance = RAY_MARCH_MAX_DIST,
										.pos = ray->origin,
										.normal = r_plus.dir};
								if (!(ray_march(&r_plus, &objects[i], &p) &&
										ray_march(&r_minus, &objects[i], &m)))
									break ;
								out_best_hit->distance = RAY_MIN_EPSILON;
								out_best_hit->pos = ray->origin;
								out_best_hit->normal = -ray->dir;
								*out_closest_obj_index = i;
								break ;
							}
							/**
							 *
							 * работает только для выпуклых поверхностей,
							 * для тора может лагать
							 *
							**/
							else if (objects[i].raymarch_type
															== INTERSECTION
									&& objects[i].raymarch_index > i)
							{
								t_rayhit	a = *out_best_hit;
								t_rayhit	b = *out_best_hit;
								if (ray_march(ray, &objects[objects[i]
											.raymarch_index], &b)
										&& ray_march(ray, &objects[i], &a))
								{
									float	max_dist = (a.distance > b.distance
											? a.distance : b.distance);
									t_ray	r = (t_ray){
										.origin = ray->origin + max_dist
												* ray->dir,
										.dir = -ray->dir,
						   				.energy = ray->energy};
									t_rayhit	c = (t_rayhit){
										.distance = max_dist,
										.pos = ray->origin,
										.normal = ray->dir};
									if (!ray_march(&r, a.distance > b.distance
										? &objects[objects[i].raymarch_index]
										: &objects[i], &c) && (a.distance
												> b.distance ? a : b).distance
										< out_best_hit->distance)
									{
										*out_best_hit = a.distance > b.distance
												? a : b;
										*out_closest_obj_index = a.distance
												> b.distance ? i
												: objects[i].raymarch_index;
									}
								}
							}
							/**
							 *
							 * работает только для выпуклых поверхностей,
							 * для тора может лагать
							 *
							 **/
							else if (objects[i].raymarch_type == DIFFERENT)
							{
								{
									int a, b;

									if (objects[i].raymarch_index < 0)
										break;
									else
									{
										b = i;
										a = objects[i].raymarch_index;
									}
									t_rayhit	a_h = *out_best_hit;
									bool		a_hit = ray_march(ray,
											&objects[a], &a_h);
									if (!a_hit)
										break;
									t_rayhit	b_h = *out_best_hit;
									bool		b_hit = ray_march(ray,
											&objects[b], &b_h);

									if (((a_hit && !b_hit)
											|| a_h.distance < b_h.distance)
											&& out_best_hit->distance
											> a_h.distance)
									{
										*out_best_hit = a_h;
										*out_closest_obj_index = a;
										break;
									}
									t_ray		r = *ray;

									r.origin = a_h.pos - r.dir * RAY_MIN_EPSILON;
									r.dir = -r.dir;
									b_h.pos = ray->origin;
									if (ray_march(&r, &objects[b], &b_h) &&
											out_best_hit->distance
											> a_h.distance)
									{
										*out_best_hit = a_h;
										*out_closest_obj_index = a;
										break;
									}
									r.origin = ray->origin +
											(RAY_MARCH_MAX_DIST) * ray->dir;
									a_h.distance = RAY_MARCH_MAX_DIST;
									a_h.pos = ray->origin;
									b_h.distance = RAY_MARCH_MAX_DIST;
									b_h.pos = ray->origin;
									ray_march(&r, &objects[a], &a_h);
									ray_march(&r, &objects[b], &b_h);
									if (a_h.distance > b_h.distance)
										break;
									b_h.distance = RAY_MARCH_MAX_DIST
											- b_h.distance;
									b_h.normal = -b_h.normal;
									if (out_best_hit->distance > b_h.distance)
									{
										*out_best_hit = b_h;
										*out_closest_obj_index = b;
									}
								}
								break ;
							}
						}
					break;
					}
			}
		}
#endif
/**
 *
 * RENDER_OBJECTS
 *
 **/

#ifdef RENDER_MESH
*out_closest_polygon_index = kd_tree_traverse(kd_info, kd_tree, kd_indices, polygons, vertices, v_normals, ray, out_best_hit);
//	*out_closest_polygon_index = ray_mesh_intersect(&scene->meshes, polygons, vertices, v_normals, ray, out_best_hit);
	if (isset(*out_closest_polygon_index))
		*out_closest_obj_index = NOT_SET;
#endif
}
