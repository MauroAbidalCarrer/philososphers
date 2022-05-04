/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 00:58:43 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/04 17:42:41 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

//real_time is in milli seconds
static void print(int id, char *str)
{
	struct timeval	tv;
	t_time			real_time;

	gettimeofday(&tv, NULL);
	real_time = (t_time)tv.tv_sec * 1000 + (t_time)tv.tv_usec / 1000;
	printf("%ld %d %\n", real_time, id, str);
}

//real_time is in micro seconds
//should check for deaths even if tt_wait is = 0
//for cases where tt_die/eat/sleep = 0
static int	_wait(t_philo me, t_general *g, t_time tt_wait, int is_eating)
{
	struct timeval	tv;
	t_time			real_time;

	gettimeofday(&tv, NULL);
	real_time = (t_time)tv.tv_sec * 1000000 + (t_time)tv.tv_usec;
	tt_wait += g->theo_time - real_time - g->sim_start;
	usleep(tt_wait * 1000);
	g->theo_time += tt_wait;
}

static int	_sleep(t_philo *me, t_general g)
{
	print(me, sleep);
	if (_wait(me, g, g.tt_sleep))
		return (1);
	print(me->id, THINK);
	return (_wait(me, g, g.tt_wait));
}

//think about the case where nb_philo = 1
//think about the case where tt_die < tt_eat
static int	eat(t_philo *me, t_general g)
{
	int	es;

	if (sem_wait(sem) == -1 || sem_wait(sem) == -1)
		return (1);
	print(me->id, FORK);
	print(me->id, FORK);
	print(me->id, EAT);
	es = _wait(me, g, g.tt_eat);
	if (sem_post(sem) == -1 || sem_post(sem) == -1)
		return (1);
	me->last_meal = g->theoretical_time;
	me->nb_eat++;
	return (es || (g->nb_eat != -1 && me->nb_eat >= g->nb_eat));
}

//nb_eat is set to -1 in case nb_eat and tt_die = 0
void	philosophize(int id, t_general g, sem_t *sem)
{
	t_philo	me;

	me.type = id % 2 + (id != 0 && nb_philo % 2);
	me.nb_eat = 0;
	me.id = id;
	me.last_meal = g.sim_start;
	me.theoretical_time = 0;
	if (_wait(me, g, me.type * g->tt_eat))
	{
		sem_close(sem);
		exit(!(g.nb_philo != -1 && me.nb_philo == g.nb_eat));
	}
	if (g->nb_eat != 0)
	{
		print(me, THINK);
		while (eat(me, g) || _sleep(me, g))
			;
	}
	sem_close(sem);
	exit(!(g.nb_philo != -1 && me.nb_philo == g.nb_eat));
}
