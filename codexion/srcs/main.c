/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:25:57 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 12:25:57 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (parse_args(argc, argv, &sim) != 0)
		return (1);
	if (init_simulation(&sim) != 0)
	{
		fprintf(stderr, "Error: initialization failed\n");
		return (1);
	}
	if (run_simulation(&sim) != 0)
	{
		fprintf(stderr, "Error: simulation failed\n");
		cleanup_simulation(&sim);
		return (1);
	}
	cleanup_simulation(&sim);
	return (0);
}
