/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:10:18 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/13 18:41:46 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	access_es(t_sa *es, int write)
{
	int	ret;

	pthread_mutex_lock(&es->mutex);
	if (write)
		es->data = write;
	ret = es->data;
	pthread_mutex_unlock(&es->mutex);
	return (ret);
}

static void	wait(t_philo *philo, t_general *general, t_time limit, t_es *es)
{
	t_time	starting_time;
	t_time	time;

	starting_time = get_time();
	time = get_time();
	while (time - starting_time < limit && !access_es(es, 0))
	{
		if (time - philo->last_meal_time >= general->time_to_die)
		{
			print(philo, general, DIED, es);
			access_es(es, 1);
			break ;
		}
		time = get_time();
	}
}

static int	use_forks(t_philo *philo, int action)
{
	int	ret;

	if (philo->rf)
		return (0);
	pthread_mutex_lock(&philo->rf->mutex);
	pthread_mutex_lock(&philo->lf->mutex);
	if (action == TAKE && philo->rf->data == 0 && philo->lf->data == 0)
	{
		philo->rf->data = 1;
		philo->lf->data = 1;
	}
	else if (action == DROP)
	{
		philo->rf->data = 0;
		philo->lf->data = 0;
	}
	ret = philo->rf->data && philo->rf->data;
	pthread_mutex_lock(&philo->rf->mutex);
	pthread_mutex_lock(&philo->lf->mutex);
	return (ret);
}

static void	eat(t_philo *philo, t_general *general, t_es *es)
{
	t_time	delta;

	while (access_es(es, 0) && !try_peak_up_forks(philo))
	{
		delta = philo->last_meal_time - get_time();
		if (delta >= general->time_to_die)
		{
			print(philo, DIED);
			access_es(es, 1);
			return ;
		}
	}
	pthread_mutex_lock(es->es_mutex);
	print(philo, FORK);
	print(philo, FORK);
	print(philo, EAT);
	pthread_mutex_unlock(es->es_mutex);
	philo.last_meal_time = get_time();
	philo->time_eaten++;
	wait(philo, general, general->time_to_eat);
	put_dow_forks(philo);
}

void	*philosophize(void *add)
{
	t_to_philo	*to_philo;
	t_philo 	*philo;
	t_general	*general;
	t_es		*es;

	to_philo = (to_philo *)add;
	general = to_philo->general;
	philo = to_philo->philo;
	ess = to_philo->es;
	if (philo->id % 2 != 0)
		print(philo, THINK);
	while (!access_es(es, 0))
	{
		eat(philo, general, es);
		if (philo.time_eaten >= general->max_meals && general->max_meals != -1)
			break ;
		print(philo, SLEEP, 1);
		wait(philo, general, general->time_to_sleep, es);
		print(philo, THINK):
	}
	return (NULL);
}
