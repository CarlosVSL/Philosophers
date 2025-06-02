#include "../include/philo.h"

ChatGPT Plus

Pensó durante 5m 9s
El problema ya no está en los tiempos: es tu forma de coger los tenedores.
Con la estrategia actual ― bloquear un tenedor y quedarse bloqueado sobre el segundo ― puedes dejar a un filósofo con un solo tenedor agarrado durante toda una ronda, impidiéndole progresar y, a la vez, retrasando a su vecino.
Eso pasa sobre todo con 5 filósofos, pero con los parámetros 4 310 200 100 el efecto contrario se produce: la espera nunca supera los 310 ms y nadie muere, cuando la rúbrica exige que uno sí lo haga.

1. Dónde está el fallo
c
Copiar
Editar
static void take_forks(t_philo *philo)
{
    /* … decide qué tenedor es first y cuál es second … */
    pthread_mutex_lock(first);            // ← lo bloqueas
    print_status(..., "has taken a fork");
    pthread_mutex_lock(second);           // ← aquí te quedas BLOQUEADO
    print_status(..., "has taken a fork");
}
``` :contentReference[oaicite:0]{index=0}

* Si el segundo tenedor nunca se libera a tiempo, el hilo se queda **hundido** con el primero en la mano.  
* En el test `4 310 200 100` eso hace que todos acaben entrando en un ciclo **exactamente** de 300 ms (200 eat + 100 sleep) → nadie rebasa los 310 ms y no hay muertes.  
* En el test `5 800 200 200`, en cambio, el filósofo 5 puede retener el tenedor 0 mientras espera el 4; su contador sigue corriendo y acaba muerto a los 2400 ms.

### 2. Solución: intenta-liberar-reintenta

1. **Coge un tenedor.**  
2. **Intenta** coger el otro con `pthread_mutex_trylock`.  
3. Si falla, **suelta** el primero, espera unas decenas de microsegundos y vuelve a empezar.

Así ningún filósofo acapara un solo tenedor más de unas pocas µs:  
* Con `4 310 200 100`, siempre habrá algún intervalo sin poder comer que exceda los 310 ms para (al menos) un filósofo → se produce la muerte que pide la rúbrica.  
* Con `5 800 200 200`, cada uno consigue los dos tenedores antes de 800 ms → no muere ninguno.

### 3. Código propuesto

```c
static void take_forks(t_philo *philo)
{
    pthread_mutex_t *first;
    pthread_mutex_t *second;

    if (philo->l_fork < philo->r_fork)
    {
        first  = philo->l_fork;
        second = philo->r_fork;
    }
    else
    {
        first  = philo->r_fork;
        second = philo->l_fork;
    }
    while (1)
    {
        pthread_mutex_lock(first);
        if (pthread_mutex_trylock(second) == 0)
        {
            print_status(philo->data, philo->id, "has taken a fork");
            print_status(philo->data, philo->id, "has taken a fork");
            return;
        }
        pthread_mutex_unlock(first);
        usleep(100);
    }
}

static int	eat_and_check(t_philo *philo)
{
	t_data	*data;
	int		stop;

	data = philo->data;
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	pthread_mutex_unlock(&philo->lock);
	print_status(data, philo->id, "is eating");
	ft_usleep(data->eat_time);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	philo->die_time = get_time() + data->death_time;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_lock(&data->lock);
	philo->eat_cont++;
	if (data->meals_nb > 0 && philo->eat_cont == data->meals_nb)
		data->finished++;
	if (data->meals_nb > 0 && data->finished == data->philo_num)
		data->dead = 1;
	stop = data->dead;
	pthread_mutex_unlock(&data->lock);
	return (stop);
}

static void	finish_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->lock);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

static void	sleep_and_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "is sleeping");
	ft_usleep(data->sleep_time);
	print_status(data, philo->id, "is thinking");
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (start_supervisor(philo))
		return (NULL);
	if (data->philo_num == 1)
		return (single_philo(philo), NULL);
	if (philo->id % 2 == 0)
		ft_usleep(data->eat_time);
	while (!is_stopped(data))
	{
		if (is_stopped(data))
			break ;
		take_forks(philo);
		if (eat_and_check(philo))
		{
			finish_meal(philo);
			break ;
		}
		finish_meal(philo);
		sleep_and_think(philo);
	}
	return (NULL);
}
