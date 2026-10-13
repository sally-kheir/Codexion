/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:40:00 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 12:40:00 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	check_compile_requirement(t_coder *coder)
{
	int	reached;

	reached = 0;
	pthread_mutex_lock(&coder->coder_mutex);
	if (coder->compiles_done == coder->sim->compiles_required)
		reached = 1;
	pthread_mutex_unlock(&coder->coder_mutex);
	if (!reached)
		return ;
	pthread_mutex_lock(&coder->sim->progress_lock);
	coder->sim->completed_coders++;
	if (coder->sim->completed_coders == coder->sim->nb_coders)
	{
		pthread_mutex_lock(&coder->sim->stop_lock);
		coder->sim->stop = 1;
		pthread_mutex_unlock(&coder->sim->stop_lock);
	}
	pthread_mutex_unlock(&coder->sim->progress_lock);
}

int	coder_can_compile(t_coder *coder)
{
	int	can_compile;

	can_compile = 1;
	pthread_mutex_lock(&coder->coder_mutex);
	if (coder->compiles_done >= coder->sim->compiles_required)
		can_compile = 0;
	pthread_mutex_unlock(&coder->coder_mutex);
	return (can_compile);
}

void	set_compile_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile_start = elapsed_ms(coder->sim);
	pthread_mutex_unlock(&coder->coder_mutex);
}

void	increment_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->coder_mutex);
}

void	do_debug(t_coder *coder)
{
	log_action(coder->sim, coder->id, "is debugging");
	usleep(coder->sim->time_to_debug * 1000);
}
