/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:47:06 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/04 18:20:20 by jdebrull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	while ((get_time() - start) < time_ms)
	{
		if (check_death(philo))
			break ;
		usleep(100);
	}
}
