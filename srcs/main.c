/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:02:35 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/04 18:19:54 by jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	ft_atol_philo(char *str)
{
	int		i;
	long	num;

	i = 0;
	num = 0;
	if (!str)
		exit_error("Error void string");
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		exit_error("Number is not positive.");
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i] - 48);
		if (num > 2147483647)
			exit_error("Number is to big.");
		i++;
	}
	if (num == 0)
		exit_error("Error arguments try ./philo nb_philo time_die time_eat time_sleep [nb_of_meals]");
	return (num);
}

void	table_init(t_table *table, char **av) // need to check later why it still works when i write /philo 5.5 40.0 ... as input 
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
	philo->table = table;
	philo->next = NULL;
	philo->time_last_meal = 0;
	return (philo);
}

t_philo	*create_philos(t_table *table, int count)
{
	int		i;
	t_philo	*head;
	t_philo	*curr;
	t_philo	*new;

	head = NULL;
	curr = NULL;
	i = 0;
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

void	take_forks(t_philo *philo)
{
	if (check_death(philo))
		return ;
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", get_time() - philo->table->start_sim, philo->philo_id);
		pthread_mutex_unlock(&philo->table->print_lock);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", get_time() - philo->table->start_sim, philo->philo_id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
	else
	{
		ft_usleep(philo, philo->table->time_to_eat * 0.9);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", get_time() - philo->table->start_sim, philo->philo_id);
		pthread_mutex_unlock(&philo->table->print_lock);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", get_time() - philo->table->start_sim, philo->philo_id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
}

void	eating_sleeping(t_philo *philo)
{
	t_table	*table;
	long	now;

	table = philo->table;
	now = get_time();
	if (check_death(philo))
		return ;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%ld %d is eating\n", now - table->start_sim, philo->philo_id);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->philo_lock);
	philo->time_last_meal = now;
	philo->meals_count++;
	pthread_mutex_unlock(&philo->philo_lock);
	pthread_mutex_unlock(philo->left_fork);
	ft_usleep(philo, table->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	ft_usleep(philo, table->time_to_sleep);
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%ld %d is sleeping\n", get_time() - table->start_sim, philo->philo_id);
	pthread_mutex_unlock(&philo->table->print_lock);
}

int	philo_full(t_philo *philo)
{
	int	meals_max;

	meals_max = philo->table->nb_of_meals;
	if (meals_max == -1)
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
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d is full\n", get_time() - philo->table->start_sim, philo->philo_id);
		pthread_mutex_unlock(&philo->table->print_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("Philo %d started\n", philo->philo_id);
	pthread_mutex_unlock(&philo->table->print_lock);

	while (1)
	{
		if (check_death(philo))
			break ;
		if (philo_full(philo))
			break ;
		take_forks(philo);
		eating_sleeping(philo);

	}
	return (NULL);
}

void	time_last_meal(t_philo *philo)
{
	t_philo	*tmp;

	tmp = philo;
	while(tmp)
	{
		tmp->time_last_meal = tmp->table->start_sim;
		tmp = tmp->next;
	}
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
			pthread_mutex_lock(&philo->table->print_lock);
			printf("%ld %d died\n", get_time() - philo->table->start_sim, philo->philo_id);
			pthread_mutex_unlock(&philo->table->print_lock);
			return (1);
		}
		philo = philo->next;
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_table	*table;
	t_philo *philo;

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

void	dinner_sim(t_table *table)
{
	t_philo	*philo;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		exit(1);
	philo = create_philos(table ,table->nb_philo);
	table->philo = philo;
	init_forks(table, table->nb_philo);
	if (!philo)
		printf("create philo failed!\n");
	table->start_sim = get_time();
	time_last_meal(philo);
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
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
		return (printf("not the right number of args\n Try with number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"), 0);
	else
	{
		table_init(&table, av);
		dinner_sim(&table);
	}
}