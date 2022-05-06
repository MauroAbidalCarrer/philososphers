/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   predictions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabidal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 19:30:53 by maabidal          #+#    #+#             */
/*   Updated: 2022/05/06 21:15:29 by maabidal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	range(t_time a, t_time b, t_time c)
{
	return (c >= a && c <= b);
}

t_time	type_dies_before(t_general g, int type, t_time tt_wait, int *w_type, int id)
{
	t_time	cycle;
	t_time 	offset;
	t_time	starve;
	t_time	end;

	cycle = g.tt_eat + g.tt_sleep + g.tt_wait;
	offset = type * g.tt_eat;
	end = g.sim_t + tt_wait;
	if (offset >= g.tt_die && range(g.sim_t, end, g.tt_die))
	{

printf("	id = %d, type %d will die_offset at %llu(wait = %llu, sim_t = %llu)\n", id, type, g.tt_die, tt_wait, g.sim_t);
		*w_type = type;
		return (g.tt_die - g.sim_t);
	}
	starve = g.sim_t - g.sim_t % cycle + g.tt_eat + g.tt_die;
	if (g.sim_t >= offset && g.tt_eat + g.tt_die <= cycle && range(g.sim_t, end, starve))
	{
printf("	id = %d, type %d will die_starve at %llu(wait = %llu, sim_t = %llu)\n", id, type, starve, tt_wait, g.sim_t);
		*w_type = type;
		return (starve - g.sim_t);
	}
	return (tt_wait);
}
