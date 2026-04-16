/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_operations_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 19:52:26 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/08 02:32:35 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	insert_request(t_sim *sim, t_dongle *dongle, t_coder *coder)
{
	t_heap_node	node;

	node.coder_id = coder->id;
	if (sim->scheduler == EDF)
	{
		pthread_mutex_lock(&sim->stop_mutex);
		node.priority = coder->last_compile_start + sim->time_to_burnout;
		pthread_mutex_unlock(&sim->stop_mutex);
	}
	else
		node.priority = get_time_ms();
	heap_insert(dongle->waitqueue, node);
}

void	timedwait(t_dongle *dongle, struct timespec *ts, long wait_until)
{
	ts->tv_sec = wait_until / 1000;
	ts->tv_nsec = (wait_until % 1000) * 1000000L;
	pthread_cond_timedwait(&dongle->cond, &dongle->lock, ts);
}

void	wait_for_turn(t_sim *sim, t_dongle *dongle, t_coder *coder)
{
	long			now;
	long			wait_until;
	struct timespec	ts;

	while (1)
	{
		if (sim_ended(sim))
			return ;
		if (!sim_ended(sim) && can_take_dongle(dongle, coder))
			return ;
		if (!dongle->available
			|| dongle->waitqueue->nodes[0].coder_id != coder->id)
			pthread_cond_wait(&dongle->cond, &dongle->lock);
		else
		{
			now = get_time_ms();
			wait_until = dongle->cooldown_until;
			if (wait_until <= now)
				continue ;
			timedwait(dongle, &ts, wait_until);
		}
	}
}

void	acquire_if_possible(t_sim *sim, t_dongle *dongle, t_coder *coder)
{
	if (!sim_ended(sim) && can_take_dongle(dongle, coder))
	{
		heap_extract(dongle->waitqueue);
		dongle->available = 0;
	}
}

void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}
