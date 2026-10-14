/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_fifo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:03:21 by skheir            #+#    #+#             */
/*   Updated: 2026/10/13 18:26:42 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	fifo_priority(t_simulation *sim)
{
	return ((sim->ticket++));
}

long	compute_priority(t_coder *coder)
{
	if (coder->sim->scheduler == FIFO)
		return (fifo_priority(coder->sim));
	return (edf_priority(coder));
}

void	prequeue_coders(t_simulation *sim)
{
	int	i;

	if (sim->nb_coders == 1)
		return ;
	i = 0;
	while (i < sim->nb_coders)
	{
		heap_push(&sim->coders[i].left->queue, sim->coders[i].id, i);
		heap_push(&sim->coders[i].right->queue, sim->coders[i].id, i);
		sim->coders[i].queued = 1;
		i++;
	}
}
