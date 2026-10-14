/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:02:13 by skheir            #+#    #+#             */
/*   Updated: 2026/10/13 18:15:25 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->arb_lock);
	dongle->available = 1;
	dongle->released_at = elapsed_ms(coder->sim);
	pthread_mutex_unlock(&coder->sim->arb_lock);
}

static void	enqueue_request(t_coder *coder)
{
	long	priority;

	pthread_mutex_lock(&coder->sim->arb_lock);
	priority = compute_priority(coder);
	heap_push(&coder->left->queue, coder->id, priority);
	heap_push(&coder->right->queue, coder->id, priority);
	pthread_mutex_unlock(&coder->sim->arb_lock);
}

static int	take_single(t_coder *coder)
{
	log_action(coder->sim, coder->id, "has taken a dongle");
	while (!is_stopped(coder->sim))
		usleep(1000);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	if (coder->sim->nb_coders == 1)
		return (take_single(coder));
	if (!coder->queued)
		enqueue_request(coder);
	coder->queued = 0;
	while (!is_stopped(coder->sim))
	{
		if (try_take_dongles(coder))
			return (0);
		usleep(100);
	}
	return (1);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder, coder->left);
	release_dongle(coder, coder->right);
}
