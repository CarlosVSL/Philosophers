#include "../include/philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die "
			"time_to_eat time_to_sleep "
			"[number_of_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (1);
	}
	if (check_args(argc, argv, &data))
		return (1);
	if (init_data(&data))
		return (error("Initialization failed", &data));
	if (launch_simulation(&data))
		return (error("Simulation failed", &data));
	ft_exit(&data);
	return (0);
}
