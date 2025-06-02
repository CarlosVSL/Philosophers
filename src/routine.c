#include "../include/philo.h"

static void	take_forks(t_philo *philo)
{
	t_data			*d;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	d = philo->data;
	if (philo->l_fork < philo->r_fork)
	{
		first = philo->l_fork;
		second = philo->r_fork;
	}
	else
	{
		first = philo->r_fork;
		second = philo->l_fork;
	}
	pthread_mutex_lock(first);
	print_status(d, philo->id, "has taken a fork");
	pthread_mutex_lock(second);
	print_status(d, philo->id, "has taken a fork");
}

static int	eat_and_check(t_philo *philo)
{
	t_data	*data;
	int		stop;

	data = philo->data;
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	pthread_mutex_unlock(&philo->lock);
	print_status(data, philo->id, "is eating");
	ft_usleep(data->eat_time);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_lock(&data->lock);
	philo->eat_cont++;
	if (data->meals_nb > 0 && philo->eat_cont == data->meals_nb)
		data->finished++;
	if (data->meals_nb > 0 && data->finished == data->philo_num)
		data->dead = 1;
	stop = data->dead;
	pthread_mutex_unlock(&data->lock);
	return (stop);
}

static void	finish_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

static void	sleep_and_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "is sleeping");
	ft_usleep(data->sleep_time);
	print_status(data, philo->id, "is thinking");
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (start_supervisor(philo))
		return (NULL);
	if (data->philo_num == 1)
		return (single_philo(philo), NULL);
	if (philo->id % 2 == 0)
		ft_usleep(data->eat_time);
	while (!is_stopped(data))
	{
		if (is_stopped(data))
			break ;
		take_forks(philo);
		if (eat_and_check(philo))
		{
			finish_meal(philo);
			break ;
		}
		finish_meal(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
