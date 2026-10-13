/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:25:34 by skheir            #+#    #+#             */
/*   Updated: 2026/10/08 12:30:29 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	heap->size = 0;
	heap->capacity = capacity;
	pthread_mutex_init(&heap->lock, NULL);
	pthread_cond_init(&heap->cond, NULL);
}

void	heap_push(t_heap *heap, int coder_id, long priority)
{
	pthread_mutex_lock(&heap->lock);
	heap->nodes[heap->size].coder_id = coder_id;
	heap->nodes[heap->size].priority = priority;
	heap_bubble_up(heap, heap->size);
	heap->size++;
	pthread_cond_broadcast(&heap->cond);
	pthread_mutex_unlock(&heap->lock);
}

int	heap_pop(t_heap *heap)
{
	int	coder_id;

	pthread_mutex_lock(&heap->lock);
	if (heap->size == 0)
	{
		pthread_mutex_unlock(&heap->lock);
		return (-1);
	}
	coder_id = heap->nodes[0].coder_id;
	heap->size--;
	heap->nodes[0] = heap->nodes[heap->size];
	heap_bubble_down(heap, 0);
	pthread_mutex_unlock(&heap->lock);
	return (coder_id);
}

int	heap_peek(t_heap *heap)
{
	int	coder_id;

	pthread_mutex_lock(&heap->lock);
	if (heap->size == 0)
		coder_id = -1;
	else
		coder_id = heap->nodes[0].coder_id;
	pthread_mutex_unlock(&heap->lock);
	return (coder_id);
}

void	heap_destroy(t_heap *heap)
{
	free(heap->nodes);
	pthread_mutex_destroy(&heap->lock);
	pthread_cond_destroy(&heap->cond);
}
