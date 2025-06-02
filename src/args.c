#include "../include/philo.h"

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static long	ft_atol(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

static int	validate_args(int argc, char **argv)
{
	int		i;
	int		j;
	long	val;

	i = 0;
	while (++i < argc)
	{
		if (argv[i][0] == '\0')
			return (printf("Error: argument %d is empty\n", i), 1);
		j = -1;
		while (argv[i][++j])
			if (!ft_isdigit(argv[i][j]))
				return (printf("Error: argument %d is not numeric\n", i), 1);
		val = ft_atol(argv[i]);
		if (i == 1 && (val < 1 || val > 200))
			return (printf("Error: number_of_philosophers must be 1-200\n"), 1);
		if (i >= 2 && val < 1)
			return (printf("Error: time values must be >= 1\n"), 1);
		if (i == 5 && val < 1)
			return (printf("Error: number_of_times_each_philosopher_"
				"must_be >= 1\n"), 1);
	}
	return (0);
}

static void	fill_data(int argc, char **argv, t_data *data)
{
	data->philo_num  = (int)ft_atol(argv[1]);
	data->death_time = (uint64_t)ft_atol(argv[2]);
	data->eat_time   = (uint64_t)ft_atol(argv[3]);
	data->sleep_time = (uint64_t)ft_atol(argv[4]);
	if (argc == 6)
		data->meals_nb = (int)ft_atol(argv[5]);
	else
		data->meals_nb = 0;
	data->dead     = 0;
	data->finished = 0;
}

int	check_args(int argc, char **argv, t_data *data)
{
	if (validate_args(argc, argv))
		return (1);
	fill_data(argc, argv, data);
	return (0);
}
