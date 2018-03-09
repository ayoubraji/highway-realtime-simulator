/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//vehicles quantity... da cambiare e rendere dinamico
#define N 100

//Highway lanes
#define L 4

//Road lenght
//Lo si può vedere anche come numero di caselle,
//dove ogni veicolo occupa n caselle
#define ROAD_LENGHT 1000

/* struttura condivisa */
struct highway_t {

    pthread_mutex_t m;
    pthread_cond_t priv_Vehicle[N];
    pthread_cond_t priv_Lane[L];//forse non serve

    //Array that represents each portion of the highway
    //NULL (or 0) if there is no vehicle in the [L][L][ROAD_LENGTH] position
    //the vehicle id otherwise
    int road[L][L][ROAD_LENGHT];

    //queue of blocked vechicles
    int coda_road[L][N];

    //queue iterator
    int next[N];

} highway;

void initHighway(struct highway_t *h)
{
    int i,j;
    pthread_mutexattr_t m_attr;
    pthread_condattr_t c_attr;

    pthread_mutexattr_init(&m_attr);
    pthread_condattr_init(&c_attr);
    pthread_mutex_init(&h->m, &m_attr);

    for(i=0; i<L; i++)
    {
        //Init private semaphores for each lane
        pthread_cond_init(&h->priv_Lane[i], &c_attr);

        //Init queue and iterator
        for(j=0; j<N; j++)
        {
            coda_road[i][j] = 0; //or NULL?
            next[j] = j+1;

            //Init private semaphore for each veichle
            if(!i)
            {
                pthread_cond_init(&h->priv_Vehicle[j], &c_attr);
            }

        }

        //Init highway 'status' for each portion of the road
        for(k=0; k<L; k++)
        {
            for(l=0; l<ROAD_LENGHT; l++)
            {
                road[i][k][l] = 0; //or NULL?
            }
        }

    }

    next[j-1] = 0;

    pthread_condattr_destroy(&c_attr);
    pthread_mutexattr_destroy(&m_attr);

}

//3 types of thread: car_thread, motorbike_thread and truck_thread

//Each car follow a speed, can overtake but it must return to the right free lane
void car_thread(int vehicle_id, int speed, struct highway_t *h)
{

}

//Behaviour like cars one, but they must mantain a greater safety distance
void motorbike_thread(int vehicle_id, int speed, struct highway_t *h)
{

}

//Behaviour like cars one but they can't overtake, and they have speed limit
void truck_thread(int vehicle_id, int speed, struct highway_t *h)
{

}




//Compulsory???
void pausetta(void)
{
  struct timespec t;
  t.tv_sec = 0;
  t.tv_nsec = (rand()%10+1)*1000000;
  nanosleep(&t,NULL);
}

/* ---------------------------------DEBUGGING functions-------------------- */

void mydebugprint()
{

}


/* ---------------------------------------MAIN----------------------------- */

int main(int argc, char **argv)
{
  pthread_attr_t a;
  pthread_t p;
  
  /* inizializzo il sistema */
  initHighway(&highway);

  /* inizializzo i numeri casuali, usati nella funzione pausetta */
  srand(555);

  pthread_attr_init(&a); //check commas!!!

  pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

  //Waiting user choices



  //Create threads


  pthread_create(&p, &a, PA, NULL);
  pthread_create(&p, &a, PB, NULL);
  pthread_create(&p, &a, PR, NULL);

  pthread_attr_destroy(&a);

  /* aspetto 10 secondi prima di terminare tutti quanti */
  sleep(5);

  return 0;
}
