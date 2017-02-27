/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srabah <srabah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 16:23:41 by srabah            #+#    #+#             */
/*   Updated: 2017/02/27 08:57:30 by srabah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	(void)count;
	(void)size;
	dprintf(2, "%s\n", "SUPER");
	return(NULL);
}



void	*realloc(void *count, size_t size)
{
	(void)count;
	(void)size;
	dprintf(2, "%s\n", "SUPER REALLOC");
	return(NULL);
}

void	*malloc(size_t size) // attention au size_t max ====> 18446744073709551615
{
	size_t	len;
	size_t	len_small;

	len = 5;
	len_small = 5;
	pthread_mutex_lock(&(g_mem.mutex));
	if (g_mem.page == 0)
		g_mem.page = getpagesize();
	if (!g_mem.size_tyni && !g_mem.size_small)
		init_memory(100, 100);
	len = (size <= TYNI_MAX) ? 1 : (ROUND_UP_PAGE(size, TYNI_BLOCK));
	len_small = (size <= SMALL_MIN) ? 1 : (ROUND_UP_PAGE(size, SMALL_BLOCK));
	if (len <= 4)
		return (alloc_tyni(len));
	else if (len_small <= 4)
		return (alloc_small(len_small));
	else
		return (alloc_large(size));
}

// int main(int argc, char const *argv[])
// {

// 	char	*ptr;
// 	char	*ptr2;
// 	int		i;

// 	i = 0;
// 	ptr = NULL;
// 	ptr2 = NULL;
// 	printf("taille du block ====		[%zu]		====\n", sizeof(t_block));
// 	if (argc != 2)
// 		return (1);
// 	ptr = (char *)ft_malloc(atol(argv[1]));
// 	ft_free(ptr); 
// 	printf("%lu\n", atol(argv[1]));
// 	while(i < 100)
// 	{
// 		printf("%s\n", "SIII");
// 		ptr = (char *)ft_malloc(atol(argv[1]));
// 		printf("[%d] === > %p\n", i, ptr);
// 		ft_free(ptr);
// 		ptr = NULL;
// 		printf("%s\n", ";SIPPPPP");
	
// 		i++;
// 	}
// 	// if (!ptr)
// 		// return(1);
// 	// while(i <= atoi(argv[1]))
// 	// {
// 	// 	ptr[i] = 'a';
// 	// 	i++;
// 	// }
// 	// printf("value ptr [%s]\n", ptr);
// 	// if (argc == 2)
// 		// ptr2 = (char *)ft_malloc(atol(argv[1]));
// 	// i = 0;
// 	// while(i <= atoi(argv[1]))
// 	// {
// 	// 	ptr2[i] = 'b';
// 	// 	i++;
// 	// }
// 	// printf("------addr ptr2	[%p]\n", ptr2);
// 	// printf("value ptr2	[%s]\n\n\n\n\n\n\n",ptr2);

// 	// printf("taille de la espace[%zu]\ntaille de l'espace uttiliser[%zu]\n\n\n", g_mem.size_tyni, g_mem.use_tyni);
// 	// printf("addr ptr  [%p]\n", ptr);
// 	// printf("value ptr [%s]\n", ptr);
// 	// i = 0;
// 	// while (i < 300)
// 	// {
// 	// 	printf("%d => ", i);
// 	// 	ptr2 = (char *)ft_malloc(atol(argv[1]));
// 	// 	printf("%p null = %p\n", ptr2, NULL);
// 	// 	i++;
// 	// }

// 	// LIST_HEAD(head);
// 	// int page = getpagesize();
// 	// t_block *ptr = malloc(sizeof(t_block));
// 	// list_add_tail(&(ptr->list), &head);
// 	// ptr->free = 88999;
// 	// t_block *ptr2 = (t_block *)malloc(sizeof(t_block));
// 	// list_add_tail(&(ptr2->list), &head);
// 	// ptr2->free = 999999;
// 	// int *test = &(ptr2->free);
// 	// t_block *ptr3 = list_entry(test, t_block, free);
// 	// printf("ptr2 == [%p] -> ptr3 [%p] \n", ptr2, ptr3);
// 	// printf("page size ===> [%d]", getpagesize());
// 	// printf("\ntaille du block[%lu]\n", sizeof(t_block));
// 	// char *toto = (char *)mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
// 	// printf("[%zd]", write(1, toto, page));
// 	// printf("[%p]", toto);
// 	// toto = malloc(0);
// 	// printf("[%p]", toto);
// 	t_block *tmp;

// 	tmp = g_mem.m_tyni;
// 	int j = 0;
// 	while(tmp)
// 	{
// 		printf("TYNI "RED"pos = [%d]"GRN"addr = [%p]" CYN "size = [%zu]"RESET" info = [%d]\n", j, tmp, tmp->size, tmp->info);
// 		tmp = tmp->next;
// 		j++;
// 	}

// 	tmp = g_mem.m_small;
// 	j = 0;
// 	while(tmp)
// 	{
// 		printf("SMALL "RED"pos = [%d]"GRN"addr = [%p]" CYN "size = [%zu]"RESET" info = [%d]\n", j, tmp, tmp->size, tmp->info);
// 		tmp = tmp->next;
// 		j++;
// 	}

// 	tmp = g_mem.m_large;
// 	j = 0;
// 	while(tmp)
// 	{
// 		printf("LARGE "RED"pos = [%d]"GRN"addr = [%p]" CYN "size = [%zu]"RESET" info = [%d]\n", j, tmp, tmp->size, tmp->info);
// 		tmp = tmp->next;
// 		j++;
// 	}
// 	printf("\ng_Memory tyni -> (%zu / %zu)\n", g_mem.use_tyni, g_mem.size_tyni);
// 	printf("\ng_Memory small -> (%zu / %zu)\n", g_mem.use_small, g_mem.size_small);
// 	printf("\ng_Memory large -> (%zu / %zu)\n", g_mem.use_large, g_mem.size_large);
// 	printf("%p\n", ptr);

// 	return 0;
// }