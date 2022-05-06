/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 00:58:43 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/06 21:48:11 by maabidal         ###   ########.fr       */
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
	if (real_time - g->sim_start - g->sim_t * K < utt_wait)
	{
		utt_wait -= real_time - g->sim_start - g->sim_t * K;
		usleep(utt_wait);
	}
	if (type != -1)
		printf("	id = %d, type_to_die = %d \n", me->id, type);
	if (type == me->id)
{
printf("called\n");
		print(me->id, DIED, *g);
//printf("yes\n");
}
	g->sim_t += tt_wait;
	return (type != -1);
}

static int	_sleep(t_philo *me, t_general *g)
{
	print(me->id, SLEEP, *g);
	if (_wait(me, g, g->tt_sleep))
{
//printf("	id = %d, sleep es = 1\n", me->id);
		return (1);
}
	print(me->id, THINK, *g);
int	es = _wait(me, g, g->tt_wait);
//printf("	id = %d, sleep es = %d\n", me->id, es);
	return (es);
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
//printf("	id = %d, eat es = %d, sim_t = %llu\n", me->id, es, g->sim_t);
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
	me.nb_eat = 0;
	print(me.id, THINK, g);
	if (g.nb_philo == 1)
	{
		print(me.id, FORK, g);
		usleep(g.tt_die * K);
		print(me.id, DIED, g);
	}
	else if (_wait(&me, &g, offset))
		printf("died at offset(%llu) id = %d\n", offset, id);
	else while (!eat(&me, &g, sem) && !_sleep(&me, &g))
		;
//	else
	//	printf("stopped in loop id = %d\n", id);
	sem_close(sem);
	exit(!(g.nb_eat != -1 && me.nb_eat == g.nb_eat));
}
