/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 20:01:18 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/07 19:33:30 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_sim *sim, int coder_id, char *msg)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (!sim->sim_ended)
		printf("%ld %d %s\n", get_time_ms() - sim->start_time, coder_id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}
