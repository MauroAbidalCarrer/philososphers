/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:10:18 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/10 16:46:33 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	print(t_philo *me, t_general *g, t_sa *es, char *str)
{
	int	ret;

	pthread_mutex_lock(&es->mutex);
	if (!es->data)
		printf("%ld %d %s\n", get_time() - g->sim_start, me->id + 1, str);
	if (str_equal(str, DIED))
		es->data = 1;
	ret = es->data;
	pthread_mutex_unlock(&es->mutex);
	return (ret);
}

static int	wait(t_philo *me, t_general *g, t_sa *es, t_time wait_time)
{
	t_time	c_time;

	c_time = get_time();
//printf("	id = %d, wait_time = %lu, last_meal = %lu", me->id, wait_time, me->last_meal - g->sim_start);
	if (c_time + wait_time - me->last_meal >= g->tt_die)
	{
//printf(" GONNA DIE\n");
printf("	id = %d, tt_die = %lu\n", me->id, me->last_meal + g->tt_die - c_time);
		usleep((me->last_meal + g->tt_die - c_time) * 1000);
		print(me, g, es, DIED);
		return (1);
	}
	else
	{
//printf(" NOT gonna die\n");
		usleep(wait_time * 1000);
		return (0);
	}
}

//sleep already exists
static int	_sleep(t_philo *me, t_general *g, t_sa *es)
{
	if (print(me, g, es, SLEEP) || wait(me, g, es, g->tt_sleep))
		return (1);
	return (print(me, g, es, THINK) || wait(me, g, es, g->tt_wait));
}

//philo->time_eaten >= g->max_meals && g->max_meals != -1
static int	eat(t_philo *philo, t_general *g, t_sa *es)
{	
int	i;

	if (philo->time_eaten >= g->nb_eat && g->nb_eat != -1)
		return (1);
	if (!philo->lf)
		return (wait(philo, g, es, g->tt_die));
	philo->last_meal = get_time();
	pthread_mutex_lock(philo->rf);
	pthread_mutex_lock(philo->lf);
i = print(philo, g, es, FORK);
i = (i || print(philo, g, es, FORK));
i = (i || print(philo, g, es, EAT));
i = (i || wait(philo, g, es, g->tt_eat));
	pthread_mutex_unlock(philo->rf);
	pthread_mutex_unlock(philo->lf);
	philo->time_eaten++;
	return (i || (philo->time_eaten >= g->nb_eat && g->nb_eat != -1));
}

void	*philosophize(void *add)
{
	t_to_philo	*to_philo;
	t_philo		*me;
	t_general	*g;
	t_sa		*es;

	to_philo = (t_to_philo *)add;
	g = to_philo->general;
	me = to_philo->philo;
	es = to_philo->es;
	if (print(me, g, es, THINK))
		return (NULL);
	if (wait(me, g, es, me->type * g->tt_eat))
		return (NULL);
	while (!eat(me, g, es) && !_sleep(me, g, es))
		;
	return (NULL);
}
