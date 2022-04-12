/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 19:22:04 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/12 21:52:11 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static t_mutex	*setup_mutexes(int nb_philo)
{
	mutexes = malloc(sizeof(t_mutex) * (nb_philo + 1));
	if (mutexes == NULL)
		exit(1);
	while (--nb_philo >= 0)
		pthread_mutex_init(mutexes + nb_philo, NULL);
	return (mutexes);
}

static t_philo setup_philo(int index, t_general *general, int nb_philo, t_mutex *mutexes)
{
	t_philo	philo;

	philo.last_meal_time = get_time_of_the_day();
	if (index < nb_philo / 2)
		philo.id = index * 2;
	else
		philo.id = index * 2 - nb_philo / 2 + 1;
	philo.rf = mutexes[id + 1];
	if (nb_philo <= 1)
		philo.lf = NULL;
	else
		philo.lf = mutexes[(id + 1) % nb_philo + 1];
	philo.general = general;
	philo.time_to_eat = 0;
	return (philo);
}

static int launch_philo(int i, t_general *g, int nb_philo, t_mutex *mutexes)
{
	t_philo		philo;
	pthread_t	thread;
	int			ret ;
	t_to_philo	to_philo;

	if (nb_philo <= 0)
		return (0);
	philo = setup_philo(index, g, nb_philom mutexes);
	to_philo = {.philo = &philo, .general = g, .mutex_g = *mutexes};
	if (pthread_create(&thread, NULL, philosophize, &to_philo) == -1)
		return (1);
	ret = launch_philo(index - 1, general, nb_philo, mutexes);
	pthread_join(thread);
	return (ret);
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
	general.es = 0;
	general.sim_start = get_time_of_the_day
	return (general);
}

int	main(int ac, char **av)
{
	t_general	general;
	t_mutex		*mutexes;
	int			i;
	int			nb_philo;
	int			ret;

	if (ac != 4 || ac != 5)
		return (1);
	nb_philo = ft_atoi(av[1]);
	if (nb_philo < 0 || nb_philo > 255)
		return (1);
	if (nb_philo == 0)
		return (0);
	mutexes = setup_mutexes(nb_philo);
	general = setup_general(nb_philo, av + 2);
	if (launch_philo(0, &general, nb_philo, mutexes))
	{
		write(2, "error while launching threads\n", 30);
		ret = 1;
	}
	else
		ret = general.es;
	while (--nb_philo >= 0)
		pthread_mutex_destroy(mutexes[nb_philo]);
	free(mutexes);
	return (ret);
}
