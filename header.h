/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 20:06:19 by maabidal          #+#    #+#             */
/*   Updated: 2022/04/17 19:41:30 by maabidal         ###   ########.fr       */
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
# define WRITE 1
# define ADD 2
# define EAGAIN_MSG "Lacked the necessary resources to initialize a mutex\n"
# define ENOMEM_MSG "Insufficient memory exists to initialize a mutex\n"
# define EPERM_MSG "Do not have the privilege to init mutex\n"

typedef unsigned long	t_time;
typedef pthread_mutex_t	t_mutex;

typedef struct s_shared_data
{
	int		data;
	t_mutex	mutex;
}	t_sa;

//tt = time to
//pt = philospher to
typedef struct s_general
{
	int		nb_eat;
	t_time	tt_eat;
	t_time	tt_sleep;
	t_time	tt_die;
	t_time	tt_wait;
	t_time	sim_start;
	int		nb_philo;
}	t_general;

typedef struct s_philo
{
	int		id;
	t_mutex	*rf;
	t_mutex	*lf;
	int		time_eaten;
	char	type;
}	t_philo;

typedef struct s_to_philo
{
	t_philo		*philo;
	t_general	*general;
	t_sa		*es;
}	t_to_philo;

t_time	get_time(void);
int		access_sa(t_sa *es, int write);
int		ft_malloc(void **add, size_t size);
int		init_sa(t_sa *sa);
int		ft_atoi(const char *nptr);
void	*philosophize(void *add);
#endif
