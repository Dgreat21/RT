/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_opencl_files_parser.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gfoote <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 12:07:40 by gfoote            #+#    #+#             */
/*   Updated: 2020/06/12 12:07:46 by gfoote           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char	*get_opencl_kernel_code_text(const char *kernel_path, size_t *out_size)
{
	char		*result_cl_file;
	char		*cl_file;

	cl_file = ft_readfile(open(kernel_path, O_RDONLY), out_size, 10000);
	if (!cl_file)
	{
		printf("path to kernel file: [%s]\n", kernel_path);
		rt_raise_error(ERR_READ_KERNEL_FILE);
	}
	ft_sprintf(&result_cl_file, "%s%s", OPENCL_APPLE_DEFINE, cl_file);
	if (out_size)
		*out_size = ft_strlen(result_cl_file);
	free(cl_file);
	return (result_cl_file);
}
