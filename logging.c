/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 20:01:18 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/10 11:00:00 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_sim *sim, int coder_id, char *msg)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (!sim_ended(sim))
	{
		ft_putnbr_fd(get_time_ms() - sim->start_time, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(coder_id, 1);
		ft_putstr_fd(" ", 1);
		ft_putstr_fd(msg, 1);
		ft_putstr_fd("\n", 1);
	}
	pthread_mutex_unlock(&sim->log_mutex);
}
