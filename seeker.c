/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seeker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:09:27 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:42:34 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_fetch_key(t_phi *p)
{
	int	res;

	pthread_mutex_lock(&p->data->key_seeker);
	if (p->left == p->right)
		res = INCORRECT;
	else if (!p->right->lock && !p->left->lock)
	{
		p->left->lock = CORRECT;
		p->right->lock = CORRECT;
		res = CORRECT;
	}
	else
		res = INCORRECT;
	pthread_mutex_unlock(&p->data->key_seeker);
	return (res);
}
