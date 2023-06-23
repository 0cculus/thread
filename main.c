/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:17:33 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/23 15:01:45 by brheaume         ###   ########.fr       */
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
# define CORRECT_EXIT 0
# define INCORRECT_EXIT 1
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
	int				id;
	pthread_t		tid;
	struct s_data	*data;
	int				alive;
	t_key			*left;
	t_key			*right;
	int				nb_meals;
	long long		last_meal;
}	t_phi;

typedef struct s_data
{
	t_phi			*phi;
	t_info			info;
	t_key			*keys;
	int				nb_phi;
	pthread_mutex_t	scribe;
	int				nb_meals;
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
	return ((current.tv_sec * 1000 - start.tv_sec * 1000) + \
			(current.tv_usec / 1000 - start.tv_usec / 1000));
}

int	ft_azrael(t_phi *p)
{
	int	reap;

	pthread_mutex_lock(&p->data->death_seeker);
	if (!p->data->all_alive)
		reap = CORRECT;
	else
		reap = INCORRECT;
	pthread_mutex_unlock(&p->data->death_seeker);
	return (reap);
}

void	ft_print(t_phi *p, char *src)
{
	pthread_mutex_lock(&p->data->scribe);
	if (!ft_azrael(p) || (ft_azrael(p) && !p->alive))
		printf ("%lld %d %s\n", ft_get_time(), p->id, src);
	pthread_mutex_unlock(&p->data->scribe);
}

int	ft_samael(t_phi *p)
{
	pthread_mutex_lock(&p->data->death_seeker);
	if (p->alive)
	{
		p->alive = INCORRECT;
		p->data->all_alive = INCORRECT;
		ft_print(p, "is deadge, smodge smodge");
	}
	pthread_mutex_unlock(&p->data->death_seeker);
	return (INCORRECT);
}

void	ft_slumber(t_phi *p, long long current)
{
	long long	start;

	start = ft_get_time();
	if (p->last_meal < current + start && p->alive && \
		p->last_meal > start)
	{
		usleep((p->last_meal - start) * MILLI_SEC);
		ft_samael(p);
	}
	else
	{
		usleep(current * MILLI_SEC * 0.9);
		while (ft_get_time() < current + start)
			usleep (0.15 * MILLI_SEC);
	}
}

int	ft_fetch_key(t_phi *p)
{
	int	res;

	pthread_mutex_lock(&p->data->key_seeker);
	if (p->left == p->right)
		res = 0;
	else if (!p->right->lock && !p->left->lock)
	{
		p->left->lock = CORRECT;
		p->right->lock = CORRECT;
		res = 1;
	}
	else
		res = 0;
	pthread_mutex_unlock(&p->data->key_seeker);
	return (res);
}

int	ft_phi_eat(t_phi *p)
{
	usleep(500);
	while (!ft_fetch_key(p) || p->last_meal <= ft_get_time())
	{
		if (p->last_meal <= ft_get_time())
			return (ft_samael(p));
		if (p->data->all_alive)
			break ;
		usleep(100);
	}
	pthread_mutex_lock(&p->right->mutex);
	ft_print(p, "has taken his right fork");
	pthread_mutex_lock(&p->left->mutex);
	ft_print(p, "has taken his left fork");
	ft_print(p, "is eating");
	p->last_meal = ft_get_time() + p->data->info.time_death;
	ft_slumber(p, p->data->info.time_eat);
	pthread_mutex_unlock(&p->right->mutex);
	p->left->lock = INCORRECT;
	pthread_mutex_unlock(&p->left->mutex);
	p->right->lock = INCORRECT;
	return (CORRECT);
}

int	ft_phi_sleep(t_phi *p)
{
	ft_print(p, "is sleeping");
	usleep(p->data->info.time_sleep * MILLI_SEC);
	return (CORRECT);
}

int	ft_phi_think(t_phi *p, int time)
{
	ft_print(p, "is thinking");
	usleep(time * MILLI_SEC);
	return (CORRECT);
}

void	*ft_lives(void *arg)
{
	t_phi	*p;

	p = (t_phi *)arg;
	if (p->id % 2)
		ft_phi_think(p, p->data->info.time_eat);
	while (!ft_azrael(p))
	{
		ft_print(p, "is thinking");
		if (!ft_azrael(p))
			ft_phi_eat(p);
		if (p->nb_meals)
			p->nb_meals--;
		if (!p->nb_meals)
			return (NULL);
		if (!ft_azrael(p))
			ft_print(p, "is sleeping");
		ft_slumber(p, p->data->info.time_sleep);
	}
	return (NULL);
}

int	ft_join_phi(int nb_p, t_data *data)
{
	int	i;

	i = 0;
	while (i < nb_p)
		if (!pthread_join(data->phi[i++].tid, NULL))
			return (INCORRECT);
	return (CORRECT);
}

int	ft_init_keys(int nb, t_data *data)
{
	data->keys = calloc(nb, sizeof(pthread_mutex_t));
	if (!data->keys)
		return (INCORRECT);
	while (nb--)
	{
		data->keys[nb].lock = INCORRECT;
		if (pthread_mutex_init(&data->keys[nb].mutex, NULL) != 0)
			return (INCORRECT);
	}
	if (pthread_mutex_init(&data->death_seeker, NULL) != 0 || \
	pthread_mutex_init(&data->key_seeker, NULL) != 0 || \
	pthread_mutex_init(&data->scribe, NULL) != 0)
		return (INCORRECT);
	return (CORRECT);
}

void	ft_init_data(char **av, int ac, int size, t_data *data)
{
	t_info	info;

	info.time_death = atol(av[2]);
	info.time_eat = atol(av[3]);
	info.time_sleep = atol(av[4]);
	info.time_think = 30;
	if (ac == 6)
		data->nb_meals = atol(av[5]);
	else
		data->nb_meals = -1;
	while (size--)
		data->phi[size].data->info = info;
}

int ft_release(int nb_p, t_data *data)
{
	int	current;

	current = nb_p;
	while (current--)
		if (pthread_create(&data->phi[current].tid,
			NULL, &ft_lives, &data->phi[current]) != 0)
			return (INCORRECT);
	return (CORRECT);
}

int	ft_init_phi(int size, t_data *data)
{
	int		current;
	t_phi	*phi;

	current = size;
	phi = calloc(size, sizeof(t_phi));
	if (!phi)
		return (INCORRECT);
	while (current--)
	{
		phi[current].last_meal = data->info.time_death;
		phi[current].data = data;
		phi[current].alive = CORRECT;
		phi[current].id = current + 1;
		phi[current].right = &data->keys[current];
		if (current != 0)
			phi[current].left = &data->keys[current - 1];
		else
			phi[current].left = &data->keys[size - 1];
		phi[current].nb_meals = data->nb_meals;
	}
	data->phi = phi;
	return (CORRECT);
}

int	ft_destroy(t_data *data, int nb)
{
	while (nb--)
		if (pthread_mutex_destroy(&data->keys[nb].mutex))
			return (INCORRECT);
	if (pthread_mutex_destroy(&data->scribe) || \
		pthread_mutex_destroy(&data->key_seeker) || \
		pthread_mutex_destroy(&data->death_seeker))
		return (INCORRECT);
	return (CORRECT);
}

int	ft_purge(t_data *data, int res)
{
	if (data->keys)
		free(data->keys);
	if (data->phi)
		free(data->phi);
	return (res);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		nb_p;

	if (ac == 5 || ac == 6)
	{
		nb_p = atoi(av[1]);
		if (!ft_init_keys(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (!ft_init_phi(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		ft_init_data(av, ac, nb_p, &data);
		if (!ft_release(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (!ft_join_phi(nb_p, &data))
			return (ft_purge(&data, INCORRECT_EXIT));
		if (!ft_destroy(&data, nb_p))
			return (ft_purge(&data, INCORRECT_EXIT));
		return (ft_purge(&data, CORRECT_EXIT));
	}
	else
		printf("bad args\n");
}
