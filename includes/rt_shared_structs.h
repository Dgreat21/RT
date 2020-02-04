#ifndef RT_SHARED_STRUCTS_H
# define RT_SHARED_STRUCTS_H

typedef struct			s_rgb
{
# ifndef FT_OPENCL___

	cl_uchar			b;
	cl_uchar			g;
	cl_uchar			r;
	cl_uchar			a;
# else

	unsigned char		b;
	unsigned char		g;
	unsigned char		r;
	unsigned char		a;
# endif

}						t_rgb;

typedef union			u_color
{
# ifndef FT_OPENCL___

	cl_int			value;
# else

	int				value;
# endif

	t_rgb			rgb;
}						t_color;

typedef struct			s_point
{
# ifndef FT_OPENCL___
	cl_int				x;
	cl_int				y;
	cl_int				z;
	t_color				color;
# else
	int					x;
	int					y;
	int					z;
	t_color				color;
# endif
}						t_point;

typedef struct			s_texture_info
{
# ifndef FT_OPENCL___

	cl_int				width;
	cl_int				height;
	cl_int				start;
	cl_int				bpp;
# else

	int					width;
	int					height;
	int 				start;
	int					bpp;
# endif
}						t_texture_info;

#define TEXTURE_NUM 1

# ifndef FT_OPENCL___
typedef struct			s_textures
{
	//cl_uchar			*texture_list;
	cl_float 			*texture_list;
	t_texture_info		texture_info[TEXTURE_NUM];
	size_t				texture_list_size;
	size_t				texture_info_size;
}						t_textures;
# endif

typedef struct			s_camera
{
# ifndef FT_OPENCL___

	cl_float			viewport_width;
	cl_float			viewport_height;
	cl_float			viewport_distance;
	cl_float3			pos;
	cl_float3			rotation;
# else

	float				viewport_width;
	float				viewport_height;
	float				viewport_distance;
	float3				pos;
	float3				rotation;
# endif

}						t_camera;

/*
** Objects
*/

typedef enum			e_object_type
{
	SPHERE = 1,
	PLANE,
	CYLINDER,
	CONE,
	TRIANGLE,
}						t_object_type;

typedef enum			e_light_type
{
	AMBIENT = 1,
	POINT,
	DIRECTIONAL,
	PARALLEL,
}						t_light_type;

typedef struct			s_light
{
# ifndef FT_OPENCL___

	t_light_type		type;
	cl_float			intensity;
	cl_float3			pos;
	cl_float3			dir;
	cl_float3			color;
# else

	t_light_type		type;
	float				intensity;
	float3				pos;
	float3				dir;
	float3				color;
# endif

}						t_light;

typedef struct			s_material
{
# ifndef FT_OPENCL___

	cl_float3			ambient;
	cl_float3			diffuse;
	cl_float3			specular;
	cl_float			phong_exp;
	cl_float			smoothness;
	cl_float			transmittance;
	cl_float			refraction;
	cl_float3			emission_color;
	cl_float			emission_power;
	cl_int				texture_number;
	cl_float3			texture_position;
# else

	float3				ambient;
	float3				diffuse;
	float3				specular;
	float				phong_exp;
	float				smoothness;
	float				transmittance;
	float				refraction;
	float3				emission_color;
	float				emission_power;
	int					texture_number;
	float3				texture_position;
# endif

}						t_material;

typedef struct			s_object
{
# ifndef FT_OPENCL___

	t_object_type		type;
	t_material			material;
	cl_float3			center;
	cl_float3			normal;
	cl_float			radius;
	cl_float			angle;
	cl_float			len;
	cl_float3			vertices[3];
# else

	t_object_type		type;
	t_material			material;
	float3				center;
	float3				normal;
	float				radius;
	float				angle;
	float				len;
	float3				vertices[3];
# endif

}						t_object;

# define RT_DEFAULT_POLYGON_VERTICES 3

typedef struct			s_mesh_info
{
# ifndef FT_OPENCL___

	t_material			material;
# else

	t_material			material;
# endif
}						t_mesh_info;

typedef struct			s_polygon
{
# ifndef FT_OPENCL___

	cl_int				vert_i[RT_DEFAULT_POLYGON_VERTICES];
	cl_int				vnorm_i;
	cl_int				vtex_i[RT_DEFAULT_POLYGON_VERTICES];
	cl_int				mesh_index;
# else

	int					vert_i[RT_DEFAULT_POLYGON_VERTICES];
	int					vnorm_i;
	int					vtex_i[RT_DEFAULT_POLYGON_VERTICES];
	int					mesh_index;
#endif

}						t_polygon;

typedef struct			s_meshes
{
# ifndef FT_OPENCL___

	cl_int				num_polygons;
	cl_int				num_vertices;
	cl_int				num_v_normals;
	cl_int				num_v_textures;
	cl_int				num_meshes;
	t_mesh_info			*meshes_info;
	t_polygon			*polygons;
	cl_float3			*vertices;
	cl_float3			*v_normals;
	cl_float3			*v_textures;
# else

	int					num_polygons;
	int					num_vertices;
	int					num_v_normals;
	int					num_v_textures;
	int					num_meshes;
	t_mesh_info			*meshes_info;
	t_polygon			*polygons;
	float3				*vertices;
	float3				*v_normals;
	float3				*v_textures;
# endif

}						t_meshes;

typedef struct			s_scene
{
# ifndef FT_OPENCL___

	t_camera			camera;
	cl_int				obj_nbr;
	cl_int				lights_nbr;
	t_meshes			meshes;
	t_object			*objects;
	t_light				*lights;
# else

	t_camera			camera;
	int					obj_nbr;
	int					lights_nbr;
	t_meshes			meshes;
	t_object			*objects;
	t_light				*lights;
# endif

}						t_scene;

typedef struct			s_pathtrace_params
{
# ifndef FT_OPENCL___

	cl_int				current_samples_num;
	cl_int				max_depth;
# else

	int					current_samples_num;
	int					max_depth;
# endif

}						t_pathtrace_params;

typedef struct			s_raytrace_params
{
# ifndef FT_OPENCL___

	cl_int				max_depth;
# else

	int					max_depth;
# endif

}						t_raytrace_params;

typedef struct			s_renderer_params
{
# ifndef FT_OPENCL___

	t_pathtrace_params	pathtrace_params;
	t_raytrace_params	raytrace_params;
	cl_float			seed;
	cl_float			exposure;
	cl_float			gamma;
# else

	t_pathtrace_params	pathtrace_params;
	t_raytrace_params	raytrace_params;
	float				seed;
	float				exposure;
	float				gamma;
# endif
}						t_renderer_params;

#endif