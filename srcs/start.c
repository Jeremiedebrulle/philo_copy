/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:47:25 by jdebrull          #+#    #+#             */
/*   Updated: 2025/07/09 14:21:45 by jdebrull         ###   ########.fr       */
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

void	ft_usleep(t_philo *philo, long time_ms)
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
		exit_error("Error try ./philo nb_philo die eat sleep [meals]");
	return (num);
}
