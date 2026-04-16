/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 02:13:45 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/07 16:48:52 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

void	destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->lock);
	pthread_cond_destroy(&dongle->cond);
	free(dongle->waitqueue->nodes);
	free(dongle->waitqueue);
}

void	cleanup_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		destroy_dongle(&sim->dongles[i]);
		pthread_mutex_destroy(&sim->coders[i].sleep_lock);
		pthread_cond_destroy(&sim->coders[i].sleep_cond);
		i++;
	}
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->monitor_sleep_lock);
	pthread_cond_destroy(&sim->monitor_sleep_cond);
	free(sim->dongles);
	free(sim->coders);
}
