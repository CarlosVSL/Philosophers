#include "../include/philo.h"
/*
** @brief  Print an error message, clean up and exit.
** @return Always returns 1.
*/

int	error(char *str, t_data *data)
{
	printf("%s\n", str);
	if (data)
		ft_exit(data);
	return (1);
}

uint64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return ((uint64_t)error("gettimeofday() failure", NULL));
	return ((uint64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_usleep(useconds_t time)
{
	uint64_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(time / 10);
	return (0);
}
