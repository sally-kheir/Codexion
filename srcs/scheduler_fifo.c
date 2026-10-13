/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_fifo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:03:21 by skheir            #+#    #+#             */
/*   Updated: 2026/10/13 09:49:24 by skheir           ###   ########.fr       */
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

long	start_delay_for(t_simulation *sim, int id)
{
	long	slot;

	if (sim->nb_coders == 1)
		return (0);
	if (sim->nb_coders % 2 == 0)
		return (((id - 1) % 2) * (sim->time_to_compile
				+ sim->dongle_cooldown));
	slot = (sim->time_to_compile + sim->dongle_cooldown)
		/ (sim->nb_coders / 2);
	return (((2 * (id - 1)) % sim->nb_coders) * slot);
}

static long	coder_rank(t_simulation *sim, int i)
{
	int		j;
	long	rank;

	rank = 0;
	j = 0;
	while (j < sim->nb_coders)
	{
		if (sim->coders[j].start_delay < sim->coders[i].start_delay
			|| (sim->coders[j].start_delay == sim->coders[i].start_delay
				&& j < i))
			rank++;
		j++;
	}
	return (rank);
}

void	prequeue_coders(t_simulation *sim)
{
	int		i;
	long	prio;

	if (sim->nb_coders == 1)
		return ;
	i = 0;
	while (i < sim->nb_coders)
	{
		if (sim->scheduler == FIFO)
			prio = coder_rank(sim, i);
		else
			prio = sim->coders[i].start_delay;
		heap_push(&sim->coders[i].left->queue, sim->coders[i].id, prio);
		heap_push(&sim->coders[i].right->queue, sim->coders[i].id, prio);
		sim->coders[i].queued = 1;
		i++;
	}
}
