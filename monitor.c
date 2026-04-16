/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 23:08:07 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/10 11:30:00 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_coder_burnout(t_sim *sim, int i)
{
	long	deadline;

	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->sim_ended)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return (1);
	}
	deadline = get_time_ms() - sim->coders[i].last_compile_start;
	if (deadline > sim->time_to_burnout)
	{
		sim->sim_ended = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		pthread_mutex_lock(&sim->log_mutex);
		ft_putnbr_fd(get_time_ms() - sim->start_time, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(sim->coders[i].id, 1);
		ft_putstr_fd(" burned out\n", 1);
		pthread_mutex_unlock(&sim->log_mutex);
		wake_all_dongles(sim);
		return (1);
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			if (check_coder_burnout(sim, i))
				return (NULL);
			i++;
		}
		ft_usleep_monitor(500, sim);
	}
}
