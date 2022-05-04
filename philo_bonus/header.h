/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 18:40:21 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/04 02:33:32 by maabidal         ###   ########.fr       */
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

typedef unsigned long	t_time;
typedef pthread_mutex_t	t_mutex;

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

//last meal should the theoretical value
typedef struct s_philo
{
	int		id;
	int		type;
	int		nb_eat;
	t_time	last_meal;
	t_time	theoretical_time;
}	t_philo;

t_time	get_time(void);
void	philosophize(int id, sem_t *sem);
#endif
