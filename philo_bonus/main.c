/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:22:37 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/06 05:53:36 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

//1. getting last end of meal
//2. adding to it tt_die
//3. comparing
t_time	type_dies_before(t_general g, int type, t_time tt_wait, int *w_type, int id)
{
	t_time	cycle;
	t_time	t;
	t_time	offset;

	if (*w_type != -1)
		return (tt_wait);
	cycle = g.tt_eat + g.tt_sleep + g.tt_wait;
	offset = type * g.tt_eat;
	if (g.theo_time < offset)
	{
		if (g.theo_time + tt_wait < offset)
			t = 0;
		else
			t = (g.theo_time + tt_wait - offset) / cycle + g.tt_eat;
	}
	else
		t = (g.theo_time - offset) / cycle + g.tt_eat;
	t += g.tt_die;
	if (t > tt_wait)
		return (tt_wait);
printf("	id = %d, type %d will die at %llu, theo_time = %llu, tt_wait = %llu\n", id, type, t,g.theo_time, tt_wait);
	*w_type = type;
	return (t);
}

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
	g->theo_time = 0;
	gettimeofday(&tv, NULL);
	g->sim_start = ((t_time)tv.tv_sec * M) + (t_time)tv.tv_usec;

	printf("tt_wait = %llu, sim_start = %llu, nb_eat = %d\n\n", g->tt_wait, g->sim_start, g->nb_eat);

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
if (sem == SEM_FAILED){printf("sem open failed\n");}
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
