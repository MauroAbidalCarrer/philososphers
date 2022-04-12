/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:06:19 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/12 21:56:13 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <pthread.h>
# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIED "died" 


typedef pthread_mutex_t	t_mutex;
typedef unsigned long	t_time;

typedef struct s_general
{
	int		max_meals;
	t_time	time_to_eat;
	t_time	time_to_sleep;
	t_time	time_to_die;
	t_time	sim_start;
}	t_general;

typedef struct s_philo
{
	t_time	last_meal_time;
	char	id;
	t_mutex	*rf;
	t_mutex	*lf;
	int		time_eaten;
}	t_philo;

typedef struct s_es
{
	int		*es;
	t_mutex	es_mutex;
}	t_es;

typedef struct s_to_philo
{
	t_philo		*philo;
	t_general	*general;
	t_mutex		*es_mutex;
	int			*es;
}	t_to_philo;
void	*philosophize(void *add);
#endif
