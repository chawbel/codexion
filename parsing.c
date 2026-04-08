/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 02:56:41 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/03 02:56:41 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	check_scheduler(char *str, t_sim *sim)
{
	if (strcmp(str, "fifo") == 0)
		sim->scheduler = FIFO;
	else
		sim->scheduler = EDF;
}

void	fill_values(char **args, t_sim *sim)
{
	sim->number_of_coders = atoi(args[1]);
	sim->time_to_burnout = atoi(args[2]);
	sim->time_to_compile = atoi(args[3]);
	sim->time_to_debug = atoi(args[4]);
	sim->time_to_refactor = atoi(args[5]);
	sim->number_of_compiles_required = atoi(args[6]);
	sim->dongle_cooldown = atoi(args[7]);
	check_scheduler(args[8], sim);
}

void	help(void)
{
	printf("Valid inputs are: \n1 number_of_coders");
	printf("\n2 time_to_burnout\n3 time_to_compile\n4 time_to_debug");
	printf("\n5 time_to_refactor \n6 number_of_compiles_required");
	printf("\n7 dongle_cooldown\n8 scheduler\n");
}

void	validate_sim(t_sim *sim)
{
	if (sim->number_of_coders < 1)
		print_error("number_of_coders must be >= 1");
	if (sim->time_to_burnout < 1)
		print_error("time_to_burnout must be >= 1");
	if (sim->time_to_compile < 1)
		print_error("time_to_compile must be >= 1");
	if (sim->time_to_debug < 1)
		print_error("time_to_debug must be >= 1");
	if (sim->time_to_refactor < 1)
		print_error("time_to_refactor must be >= 1");
	if (sim->number_of_compiles_required < 1)
		print_error("number_of_compiles_required must be >= 1");
	if (sim->dongle_cooldown < 0)
		print_error("dongle_cooldown must be >= 0");
}

void	parse_args(char **str, int argc, t_sim *sim)
{
	int	i;

	if (argc != 9)
		print_error("Invalid input, use './codexion help' for usage info");
	i = 1;
	ft_to_lower(str[8]);
	if (strcmp(str[8], "edf") != 0
		&& strcmp(str[8], "fifo") != 0)
		print_error("Scheduler must be either fifo/FIFO or edf/EDF\n");
	while (i < 8)
	{
		if (!is_number(str[i]))
			print_error("Error, input must be a valid number");
		if (atoi(str[i]) < 0)
			print_error("Error, input must be positive");
		i++;
	}
	fill_values(str, sim);
	validate_sim(sim);
}
