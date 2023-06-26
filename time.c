/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:07:45 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:49:55 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_chrono	ft_get_time(void)
{
	static struct timeval	start = {-1, -1};
	struct timeval			current;

	if (start.tv_sec == -1 && start.tv_usec == -1)
		gettimeofday(&start, NULL);
	gettimeofday(&current, NULL);
	return ((current.tv_sec * 1000 - start.tv_sec * 1000) + \
			(current.tv_usec / 1000 - start.tv_usec / 1000));
}

void	ft_slumber(t_phi *p, t_chrono current)
{
	t_chrono	start;

	start = ft_get_time();
	if (p->last_meal < current + start && !p->dead && \
		p->last_meal > start)
	{
		usleep((p->last_meal - start) * MILLI_SEC);
		ft_samael(p);
	}
	else
	{
		usleep(current * MILLI_SEC * 0.9);
		while (ft_get_time() < current + start)
			usleep (MILLI_SEC);
	}
}
