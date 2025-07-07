/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jdebrull <jdebrull@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:47:06 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/07 20:05:29 by Jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_list(t_philo *philo)
{
	t_philo	*next;

	if (!philo)
		return ;
	while(philo)
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

void	exit_error(char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

long	get_time(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (long)1000 + tv.tv_usec / (long)1000);
}

int	check_death(t_philo *philo)
{
	int status;

	pthread_mutex_lock(&philo->table->table_lock);
	status = philo->table->death;
	pthread_mutex_unlock(&philo->table->table_lock);
	return (status);
}

void	ft_usleep(t_philo *philo, long	time_ms)
{
	long	start;

	start = get_time();
	if (check_death(philo))
		return ;
	while ((get_time() - start) < time_ms)
	{
		if (check_death(philo))
			break ;
		usleep(100);
	}
}

void	safe_print(t_philo *philo, const char *status)
{
    if (check_death(philo))
        return;
    pthread_mutex_lock(&philo->table->print_lock);
    printf("%ld %d %s\n", get_time() - philo->table->start_sim, philo->philo_id, status);
    pthread_mutex_unlock(&philo->table->print_lock);
}
