/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:40:21 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/04 17:37:08 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <signal.h>

# define SEM_NAME "/forks"
# define SEM_OPTIONS O_CREAT | O_EXCL
# define SEM_MODE S_IRUSR | S_IWUSR
# define WAIT_OPTIONS WUNTRACED | WCONTINUED

//real time is in micro seconds
typedef unsigned long	t_time;
typedef pthread_mutex_t	t_mutex;

//sim_start is in real time
typedef struct s_general
{
	int		nb_eat;
	t_time	tt_eat;
	t_time	tt_sleep;
	t_time	tt_die;
	t_time	tt_wait;
	t_time	sim_start;
	t_time	theo_time;
	int		nb_philo;
}	t_general;

//last meal is in theoretical time
typedef struct s_philo
{
	int		id;
	int		type;
	int		nb_eat;
	t_time	last_meal;
}	t_philo;

t_time	get_time(void);
void	philosophize(int id, sem_t *sem);
#endif
