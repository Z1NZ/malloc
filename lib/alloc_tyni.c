/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_tyni.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srabah <srabah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 15:33:47 by srabah            #+#    #+#             */
/*   Updated: 2017/03/14 20:20:00 by srabah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static inline	void	set_block(t_block *ptr, size_t size, int on)
{
	ptr->size = size;
	ptr->info |= OPT_TYNI;
	if (on)
		ptr->info |= OPT_FREE;
	ptr->ptr = ptr->data;
	if (on)
		g_mem.use_tyni += size;
}

static inline	void	set_page(t_block *ptr, size_t size_block, int nb)
{
	int		i;
	int		len;
	t_block *new;
	t_block *tmp;

	i = 1;
	new = NULL;
	len = g_mem.page / (size_block);
	len = len * nb;
	tmp = ptr;
	while (i < len)
	{
		new = tmp->ptr + (size_block - SIZE_ST_HEAD);
		tmp->next = new;
		set_block(new, size_block, 0);
		tmp = tmp->next;
		i++;
	}
	tmp->next = NULL;
}

static void				*add_page(size_t size)
{
	t_block		*ptr;
	t_block		*tmp;

	if ((ptr = (t_block *)mmap(NULL,
			(g_mem.page * size), FLAG_MALLOC, -1, 0)) == MAP_FAILED)
		return (NULL);
	g_mem.size_tyni += g_mem.page * size;
	tmp = g_mem.m_tyni;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = ptr;
	ptr->info |= OPT_MAP_HEAD;
	set_block(ptr, TYNI_BLOCK, 0);
	set_page(ptr, TYNI_BLOCK, size);
	return (ptr);
}

static int				init_tyni_page(size_t nb)
{
	t_block *ptr;

	if ((ptr = (t_block *)mmap(NULL,
		g_mem.page * nb, FLAG_MALLOC, -1, 0)) == MAP_FAILED)
		return (1);
	g_mem.size_tyni = g_mem.page * nb;
	ptr->info |= OPT_MAP_HEAD;
	set_block(ptr, TYNI_BLOCK, 0);
	ptr->next = NULL;
	g_mem.m_tyni = ptr;
	set_page(ptr, TYNI_BLOCK, nb);
	return (0);
}

void					*alloc_tyni(size_t size)
{
	t_block *ptr;

	ptr = NULL;
	if (g_mem.size_tyni == 0)
		if (init_tyni_page(ROUND_UP_PAGE(size * TYNI_BLOCK, g_mem.page)) == 1)
			return (unlock_return_null(&(g_mem.mutex)));
	if ((g_mem.size_tyni - g_mem.use_tyni) >= TYNI_BLOCK * size)
		ptr = find_fusion_location(g_mem.m_tyni, size);
	if (!ptr)
	{
		ptr = add_page(ROUND_UP_PAGE(size * TYNI_BLOCK, g_mem.page));
		if (!ptr)
			return (unlock_return_null(&(g_mem.mutex)));
		ptr = find_fusion_location(g_mem.m_tyni, size);
	}
	if (ptr && ptr != ((void *)-1))
		set_block(ptr, TYNI_BLOCK * size, OPT_FREE);
	pthread_mutex_unlock(&(g_mem.mutex));
	return (((ptr && ptr != ((void *)-1)) ? ptr->data : NULL));
}
