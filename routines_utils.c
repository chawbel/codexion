/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 20:16:06 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/06 20:16:06 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_and_sleep(t_coder *coder)
{
	take_dongles(coder);
	pthread_mutex_lock(&coder->sim->stop_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->sim->stop_mutex);
	if (sim_ended(coder->sim))
		return ;
	log_state(coder->sim, coder->id, "is compiling");
	usleep(coder->sim->time_to_compile * 1000);
	release_dongles(coder);
	if (sim_ended(coder->sim))
		return ;
	log_state(coder->sim, coder->id, "is debugging");
	usleep(coder->sim->time_to_debug * 1000);
	if (sim_ended(coder->sim))
		return ;
	log_state(coder->sim, coder->id, "is refactoring");
	usleep(coder->sim->time_to_refactor * 1000);
}
