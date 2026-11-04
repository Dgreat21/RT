#define FT_OPENCL___

# define RAY_MIN_EPSILON 0.001f
# define RT_EPSILON 0.001f
# define RAY_MARCH_MAX_STEPS 10000
# define RAY_MARCH_MAX_DIST 1000.f
# define RAY_MARCH_SURFACE_ACC .001f

# define M_PI_360 0.00872664625997

/*
**	M_PI is a double constant that OpenCL C only guarantees when cl_khr_fp64
**	is supported (Apple Silicon GPUs have no fp64), so fall back to float.
*/

# ifndef M_PI
#  define M_PI M_PI_F
# endif

# define LAMBERT_ALPHA 1.f


