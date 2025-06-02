#include "../include/philo.h"

void *supervisor(void *arg)
{
    t_philo *ph = arg;
    t_data  *d  = ph->data;
    uint64_t now;

    while (1)
    {
        usleep(1000);
        pthread_mutex_lock(&ph->lock);
        now = get_time();
        // Solo muere si NO está comiendo Y ya hemos pasado die_time
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
            break;
        }
        pthread_mutex_unlock(&ph->lock);
        pthread_mutex_lock(&d->lock);
        if (d->dead)
        {
            pthread_mutex_unlock(&d->lock);
            break;
        }
        pthread_mutex_unlock(&d->lock);
    }
    return (NULL);
}