/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 23:08:07 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/07 21:30:21 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_sim_ended(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->sim_ended = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

int	detect_burnout(t_sim *sim, long deadline, int id)
{
	if (deadline > sim->time_to_burnout)
	{
		set_sim_ended(sim);
		// pthread_mutex_unlock(&sim->stop_mutex);
		pthread_mutex_lock(&sim->log_mutex);
		printf("%ld %d burned out\n", get_time_ms() - sim->start_time, id);
		pthread_mutex_unlock(&sim->log_mutex);
		wake_all_dongles(sim);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	long	deadline;

	sim = (t_sim *)arg;
	while (1)
	{
		i = 0;
		while (i < sim->number_of_coders)
		{
			pthread_mutex_lock(&sim->stop_mutex);
			if (!sim->sim_ended)
			{
				pthread_mutex_unlock(&sim->stop_mutex);
				return (NULL);
			}
			deadline =  get_time_ms() - sim->coders[i].last_compile_start;
			if (deadline > sim->time_to_burnout)
			{
				sim->sim_ended = 1;
				pthread_mutex_unlock(&sim->stop_mutex);
				pthread_mutex_lock(&sim->log_mutex);
				printf("%ld %d burned out\n", get_time_ms() - sim->start_time, sim->coders[i].id);
				pthread_mutex_unlock(&sim->log_mutex);
				wake_all_dongles(sim);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->stop_mutex);
			i++;
		}
		usleep(1000);
	}
}


// void	handle_sim_ended(t_sim *sim, int id)
// {
// 	long	current_time;
//
// 	set_sim_ended(sim);
// 	current_time = get_time_ms();
// 	pthread_mutex_lock(&sim->log_mutex);
// 	printf("%ld %d burned out\n", current_time - sim->start_time,
// 			sim->coders[id].id);
// 	pthread_mutex_unlock(&sim->log_mutex);
// }
//
// void	*monitor_routine(void *arg)
// {
// 	t_sim	*sim;
// 	int		i;
//
// 	sim = (t_sim *)arg;
// 	while (!sim->sim_ended)
// 	{
// 		i = 0;
// 		while (i < sim->number_of_coders)
// 		{
// 			if (sim->sim_ended)
// 				return (NULL);
// 			if (get_time_ms() - sim->coders[i].last_compile_start > sim->time_to_burnout)
// 			{
// 				handle_sim_ended(sim, i);
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 		usleep(1000);
// 	}
// 	return (NULL);
// }
