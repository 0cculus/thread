/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:12:40 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:52:37 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_print(t_phi *p, char *src)
{
	pthread_mutex_lock(&p->data->scribe);
	if (!ft_azrael(p) || (ft_azrael(p) && p->dead))
		printf ("%lld %d %s\n", ft_get_time(), p->id, src);
	pthread_mutex_unlock(&p->data->scribe);
}

void	ft_bzero(void *src, int nb)
{
	if (nb != 0)
		src = memset(src, 0, nb);
}

long	ft_atol(char *src)
{
	long	i;
	long	res;
	long	neg;

	i = 0;
	res = 0;
	neg = 1;
	while (src[i] && (src[i] == 32 || (src[i] >= 9 && src[i] <= 13)))
		i++;
	if (src[i] == '-' || src[i] == '+')
	{
		if (src[i] == '-')
			neg *= -1;
		i++;
	}
	while (src[i] >= '0' && src[i] <= '9')
	{
		res *= 10;
		res += (src[i] - '0');
		i++;
	}
	return (neg * res);
}

void	*ft_calloc(int nb, int size)
{
	void	*res;

	res = malloc(size * nb);
	if (res)
	{
		ft_bzero(res, nb * size);
		return (res);
	}
	return (NULL);
}
