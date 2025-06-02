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

static int	check_arg_numeric(const char *arg, int index)
{
	int	j;

	j = 0;
	if (arg[0] == '\0')
	{
		printf("Error: argument %d is empty\n", index);
		return (1);
	}
	while (arg[j])
	{
		if (!ft_isdigit(arg[j]))
		{
			printf("Error: argument %d is not numeric\n", index);
			return (1);
		}
		j++;
	}
	return (0);
}

static int	check_arg_range(long val, int index)
{
	if (index == 1 && (val < 1 || val > 200))
	{
		printf("Error: number_of_philosophers must be 1-200\n");
		return (1);
	}
	if (index >= 2 && val < 1)
	{
		printf("Error: time values must be >= 1\n");
		return (1);
	}
	if (index == 5 && val < 1)
	{
		printf("Error: number_of_times_each_philosopher_must_eat must be >= 1\n");
		return (1);
	}
	return (0);
}

int	check_args(int argc, char **argv, t_data *data)
{
	int		i;
	long	val;

	i = 0;
	while (++i < argc)
	{
		if (check_arg_numeric(argv[i], i))
			return (1);
		val = ft_atol(argv[i]);
		if (check_arg_range(val, i))
			return (1);
	}
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
	return (0);
}
