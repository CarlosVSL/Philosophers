#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdint.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	t_data			*data;
	int				id;
	int				eat_cont;
	int				eating;
	uint64_t		die_time;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	lock;
}					t_philo;

struct s_data
{
	pthread_t		*tid;
	int				philo_num;
	int				meals_nb;
	int				dead;
	int				finished;
	t_philo			*philos;
	uint64_t		death_time;
	uint64_t		eat_time;
	uint64_t		sleep_time;
	uint64_t		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
};

int		    check_args(int argc, char **argv, t_data *data);
int		    init_data(t_data *data);
int		    launch_simulation(t_data *data);
void	    clear_data(t_data *data);
void	    ft_exit(t_data *data);
int		    error(char *str, t_data *data);
uint64_t    get_time(void);
int		    ft_usleep(useconds_t time);
void	    *routine(void *arg);
void	    print_status(t_data *data, int id, char *msg);
int		    start_supervisor(t_philo *philo);
void	    *supervisor(void *arg);

#endif