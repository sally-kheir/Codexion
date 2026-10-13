/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:26:31 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 12:26:31 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coder_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	join_coder_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	run_simulation(t_simulation *sim)
{
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (1);
	if (create_coder_threads(sim) != 0)
	{
		pthread_mutex_lock(&sim->stop_lock);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_lock);
		pthread_join(sim->monitor_thread, NULL);
		return (1);
	}
	join_coder_threads(sim);
	pthread_join(sim->monitor_thread, NULL);
	return (0);
}
