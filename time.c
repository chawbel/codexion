/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 18:36:41 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/05 18:36:41 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	ft_usleep(size_t microseconds, t_coder *coder)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (microseconds / 1000000);
	ts.tv_nsec = (tv.tv_usec * 1000) + ((microseconds % 1000000) * 1000);
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	pthread_mutex_lock(&coder->sleep_lock);
	pthread_cond_timedwait(&coder->sleep_cond, &coder->sleep_lock, &ts);
	pthread_mutex_unlock(&coder->sleep_lock);
}

void	ft_usleep_monitor(size_t microseconds, t_sim *sim)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (microseconds / 1000000);
	ts.tv_nsec = (tv.tv_usec * 1000) + ((microseconds % 1000000) * 1000);
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	pthread_mutex_lock(&sim->monitor_sleep_lock);
	pthread_cond_timedwait(&sim->monitor_sleep_cond,
		&sim->monitor_sleep_lock, &ts);
	pthread_mutex_unlock(&sim->monitor_sleep_lock);
}
