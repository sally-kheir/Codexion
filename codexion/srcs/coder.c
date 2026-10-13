/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:25:03 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 12:40:34 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	should_stop(t_simulation *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_lock);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stop);
}

static int	start_compile(t_coder *coder)
{
	if (should_stop(coder->sim))
		return (1);
	if (!coder_can_compile(coder))
		return (1);
	if (take_dongles(coder) != 0)
		return (1);
	if (should_stop(coder->sim))
	{
		release_dongles(coder);
		return (1);
	}
	if (!coder_can_compile(coder))
	{
		release_dongles(coder);
		return (1);
	}
	set_compile_start(coder);
	log_compile_start(coder->sim, coder->id);
	return (0);
}

static int	do_compile(t_coder *coder)
{
	if (start_compile(coder) != 0)
		return (1);
	usleep(coder->sim->time_to_compile * 1000);
	release_dongles(coder);
	increment_compiles(coder);
	check_compile_requirement(coder);
	return (0);
}

void	do_refactor(t_coder *coder)
{
	log_action(coder->sim, coder->id, "is refactoring");
	usleep(coder->sim->time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	set_compile_start(coder);
	while (!should_stop(coder->sim))
	{
		if (do_compile(coder) != 0)
			break ;
		if (should_stop(coder->sim))
			break ;
		do_debug(coder);
		if (should_stop(coder->sim))
			break ;
		do_refactor(coder);
	}
	return (NULL);
}
