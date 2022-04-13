/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:06:19 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/13 21:12:45 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <pthread.h>
# include <errno.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIED "died" 
# define TAKE 0
# define DROP 1
# define EAGAIN_MSG "Lacked the necessary resources to initialize a mutex\n"
# define ENOMEM_MSG "Insufficient memory exists to initialize a mutex\n"
# define EPERM_MSG "Do not have the privilege to init mutex\n"

typedef unsigned long	t_time;

typedef struct s_general
{
	int		max_meals;
	t_time	time_to_eat;
	t_time	time_to_sleep;
	t_time	time_to_die;
	t_time	sim_start;
}	t_general;

typedef pthread_mutex_t	t_mutex;

typedef struct s_shared_data
{
	int		data;
	t_mutex	mutex;
}	t_sa;

typedef struct s_philo
{
	t_time	last_meal_time;
	char	id;
	t_sa	*rf;
	t_sa	*lf;
	int		time_eaten;
}	t_philo;

typedef struct s_to_philo
{
	t_philo		*philo;
	t_general	*general;
	t_sa		*es;
}	t_to_philo;

t_time	get_time(void);
int		access_es(t_sa *es, int write);
int		ft_malloc(void **add, size_t size);
int		init_sa(t_sa *sa);
int		ft_atoi(const char *nptr);
void	print(t_philo *philo, t_general *general, char *str, t_sa *es);
void	*philosophize(void *add);
#endif
