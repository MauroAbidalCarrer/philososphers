/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:40:21 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/06 20:50:20 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <fcntl.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <stdio.h>
# include <signal.h>

# define SEM_NAME "/forks"
# define SEM_OPTIONS O_CREAT | O_EXCL
# define SEM_MODE S_IRUSR | S_IWUSR
# define WAIT_OPTIONS WUNTRACED | WCONTINUED
# define K 1000
# define M 1000000
# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIED "died"

//real time is in micro seconds
typedef unsigned long long	t_time;

//sim_start is in real time
typedef struct s_general
{
	int		nb_eat;
	t_time	tt_eat;
	t_time	tt_sleep;
	t_time	tt_die;
	t_time	tt_wait;
	t_time	sim_start;
	t_time	sim_t;
	int		nb_philo;
}	t_general;

typedef struct s_philo
{
	int	id;
	int	nb_eat;
}	t_philo;

void	philosophize(int id, t_general g, sem_t *sem);
t_time	type_dies_before(t_general g, int type, t_time tt_wait, int *w_type, int id);
#endif
