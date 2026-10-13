/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:00:00 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 14:00:00 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_coder_burnout(t_simulation *sim, int i)
{
	long	now;
	long	deadline;
	long	last_start;
	int		compiles;

	pthread_mutex_lock(&sim->coders[i].coder_mutex);
	compiles = sim->coders[i].compiles_done;
	last_start = sim->coders[i].last_compile_start;
	pthread_mutex_unlock(&sim->coders[i].coder_mutex);
	if (compiles >= sim->compiles_required)
		return (0);
	now = elapsed_ms(sim);
	deadline = last_start + sim->time_to_burnout;
	if (now > deadline)
	{
		log_burnout(sim, sim->coders[i].id);
		return (1);
	}
	return (0);
}

void	set_monitor_stop(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_lock);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_lock);
}
