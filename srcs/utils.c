/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:47:06 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/09 15:06:52 by jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_list(t_philo *philo)
{
	t_philo	*next;

	if (!philo)
		return ;
	while (philo)
	{
		next = philo->next;
		pthread_mutex_destroy(&philo->philo_lock);
		free(philo);
		philo = next;
	}
}

void	clean_table(t_table *table)
{
	int	i;

	i = 0;
	if (!table)
		return ;
	pthread_mutex_destroy(&table->table_lock);
	pthread_mutex_destroy(&table->print_lock);
	while (i < table->nb_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	free(table->forks);
}

int	check_value(pthread_mutex_t *mtx, t_table *table)
{
	pthread_mutex_lock(mtx);
	if (table->death)
	{
		pthread_mutex_unlock(mtx);
		return (0);
	}
	pthread_mutex_unlock(mtx);
	return (1);
}

void	safe_print(t_philo *philo, const char *status)
{
	if (check_death(philo))
		return ;
	pthread_mutex_lock(&philo->table->print_lock);
	if (check_value(&philo->table->table_mtx, philo->table))
	{
		printf("%ld %d %s\n", get_time() - philo->table->start_sim,
			philo->philo_id, status);
	}
	pthread_mutex_unlock(&philo->table->print_lock);
}

void	lone_philo(t_table *table)
{
	long	now;

	now = get_time();
	printf("%ld 1 has taken a fork\n", get_time() - now);
	usleep(table->time_to_die * 1000);
	printf("%ld 1 died\n", get_time() - now);
}
