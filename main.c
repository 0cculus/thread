/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:17:33 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/16 15:53:45 by brheaume         ###   ########.fr       */
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
	int				nb_phi;
	short			all_alive;
	t_info			info;
	pthread_mutex_t	*keys;
	pthread_mutex_t	*lock;
	pthread_mutex_t	*scribe;
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
	long long	last_meal;
}	t_phi;

long long	ft_get_time(void)
{
	static struct timeval	start = {-1, -1};
	struct timeval			current;

	if (start.tv_sec == -1 && start.tv_usec == -1)
		gettimeofday(&start, NULL);
	gettimeofday(&current, NULL);
	return ((current.tv_sec) + (current.tv_usec));
}

void	ft_print(t_phi *p, char *src)
{
	pthread_mutex_lock(p->data.scribe);
	printf ("%d %s\n", p->id, src);
	pthread_mutex_unlock(p->data.scribe);
}

int	ft_unleash_keys(t_phi *p)
{
	ft_print(p, "has unleashed both forks");
	pthread_mutex_unlock(&p->data.keys[p->right]);
	pthread_mutex_unlock(&p->data.keys[p->left]);

	return (CORRECT);
}

int	ft_fetch_key(t_phi *p)
{
	(void)p;
	return (CORRECT);
}

int	ft_phi_eat(t_phi *p)
{
	pthread_mutex_lock(&p->data.keys[p->right]);
	ft_print(p, "has taken his right fork");
	pthread_mutex_lock(&p->data.keys[p->left]);
	ft_print(p, "has taken his left fork");
	ft_print(p, "is eating");
	p->nb_meals--;
	usleep(p->data.info.time_eat * MILLI_SEC);
	pthread_mutex_unlock(&p->data.keys[p->right]);
	pthread_mutex_unlock(&p->data.keys[p->left]);
	return (CORRECT);
}

int	ft_phi_sleep(t_phi *p)
{
	ft_print(p, "is sleeping");
	usleep(p->data.info.time_sleep * MILLI_SEC);
	return (CORRECT);
}

int	ft_phi_think(t_phi *p, int time)
{
	ft_print(p, "is thinking");
	usleep(time * MILLI_SEC);
	return (CORRECT);
}

int	ft_solo_phi(t_phi *p)
{
	(void)p;
	return (CORRECT);
}

int	ft_phi_lives(t_phi *p)
{
	if (ft_fetch_key(p))
		ft_phi_eat(p);
	ft_phi_sleep(p);
	ft_phi_think(p, p->data.info.time_think);
	usleep(10 * MILLI_SEC);
	return (CORRECT);
}

void	*ft_action(void *arg)
{
	t_phi	*p;

	p = (t_phi *)arg;
	if (!(p->id % 2))
		ft_phi_think(p, p->data.info.time_eat / 10);
	if (p->data.nb_phi == 1)
		ft_solo_phi(p);
	else if (p->nb_meals >= 0)
	{
		while (p->nb_meals)
			if (!ft_phi_lives(p))
				break ;
	}
	else
	{
		while (p->alive)
		{
			if (!ft_phi_lives(p))
				break ;
		}
	}
	return (NULL);
}

void	ft_init_key(t_phi *p, int nb)
{
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*lock;
	pthread_mutex_t	*scribe;
	int				current;

	current = nb;
	mutex = calloc(nb, sizeof(pthread_mutex_t));
	lock = calloc(1, sizeof(pthread_mutex_t));
	scribe = calloc(1, sizeof(pthread_mutex_t));
	while (nb--)
		pthread_mutex_init(&mutex[nb], NULL);
	while (current--)
	{
		p[current].data.keys = mutex;
		p[current].data.lock = lock;
		p[current].data.scribe = scribe;
	}
}

void	ft_init_info(t_phi *p, int size)
{
	t_info	info;

	info.time_death = 300;
	info.time_think = 300;
	info.time_sleep = 100;
	info.time_eat = 100;
	while (size--)
		p[size].data.info = info;
}

void	ft_init_phi(t_phi *phi, int size, int nb_meals)
{
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
			phi[current].left = size - 1;
		phi[current].nb_meals = nb_meals;
		pthread_create(&phi[current].tid, NULL, &ft_action, &phi[current]);
	}
}

void	*ft_purge(t_phi *phi)
{
	free(phi[0].data.keys);
	free(phi[0].data.lock);
	free(phi[0].data.scribe);
	free(phi);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_phi	*phi;
	int		nb_p;
	int		i;

	i = 0;
	if (ac == 3 || ac == 2)
	{
		nb_p = atoi(av[1]);
		phi = calloc(nb_p, sizeof(t_phi));
		if (ac == 3)
			ft_init_phi(phi, nb_p, atoi(av[2]));
		else
			ft_init_phi(phi, nb_p, -1);
		while (i < nb_p)
			pthread_join(phi[i++].tid, NULL);
		while (i--)
			pthread_mutex_destroy(&phi[0].data.keys[i]);
		phi = ft_purge(phi);
	}
	else
		printf("bad args\n");
}
