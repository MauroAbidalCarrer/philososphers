/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:22:37 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/06 20:50:34 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
/*
//1. getting last end of meal
//2. adding to it tt_die
//3. comparing
//pour l'instant, on considere que si death_time = picku_fork_rime le philo meurt
t_time	type_dies_before(t_general g, int type, t_time tt_wait, int *w_type, int id)
{
	t_time	cycle;
	t_time 	offset;
	t_time	eo_last_meal;
	t_time	death_time;

	if (*w_type != -1)
		return (tt_wait);
	cycle = g.tt_eat + g.tt_sleep + g.tt_wait;
	offset = type * g.tt_eat;
//contradiction
//si offset et sim_t = 0 
//	eo_last_meal = 0, alors qu'il devrait etre a tt_eat
//si offset == sim_t et que sim_t != 0 
//	alors eo_last_meal
	if (g.sim_t <= offset && offset != 0)
		eo_last_meal = 0;
	else
		eo_last_meal = g.sim_t - g.sim_t % cycle + g.tt_eat;
	death_time = eo_last_meal + g.tt_die + g.sim_t;
	if (death_time > tt_wait + g.sim_t)
{
printf("no d id = %d, type, = %d, wait = %llu, theo = %llu, eof_lm = %llu, dt = %llu end_wait = %llu\n", id, type, tt_wait, g.sim_t, eo_last_meal, death_time, tt_wait + g.sim_t);
		return (tt_wait);
}
printf("death id = %d, type, = %d, wait = %llu, theo = %llu, eof_lm = %llu, dt = %llu end_wait = %llu\n", id, type, tt_wait, g.sim_t, eo_last_meal, death_time, tt_wait + g.sim_t);
	*w_type = type;
	return (eo_last_meal);
}

t_time	type_dies_before(t_general g, int type, t_time tt_wait, int *w_type, int id)
{
	t_time	cycle;
	t_time	t;
	t_time	offset;

	if (*w_type != -1)
		return (tt_wait);
	cycle = g.tt_eat + g.tt_sleep + g.tt_wait;
	offset = type * g.tt_eat;
	if (g.sim_t == offset)
		t = 0;
	else if (g.sim_t < offset)
	{
		if (g.sim_t + tt_wait <= offset)
			t = 0;
		else
			t = (g.sim_t + tt_wait - offset) / cycle + g.tt_eat;
	}
	else
		t = (g.sim_t - offset) / cycle + g.tt_eat;
	t += g.tt_die;
	if (t > tt_wait)
{
printf("	no death id = %d, theo = %llu, wait = %llu, type = %d, t = %llu\n", id, g.sim_t, tt_wait, type, t);
		return (tt_wait);
}
printf("	id = %d, type %d will die at %llu, sim_t = %llu, tt_wait = %llu\n", id, type, t,g.sim_t, tt_wait);
	*w_type = type;
	return (t);
}
*/

static int	ft_atoi(const char *nptr)
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

//set sim_start to time of day in miros seconds
static int	set_g(int nb_philo, char **av, t_general *g)
{
	struct timeval	tv;

	if (ft_atoi(*av) < 0 || ft_atoi(av[1]) < 0 || ft_atoi(av[2]) < 0)
		return (1);
	g->tt_die = ft_atoi(*av);
	g->tt_eat = ft_atoi(av[1]);
	g->tt_sleep = ft_atoi(av[2]);
	if (av[3] == NULL)
		g->nb_eat = -1;
	else if (ft_atoi(av[3]) < 0)
		return (1);
	else
		g->nb_eat = ft_atoi(av[3]);
	g->nb_philo = nb_philo;
	g->tt_wait = (nb_philo % 2 + 1) * g->tt_eat;
	if (g->tt_wait <= g->tt_sleep)
		g->tt_wait = 0;
	else
		g->tt_wait -= g->tt_sleep;
	g->sim_t = 0;
	gettimeofday(&tv, NULL);
	g->sim_start = ((t_time)tv.tv_sec * M) + (t_time)tv.tv_usec;
	return (0);
}

static int	launch_philos(int id, sem_t *sem, t_general g)
{
	int	pid;
	int	wstatus;

	if (id == g.nb_philo)
		return (0);
	pid = fork();
	if (pid == -1)
		return (1);
	if (!pid)
		philosophize(id, g, sem);
	if (launch_philos(id + 1, sem, g))
	{
		kill(pid, SIGINT);
		return (1);
	}
	if (waitpid(pid, &wstatus, WAIT_OPTIONS) == -1 || !WIFEXITED(wstatus))
		return (1);
	return (WEXITSTATUS(wstatus));
}

int	main(int ac, char **av)
{
	sem_t		*sem;
	int			nb_p;
	t_general	g;
	int			es;

	if (ac != 5 && ac != 6)
		return (1);
	nb_p = ft_atoi(av[1]);
	sem = sem_open(SEM_NAME, SEM_OPTIONS, SEM_MODE, nb_p);
	if (sem == SEM_FAILED || nb_p <= 0 || nb_p > 255 || set_g(nb_p, av + 2, &g))
	{
		sem_close(sem);
		sem_unlink(SEM_NAME);
		exit(1);
	}
	es = 0;
	if (g.nb_eat != 0)
		es = launch_philos(0, sem, g);
	if (sem_close(sem) == -1 || sem_unlink(SEM_NAME) == -1)
		exit(1);
	exit(es);
}
