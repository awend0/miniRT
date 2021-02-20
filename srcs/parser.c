/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasv <hasv@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 22:18:53 by hasv              #+#    #+#             */
/*   Updated: 2021/02/20 20:12:29 by hasv             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

extern double 	g_width;
extern double 	g_height;
extern t_color	g_background_color;

double			ft_stof(char *line)
{
	char	**temp;

	temp = ft_split(line, '.');
	if (!temp[1])
		return ((double)(ft_atoi(temp[0])));
	return ((double)ft_atoi(temp[0]) + ((double)ft_atoi(temp[1]) / powf(10, ft_strlen(temp[1]))));
}

t_color			ft_stoc(char *line)
{
	char	**temp;

	temp = ft_split(line, ',');
	return ((t_color){ft_stof(temp[0]), ft_stof(temp[1]), ft_stof(temp[2])});	
}

t_point			ft_stop(char *line)
{
	char	**temp;

	temp = ft_split(line, ',');
	return ((t_point){ft_stof(temp[0]), ft_stof(temp[1]), ft_stof(temp[2])});	
}

t_light			*ft_parse_pnt(char *line)
{
	char	**words;
	t_light	*ret;

	ret = malloc(sizeof(t_light));
	words = ft_split(line, ' ');
	ret->e_type = POINT;
	ret->position = ft_stop(words[1]);
	ret->intensity = ft_stof(words[2]);
	ret->color = ft_stoc(words[3]);
	free(words);
	return (ret);
}

t_light			*ft_parse_amb(char *line)
{
	char	**words;
	t_light	*ret;

	ret = malloc(sizeof(t_light));
	words = ft_split(line, ' ');
	ret->e_type = AMBIENT;
	ret->intensity = ft_stof(words[1]);
	ret->color = ft_stoc(words[2]);
	return (ret);
}

void			ft_parse_res(char *line)
{
	char	**words;

	words = ft_split(line, ' ');
	g_width = 0;
	g_height = 0;
	g_width = (double)ft_atoi(words[1]);
	g_height = (double)ft_atoi(words[2]);
	free (words);
}

double			**ft_rotation_matrix(t_point A)
{
	double	**ret;
	t_point B;
	t_point C;

	ret = malloc(3 * sizeof(double));
	ret[0] = malloc(3 * sizeof(double));
	ret[1] = malloc(3 * sizeof(double));
	ret[2] = malloc(3 * sizeof(double));
	B = (t_point){0.0, 0.0, 1.0};
	C = ft_vec_cross(A, B);
	B = ft_vec_cross(C, A);
	ret[0][0] = B.x;
	ret[0][1] = C.x;
	ret[0][1] = C.y;
	ret[1][0] = B.y;
	ret[1][1] = C.y;
	ret[1][2] = A.y;
	ret[2][0] = B.z;
	ret[2][1] = C.z;
	ret[2][2] = A.z;
	return (ret); 	
}
t_camera		ft_parse_camera(char *line)
{
	char	**words;
	t_camera ret;

	words = ft_split(line, ' ');
	ret.pos = ft_stop(words[1]);
	ret.rotation = ft_rotation_matrix(ft_stop(words[2]));
	ret.fov = ft_stof(words[3]);
	ret.viewport = (t_viewport){1.0, 1.0, 1.0};
	free (words);
	return (ret);
}

t_object		*ft_parse_sphere(char *line)
{
	char			**words;
	t_object		*ret;
	t_sphereParams	params;

	ret = malloc(sizeof(t_sphere));
	words = ft_split(line, ' ');
	params.center = ft_stop(words[1]);
	params.radius = ft_stof(words[2]);
	params.color = ft_stoc(words[3]);
	params.reflection = 0.5;
	params.spec = 500;
	if (words[4])
		params.reflection = ft_stof(words[4]);
	if (words[5])
		params.spec = ft_stof(words[5]);
	ret = ft_create_sphere(params);
	free (words);
	return (ret);
}

t_objectsList	*ft_cylinder_caps(t_object *obj, t_cylinderParams params)
{
	t_objectsList	*ret;
	t_object		*top_cap;
	t_object		*bottom_cap;

	top_cap = ft_create_disc((t_discParams){params.p, ft_vec_multiply(-1, ft_vec_norm(params.orient)),
		params.color, params.diam / 2.0, params.reflection, params.spec});
	bottom_cap = ft_create_disc((t_discParams){ft_vec_add(params.p, ft_vec_multiply(params.height, ft_vec_norm(params.orient))), params.orient,
		params.color, params.diam / 2.0, params.reflection, params.spec});
	ret = ft_olstnew(obj);
	ret->next = ft_olstnew(top_cap);
	((t_objectsList*)ret->next)->next = ft_olstnew(bottom_cap);
	return (ret);
}

t_objectsList	*ft_parse_cylinder(char *line)
{
	char				**words;
	t_object			*ret;
	t_cylinderParams	params;

	ret = malloc(sizeof(t_object));
	words = ft_split(line, ' ');
	params.p = ft_stop(words[1]);
	params.orient = ft_stop(words[2]);
	params.color = ft_stoc(words[3]);
	params.diam = ft_stof(words[4]);
	params.height = ft_stof(words[5]);
	params.reflection = 0.5;
	params.spec = 500;
	if (words[6])
		params.reflection = ft_stof(words[6]);
	if (words[7])
		params.spec = ft_stof(words[7]);
	ret = ft_create_cylinder(params);
	free (words);
	return (ft_cylinder_caps(ret, params));
}


t_object		*ft_parse_triangle(char *line)
{
	char				**words;
	t_object			*ret;
	t_triangleParams	params;

	ret = malloc(sizeof(t_object));
	words = ft_split(line, ' ');
	params.a = ft_stop(words[1]);
	params.b = ft_stop(words[2]);
	params.c = ft_stop(words[3]);
	params.color = ft_stoc(words[4]);
	params.reflection = 0.5;
	params.spec = 500;
	if (words[5])
		params.reflection = ft_stof(words[5]);
	if (words[6])
		params.spec = ft_stof(words[6]);
	ret = ft_create_triangle(params);
	free (words);
	return (ret);
}

t_object		*ft_parse_plane(char *line)
{
	char			**words;
	t_object		*ret;
	t_planeParams	params;

	ret = malloc(sizeof(t_object));
	words = ft_split(line, ' ');
	params.p = ft_stop(words[1]);
	params.norm = ft_stop(words[2]);
	params.color = ft_stoc(words[3]);
	params.reflection = 0.5;
	params.spec = 500;
	if (words[4])
		params.reflection = ft_stof(words[4]);
	if (words[5])
		params.spec = ft_stof(words[5]);
	ret = ft_create_plane(params);
	free (words);
	return (ret);
}

t_object		*ft_parse_disc(char *line)
{
	char			**words;
	t_object		*ret;
	t_discParams	params;

	ret = malloc(sizeof(t_object));
	words = ft_split(line, ' ');
	params.p = ft_stop(words[1]);
	params.norm = ft_stop(words[2]);
	params.r = ft_stof(words[3]);
	params.color = ft_stoc(words[4]);
	params.reflection = 0.5;
	params.spec = 500;
	if (words[5])
		params.reflection = ft_stof(words[5]);
	if (words[6])
		params.spec = ft_stof(words[6]);
	ret = ft_create_disc(params);
	free (words);
	return (ret);
}

t_parsedData	*ft_parse_processor(char *line, t_parsedData *data)
{
	if (line[0] == 'R')
		ft_parse_res(line);
	if (line[0] == 'A')
		data->lights = ft_lstadd_back(data->lights, ft_lstnew(ft_parse_amb(line)));
	if (line[0] == 'c' && line[1] != 'y')
		data->camera = ft_parse_camera(line);
	if (line[0] == 'l')
		data->lights = ft_lstadd_back(data->lights, ft_lstnew(ft_parse_pnt(line)));
	if (line[0] == 's' && line[1] == 'p')
		data->objects = ft_olstadd_back(data->objects, ft_olstnew(ft_parse_sphere(line)));
	if (line[0] == 't' && line[1] == 'r')
		data->objects = ft_olstadd_back(data->objects, ft_olstnew(ft_parse_triangle(line)));
	if (line[0] == 'p' && line[1] == 'l')
		data->objects = ft_olstadd_back(data->objects, ft_olstnew(ft_parse_plane(line)));
	if (line[0] == 'c' && line[1] == 'y')
		data->objects = ft_olstadd_back(data->objects, ft_parse_cylinder(line));
	if (line[0] == 'd' && line[1] == 's')
		data->objects = ft_olstadd_back(data->objects, ft_olstnew(ft_parse_disc(line)));
	return (data);
}

t_parsedData	*ft_parser(int argc, char *argv[])
{
	t_parsedData	*ret;
	int				fd;
	char			*line;
	int				i;

	ret = malloc(sizeof(t_parsedData));
	ret->lights = 0;
	ret->objects = 0;
	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		while (1)
		{
			i = get_next_line(fd, &line);
			if (i == 1)
				ret = ft_parse_processor(line, ret);
			else if (i == -1)
				return (0);
			else if (i == 0)
			{
				ret = ft_parse_processor(line, ret);
				break;
			}
		}
		close(fd);
	}
	return (ret);
}
