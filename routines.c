/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 22:27:50 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/06 19:05:52 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_ended(t_sim *sim)
{
	int	ended;

	pthread_mutex_lock(&sim->stop_mutex);
	ended = sim->sim_ended;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (ended);
}

int	number_of_compiles_reached(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (sim->coders[i].compile_count < sim->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	take_dongles(t_coder *coder)
{
	if (coder->left->id < coder->right->id)
	{
		take_dongle(coder->sim, coder->left, coder);
		log_state(coder->sim, coder->id, "has taken a dongle");
		take_dongle(coder->sim, coder->right, coder);
		log_state(coder->sim, coder->id, "has taken a dongle");
	}
	else
	{
		take_dongle(coder->sim, coder->right, coder);
		log_state(coder->sim, coder->id, "has taken a dongle");
		take_dongle(coder->sim, coder->left, coder);
		log_state(coder->sim, coder->id, "has taken a dongle");
	}
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left, coder->sim);
	release_dongle(coder->right, coder->sim);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *) arg;
	while (!sim_ended(coder->sim))
	{
		log_and_sleep(coder);
		pthread_mutex_lock(&coder->sim->stop_mutex);
		coder->compile_count += 1;
		if (!coder->sim->sim_ended
			&& number_of_compiles_reached(coder->sim))
		{
			coder->sim->sim_ended = 1;
			pthread_mutex_unlock(&coder->sim->stop_mutex);
			wake_all_dongles(coder->sim);
			return (NULL);
		}
		else
			pthread_mutex_unlock(&coder->sim->stop_mutex);
	}
	return (NULL);
}
