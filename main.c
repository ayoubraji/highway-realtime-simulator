/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//vehicles type
enum VEHICLES{CAR, MOTORCYCLE, TRUCK};

//Highway lanes
#define L 4

//Road lenght
//Lo si può vedere anche come numero di caselle,
//dove ogni veicolo occupa n caselle
#define ROAD_LENGHT 1000

//Movements
enum MOVEMENTS{GO_AHEAD, OVERTAKE, TURN_RIGHT};

//vehicles quantity... da cambiare e rendere dinamico
int n_vehicles;

struct vehicle_t
{
    int type;
    int actual_speed;
    int max_speed;
    int min_security_distance;
    int can_overtake;
    int lane;
    int position;
    int blocked;
};

/* struttura condivisa */
struct highway_t
{
    //array containing all the vehicles
    vehicle_t *vehicles;

    pthread_mutex_t mutex;
    pthread_cond_t *priv_Vehicle;
    pthread_cond_t priv_Lane[L];//forse non serve

    //Array that represents each portion of the highway
    //NULL (or 0) if there is no vehicle in the [L][L][ROAD_LENGTH] position
    //the vehicle id otherwise
    int road[L][L][ROAD_LENGHT];

    //queue of blocked vechicles
    int *coda_road[L]; //for each lane L, it tells if the N vehicle is blocking someone /and how many vehicles\(not necessary this last thing)
    //if it is blocking some vehicles, these vehicles will wait on the priv_Vehicle[N],
    //and then they will be waked up by the N vehicle

    //int coda_road[L][ROAD_LENGHT];

    //queue iterator
    int *next; //probabilmente non serve

} highway;

vehicle_t createVehicle(VEHICLES *type, int *max_speed)
{
    vehicle_t vehicle;
    vehicle.type = type;
    vehicle.actual_speed = 0;
    vehicle.blocked = 0;
    vehicle.max_speed = max_speed;
    vehicle.can_overtake = (type == TRUCK) ? 0 : 1;
    vehicle.min_security_distance = (type == TRUCK) ? 2 : 1;
}

vehicle_t createMockedVehicle(VEHICLES *type)
{
    vehicle_t vehicle;
    vehicle.type = type;
    vehicle.actual_speed = 0;
    vehicle.blocked = 0;
    vehicle.max_speed = (type == TRUCK) ? 80 : 130;
    vehicle.can_overtake = (type == TRUCK) ? 0 : 1;
    vehicle.min_security_distance = (type == TRUCK) ? 2 : 1;
}

void initHighway(struct highway_t *h)
{
    int i,j;
    pthread_mutexattr_t m_attr;
    pthread_condattr_t c_attr;

    pthread_mutexattr_init(&m_attr);
    pthread_condattr_init(&c_attr);
    pthread_mutex_init(&h->mutex, &m_attr);

    for(i=0; i<L; i++)
    {
        //Init private semaphores for each lane
        pthread_cond_init(&h->priv_Lane[i], &c_attr);

        //Init queue and iterator
        for(j=0; j<n_vehicles; j++)
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
                road[i][k][l] = NULL; //or -1?
            }
        }

    }

    next[n_vehicles-1] = 0;

    pthread_condattr_destroy(&c_attr);
    pthread_mutexattr_destroy(&m_attr);

}

//--- Utility functions ---//
void position_switch(int vehicle_id)
{
    int pos_vehicle, new_pos_vehicle, lane_vehicle, *actual_speed;
    pos_vehicle = &highway->vehicles[vehicle_id].position;
    lane_vehicle = &highway->vehicles[vehicle_id].lane;

    new_pos_vehicle = (actual_speed > 40) ? pos_vehicle+2 : pos_vehicle+1;
    new_pos_vehicle = (actual_speed > 80) ? new_pos_vehicle+1 : new_pos_vehicle;

    &highway->road[lane_vehicle][pos_vehicle] = NULL;
    &highway->road[lane_vehicle][new_pos_vehicle] = vehicle_id;

    if(&highway->vehicles[vehicle_id].type == TRUCK)
    {
        &highway->road[lane_vehicle][pos_vehicle+1] = NULL;
        &highway->road[lane_vehicle][new_pos_vehicle+1] = vehicle_id;
    }
}

/* 4 types of THREADS:
 *
 * 1 unique (singleton) thread:
 *      graphic_thread() always waiting
 *
 * 1 types of thread to move the vehicles:
 *      go() never waiting
 *
 * 1 type of thread to check the front distance and modify the speed:
 *      check_front_distance()
 *
 * 1 type of thread for the lane detection and the correction:
 *      lane_detection()
*/

/* Behaviour
 * 1) Increase speed if necessary
 * 2) Change the vehicle's position in the matrix rappresenting the road
 */
void go(int vehicle_id, struct highway_t *h)//NON BLOCCANTE
{
    //int pos_vehicle, new_pos_vehicle, lane_vehicle, *actual_speed;
    pthread_mutex_lock(&h->mutex);

    //actual_speed = &h->vehicles[vehicle_id].actual_speed; //to be tested
    if(!(&h->vehicles[vehicle_id].blocked) && &h->vehicles[vehicle_id].actual_speed < &h->vehicles[vehicle_id].max_speed)
    {
        &h->vehicles[vehicle_id].actual_speed = &h->vehicles[vehicle_id].actual_speed + 10;
    }

    /*
    //---POSITION SWITCH---// should be putted in a function
    pos_vehicle = &h->vehicles[vehicle_id].position;
    lane_vehicle = &h->vehicles[vehicle_id].lane;

    new_pos_vehicle = (actual_speed > 40) ? pos_vehicle+2 : pos_vehicle+1;
    new_pos_vehicle = (actual_speed > 80) ? new_pos_vehicle+1 : new_pos_vehicle;

    &h->road[lane_vehicle][pos_vehicle] = NULL;
    &h->road[lane_vehicle][new_pos_vehicle] = vehicle_id;

    if(&h->vehicles[vehicle_id].type == TRUCK)
    {
        &h->road[lane_vehicle][pos_vehicle+1] = NULL;
        &h->road[lane_vehicle][new_pos_vehicle+1] = vehicle_id;
    }
    //--------------------//
    */

    //---POSITION SWITCH---//
    position_switch(vehicle_id);

    graphic_update(vehicle_id, &h, GO_AHEAD);

    pthread_mutex_unlock(&h->mutex);
}


void check_front_distance(int vehicle_id, struct highway_t *h)
{
    pthread_mutex_lock(&h->mutex);



    graphic_update(vehicle_id, &h, GO_AHEAD);

    pthread_mutex_unlock(&h->mutex);

}

void graphic_update(int vehicle_id, struct highway_t *h, MOVEMENTS movement)
{

}


void pausetta(int sec)
{
  struct timespec t;
  t.tv_sec = &sec;
  t.tv_nsec = (rand()%10+1)*1000000;
  nanosleep(&t,NULL);
}

/* ---------------------------------DEBUGGING functions-------------------- */

void mydebugprint()
{

}


/* ---------------------------------------MAIN----------------------------- */

void *go_routine(int id)
{
    //Initialize lane and position, which will be changed by the go function
    &highway.vehicles[id].lane = 0;
    &highway.vehicles[id].position = 0;

    //starting the routine
    for (;;) {
        go(&id,&highway);
        pausetta(0);
    }
}

void *check_front_distance_routine(int id)
{
    //starting the routine
    for (;;) {
        check_front_distance(&id,&highway);
        pausetta(1);
    }
}

void *lane_detection_routine(int id)
{
    //starting the routine
    for (;;) {
        //lane_detection(&id,&highway);
        pausetta(3);
    }
}


int main(int argc, char **argv)
{
  pthread_attr_t a;
  pthread_t p;
  int i;
  VEHICLES temp_type;

  /* inizializzo i numeri casuali, usati nella funzione pausetta */
  srand(555);

  pthread_attr_init(&a); //check commas!!!

  pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

  //Taking user choices
  // &highway->vehicles = veicoli_ritornati_da_menu();

  //-------------------------------start mock
  &n_vehicles = 10;

  &highway->vehicles = malloc(&n_vehicles * sizeof(vehicle_t));
  &highway->priv_Vehicle = malloc(&n_vehicles * sizeof(pthread_cond_t));
  &highway.coda_road = malloc(&n_vehicles * sizeof(int));
  &highway.next = malloc(&n_vehicles * sizeof(int));

  for(i=0; i<n_vehicles; i++)
  {
      if(i<3)
      {
          temp_type = TRUCK;
      }
      else if(i<6)
      {
          temp_type = CAR;
      }
      else
      {
          temp_type = MOTORCYCLE;
      }

      &highway->vehicles[i] = createMockedVehicle(temp_type);
  }
  //------------------------------end mock

  /* inizializzo il sistema (da modificare una volta tolto il mock */
  initHighway(&highway);

  //Create threads
  for(i=0; i<n_vehicles; i++)
  {
      pthread_create(&p, &a, go_routine, i);
      pthread_create(&p, &a, check_front_distance_routine, i);
  }

  pthread_attr_destroy(&a);

  /* aspetto 5 secondi prima di terminare tutti quanti */
  sleep(5);

  return 0;
}