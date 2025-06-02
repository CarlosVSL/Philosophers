#include "../include/philo.h"

int	start_supervisor(t_philo *philo)
{
	pthread_t	sup;
	t_data		*data;

	data = philo->data;
	pthread_mutex_lock(&philo->lock);
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	if (pthread_create(&sup, NULL, supervisor, philo) != 0)
		return (1);
	pthread_detach(sup);
	return (0);
}

void	single_philo(t_philo *philo)
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
		pthread_mutex_lock(&data->write);
		printf("%llu %d died\n",
			(unsigned long long)(get_time() - data->start_time),
			philo->id);
		pthread_mutex_unlock(&data->write);
		data->dead = 1;
	}
	pthread_mutex_unlock(&data->lock);
	pthread_mutex_unlock(philo->r_fork);
}

void	print_status(t_data *data, int id, char *msg)
{
	uint64_t	ts;

	pthread_mutex_lock(&data->lock);
	if (data->dead)
	{
		pthread_mutex_unlock(&data->lock);
		return ;
	}
	pthread_mutex_lock(&data->write);
	ts = get_time() - data->start_time;
	printf("%llu %d %s\n", (unsigned long long)ts, id, msg);
	pthread_mutex_unlock(&data->write);
	pthread_mutex_unlock(&data->lock);
}


int	is_stopped(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->lock);
	stop = data->dead;
	pthread_mutex_unlock(&data->lock);
	return (stop);
}
