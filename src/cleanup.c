#include "../include/philo.h"

void	clear_data(t_data *data)
{
	if (data->tid)
	{
		free(data->tid);
		data->tid = NULL;
	}
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}

void	ft_exit(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philo_num)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
	}
	if (data->philos)
	{
		i = 0;
		while (i < data->philo_num)
		{
			pthread_mutex_destroy(&data->philos[i].lock);
			i++;
		}
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	clear_data(data);
}