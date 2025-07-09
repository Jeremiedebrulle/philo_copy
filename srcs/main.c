/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:02:35 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/09 15:15:27 by jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	take_forks(t_philo *philo)
{
	if (check_death(philo))
		return ;
	philo->forks_taken = 0;
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		philo->forks_taken++;
		safe_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		philo->forks_taken++;
		safe_print(philo, "has taken a fork");
	}
	else
	{
		ft_usleep(philo, philo->table->time_to_eat * 0.9);
		pthread_mutex_lock(philo->right_fork);
		philo->forks_taken++;
		safe_print(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		philo->forks_taken++;
		safe_print(philo, "has taken a fork");
	}
}

void	eating(t_philo *philo)
{
	t_table	*table;
	long	now;

	table = philo->table;
	safe_print(philo, "is eating");
	now = get_time();
	pthread_mutex_lock(&philo->philo_lock);
	philo->time_last_meal = now;
	philo->meals_count++;
	pthread_mutex_unlock(&philo->philo_lock);
	ft_usleep(philo, table->time_to_eat);
}

void	sleeping(t_philo *philo)
{
	safe_print(philo, "is sleeping");
	ft_usleep(philo, philo->table->time_to_sleep);
}

void	drop_forks(t_philo *philo)
{
	if (philo->forks_taken >= 1)
		pthread_mutex_unlock(philo->right_fork);
	if (philo->forks_taken == 2)
		pthread_mutex_unlock(philo->left_fork);
	philo->forks_taken = 0;
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (check_death(philo))
		return (NULL);
	while (!check_death(philo) && !philo_full(philo))
	{
		take_forks(philo);
		if (check_death(philo) || philo_full(philo))
		{
			drop_forks(philo);
			break ;
		}
		eating(philo);
		drop_forks(philo);
		sleeping(philo);
	}
	return (NULL);
}

t_philo	*start(t_table *table)
{
	t_philo	*philo;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		exit(1);
	philo = create_philos(table, table->nb_philo);
	table->philo = philo;
	init_forks(table, table->nb_philo);
	if (!philo)
		printf("create philo failed!\n");
	table->start_sim = get_time();
	time_last_meal(philo);
	return (philo);
}
void	dinner_sim(t_table *table)
{
	t_philo	*philo;

	philo = start(table);
	t_philo *tmp = philo;
	tmp = philo;
	while (tmp)
	{
		if (tmp->next)
			tmp->right_fork = tmp->next->left_fork;
		else
			tmp->right_fork = philo->left_fork;
		if (pthread_create(&tmp->philo_thread, NULL, &routine, tmp) != 0)
    		printf("pthread_create failed\n");
		tmp = tmp->next;
	}
	if (pthread_create(&table->monitor_death, NULL, &monitor_death, table))
   		printf("pthread_create failed\n");
	pthread_join(table->monitor_death, NULL);
	tmp = philo;
	while (tmp)
	{
		pthread_join(tmp->philo_thread, NULL);
		tmp = tmp->next;
	}
	free_list(philo);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
		return (printf("not the right number of args\n Try with number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"), 0);
	else
	{
		table_init(&table, av);
		if (table.nb_philo == 1)
		{
			lone_philo(&table);
			pthread_mutex_destroy(&table.table_lock);
			pthread_mutex_destroy(&table.print_lock);
			return (0);
		}
		dinner_sim(&table);
		clean_table(&table);
	}
}