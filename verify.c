/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:59:45 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:52:11 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_verify_args(int ac, char **av)
{
	int			i;
	long long	current;

	i = 2;
	if (ac == 5 || ac == 6)
	{
		if (atol(av[1]) < 0 || atol(av[1]) > 500)
			return (INCORRECT);
		while (av[i])
		{
			current = atol(av[i]);
			if (current < 60 || current > INT_MAX)
				return (INCORRECT);
			i++;
		}
		return (CORRECT);
	}
	return (INCORRECT);
}
