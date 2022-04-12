/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 19:22:04 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/12 20:05:23 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
#define FORK "has taken a fork"
#define EAT "is eating"
#define SLEEP "is sleeping"
#define THINK "is thinking"
#define DIED "died" 

typedef pthread_mutex_t	t_mutex;

void	destroy_fork(fork_t fork)
{
	if (rf->lock)
		pthread_mutex_unlock(fork);
	pthread_mutex_destroy(fork);
}

void	die(t_philo philo)
{
	print(philo, DIED);
	philo.general->exit_status = 1;
}

void	wait(t_philo *philo, unsigned float limit)
{
	double	starting_time;
	double	time;

	starting_time = get_time_of_the_day();
	time = get_time_of_the_day();
	while (time - starting_time < limit && !philo->exit_status)
		time = get_time_of_the_day();
}

void	print(t_philo *philo, char *str)
{
	if (philo->last_meal_time <= philo->general->time_to_eat)
		die(philo);
	else if (!philo->general->exit_status)
		printf("%d %u %s\n", get_time_of_the_day(), philo.id, str);
}

void	eat(t_philo *philo)
{
	while (!philo->rf  philo.rf->lock || philo.lf->lock)
		if (philo->last_meal_time 
	mutex_lock(philo.rf);
	print(philo, FORK);
	mutex_lock(philo.lf);
	print(philo, FORK);
	pint(philo, EAT);
	philo.last_meal_time = get_time_of_the_day();
	philo->time_eaten++;
	wait(philo, philo.general->time_to_eat);
	pthread_mutex_unlock(philo->rf);
	pthread_mutex_unlock(philo->lf);
}

void	sleep(t_philo philo)
{
	print(philo, SLEEP);
	wait(philo->general.time_to_sleep);
	print(philo, THINK):
}

void	*philosophize(void *add)
{
	t_philo *philo;

	philo = (t_philo *)add;
	while (philo.time_eaten < philo.general->max_time_to_eat && !philo->general->exit_status)
	{
		eat(&philo);
		sleep(philo);
	}
	free(philo);
}

static t_mutex	*setup_forks(int nb_philo)
{
	forks = malloc(sizeof(t_mutex) * nb_philo);
	if (forks == NULL)
		exit(1);
	while (--nb_philo >= 0)
		pthread_mutex_init(forks[nb_philo], NULL);
	return (forks);
}

typedef struct s_philo
{
	unsigned float		last_meal_time;
	unsigned char		id;
	pthread_mutex_t		*rf;
	pthread_mutex_t		*lf;
	struct s_general	*general;
	int					time_eaten;
}	t_philo;

static t_philo setup_philo(int index, t_general *general, int nb_philo, t_mutex *forks)
{
	t_philo	philo;

	philo.last_meal_time = get_time_of_the_day();
	if (index < nb_philo / 2)
		philo.id = index * 2;
	else
		philo.id = index * 2 - nb_philo / 2 + 1;
	philo.rf = forks[id];
	if (nb_philo <= 1)
		philo.lf = NULL;
	else
		philo.lf = forks[(id + 1) % nb_philo];
	philo.general = general;
	philo.time_to_eat = 0;
	return (philo);
}

static int launch_philo(int index, t_general *general, int nb_philo, t_mutex *forks)
{
	t_philo		philo;
	pthread_t	thread;
	int			ret ;

	if (nb_philo <= 0)
		return (0);
	philo = setup_philo(index, general, nb_philom forks);
	if (pthread_create(&thread, NULL, philosophize, &philo) == -1)
		return (1);
	ret = launch_philo(index - 1, general, nb_philo, forks);
	pthread_join(thread);
	return (ret);
}

typedef struct s_general
{
	unsigned float	time_to_eat;
	unsigned float	time_to_sleep;
	int				max_time_to_eat;
	char			exit_status;
}	t_general;

static t_general setup_general(int nb_philo, char **rest_av)
{
	t_general general;

	general.time_to_eat = ft_atoi(*rest_av);
	general.time_to_sleep = ft_atoi(rest_av[1]);
	if (rest_av[2] == NULL)
		general.max_time_to_eat = -1;
	else
		general.max_time_to_eat = ft_atoi(rest_av[2]);
	general.exit_status = 0;
	return (general);
}

int	main(int ac, char **av)
{
	t_general	general;
	t_mutex		*forks;
	int			i;
	int			nb_philo;

	if (ac != 4 || ac != 5)
		return (1);
	nb_philo = ft_atoi(av[1]);
	if (nb_philo < 0 || nb_philo > 255)
		return (1);
	if (nb_philo == 0)
		return (0);
	forks = setup_forks(nb_philo);
	general = setup_general(nb_philo, av + 2);
	if (launch_philo(0, &general, nb_philo, forks))
	{
		write(2, "error while launching threads\n", 30);
		return (1);
	}
	return (general.exit_status);
}
