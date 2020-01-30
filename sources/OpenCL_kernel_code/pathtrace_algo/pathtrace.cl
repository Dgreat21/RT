
float3		pathtrace(
		__global const t_scene *scene,
		__global const t_object *objects,
		__global const t_light *lights,
		__global const t_mesh_info *meshes_info,
		__global const t_polygon *polygons,
		__global const float3 *vertices,
		__global const float3 *v_normals,
		__global const t_opencl_params *params,
		t_ray ray,
		int depth,
		float *seed,
		float2 pixel)
{
	float3		result_color = (float3)(0);
	t_rayhit	hit = (t_rayhit){(float3)(0), INFINITY, (float3)(0)};
	int			closest_obj_index = NOT_SET;
	int			closest_polygon_index = NOT_SET;

	for (int i = 0; i < params->pathtrace_params.max_depth; ++i)
	{
		hit = (t_rayhit){(float3)(0), INFINITY, (float3)(0)};
		closest_intersection(scene, objects, polygons, vertices, v_normals,
				&ray, &hit, &closest_polygon_index, &closest_obj_index);
		result_color += ray.energy;

		float3	shade_color;
		if (closest_obj_index != NOT_SET)
			shade_color = shade_pathtrace(&ray, &hit, &objects[closest_obj_index].material, seed, pixel);
		else if (closest_polygon_index != NOT_SET)
			shade_color = shade_pathtrace(&ray, &hit, &meshes_info[polygons[closest_polygon_index].mesh_index].material, seed, pixel);
		else
			shade_color = shade_pathtrace(&ray, &hit, 0, seed, pixel);

		/// можно раскомментить, чтобы цвета светящихся объектов где emisson_power больше 1 были не белыми
//		if (i == 0 && round(fast_length(shade_color)) != 0)
//			shade_color /= objects[closest_obj_index].material.emission_power;
		result_color *= shade_color;
		if (!ray_has_energy(&ray))
			break;
	}
	return result_color;
}