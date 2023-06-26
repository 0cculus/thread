/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:16:17 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:49:11 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_keys(int nb_p, t_data *data)
{
	data->keys = ft_calloc(nb_p, sizeof(t_key));
	if (!data->keys)
		return (INCORRECT_EXIT);
	while (nb_p--)
	{
		data->keys[nb_p].lock = INCORRECT;
		if (pthread_mutex_init(&data->keys[nb_p].mutex, NULL) != 0)
			return (INCORRECT_EXIT);
	}
	if (pthread_mutex_init(&data->death_seeker, NULL) != 0 || \
	pthread_mutex_init(&data->key_seeker, NULL) != 0 || \
	pthread_mutex_init(&data->scribe, NULL) != 0)
		return (INCORRECT_EXIT);
	return (CORRECT_EXIT);
}

void	ft_init_data(char **av, int ac, t_data *data)
{
	t_info	info;

	ft_bzero(data, sizeof(t_data));
	ft_bzero(&info, sizeof(t_info));
	info.time_death = ft_atol(av[2]);
	info.time_eat = ft_atol(av[3]);
	info.time_sleep = ft_atol(av[4]);
	info.time_think = 30;
	if (ac == 6)
	{
		data->all_you_can_eat = CORRECT;
		data->nb_meals = ft_atol(av[5]);
	}
	else
		data->all_you_can_eat = INCORRECT;
	data->info = info;
}

int	ft_init_phi(int nb_p, t_data *data)
{
	int		current;

	current = -1;
	data->phi = ft_calloc(nb_p, sizeof(t_phi));
	if (!data->phi)
		return (INCORRECT_EXIT);
	while (++current < nb_p)
	{
		data->phi[current].last_meal = data->info.time_death;
		data->phi[current].id = current + 1;
		data->phi[current].data = data;
		data->phi[current].dead = INCORRECT;
		data->phi[current].nb_meals = data->nb_meals;
		if (current != 0)
			data->phi[current].left = &data->keys[current - 1];
		else
			data->phi[current].left = &data->keys[nb_p - 1];
		data->phi[current].right = &data->keys[current];
	}
	return (CORRECT_EXIT);
}
