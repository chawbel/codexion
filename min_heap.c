/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   min_heap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 00:34:34 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/06 03:01:19 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_insert(t_heap *heap, t_heap_node node)
{
	int	i;

	if (heap->size >= heap->capacity)
	{
		print_error("heap overflow");
		return ;
	}
	heap->nodes[heap->size] = node;
	i = heap->size;
	heap->size++;
	while (i > 0 && heap->nodes[(i - 1) / 2].priority > heap->nodes[i].priority)
	{
		swap(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void	heapify_down(t_heap *heap)
{
	int	i;
	int	left;
	int	right;
	int	smallest;

	i = 0;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size
			&& (heap->nodes[left].priority < heap->nodes[smallest].priority))
			smallest = left;
		if (right < heap->size
			&& heap->nodes[right].priority < heap->nodes[smallest].priority)
			smallest = right;
		if (smallest != i)
		{
			swap(&heap->nodes[i], &heap->nodes[smallest]);
			i = smallest;
		}
		else
			break ;
	}
}

int	heap_extract(t_heap *heap)
{
	if (heap->size == 0)
		return (-1);
	heap->nodes[0] = heap->nodes[heap->size - 1];
	heap->size--;
	heapify_down(heap);
	return (0);
}

void	heap_init(t_heap *heap, t_sim *sim)
{
	heap->capacity = sim->number_of_coders;
	heap->size = 0;
	heap->nodes = malloc(sizeof(t_heap_node) * heap->capacity);
	if (!heap->nodes)
		print_error("malloc failed");
}
