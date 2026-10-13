/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:26:04 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 13:03:40 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_compiles_done(t_simulation *sim)
{
	int	i;
	int	all_done;
	int	compiles;

	i = 0;
	all_done = 1;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].coder_mutex);
		compiles = sim->coders[i].compiles_done;
		pthread_mutex_unlock(&sim->coders[i].coder_mutex);
		if (compiles < sim->compiles_required)
			all_done = 0;
		i++;
	}
	return (all_done);
}

static int	check_burnout(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (check_coder_burnout(sim, i))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (1)
	{
		if (is_stopped(sim))
			break ;
		if (check_burnout(sim))
		{
			set_monitor_stop(sim);
			break ;
		}
		if (check_compiles_done(sim))
		{
			set_monitor_stop(sim);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
