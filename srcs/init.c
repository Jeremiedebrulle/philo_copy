/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:49:09 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/09 15:06:20 by jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	table_init(t_table *table, char **av)
{
	table->nb_philo = ft_atol_philo(av[1]);
	table->time_to_die = ft_atol_philo(av[2]);
	table->time_to_eat = ft_atol_philo(av[3]);
	table->time_to_sleep = ft_atol_philo(av[4]);
	if (av[5])
		table->nb_of_meals = ft_atol_philo(av[5]);
	else
		table->nb_of_meals = -1;
	table->full = 0;
	table->death = 0;
	table->start_sim = 0;
	table->philo = NULL;
	pthread_mutex_init(&table->table_lock, NULL);
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->table_mtx, NULL);
}

void	init_forks(t_table *table, int count)
{
	int		i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
}

t_philo	*philo_init(t_table *table, int id)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->philo_id = id;
	pthread_mutex_init(&philo->philo_lock, NULL);
	philo->left_fork = &table->forks[id - 1];
	philo->right_fork = 0;
	philo->forks_taken = 0;
	philo->table = table;
	philo->next = NULL;
	philo->meals_count = 0;
	philo->time_last_meal = 0;
	return (philo);
}

t_philo	*create_philos(t_table *table, int count)
{
	static int	i = 0;
	t_philo		*head;
	t_philo		*curr;
	t_philo		*new;

	head = NULL;
	curr = NULL;
	while (i < count)
	{
		new = philo_init(table, (i + 1));
		if (!new)
			return (NULL);
		if (!head)
		{
			head = new;
			curr = head;
		}
		else
		{
			curr->next = new;
			curr = new;
		}
		i++;
	}
	return (head);
}
