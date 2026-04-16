/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbahry <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 01:50:23 by cbahry            #+#    #+#             */
/*   Updated: 2026/04/04 20:11:36 by cbahry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_scheduler {
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_heap_node {
	int		coder_id;
	long	priority;
}	t_heap_node;

typedef struct s_heap {
	t_heap_node	*nodes;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle {
	int				available;
	long			cooldown_until;
	int				id;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	t_heap			*waitqueue;
}	t_dongle;

typedef struct s_coder {
	int				id;
	pthread_t		t;
	t_dongle		*left;
	t_dongle		*right;
	long			last_compile_start;
	int				compile_count;
	struct s_sim	*sim;
	pthread_mutex_t	sleep_lock;
	pthread_cond_t	sleep_cond;
}	t_coder;

typedef struct s_sim {
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	t_scheduler		scheduler;
	t_dongle		*dongles;
	t_coder			*coders;
	int				sim_ended;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_t		monitor;
	pthread_mutex_t	monitor_sleep_lock;
	pthread_cond_t	monitor_sleep_cond;
	long			start_time;
}	t_sim;

char	**ft_split(char const *s, char c);
void	ft_to_lower(char *str);
int		print_error(char *error_msg);
int		is_number(const char *str);
void	parse_args(char **str, int argc, t_sim *sim);
size_t	ft_count_words(char const *s, char c);
char	*ft_strdup(const char *s);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(long n, int fd);
void	help(void);
void	init_sim(t_sim *sim);
void	init_coder(t_coder *coder, t_sim *sim, int id);
void	init_dongle(t_dongle *dongle, t_sim *sim, int id);
void	init_arrays(t_sim *sim);
long	get_time_ms(void);
void	log_state(t_sim *sim, int id, char *msg);
void	*coder_routine(void *arg);
void	spawn_threads(t_sim *sim);
void	swap(t_heap_node *a, t_heap_node *b);
void	heap_insert(t_heap *heap, t_heap_node node);
int		heap_extract(t_heap *heap);
void	heap_init(t_heap *heap, t_sim *sim);
void	take_dongle(t_sim *sim, t_dongle *dongle, t_coder *coder);
void	release_dongle(t_dongle *dongle, t_sim *sim);
int		sim_ended(t_sim *sim);
int		can_take_dongle(t_dongle *dongle, t_coder *coder);
void	insert_request(t_sim *sim, t_dongle *dongle, t_coder *coder);
void	wait_for_turn(t_sim *sim, t_dongle *dongle, t_coder *coder);
void	acquire_if_possible(t_sim *sim, t_dongle *dongle, t_coder *coder);
void	release_dongles(t_coder *coder);
void	take_dongles(t_coder *coder);
void	log_and_sleep(t_coder *coder);
void	wake_all_dongles(t_sim *sim);
void	*monitor_routine(void *arg);
int		check_coder_burnout(t_sim *sim, int i);
void	cleanup_sim(t_sim *sim);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(long n, int fd);
void	ft_usleep(size_t microseconds, t_coder *coder);
void	ft_usleep_monitor(size_t microseconds, t_sim *sim);

#endif
