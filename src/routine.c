#include "../include/philo.h"

static void	handle_single_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(philo->r_fork);
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&philo->lock);
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	ft_usleep(data->death_time);
	pthread_mutex_lock(&data->lock);
	if (!data->dead)
	{
		data->dead = 1;
		print_status(data, philo->id, "died");
	}
	pthread_mutex_unlock(&data->lock);
	pthread_mutex_unlock(philo->r_fork);
}

static void	stagger_start(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data->eat_time);
}

static int	check_and_take(t_philo *philo)
{
	t_data	*data;
	int		stop;

	data = philo->data;
	pthread_mutex_lock(&data->lock);
	stop = data->dead;
	pthread_mutex_unlock(&data->lock);
	if (stop)
		return (1);
	pthread_mutex_lock(philo->r_fork);
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(philo->l_fork);
	print_status(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	print_status(data, philo->id, "is eating");
	ft_usleep(data->eat_time);
	return (0);
}

static int	post_eat_and_check(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->lock);
	philo->eat_cont++;
	if (philo->data->meals_nb > 0
		&& philo->eat_cont == philo->data->meals_nb)
		philo->data->finished++;
	if (philo->data->meals_nb > 0
		&& philo->data->finished == philo->data->philo_num)
		philo->data->dead = 1;
	stop = philo->data->dead;
	pthread_mutex_unlock(&philo->data->lock);
	if (stop)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	return (0);
}

void	*routine(void *arg)
{
	t_philo		*philo;
	t_data		*data;

	philo = (t_philo *)arg;
	data  = philo->data;
	if (start_supervisor(philo))
		return (NULL);
	if (data->philo_num == 1)
	{
		handle_single_philo(philo);
		return (NULL);
	}
	stagger_start(philo);
	while (1)
	{
		if (check_and_take(philo))
			break ;
		if (post_eat_and_check(philo))
			break ;
		print_status(data, philo->id, "is sleeping");
		ft_usleep(data->sleep_time);
		print_status(data, philo->id, "is thinking");
	}
	return (NULL);
}
