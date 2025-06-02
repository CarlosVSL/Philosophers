#include "../include/philo.h"

void	print_status(t_data *data, int id, char *msg)
{
	uint64_t	ts;

	pthread_mutex_lock(&data->write);
	ts = get_time() - data->start_time;
	if (!data->dead)
		printf("%llu %d %s\n", (unsigned long long)ts, id, msg);
	pthread_mutex_unlock(&data->write);
}

int	start_supervisor(t_philo *philo)
{
	t_data		*data;
	pthread_t	sup;

	data = philo->data;
	pthread_mutex_lock(&philo->lock);
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	if (pthread_create(&sup, NULL, supervisor, philo) != 0)
		return (1);
	pthread_detach(sup);
	return (0);
}
