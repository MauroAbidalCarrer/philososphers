/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 19:22:04 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/03 18:04:04 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static t_mutex	*setup_mutexes(int nb_philo, t_sa *es)
{
	t_mutex	*forks;
	int		i;

	if (ft_malloc((void **)&forks, sizeof(t_mutex) * nb_philo))
		exit(1);
	i = -1;
	while (++i < nb_philo)
	{
		if (pthread_mutex_init(forks + i, NULL) == -1)
		{
			while (--i >= 0)
				pthread_mutex_destroy(forks + i);
			free(forks);
			exit(1);
		}
	}
	if (init_sa(es))
	{
		while (--i >= 0)
			pthread_mutex_destroy(forks + i);
		free(forks);
		exit(1);
	}
	return (forks);
}

static int	set_general(int nb_philo, char **av, t_general *g)
{
	av += 2;
	g->tt_die = ft_atoi(*av);
	if (g->tt_die <= 0)
		return (1);
	g->tt_eat = ft_atoi(av[1]);
	if (g->tt_eat <= 0)
		return (1);
	g->tt_sleep = ft_atoi(av[2]);
	if (g->tt_sleep <= 0)
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

typedef struct s_ref
{
	int	index;
	int	nb_philo;
}	t_ref;

//even
//	0 = even
//	1 = odd
//odd
//	0 = first philo
//	1 = even
//	2 = odd
static t_philo	set_philo(t_ref ref, t_mutex *forks, t_general g)
{
	t_philo	philo;

	philo.id = ref.index;
	philo.rf = forks + philo.id;
	if (ref.nb_philo <= 1)
		philo.lf = NULL;
	else
		philo.lf = forks + ((philo.id + 1) % ref.nb_philo);
	philo.time_eaten = 0;
	philo.type = philo.id % 2 + (philo.id != 0 && ref.nb_philo % 2);
	philo.last_meal = g.sim_start;
	return (philo);
}

static int	launch_philo(t_ref ref, t_general general, t_sa *es, t_mutex *forks)
{
	t_philo		philo;
	pthread_t	thread;
	int			ret;
	t_to_philo	to_philo;
	void		*add;

	if (ref.index >= ref.nb_philo)
		return (0);
	philo = set_philo(ref, forks, general);
	to_philo.philo = &philo;
	to_philo.general = &general;
	to_philo.es = es;
	if (pthread_create(&thread, NULL, philosophize, &to_philo))
		return (1);
	ref.index++;
	ret = launch_philo(ref, general, es, forks);
	pthread_join(thread, &add);
	return (ret);
}

int	main(int ac, char **av)
{
	t_sa		es;
	t_mutex		*forks;
	t_general	g;
	t_ref		ref;

	if (ac != 5 && ac != 6)
		return (1);
	ref.nb_philo = ft_atoi(av[1]);
	if (ref.nb_philo <= 0 || ref.nb_philo > 255)
		return (1);
	forks = setup_mutexes(ref.nb_philo, &es);
	ref.index = 0;
	es.data = set_general(ref.nb_philo, av, &g);
	if (es.data || launch_philo(ref, g, &es, forks))
			es.data = 1;
	while (--ref.nb_philo >= 0)
		pthread_mutex_destroy(forks + ref.nb_philo);
	free(forks);
	pthread_mutex_destroy(&es.mutex);
	return (es.data);
}
