/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brheaume <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 17:04:13 by brheaume          #+#    #+#             */
/*   Updated: 2023/06/26 17:53:24 by brheaume         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

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

t_chrono	ft_get_time(void);
long		ft_atol(char *src);
int			ft_azrael(t_phi *p);
void		ft_samael(t_phi *p);
void		*ft_lives(void *arg);
void		ft_phi_eat(t_phi *p);
int			ft_fetch_key(t_phi *p);
void		ft_bzero(void *src, int nb);
void		*ft_calloc(int nb, int size);
void		ft_print(t_phi *p, char *src);
int			ft_purge(t_data *data, int res);
int			ft_destroy(t_data *data, int nb);
int			ft_verify_args(int ac, char **av);
int			ft_release(int nb_p, t_data *data);
int			ft_join_phi(int nb_p, t_data *data);
int			ft_init_phi(int nb_p, t_data *data);
int			ft_init_keys(int nb_p, t_data *data);
void		ft_slumber(t_phi *p, t_chrono current);
void		ft_init_data(char **av, int ac, t_data *data);

#endif
