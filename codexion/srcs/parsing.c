/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42beirut.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:26:11 by skheir            #+#    #+#             */
/*   Updated: 2026/10/05 13:03:56 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long	parse_positive_long(const char *str, const char *field_name)
{
	long	value;

	if (!is_valid_number(str))
		error_exit_field(field_name);
	value = atol(str);
	if (value <= 0)
		error_exit_field(field_name);
	return (value);
}

static long	parse_non_negative_long(const char *str, const char *field_name)
{
	long	value;

	if (!is_valid_number(str))
		error_exit_field(field_name);
	value = atol(str);
	if (value < 0)
		error_exit_field(field_name);
	return (value);
}

static void	parse_scheduler(const char *str, t_simulation *sim)
{
	if (strcmp(str, "fifo") == 0)
		sim->scheduler = FIFO;
	else if (strcmp(str, "edf") == 0)
		sim->scheduler = EDF;
	else
		error_exit("scheduler must be \"fifo\" or \"edf\"");
}

int	parse_args(int argc, char **argv, t_simulation *sim)
{
	if (argc != 9)
	{
		usage_error(argv[0]);
		return (1);
	}
	sim->nb_coders = (int)parse_positive_long(argv[1], "number_of_coders");
	sim->time_to_burnout = parse_positive_long(argv[2], "time_to_burnout");
	sim->time_to_compile = parse_positive_long(argv[3], "time_to_compile");
	sim->time_to_debug = parse_positive_long(argv[4], "time_to_debug");
	sim->time_to_refactor = parse_positive_long(argv[5], "time_to_refactor");
	sim->compiles_required = (int)parse_positive_long(argv[6],
			"number_of_compiles_required");
	sim->dongle_cooldown = parse_non_negative_long(argv[7],
			"dongle_cooldown");
	parse_scheduler(argv[8], sim);
	return (0);
}
