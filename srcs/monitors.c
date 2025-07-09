/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:50:24 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/09 14:21:15 by jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	time_last_meal(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while (tmp)
	{
		tmp->time_last_meal = tmp->table->start_sim;
		tmp = tmp->next;
	}
}

int	check_death(t_philo *philo)
{
	int	status;

	pthread_mutex_lock(&philo->table->table_lock);
	status = philo->table->death;
	pthread_mutex_unlock(&philo->table->table_lock);
	return (status);
}

int	philo_died(t_philo *philo)
{
	long	last_meal;
	int		max_meals;

	max_meals = philo->table->nb_of_meals;
	if (!philo)
		return (0);
	while (philo)
	{
		pthread_mutex_lock(&philo->philo_lock);
		last_meal = philo->time_last_meal;
		if (max_meals != -1 && philo->meals_count >= max_meals)
		{
			pthread_mutex_unlock(&philo->philo_lock);
			philo = philo->next;
			continue ;
		}
		pthread_mutex_unlock(&philo->philo_lock);
		if (get_time() - last_meal > philo->table->time_to_die)
		{
			safe_print(philo, "died");
			return (1);
		}
		philo = philo->next;
	}
	return (0);
}

int	philo_full(t_philo *philo)
{
	int	meals_max;

	meals_max = philo->table->nb_of_meals;
	if (meals_max == -1 || check_death(philo))
		return (0);
	if (!philo || philo->table->nb_of_meals == -1)
		return (0);
	pthread_mutex_lock(&philo->philo_lock);
	if (philo->meals_count == meals_max)
	{
		pthread_mutex_unlock(&philo->philo_lock);
		pthread_mutex_lock(&philo->table->table_lock);
		philo->table->full++;
		pthread_mutex_unlock(&philo->table->table_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)arg;
	philo = table->philo;
	while (1)
	{
		if (philo_died(philo))
		{
			pthread_mutex_lock(&table->table_lock);
			table->death = 1;
			pthread_mutex_unlock(&table->table_lock);
			break ;
		}
		pthread_mutex_lock(&table->table_lock);
		if (table->full == table->nb_philo)
		{
			pthread_mutex_unlock(&table->table_lock);
			break ;
		}
		pthread_mutex_unlock(&table->table_lock);
	}
	return (NULL);
}
