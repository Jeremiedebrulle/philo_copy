/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jdebrull <jdebrull@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:01:18 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/07 19:58:31 by Jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define FALSE 0
# define TRUE 1

typedef struct s_philo
{
	int				philo_id;
	pthread_mutex_t	philo_lock;
	pthread_t		philo_thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				meals_count;
	long			time_last_meal;
	struct s_table	*table;
	struct s_philo	*next;
}					t_philo;

typedef struct s_table
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_of_meals;
	int				full;
	int				death;
	long			start_sim;
	pthread_mutex_t	table_lock;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_t		monitor_death;
	t_philo			*philo;
}					t_table;

int		only_digits(char *string);
int		pars_input(char **av);

//UTILS
void	exit_error(char *str);
long	get_time(void);
void	ft_usleep(t_philo *philo, long	time_ms);
int		check_death(t_philo *philo);
void	safe_print(t_philo *philo, const char *status);
void	free_list(t_philo *philo);
void	clean_table(t_table *table);

#endif