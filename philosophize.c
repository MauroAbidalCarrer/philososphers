/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:10:18 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/12 22:13:44 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	access_es(t_es es, int write)
{
	int	ret;

	pthread_mutex_lock(es.es_mutex);
	if (write)
		*es.es = write;
	ret = *es.es;
	pthread_mutex_unlock(es.es_mutex);
	return (ret);
}

static void	print(t_philo *philo, char *str, t_es es)
{
	if (get_access(es, 0))
		printf("%d %u %s\n", get_time_of_the_day(), philo.id, str);
}

static void	wait(t_philo *philo, t_general *general, t_time limit, t_es es)
{
	t_time	starting_time;
	t_time	time;

	starting_time = get_time_of_the_day();
	time = get_time_of_the_day();
	while (time - starting_time < limit && !get_access(es, 0))
	{
		if (time - philo->last_meal_time >= general->time_to_die)
		{
			print(philo, DIED);
			access_es(es, 1);
			break ;
		}
		time = get_time_of_the_day();
	}
}

static void	eat(t_philo *philo, t_general *general, t_es es)
{
	t_time	delta;

	while (!philo->rf || philo.rf->lock || philo.lf->lock && access_es(es, 0))
	{
		delta = philo->last_meal_time - get_time_of_the_day();
		if (delta >= general->time_eaten)
		{
			print(philo, DIED);
			access_es(es, 1);
			return ;
		}
	}
	mutex_lock(philo.rf);
	mutex_lock(philo.lf);
	print(philo, FORK);
	print(philo, FORK);
	print(philo, EAT);
	philo.last_meal_time = get_time_of_the_day();
	philo->time_eaten++;
	wait(philo, general, general->time_to_eat);
	pthread_mutex_unlock(philo->rf);
	pthread_mutex_unlock(philo->lf);
}

void	*philosophize(void *add)
{
	t_to_philo	*to_philo;
	t_philo 	*philo;
	t_general	*general;
	t_es		es;

	to_philo = (to_philo *)add;
	general = to_philo->general;
	philo = to_philo->philo;;
	es.es = to_philo->es;
	es.es_mutex = to_philo->es_mutex;
	if (philo->id % 2 != 0)
		print(philo, THINK);
	while (philo.time_eaten < general->max_meals && !access_es(es, 0))
	{
		eat(philo, general, es);
		print(philo, SLEEP, 1);
		wait(philo, general, general->time_to_sleep, es);
		print(philo, THINK):
	}
	return (NULL);
}
