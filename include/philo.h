
#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdint.h>

typedef struct s_data   t_data;

typedef struct s_philo
{
    t_data          *data;
    int             id;
    int             eat_cont;
    int             eating;
    uint64_t        die_time;   /* instante absoluto en ms en que muere si no come */
    pthread_mutex_t *r_fork;
    pthread_mutex_t *l_fork;
    pthread_mutex_t lock;
}                   t_philo;

struct s_data
{
    pthread_t       *tid;
    int             philo_num;
    int             meals_nb;
    int             dead;
    int             finished;
    t_philo         *philos;
    uint64_t        death_time; /* intervalo en ms para morir tras última comida */
    uint64_t        eat_time;
    uint64_t        sleep_time;
    uint64_t        start_time;
    pthread_mutex_t *forks;
    pthread_mutex_t lock;
    pthread_mutex_t write;
};

/*
** args.c
*/
int     check_args(int argc, char **argv, t_data *data);

/*
** init.c
*/
int     init_data(t_data *data);

/*
** structs.c
*/
int     launch_simulation(t_data *data);

/*
** cleanup.c
*/
void    clear_data(t_data *data);
void    ft_exit(t_data *data);

/*
** utils.c
*/
int     error(char *str, t_data *data);
uint64_t get_time(void);
int     ft_usleep(useconds_t time);

/*
** routine.c
*/
void    *routine(void *arg);

/*
** supervisor.c
*/
void    *supervisor(void *arg);

#endif