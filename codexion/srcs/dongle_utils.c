/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 13:00:00 by skheir            #+#    #+#             */
/*   Updated: 2026/10/08 11:22:45 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_stopped(t_simulation *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_lock);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_lock);
	return (stop);
}

static int	dongles_state(t_coder *c, long now, int with_cooldown)
{
	long	cd;

	cd = 0;
	if (with_cooldown)
		cd = c->sim->dongle_cooldown;
	return (c->left->available && c->right->available
		&& now - c->left->released_at >= cd
		&& now - c->right->released_at >= cd);
}

int	dongle_ready(t_coder *coder, t_dongle *dongle, long now)
{
	int	head;

	head = heap_peek(&dongle->queue);
	if (head == coder->id || head == -1)
		return (1);
	return (!dongles_state(&coder->sim->coders[head - 1], now, 0));
}

int	try_take_dongles(t_coder *coder)
{
	long	now;
	int		ok;

	pthread_mutex_lock(&coder->sim->arb_lock);
	now = elapsed_ms(coder->sim);
	ok = dongles_state(coder, now, 1)
		&& dongle_ready(coder, coder->left, now)
		&& dongle_ready(coder, coder->right, now);
	if (ok)
	{
		heap_remove(&coder->left->queue, coder->id);
		heap_remove(&coder->right->queue, coder->id);
		coder->left->available = 0;
		coder->right->available = 0;
	}
	pthread_mutex_unlock(&coder->sim->arb_lock);
	return (ok);
}
