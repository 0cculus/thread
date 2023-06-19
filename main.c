/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:17:33 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/19 15:46:24 by brheaume         ###   ########.fr       */
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

typedef struct s_info
{
	int	time_eat;
	int	time_death;
	int	time_sleep;
	int	time_think;
}	t_info;

typedef struct s_key
{
	pthread_mutex_t	mutex;
	short			lock;
}	t_key;

typedef struct s_phi
{
	pthread_t	tid;
	t_key		left;
	t_key		right;
	int			id;
	int			alive;
	int			nb_meals;
	long long	last_meal;
}	t_phi;

typedef struct s_data
{
	t_phi			*phi;
	t_info			info;
	t_key			*keys;
	int				nb_phi;
	pthread_mutex_t	scribe;
	short			all_alive;
	pthread_mutex_t	key_seeker;
	pthread_mutex_t	death_seeker;
}	t_data;

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

int	ft_unleash_keys(t_data *data)
{
	ft_print(p, "has unleashed both forks");
	pthread_mutex_unlock(&p->data.keys[p->right]);
	pthread_mutex_unlock(&p->data.keys[p->left]);

	return (CORRECT);
}

int	ft_fetch_key(t_data *data)
{
	(void)p;
	return (CORRECT);
}

int	ft_phi_eat(t_data *data)
{
	pthread_mutex_lock(&data->keys[data->right]);
	ft_print(p, "has taken his right fork");
	pthread_mutex_lock(&data->keys[p->left]);
	ft_print(p, "has taken his left fork");
	ft_print(p, "is eating");
	p->nb_meals--;
	usleep(data->info.time_eat * MILLI_SEC);
	pthread_mutex_unlock(&data->keys[p->right]);
	pthread_mutex_unlock(&p->data.keys[p->left]);
	return (CORRECT);
}

int	ft_phi_sleep(t_data *data)
{
	ft_print(p, "is sleeping");
	usleep(data->info.time_sleep * MILLI_SEC);
	return (CORRECT);
}

int	ft_phi_think(t_phi *p, int time)
{
	ft_print(p, "is thinking");
	usleep(time * MILLI_SEC);
	return (CORRECT);
}

int	ft_solo_phi(t_data *data)
{
	(void)p;
	return (CORRECT);
}

int	ft_phi_lives(t_data *data)
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
		ft_phi_think(p, 50);
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

int	ft_init_keys(int nb, t_data *data)
{
	data = calloc(nb, sizeof(pthread_mutex_t));
	if (!data->keys)
		return (INCORRECT);
	while (nb--)
	{
		data->keys[nb].lock = INCORRECT;
		if (!pthread_mutex_init(data->keys[nb].mutex, NULL))
			return (INCORRECT);
	}
	if (!pthread_mutex_init(&data->death_seeker, NULL) || \
	!pthread_mutex_init(&data->key_seeker, NULL) || \
	!pthread_mutex_init(&data->scribe, NULL))
		return (INCORRECT);
	return (CORRECT);
}

void	ft_init_data(char **av, int ac, t_data *data)
{
	t_info	info;

	info.time_death = atol(av[1]);
	info.time_eat = atol(av[2]);
	info.time_sleep = atol(av[3]);
	info.time_think = 30;
	if (ac == 5)
		data->nb_meals = atol(av[4]);
	else
		data->nb_meals = -1;
	while (size--)
		p[size].data.info = info;
}

int	ft_init_phi(int size, t_data *data)
{
	int		current;
	t_phi	*phi

	current = size;
	phi = calloc(nb_p, sizeof(t_phi));
	if (!phi)
		return (INCORRECT);
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
	data->phi = phi;
	return (CORRECT)
}

void	*ft_purge(t_data *data)
{
	free(phi[0].data.keys);
	free(phi[0].data.lock);
	free(phi[0].data.scribe);
	free(phi);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		nb_p;
	int		i;

	i = 0;
	if (ac == 3 || ac == 2)
	{
		nb_p = atoi(av[1]);
		ft_init_data(av, ac, &data);
		ft_init_key(nb_p, &data);
		ft_init_phi(nb_p, &data);
		while (i < nb_p)
			pthread_join(phi[i++].tid, NULL);
		while (i--)
			pthread_mutex_destroy(&phi[0].data.keys[i]);
		phi = ft_purge(phi);
	}
	else
		printf("bad args\n");
}
