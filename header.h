/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:06:19 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/13 18:41:14 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <pthread.h>
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

typedef unsigned long	t_time;

typedef struct s_general
{
	int		max_meals;
	t_time	time_to_eat;
	t_time	time_to_sleep;
	t_time	time_to_die;
	t_time	sim_start;
}	t_general;

typedef pthraed_mutex	t_mutex;

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
int		ft_malloc(void **add, size_t size);
int		init_mutex(t_mutex *mutex);
int		ft_atoi(const char *nptr);
void	print(t_philo *philo, t_general general, char *str, t_es *es);
void	*philosophize(void *add);
#endif
