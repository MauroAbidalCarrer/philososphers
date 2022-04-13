/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys_calls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 14:06:33 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/13 14:38:48 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	error(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(2, "Error: ", 7);
	write(2, str, i);
}

int	init_mutex(t_mutex *mutex)
{
	int	ret;

	ret = pthread_mutex_init(mutex, NULL);
	if (ret == EAGAIN)
		error("lacked the necessary resources to initialize a mutex\n");
	if (ret == ENOMEM)
		error("Error: Insufficient memory exists to initialize a mutex\n");
	if (ret == EPERM)
		error("Error: do not have the privilege to init mutex\n");
	return (ret);
}

int	ft_malloc(void **add, size_t size)
{
	*add = malloc(size);
	if (add == NULL)
		error("malloc failed\n");
	return (add == NULL);
}

t_time	get_time(void)
{
	struct timeval	tv;
	t_time			time;

	gettimeofday(&tv, NULL);
	time = (t_time)tv.tv_sec * 1000 + (t_time)t_usec / 1000;
	return (time);
}
