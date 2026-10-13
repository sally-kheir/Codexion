/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:25:49 by skheir            #+#    #+#             */
/*   Updated: 2026/10/11 12:15:00 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_action(t_simulation *sim, int coder_id, const char *action)
{
	pthread_mutex_lock(&sim->log_lock);
	if (!sim->dead)
		printf("%ld %d %s\n", elapsed_ms(sim), coder_id, action);
	pthread_mutex_unlock(&sim->log_lock);
}

void	log_burnout(t_simulation *sim, int coder_id)
{
	pthread_mutex_lock(&sim->log_lock);
	if (!sim->dead)
	{
		sim->dead = 1;
		printf("%ld %d burned out\n", elapsed_ms(sim), coder_id);
	}
	pthread_mutex_unlock(&sim->log_lock);
}

void	log_compile_start(t_simulation *sim, int coder_id)
{
	long	timestamp;

	pthread_mutex_lock(&sim->log_lock);
	if (!sim->dead)
	{
		timestamp = elapsed_ms(sim);
		printf("%ld %d has taken a dongle\n", timestamp, coder_id);
		printf("%ld %d has taken a dongle\n", timestamp, coder_id);
		printf("%ld %d is compiling\n", timestamp, coder_id);
	}
	pthread_mutex_unlock(&sim->log_lock);
}
