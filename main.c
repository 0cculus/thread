/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 10:17:33 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 16:30:54 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <strings.h>
# include <limits.h>

# define THINK 10
# define CORRECT 1
# define INCORRECT 0
# define CORRECT_EXIT 0
# define INCORRECT_EXIT 1
# define MILLI_SEC 1000

typedef long long	t_chrono;

typedef struct s_info
{
	t_chrono	time_eat;
	t_chrono	time_death;
	t_chrono	time_sleep;
	t_chrono	time_think;
}	t_info;

typedef struct s_key
{
	int				lock;
	pthread_mutex_t	mutex;
}	t_key;

typedef struct s_phi
{
	int				id;
	pthread_t		tid;
	struct s_data	*data;
	int				dead;
	t_key			*left;
	t_key			*right;
	int				nb_meals;
	t_chrono		last_meal;
}	t_phi;

typedef struct s_data
{
	t_phi			*phi;
	t_info			info;
	t_key			*keys;
	int				nb_phi;
	pthread_mutex_t	scribe;
	int				nb_meals;
	int				all_dead;
	pthread_mutex_t	key_seeker;
	pthread_mutex_t	death_seeker;
	int				all_you_can_eat;
}	t_data;

t_chrono	ft_get_time(void)
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
	if (p->data->all_dead)
		reap = CORRECT;
	else
		reap = INCORRECT;
	pthread_mutex_unlock(&p->data->death_seeker);
	return (reap);
}

void	ft_print(t_phi *p, char *src)
{
	pthread_mutex_lock(&p->data->scribe);
	if (!ft_azrael(p) || (ft_azrael(p) && p->dead))
		printf ("%lld %d %s\n", ft_get_time(), p->id, src);
	pthread_mutex_unlock(&p->data->scribe);
}

void	ft_samael(t_phi *p)
{
	pthread_mutex_lock(&p->data->death_seeker);
	if (!p->data->all_dead)
	{
		p->dead = CORRECT;
		p->data->all_dead = CORRECT;
		pthread_mutex_unlock(&p->data->death_seeker);
		ft_print(p, "is deadge, smodge smodge");
		return ;
	}
	pthread_mutex_unlock(&p->data->death_seeker);
}

void	ft_slumber(t_phi *p, t_chrono current)
{
	t_chrono	start;

	start = ft_get_time();
	if (p->last_meal < current + start && !p->dead && \
		p->last_meal > start)
	{
		usleep((p->last_meal - start) * MILLI_SEC);
		ft_samael(p);
	}
	else
	{
		usleep(current * MILLI_SEC * 0.9);
		while (ft_get_time() < current + start)
			usleep (MILLI_SEC);
	}
}

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

void	ft_phi_eat(t_phi *p)
{
	usleep(150);
	while (!ft_fetch_key(p) || p->last_meal <= ft_get_time())
	{
		if (p->last_meal <= ft_get_time())
		{
			ft_samael(p);
			return ;
		}
		if (ft_azrael(p))
			break ;
		usleep(100);
	}
	pthread_mutex_lock(&p->left->mutex);
	ft_print(p, "has taken a fork");
	pthread_mutex_lock(&p->right->mutex);
	ft_print(p, "has taken a fork");
	ft_print(p, "is eating");
	p->last_meal = ft_get_time() + p->data->info.time_death;
	ft_slumber(p, p->data->info.time_eat);
	pthread_mutex_unlock(&p->left->mutex);
	pthread_mutex_unlock(&p->right->mutex);
	pthread_mutex_lock(&p->data->key_seeker);
	p->left->lock = INCORRECT;
	p->right->lock = INCORRECT;
	pthread_mutex_unlock(&p->data->key_seeker);
}

void	*ft_lives(void *arg)
{
	t_phi	*p;

	p = (t_phi *)arg;
	if (p->data->all_you_can_eat && !p->nb_meals)
		return (NULL);
	p->last_meal = ft_get_time() + p->data->info.time_death;
	if (p->id & 1)
		ft_slumber(p, p->data->info.time_eat);
	while (!ft_azrael(p))
	{
		ft_print(p, "is thinking");
		if (!ft_azrael(p))
			ft_phi_eat(p);
		if (p->data->all_you_can_eat)
			p->nb_meals--;
		if (!p->nb_meals && p->data->all_you_can_eat)
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
		if (pthread_join(data->phi[i++].tid, NULL) != 0)
			return (INCORRECT_EXIT);
	return (CORRECT_EXIT);
}

int	ft_init_keys(int nb_p, t_data *data)
{
	data->keys = calloc(nb_p, sizeof(t_key));
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

	bzero(data, sizeof(t_data));
	bzero(&info, sizeof(t_info));
	info.time_death = atol(av[2]);
	info.time_eat = atol(av[3]);
	info.time_sleep = atol(av[4]);
	info.time_think = 30;
	if (ac == 6)
	{
		data->all_you_can_eat = CORRECT;
		data->nb_meals = atol(av[5]);
	}
	else
		data->all_you_can_eat = INCORRECT;
	data->info = info;
}

int ft_release(int nb_p, t_data *data)
{
	int	current;

	current = -1;
	while (++current < nb_p)
		if (pthread_create(&data->phi[current].tid,
			NULL, &ft_lives, &data->phi[current]) != 0)
			return (INCORRECT_EXIT);
	return (CORRECT_EXIT);
}

int	ft_init_phi(int nb_p, t_data *data)
{
	int		current;

	current = -1;
	data->phi = calloc(nb_p, sizeof(t_phi));
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

int	ft_verify_args(int ac, char **av)
{
	int	i;
	long long current;

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
