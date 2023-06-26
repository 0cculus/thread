/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   annihilation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:15:14 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:42:18 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_destroy(t_data *data, int nb)
{
	while (nb--)
		if (pthread_mutex_destroy(&data->keys[nb].mutex))
			return (INCORRECT_EXIT);
	if (pthread_mutex_destroy(&data->scribe) || \
		pthread_mutex_destroy(&data->key_seeker) || \
		pthread_mutex_destroy(&data->death_seeker))
		return (INCORRECT_EXIT);
	return (CORRECT_EXIT);
}

int	ft_purge(t_data *data, int res)
{
	if (data->keys)
		free(data->keys);
	if (data->phi)
		free(data->phi);
	return (res);
}
