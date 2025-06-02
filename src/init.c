#include "../include/philo.h"
/*
** @brief  Allocate and initialize all data structures and mutexes.
** @return 0 on success, 1 on failure.
*/
int init_data(t_data *data)
{
    int i;

    data->philos = malloc(sizeof(t_philo) * data->philo_num);
    data->forks  = malloc(sizeof(pthread_mutex_t) * data->philo_num);
    data->tid    = malloc(sizeof(pthread_t) * data->philo_num);
    if (!data->philos || !data->forks || !data->tid)
        return (1);

    if (pthread_mutex_init(&data->lock, NULL) != 0
        || pthread_mutex_init(&data->write, NULL) != 0)
        return (1);

    i = 0;
    while (i < data->philo_num)
        if (pthread_mutex_init(&data->forks[i++], NULL) != 0)
            return (1);

    data->start_time = get_time();
    if (data->start_time == 0)
        return (1);

    i = 0;
    while (i < data->philo_num)
    {
        data->philos[i].data     = data;
        data->philos[i].id       = i + 1;
        data->philos[i].eat_cont = 0;
        data->philos[i].eating   = 0;
        /* inicializamos un die_time provisional */
        data->philos[i].die_time = data->start_time + data->death_time;
        data->philos[i].r_fork   = &data->forks[i];
        data->philos[i].l_fork   = &data->forks[(i + 1) % data->philo_num];
        if (pthread_mutex_init(&data->philos[i].lock, NULL) != 0)
            return (1);
        i++;
    }
    return (0);
}