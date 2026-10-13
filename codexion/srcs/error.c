/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:02:52 by skheir            #+#    #+#             */
/*   Updated: 2026/10/12 12:09:20 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	usage_error(const char *prog_name)
{
	fprintf(stderr, "Usage: %s number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown scheduler\n",
		prog_name);
	fprintf(stderr, "  number_of_coders: positive integer\n");
	fprintf(stderr, "  time_to_burnout, time_to_compile, time_to_debug, "
		"time_to_refactor: positive integers (ms)\n");
	fprintf(stderr, "  dongle_cooldown: non-negative integer (ms)\n");
	fprintf(stderr, "  number_of_compiles_required: positive integer\n");
	fprintf(stderr, "  scheduler: \"fifo\" or \"edf\"\n");
}

void	error_exit(const char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void	error_exit_field(const char *field_name)
{
	fprintf(stderr, "Error: invalid value for \"%s\"\n", field_name);
	exit(EXIT_FAILURE);
}
