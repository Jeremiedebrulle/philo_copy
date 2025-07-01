/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdebrull <jdebrull@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:47:06 by jdebrull          #+#    #+#             */
/*   Updated: 2025/06/30 17:49:37 by jdebrull         ###   ########.fr       */
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

void	ft_usleep(long	time_ms)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time_ms)
		usleep(100);
}