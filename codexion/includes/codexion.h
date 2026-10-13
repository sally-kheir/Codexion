/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skheir <skheir@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/12 12:07:24 by skheir            #+#    #+#             */
/*   Updated: 2026/10/12 12:07:26 by skheir           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_heap_node
{
	int		coder_id;
	long	priority;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node		*nodes;
	int				size;
	int				capacity;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	int				available;
	long			released_at;
	int				id;
	t_heap			queue;
}	t_dongle;

typedef struct s_simulation	t_simulation;

typedef struct s_coder
{
	int				id;
	int				compiles_done;
	long			last_compile_start;
	long			start_delay;
	int				queued;
	pthread_mutex_t	coder_mutex;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	t_simulation	*sim;
}	t_coder;

struct s_simulation
{
	int				nb_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				compiles_required;
	long			dongle_cooldown;
	t_scheduler		scheduler;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	log_lock;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	progress_lock;
	pthread_mutex_t	arb_lock;
	int				stop;
	int				dead;
	int				completed_coders;
	long			start_time;
	long			ticket;
	pthread_t		monitor_thread;
};

int		parse_args(int argc, char **argv, t_simulation *sim);

void	error_exit(const char *msg);
void	usage_error(const char *prog_name);
void	error_exit_field(const char *field_name);

long	get_timestamp_ms(void);
long	elapsed_ms(t_simulation *sim);

void	log_action(t_simulation *sim, int coder_id, const char *action);
void	log_compile_start(t_simulation *sim, int coder_id);
void	log_burnout(t_simulation *sim, int coder_id);

int		init_simulation(t_simulation *sim);

void	heap_init(t_heap *heap, int capacity);
void	heap_push(t_heap *heap, int coder_id, long priority);
int		heap_pop(t_heap *heap);
int		heap_peek(t_heap *heap);
int		heap_remove(t_heap *heap, int coder_id);
void	heap_destroy(t_heap *heap);

long	fifo_priority(t_simulation *sim);
long	edf_priority(t_coder *coder);

int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
long	compute_priority(t_coder *coder);
long	start_delay_for(t_simulation *sim, int id);
void	prequeue_coders(t_simulation *sim);
int		is_stopped(t_simulation *sim);
int		dongle_ready(t_coder *coder, t_dongle *dongle, long now);
int		try_take_dongles(t_coder *coder);
void	do_debug(t_coder *coder);
void	*coder_routine(void *arg);
void	check_compile_requirement(t_coder *coder);
int		coder_can_compile(t_coder *coder);
void	set_compile_start(t_coder *coder);
void	increment_compiles(t_coder *coder);
void	heap_swap(t_heap_node *a, t_heap_node *b);
void	heap_bubble_up(t_heap *heap, int index);
void	heap_bubble_down(t_heap *heap, int index);

void	*monitor_routine(void *arg);
int		check_coder_burnout(t_simulation *sim, int i);
void	set_monitor_stop(t_simulation *sim);

int		run_simulation(t_simulation *sim);

void	cleanup_simulation(t_simulation *sim);

#endif