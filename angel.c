/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:56:59 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:00:21 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_samael(t_phi *p)
{
	pthread_mutex_lock(&p->data->death_seeker);
	if (!p->data->all_dead)
	{
		p->dead = CORRECT;
		p->data->all_dead = CORRECT;
		pthread_mutex_unlock(&p->data->death_seeker);
		ft_print(p, "is deadge, smodge smodge");
		return ;
	}
	pthread_mutex_unlock(&p->data->death_seeker);
}

int	ft_azrael(t_phi *p)
{
	int	reap;

	pthread_mutex_lock(&p->data->death_seeker);
	if (p->data->all_dead)
		reap = CORRECT;
	else
		reap = INCORRECT;
	pthread_mutex_unlock(&p->data->death_seeker);
	return (reap);
}
