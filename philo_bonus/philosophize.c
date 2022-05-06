/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 00:58:43 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/06 06:02:23 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

//real_time is in milli seconds
static void print(int id, char *str, t_general g)
{
	struct timeval	tv;
	t_time			real_time;

	gettimeofday(&tv, NULL);
	real_time = (t_time)tv.tv_sec * K + (t_time)tv.tv_usec / K;
	printf("%llu %d %s\n", real_time - g.sim_start / K, id + 1, str);
}

//should check for deaths even if tt_wait is = 0
//for cases where tt_die/eat/sleep = 0
static int	_wait(t_philo *me, t_general *g, t_time tt_wait)
{
	struct timeval	tv;
	t_time			real_time;
	int				type;
	t_time			utt_wait;

	type = -1;
	tt_wait = type_dies_before(*g, 0, tt_wait, &type, me->id);
	tt_wait = type_dies_before(*g, 1, tt_wait, &type, me->id);
	if (g->nb_philo % 2 == 1)
		tt_wait = type_dies_before(*g, 2, tt_wait, &type, me->id);
	gettimeofday(&tv, NULL);
	real_time = (t_time)tv.tv_sec * M + (t_time)tv.tv_usec;
	utt_wait = K * tt_wait;

/*
printf("	id = %d, utt_wait = %llu \n", me->id, utt_wait);
printf("	id = %d, time since start = %llu\n", me->id, real_time - g->sim_start);
printf("	id = %d, time gap = %llu\n", me->id, real_time - g->sim_start - g->theo_time * K);
printf("	id = %d, type = %d\n", me->id, type);
*/

	if (real_time - g->sim_start - g->theo_time * K <= utt_wait)
	{
		utt_wait -= real_time - g->sim_start - g->theo_time * K;
		printf("	id = %d, final utt_wait = %llu\n\n", me->id, utt_wait);
		usleep(utt_wait);
	}
	if (type == me->id)
		print(me->id, DIED, *g);
	g->theo_time += tt_wait;
//printf("		id = %d, theo_time = %llu\n", me->id, g->theo_time);
	return (type != -1);
}

static int	_sleep(t_philo *me, t_general *g)
{
	print(me->id, SLEEP, *g);
	if (_wait(me, g, g->tt_sleep))
		return (1);
	print(me->id, THINK, *g);
	return (_wait(me, g, g->tt_wait));
}

static int	eat(t_philo *me, t_general *g, sem_t *sem)
{
	int	es;

	if (sem_wait(sem) == -1)
		return (1);
	print(me->id, FORK, *g);
	if (sem_wait(sem) == -1)
		return (1);
	print(me->id, FORK, *g);
	print(me->id, EAT, *g);
	es = _wait(me, g, g->tt_eat);
	if (sem_post(sem) == -1 || sem_post(sem) == -1)
		return (1);
	me->nb_eat++;
//printf("	id = %d, eat done, es = %d\N", me->id, es);
	return (es || (g->nb_eat != -1 && me->nb_eat >= g->nb_eat));
}

//nb_eat cannot = 0
void	philosophize(int id, t_general g, sem_t *sem)
{
	t_philo	me;
	t_time	offset;

	if (g.nb_philo % 2 == 0)
		offset = id % 2;
	else if (id != 0)
		offset = 1 + !(id % 2);
	else
		id = 0;
	offset *= g.tt_eat;
	me.id = id;
printf("	id = %d, offset = %llu\n", id, offset);
	me.nb_eat = 0;
	print(me.id, THINK, g);
	if (g.nb_philo == 1)
	{
		print(me.id, FORK, g);
		usleep(g.tt_die * K);
		print(me.id, DIED, g);
	}
	else if (_wait(&me, &g, offset))
		;
	else while (!eat(&me, &g, sem) && !_sleep(&me, &g))
		;
printf("	id = %d, ...end\n", id);
	sem_close(sem);
	exit(!(g.nb_eat != -1 && me.nb_eat == g.nb_eat));
}
