#include "../include/philo.h"

static int	handle_die(t_philo *ph)
{
	t_data		*d;
	uint64_t	now;

	d = ph->data;
	pthread_mutex_lock(&ph->lock);
	now = get_time();
	if (!ph->eating && now >= ph->die_time)
	{
		pthread_mutex_unlock(&ph->lock);
		pthread_mutex_lock(&d->lock);
		if (!d->dead)
		{
			d->dead = 1;
			pthread_mutex_lock(&d->write);
			printf("%llu %d died\n",
				(unsigned long long)(now - d->start_time),
				ph->id);
			pthread_mutex_unlock(&d->write);
		}
		pthread_mutex_unlock(&d->lock);
		return (1);
	}
	pthread_mutex_unlock(&ph->lock);
	return (0);
}

static int	check_global_dead(t_data *d)
{
	int	stop;

	pthread_mutex_lock(&d->lock);
	stop = d->dead;
	pthread_mutex_unlock(&d->lock);
	return (stop);
}

void	*supervisor(void *arg)
{
	t_philo	*ph;
	t_data	*d;

	ph = (t_philo *)arg;
	d = ph->data;
	while (1)
	{
		usleep(1000);
		if (handle_die(ph))
			break ;
		if (check_global_dead(d))
			break ;
	}
	return (NULL);
}
