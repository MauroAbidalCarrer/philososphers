/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:22:37 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/04 02:21:11 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_time	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_time)tv.tv_sec * 1000 + (t_time)tv.tv_usec / 1000);
}

static int	set_general(int nb_philo, char **av, t_general *g)
{
	g->tt_die = ft_atoi(*av);
	if (g->tt_eat < ft_atoi(*av))
		return (1);
	g->tt_eat = ft_atoi(av[1]);
	if (g->tt_eat < ft_atoi(av[1]))
		return (1);
	g->tt_sleep = ft_atoi(av[2]);
	if (g->tt_sleep < ft_atoi(av[2]))
		return (1);
	if (av[3] == NULL)
		g->nb_eat = -1;
	else
		g->nb_eat = ft_atoi(av[3]);
	g->sim_start = get_time();
	g->nb_philo = nb_philo;
	g->tt_wait = (nb_philo % 2 + 1) * g->tt_eat;
	if (g->tt_wait <= g->tt_sleep)
		g->tt_wait = 0;
	else
		g->tt_wait -= g->tt_sleep;
	return (0);
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

static int	launch_philos(int id, sem_t *sem, t_general g)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	if (!pid)
		philosophize(id, sem);
	if (id != g.nb_philo)
		return (0);
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
	int			nb_philo;
	t_general	g;
	int			es;

	if (ac != 5 && ac != 6)
		return (1);
	nb_philo = ft_atoi(av[1]);
	if (nb_philo <= 0 || nb_philo > 255 || set_general(nb_philo, av + 2, &g))
		return (1);
	sem = sem_open(SEM_NAME, SEM_OPTIONS, SEM_MODE, nb_philo);
	if (sem == SEM_FAILED);
		return (1);
	es = launch_philos(1, sem, g);
	if (sem_close(sem) == -1 || sem_unlink(SEM_NAME) == -1)
		exit(1);
	exit(es);
}
