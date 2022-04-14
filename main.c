/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 19:22:04 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/14 16:01:48 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static t_sa	*setup_mutexes(int nb_philo, t_sa *es)
{
	t_sa	*forks;
	int		i;

	if (ft_malloc((void **)&forks, sizeof(t_sa) * nb_philo))
		exit(1);
	i = -1;
	while (++i < nb_philo)
	{
		if (init_sa(forks + i))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(forks + i)->mutex);
			free(forks);
			exit(1);
		}
	}
	if (init_sa(es))
	{
		while (--i >= 0)
			pthread_mutex_destroy(&(forks + i)->mutex);
		free(forks);
		exit(1);
	}
	return (forks);
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
//printf("time_to_die = %ld\ntime_to_eat = %ld\n time_to_sleep = %ld\n max_meals = %d\ngeneral.sim_start = %ld\n", general.time_to_die, general.time_to_eat, general.time_to_sleep, general.max_meals, general.sim_start);
	return (general);
}

static t_philo setup_philo(int index, int nb_philo, t_sa *forks, t_general *general)
{
	t_philo	philo;
	int		limit;

	philo.last_meal_time = general->sim_start;
	limit = nb_philo / 2 + nb_philo % 2;
	if (index < limit)
		philo.id = index * 2;
	else
		philo.id = (index - limit) * 2 + 1;
	philo.rf = forks + philo.id;
	if (nb_philo <= 1)
		philo.lf = NULL;
	else
		philo.lf = forks + ((philo.id + 1) % nb_philo);
	philo.time_eaten = 0;
/*
printf("to philo %d\n", index);
printf("last meal   = %ld\n", philo.last_meal_time);
printf("id = %d\n", philo.id);
printf("rf          = %p\n", philo.rf);
printf("lf          = %p\n", philo.lf);
printf("time eaten  = %d\n\n", philo.time_eaten);
*/
	return (philo);
}

typedef struct s_ref
{
	int	index;
	int	nb_philo;
}	t_ref;

static int launch_philo(t_ref ref, t_general general, t_sa *es, t_sa *forks)
{
	t_philo		philo;
	pthread_t	thread;
	int			ret;
	t_to_philo	to_philo;
	void		*add;

	if (ref.index >= ref.nb_philo)
		return (0);
//printf("index = %d\n", ref.index);
	philo = setup_philo(ref.index++, ref.nb_philo, forks, &general);
	to_philo.philo = &philo;
	to_philo.general = &general;
	to_philo.es = es;
	if (pthread_create(&thread, NULL, philosophize, &to_philo) == -1)
		return (1);
//printf("launching philo %d\n", ref.index);
	ret = launch_philo(ref, general, es, forks);
	pthread_join(thread, &add);
	return (ret);
}

int	main(int ac, char **av)
{
	t_sa	es;
	t_sa	*forks;
	int		nb_philo;
	t_ref	ref;

	if (ac != 5 && ac != 6)
		return (write(2, "nb of arg incorrect\n", 21), 1);
	nb_philo = ft_atoi(av[1]);
	if (nb_philo < 0 || nb_philo > 255)
		return (write(2, "nb philo utside of range\n", 24), 1);
	if (nb_philo == 0)
		return (0);
	forks = setup_mutexes(nb_philo, &es);
	ref.index = 0;
	ref.nb_philo = nb_philo;
	if (launch_philo(ref, setup_general(nb_philo, av + 2), &es, forks))
	{
		write(2, "error while launching threads\n", 30);
		es.data = 1;
	}
	while (--nb_philo >= 0)
		pthread_mutex_destroy(&(forks + nb_philo)->mutex);
	free(forks);
	pthread_mutex_destroy(&es.mutex);
	return (es.data);
}
