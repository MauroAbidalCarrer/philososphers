/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 14:06:33 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/13 21:13:20 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	print(t_philo *philo, t_general *general, char *str, t_sa *es)
{
	t_time	c_time;

	if (access_es(es, 0))
	{
		c_time = get_time() - general->sim_start;
		printf("%ld %d %s\n", c_time, philo->id, str);
	}
}

int	init_sa(t_sa *sa)
{
	int	ret;

	ret = pthread_mutex_init(&sa->mutex, NULL);
	if (ret == EAGAIN)
		write(2, EAGAIN_MSG, 53);
	else if (ret == ENOMEM)
		write(2, ENOMEM_MSG, 49);
	else if (ret == EPERM)
		write(2, EPERM_MSG, 41);
	sa->data = 0;
	return (ret);
}

int	ft_malloc(void **add, size_t size)
{
	*add = malloc(size);
	if (add == NULL)
		printf("malloc failed\n");
	return (add == NULL);
}

t_time	get_time(void)
{
	struct timeval	tv;
	t_time			time;

	gettimeofday(&tv, NULL);
	time = (t_time)tv.tv_sec * 1000 + (t_time)tv.tv_usec / 1000;
	return (time);
}

int	ft_atoi(const char *nptr)
{
	long	sign;
	long	nb;

	while (*nptr == ' ')
		nptr++;
	sign = 1 - 2 * (*nptr == '-');
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	nb = 0;
	while (*nptr >= '0' && *nptr <= '9')
		nb = nb * 10 + *nptr++ - '0';
	return ((int)(nb * sign));
}
