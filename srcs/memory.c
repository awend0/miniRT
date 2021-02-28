/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasv <hasv@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 03:42:39 by hasv              #+#    #+#             */
/*   Updated: 2021/02/28 05:11:51 by hasv             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

extern double	g_width;
extern double	g_height;
extern t_color	g_background_color;
extern t_list	*memory;

void	*ft_malloc_save(int size)
{
	void	*ret;
	t_list	*cur;
	t_list	*new;

	ret = malloc(size);
	new = malloc(sizeof(t_list));
	new->node = ret;
	new->next = 0;
	if (!memory)
	{
		memory = malloc(sizeof(t_list));
		memory->node = malloc(1);
		memory->next = 0;
	}
	cur = memory;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	return (ret);
}

void	ft_free(void)
{
	t_list	*cur;
	t_list	*temp;

	cur = memory;
	while (cur)
	{
		temp = cur;
		cur = temp->next;
		free(temp->node);
		free(temp);
	}
}