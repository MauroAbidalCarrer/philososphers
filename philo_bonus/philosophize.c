/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 00:58:43 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/04 02:38:52 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void print(int id, char *str)
{
	printf("%ld %d %\n", get_time(), id, str);
}

static int	_wait(t_philo *me, t_general g, t_time tt_wait)
{

	me->theoretical_time += tt_wait;
}

static int	_sleep(t_philo *me, t_general g)
{
	print(me, sleep);
	if (_wait(me, g, g.tt_sleep))
		return (1);
	return (_wait(me, g, g.tt_wait));
}

static int	eat(t_philo *me, t_general g, sem_t *sem)
{
	int	es;

	if (sem_wait(sem) == -1 || sem_wait(sem) == -1)
		return (1);
	if (g->nb_eat != -1 && me->nb_eat >= g->nb_eat)
		return (1);
	print(me->id, FORK);
	print(me->id, FORK);
	print(me->id, EAT);
	es = _wait(me, g, g.tt_eat);//think about the case where tt_die < tt_eat
	if (sem_post(sem) == -1 || sem_post(sem) == -1)
		return (1);
	me.last_meal = me->theoretical_time;
	return (es);
}

void	philosophize(int id, t_general g, sem_t *sem)
{
	t_philo	me;

	me.type = id % 2 + (id != 0 && nb_philo % 2);
	me.nb_eat = 0;
	me.id = id;
	me.last_meal = g.sim_start;
	me.theoretical_time = 0;
	if (g.tt_die == 0)
	{
		if (id == 1)
			print(id, DIED, sem);
		sem_close(sem);
		exit(1);
	}
	if (_wait(me, g, me.type * g->tt_eat))
		;
	else while (eat(me, g) || _sleep(me, g))
		;
	sem_close(sem);
	exit(!(g.nb_philo != -1 && me.nb_philo == g.nb_eat));
}
