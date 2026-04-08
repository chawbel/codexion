/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:33:25 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/05 18:27:51 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (argc == 2 && strcmp(argv[1], "help") == 0)
		help();
	else
	{
		init_sim(&sim);
		parse_args(argv, argc, &sim);
		init_arrays(&sim);
		sim.start_time = get_time_ms();
		spawn_threads(&sim);
		cleanup_sim(&sim);
	}
	return (0);
}
