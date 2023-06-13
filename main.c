/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:17:33 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/13 16:04:30 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

# define THINK 10
# define CORRECT 1
# define INCORRECT 0
# define MILLI_SEC 1000

typedef struct info
{
	int	time_eat;
	int	time_death;
	int	time_sleep;
	int	time_think;
}	t_info;

typedef struct data
{
	short			all_alive;
	t_info			info;
	pthread_mutex_t	*keys;
}	t_data;

typedef struct phi
{
	pthread_t	tid;
	t_data		data;
	int			id;
	int			left;
	int			right;
	int			alive;
	int			nb_meals;
}	t_phi;

//WIP
long long	ft_get_time(void)
{
	static struct timeval	start = {-1, -1};
	struct timeval			current;

	a;
	return ();
}
//END

int		ft_unleash_keys(t_phi *p)
{
	p->nb_meals--;
	printf("%d has unleashed both keys\n", p->id);
	if (pthread_mutex_unlock(&p->data.keys[p->right]))
		return (INCORRECT);
	if (pthread_mutex_unlock(&p->data.keys[p->left]))
		return (INCORRECT);
	return (CORRECT);
}

int		ft_phi_eat(t_phi *p)
{
	usleep(p->data.info.time_eat * MILLI_SEC);
	return (CORRECT);
}

int		ft_phi_sleep()
{
	return (CORRECT);
}

int		ft_phi_think()
{
	return (CORRECT);
}

int		ft_phi_lives(t_phi *p)
{
	if (pthread_mutex_lock(&p->data.keys[p->right]))
		return (INCORRECT);
	if (pthread_mutex_lock(&p->data.keys[p->left]))
		return (INCORRECT);
	printf("%d has grabbed both keys\n", p->id);
	usleep(1000);
	ft_unleash_keys(p);
	return (CORRECT);
}

void	*ft_action(void *arg)
{
	t_phi	*p;

	p = (t_phi *)arg;
	if (p->nb_meals > 0)
	{
		while (p->nb_meals)
			ft_phi_lives(p);
	}
	else
	{
		while (p->alive)
		{
			if (ft_phi_lives(p))
				break;
		}
	}
	return (NULL);
}

void	ft_init_key(t_phi *p, int nb)
{
	pthread_mutex_t *mutex;
	int				current;

	current = nb;
	mutex = calloc(nb, sizeof(pthread_mutex_t));
	while (nb--)
		pthread_mutex_init(&mutex[nb], NULL);
	while (current--)
		p[current].data.keys = mutex;
}

void	ft_init_phi(t_phi *phi, int size, int nb_meals)
{
	pthread_t	thread;
	int			current;

	current = size;
	ft_init_key(phi, size);
	while (current--)
	{
		phi[current].alive = CORRECT;
		phi[current].id = current + 1;
		phi[current].right = current;
		if (current != 0)
			phi[current].left = current - 1;
		else
			phi[current].left = size;
		phi[current].nb_meals = nb_meals;
		pthread_create(&thread, NULL, &ft_action, &phi[current]);
		phi[current].tid = thread;
		usleep(1000);
	}
}

int main(int ac, char **av)
{
	t_phi	*phi;
	t_data	data;
	int		nb_p;

	if (ac == 3 || ac == 2)
	{
		nb_p = atoi(av[1]);
		phi = calloc(nb_p, sizeof(t_phi));
		if (ac == 3)
			ft_init_phi(phi, nb_p, atoi(av[2]));
		else
			ft_init_phi(phi, nb_p, -1);
		for (int i = 0; i < nb_p; i++)
			pthread_join(phi[i].tid, NULL);
		free(data.keys);
		free(phi);
	}
	else
		printf("bad args");
}
