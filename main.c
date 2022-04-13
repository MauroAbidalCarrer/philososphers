/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 19:22:04 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/13 18:40:39 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static t_mutex	*setup_mutexes(int nb_philo, t_es *es)
{
	t_mutex	*forks;
	int		i;

	if (ft_malloc(&forks, sizeof(t_mutex) * nb_philo))
		exit(1);
	i = -1;
	while (++i < nb_philo)
	{
		if (init_mutex(forks + i))
		{
			while (--i >= 0)
				pthread_mutex_destroy(forks + i);
			free(forks);
			exit(1);
		}
	}
	if (init_mutex(&es->es_mutex))
	{
		while (--i >= 0)
			pthread_mutex_destroy(forks + i);
		free(forks);
		exit(1);
	}
	es->es = 0;
	return (forks);
}

static t_philo setup_philo(int index, int nb_philo, t_mutex *forks)
{
	t_philo	philo;

	philo.last_meal_time = get_time();
	if (index < nb_philo / 2)
		philo.id = index * 2;
	else
		philo.id = index * 2 - nb_philo / 2 + 1;
	philo.rf = forks + philo.id;
	if (nb_philo <= 1)
		philo.lf = NULL;
	else
		philo.lf = forks + ((philo.id + 1) % nb_philo);
	philo.time_eaten = 0;
	return (philo);
}

static t_general setup_general(int nb_philo, char **rest_av)
{
	t_general general;

	general.time_to_die = ft_atoi(*rest_av);
	general.time_to_eat = ft_atoi(rest_av[1]);
	general.time_to_sleep = ft_atoi(rest_av[2]);
	if (rest_av[3] == NULL)
		general.max_meals = -1;
	else
		general.max_meals = ft_atoi(rest_av[3]);
	general.sim_start = get_time();
	return (general);
}

typedef struct s_ref
{
	int	index;
	int	nb_philo;
}	t_ref;

static int launch_philo(t_ref ref, t_general general, t_es *es, t_mutex *forks)
{
	t_philo		philo;
	pthread_t	thread;
	int			ret;
	t_to_philo	to_philo;
	void		*add;

	if (ref.nb_philo <= 0)
		return (0);
	philo = setup_philo(ref.index++, ref.nb_philo, forks);
	to_philo.philo = &philo;
	to_philo.general = &general;
	to_philo.es = es;
	if (pthread_create(&thread, NULL, philosophize, &to_philo) == -1)
		return (1);
	ret = launch_philo(ref, general, es, forks);
	pthread_join(thread, &add);
	return (ret);
}

int	main(int ac, char **av)
{
	t_es	es;
	t_mutex	*forks;
	int		nb_philo;
	t_ref	ref;

	if (ac != 4 || ac != 5)
		return (1);
	nb_philo = ft_atoi(av[1]);
	if (nb_philo < 0 || nb_philo > 255)
		return (1);
	if (nb_philo == 0)
		return (0);
	forks = setup_mutexes(nb_philo, es);
	ref.index = 0;
	ref.nb_philo = nb_philo;
	if (launch_philo(ref, setup_general(nb_philo, av + 2), &es, forks))
	{
		write(2, "error while launching threads\n", 30);
		es.es = 1;
	}
	while (--nb_philo >= 0)
		pthread_mutex_destroy(forks + nb_philo);
	free(forks);
	pthread_mutex_destroy(&es.es_mutex);
	return (es.es);
}
