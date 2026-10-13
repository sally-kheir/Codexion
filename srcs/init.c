/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:03:06 by skheir            #+#    #+#             */
/*   Updated: 2026/10/12 12:08:44 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_simulation *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->nb_coders);
	if (!sim->dongles)
		return (1);
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available = 1;
		sim->dongles[i].released_at = -sim->dongle_cooldown;
		if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
			return (1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (1);
		heap_init(&sim->dongles[i].queue, sim->nb_coders);
		i++;
	}
	return (0);
}

static int	init_coders(t_simulation *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->nb_coders);
	if (!sim->coders)
		return (1);
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].queued = 0;
		sim->coders[i].start_delay = start_delay_for(sim, i + 1);
		if (pthread_mutex_init(&sim->coders[i].coder_mutex, NULL) != 0)
			return (1);
		sim->coders[i].sim = sim;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[
			(i + 1) % sim->nb_coders];
		i++;
	}
	return (0);
}

int	init_simulation(t_simulation *sim)
{
	if (init_dongles(sim) != 0)
		return (1);
	if (init_coders(sim) != 0)
		return (1);
	if (pthread_mutex_init(&sim->log_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->stop_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->progress_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim->arb_lock, NULL) != 0)
		return (1);
	sim->stop = 0;
	sim->dead = 0;
	sim->completed_coders = 0;
	sim->ticket = sim->nb_coders;
	sim->start_time = get_timestamp_ms();
	prequeue_coders(sim);
	return (0);
}
