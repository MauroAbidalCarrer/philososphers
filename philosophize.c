/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:10:18 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/16 19:01:27 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	print(t_philo *philo, t_general *general, char *str, t_sa *es)
{
	t_time	c_time;

	pthread_mutex_lock(&es->mutex);
	if (!es->data)
	{
		c_time = get_time();
		printf("%ld %d %s\n", c_time - general->sim_start, philo->id, str);
		es->data = (str == DIED);
	}
	pthread_mutex_unlock(&es->mutex);
}

static void	wait(t_philo *philo, t_general *general, t_time limit, t_sa *es)
{
	t_time	c_time;

	if (access_sa(es, 0))
		return ;
	c_time = get_time();
	if (philo->last_meal_time + general->time_to_die < c_time + limit)
	{
		usleep((philo->last_meal_time + general->time_to_die - c_time) * 1000);
		pthread_mutex_lock(&es->mutex);
		c_time = get_time() - general->sim_start;
		if (!es->data)
			printf("%ld %d %s\n", c_time, philo->id, DIED);
		es->data = 1;
		pthread_mutex_unlock(&es->mutex);
	}
	else
		usleep(limit * 1000);
}

static int	use_forks(t_philo *philo, t_general *general, int action)
{
	int	ret;

	if (!philo->lf)
		return (0);
	pthread_mutex_lock(&philo->rf->mutex);
	pthread_mutex_lock(&philo->lf->mutex);
	pthread_mutex_lock(&general->type_to_eat->mutex);
	ret = (philo->rf && philo->lf && general->type_to_eat->data == philo->type);
	if (action == TAKE && ret)
	{
		philo->rf->data = 0;
		philo->lf->data = 0;
	}
	else if (action == DROP)
	{
		philo->rf->data = 1;
		philo->lf->data = 1;
		general->type_to_eat->data = (philo->type + 1) % general->moduler;
	}
	pthread_mutex_unlock(&general->type_to_eat->mutex);
	pthread_mutex_unlock(&philo->lf->mutex);
	pthread_mutex_unlock(&philo->rf->mutex);
	usleep(0);
	return (ret);
}

static void	eat(t_philo *philo, t_general *general, t_sa *es)
{
	t_time	time_to_peak_up;
	t_time	time;

	time_to_peak_up = get_time();
	while (!use_forks(philo, general, TAKE))
	{
		if (get_time() - philo->last_meal_time >= general->time_to_die)
			return (print(philo, general, DIED, es));
	}
	pthread_mutex_lock(&es->mutex);
	if (es->data)
	{
		pthread_mutex_unlock(&es->mutex);
		return ;
	}
	time = get_time();
	printf("%ld %d %s\n", time - general->sim_start, philo->id, FORK);
	printf("%ld %d %s\n", time - general->sim_start, philo->id, FORK);
	printf("%ld %d %s\n", time - general->sim_start, philo->id, EAT);
	pthread_mutex_unlock(&es->mutex);
	philo->time_eaten++;
	time_to_peak_up = time - time_to_peak_up;
	usleep(general->time_to_eat * 1000 - time_to_peak_up * 0.65);
	use_forks(philo, general, DROP);
	philo->last_meal_time = get_time();
}

void	*philosophize(void *add)
{
	t_to_philo	*to_philo;
	t_philo		*philo;
	t_general	*general;
	t_sa		*es;

	to_philo = (t_to_philo *)add;
	general = to_philo->general;
	philo = to_philo->philo;
	es = to_philo->es;
	while (!access_sa(es, 0))
	{
		print(philo, general, THINK, es);
		if (philo->time_eaten >= general->max_meals && general->max_meals != -1)
			break ;
		eat(philo, general, es);
		print(philo, general, SLEEP, es);
		wait(philo, general, general->time_to_sleep, es);
	}
	return (NULL);
}
