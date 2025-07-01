/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbalyout <fbalyout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 15:51:44 by fbalyout          #+#    #+#             */
/*   Updated: 2024/10/31 13:43:59 by fbalyout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_node;
	void	*mapped_content;

	if (!lst || !f)
		return (NULL);
	new_lst = NULL;
	while (lst) 
	{
		mapped_content = f(lst->content);
		new_node = ft_lstnew(mapped_content);
		if (!new_node)
		{
			if (del)
				del(mapped_content);
			ft_lstclear(&new_lst, del);
		}
		ft_lstadd_back(&new_lst, new_node);
		(lst = lst->next);
	}
	return (new_lst);
}
