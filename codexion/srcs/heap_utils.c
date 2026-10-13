/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 13:30:00 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 13:30:00 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	heap_less(t_heap_node *a, t_heap_node *b)
{
	if (a->priority != b->priority)
		return (a->priority < b->priority);
	return (a->coder_id < b->coder_id);
}

void	heap_swap(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_bubble_up(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!heap_less(&heap->nodes[index], &heap->nodes[parent]))
			break ;
		heap_swap(&heap->nodes[parent], &heap->nodes[index]);
		index = parent;
	}
}

void	heap_bubble_down(t_heap *heap, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		smallest = index;
		if (left < heap->size
			&& heap_less(&heap->nodes[left], &heap->nodes[smallest]))
			smallest = left;
		if (right < heap->size
			&& heap_less(&heap->nodes[right], &heap->nodes[smallest]))
			smallest = right;
		if (smallest == index)
			break ;
		heap_swap(&heap->nodes[index], &heap->nodes[smallest]);
		index = smallest;
	}
}

int	heap_remove(t_heap *heap, int coder_id)
{
	int	i;

	pthread_mutex_lock(&heap->lock);
	i = 0;
	while (i < heap->size && heap->nodes[i].coder_id != coder_id)
		i++;
	if (i == heap->size)
	{
		pthread_mutex_unlock(&heap->lock);
		return (-1);
	}
	heap->size--;
	if (i < heap->size)
	{
		heap->nodes[i] = heap->nodes[heap->size];
		heap_bubble_up(heap, i);
		heap_bubble_down(heap, i);
	}
	pthread_mutex_unlock(&heap->lock);
	return (coder_id);
}
