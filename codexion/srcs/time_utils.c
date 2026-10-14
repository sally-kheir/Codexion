/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:22:00 by skheir            #+#    #+#             */
/*   Updated: 2026/10/14 09:53:22 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 199309L
#include "codexion.h"

long	get_timestamp_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L));
}

long	elapsed_ms(t_simulation *sim)
{
	return (get_timestamp_ms() - sim->start_time);
}
