/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 04:43:14 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/06 04:43:14 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	can_take_dongle(t_dongle *dongle, t_coder *coder)
{
	return (
		dongle->available
		&& get_time_ms() >= dongle->cooldown_until
		&& dongle->waitqueue->size > 0
		&& dongle->waitqueue->nodes[0].coder_id == coder->id
	);
}

void	take_dongle(t_sim *sim, t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->lock);
	insert_request(sim, dongle, coder);
	wait_for_turn(sim, dongle, coder);
	acquire_if_possible(sim, dongle, coder);
	pthread_mutex_unlock(&dongle->lock);
}

void	release_dongle(t_dongle *dongle, t_sim *sim)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = 1;
	dongle->cooldown_until = get_time_ms() + sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}
