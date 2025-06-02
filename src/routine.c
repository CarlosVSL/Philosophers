#include "../include/philo.h"


#include "philo.h"

static void	print_status(t_data *data, int id, char *msg)
{
	uint64_t	ts;

	pthread_mutex_lock(&data->write);
	ts = get_time() - data->start_time;
	if (!data->dead)
		printf("%llu %d %s\n",
			(unsigned long long)ts,
			id,
			msg);
	pthread_mutex_unlock(&data->write);
}

void	*routine(void *arg)
{
	t_philo		*philo = (t_philo *)arg;
	t_data		*data  = philo->data;
	pthread_t	sup;
	int			stop;

	/* Inicializar die_time absoluto y lanzar supervisor */
	pthread_mutex_lock(&philo->lock);
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	if (pthread_create(&sup, NULL, supervisor, philo) != 0)
		return (NULL);
	pthread_detach(sup);

	/* Caso especial: un solo filósofo */
	if (data->philo_num == 1)
	{
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
		return (NULL);
	}

	/* Retraso inicial para evitar deadlock */
	if (philo->id % 2 == 0)
		ft_usleep(data->eat_time);

	/* Bucle principal */
	while (1)
	{
		/* Salir si la simulación terminó */
		pthread_mutex_lock(&data->lock);
		stop = data->dead;
		pthread_mutex_unlock(&data->lock);
		if (stop)
			break ;

		/* Tomar tenedores */
		pthread_mutex_lock(philo->r_fork);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
		print_status(data, philo->id, "has taken a fork");

		/* Comer: actualizar die_time y marcar eating */
		pthread_mutex_lock(&philo->lock);
		philo->eating   = 1;
		philo->die_time = get_time() + data->death_time;
		pthread_mutex_unlock(&philo->lock);
		print_status(data, philo->id, "is eating");
		ft_usleep(data->eat_time);

		/* Actualizar conteo de comidas y posible fin */
		pthread_mutex_lock(&data->lock);
		philo->eat_cont++;
		if (data->meals_nb > 0 && philo->eat_cont == data->meals_nb)
			data->finished++;
		if (data->meals_nb > 0 && data->finished == data->philo_num)
			data->dead = 1;
		stop = data->dead;
		pthread_mutex_unlock(&data->lock);
		if (stop)
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			break ;
		}

		/* Terminar de comer */
		pthread_mutex_lock(&philo->lock);
		philo->eating = 0;
		pthread_mutex_unlock(&philo->lock);

		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);

		/* Dormir y pensar */
		print_status(data, philo->id, "is sleeping");
		ft_usleep(data->sleep_time);
		print_status(data, philo->id, "is thinking");
	}
	return (NULL);
}