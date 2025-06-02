#include "../include/philo.h"
/*
** @brief  Launch philosopher threads and optional monitor thread,
**         then wait for all philosopher threads to finish.
** @return 0 on success, 1 on any pthread error.
*/
int	launch_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_create(&data->tid[i], NULL,
				routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(data->tid[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}