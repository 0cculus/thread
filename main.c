/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:17:33 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:51:09 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_join_phi(int nb_p, t_data *data)
{
	int	i;

	i = 0;
	while (i < nb_p)
		if (pthread_join(data->phi[i++].tid, NULL) != 0)
			return (INCORRECT_EXIT);
	return (CORRECT_EXIT);
}

int	ft_release(int nb_p, t_data *data)
{
	int	current;

	current = -1;
	while (++current < nb_p)
		if (pthread_create(&data->phi[current].tid,
				NULL, &ft_lives, &data->phi[current]) != 0)
			return (INCORRECT_EXIT);
	return (CORRECT_EXIT);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		nb_p;

	if (ft_verify_args(ac, av))
	{
		nb_p = atoi(av[1]);
		ft_init_data(av, ac, &data);
		if (ft_init_keys(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (ft_init_phi(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (ft_release(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (ft_join_phi(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (ft_destroy(&data, nb_p))
			return (ft_purge(&data, INCORRECT_EXIT));
		return (ft_purge(&data, CORRECT_EXIT));
	}
	else
		printf("bad args\n");
}
