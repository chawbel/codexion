/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 23:30:14 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/04 23:30:14 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_sim(t_sim *sim)
{
	sim->number_of_coders = 0;
	sim->time_to_burnout = 0;
	sim->time_to_compile = 0;
	sim->time_to_debug = 0;
	sim->time_to_refactor = 0;
	sim->number_of_compiles_required = 0;
	sim->dongle_cooldown = 0;
	sim->scheduler = 0;
	sim->sim_ended = 0;
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->start_time = 0;
	sim->dongles = NULL;
	sim->coders = NULL;
}

void	init_coder(t_coder *coder, t_sim *sim, int id)
{
	coder->id = id + 1;
	coder->left = &sim->dongles[id];
	coder->right = &sim->dongles[(id + 1) % sim->number_of_coders];
	coder->last_compile_start = 0;
	coder->compile_count = 0;
	coder->sim = sim;
}

void	init_dongle(t_dongle *dongle, t_sim *sim, int id)
{
	dongle->available = 1;
	dongle->cooldown_until = 0;
	dongle->id = id;
	pthread_mutex_init(&dongle->lock, NULL);
	pthread_cond_init(&dongle->cond, NULL);
	dongle->waitqueue = malloc(sizeof(t_heap));
	if (!dongle->waitqueue)
			print_error("malloc failed");
	heap_init(dongle->waitqueue, sim);
}

void	init_arrays(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
		print_error("malloc failed");
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		print_error("malloc failed");
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		init_dongle(&sim->dongles[i], sim, i);
		init_coder(&sim->coders[i], sim, i);
		i++;
	}
}

void	spawn_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].last_compile_start = sim->start_time;
		i++;
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_create(&sim->coders[i].t, NULL, coder_routine, &sim->coders[i]);
		i++;
	}
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_join(sim->coders[i].t, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}
