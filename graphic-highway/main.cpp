/*
    Ayoub Raji
    Project of Real Time Embedded Systems course
*/

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "pthread.h"
#include "unistd.h"
#include "semaphore.h"
#include "mainwindow.h"
#include <QApplication>

//vehicles type
enum VEHICLES{CAR, MOTORCYCLE, TRUCK};
#define CAR 0
#define MOTORCYCLE 1
#define TRUCK 2

//Highway lanes
#define L 4

//Road lenght
//Lo si può vedere anche come numero di caselle,
//dove ogni veicolo occupa n caselle
#define ROAD_LENGHT 1000

//Movements
enum MOVEMENTS{GO_AHEAD, OVERTAKE, TURN_RIGHT};
#define GO_AHEAD 10
#define OVERTAKE 11
#define TURN_RIGHT 12

//vehicles quantity... da cambiare e rendere dinamico
int n_vehicles;

//Inside a lane, the vehicol has position (x,y)
struct position_t
{
    int lane;
    int x_pos;
    int y_pos;
};

struct vehicle_t
{
    int type;
    int actual_speed;
    int max_speed;
    int min_security_distance;
    int can_overtake;
    struct position_t position;
    int speed_limited;
};

/* struttura condivisa */
struct highway_t
{
    //array containing all the vehicles
    struct vehicle_t *vehicles;

    pthread_mutex_t mutex;
    pthread_cond_t *priv_Vehicle;
    pthread_cond_t priv_Lane[L];//forse non serve

    //Array that represents each portion of the highway
    //NULL (or -1) if there is no vehicle in the [L][L][ROAD_LENGTH] position
    //the vehicle id otherwise
    int road[L][L][ROAD_LENGHT];

    //queue containing vehicles that are waiting to start
    int *coda_to_start;
    //numeber of waiting vehicles
    int waiting_to_start, next_vehicle, index_coda;


    //queue of blocked vechicles
    int *coda_road[L]; //for each lane L, it tells if the N vehicle is blocking someone /and how many vehicles\(not necessary this last thing)
    //if it is blocking some vehicles, these vehicles will wait on the priv_Vehicle[N],
    //and then they will be waked up by the N vehicle

    //int coda_road[L][ROAD_LENGHT];

    //queue iterator
    int *next;

} highway;

struct vehicle_t createVehicle(int type, int max_speed)
{
    struct vehicle_t vehicle;
    vehicle.type = type;
    vehicle.actual_speed = 0;
    vehicle.speed_limited = 0;
    vehicle.max_speed = max_speed;
    vehicle.can_overtake = (type == TRUCK) ? 0 : 1;
    vehicle.min_security_distance = (type == TRUCK) ? 2 : 1;

    return vehicle;
}

struct vehicle_t createMockedVehicle(int type, int id, int max_speed)
{
    struct vehicle_t vehicle;
    vehicle.type = type;
    vehicle.actual_speed = 0;
    vehicle.speed_limited = 0;
    //vehicle.max_speed = (type == TRUCK) ? 80 : 130;
    vehicle.max_speed = max_speed;
    vehicle.can_overtake = (type == TRUCK) ? 0 : 1;
    vehicle.min_security_distance = (type == TRUCK) ? 2 : 1;
    //Initialize lane and position, which will be changed by the go function
    //highway.vehicles[id].position.lane = highway.vehicles[id].position.x_pos = highway.vehicles[id].position.y_pos = 0;

    return vehicle;
}

void initHighway(struct highway_t *h)
{
    int i,j,k,l;
    pthread_mutexattr_t m_attr;
    pthread_condattr_t c_attr;
    int max_speed, temp_type;

    pthread_mutexattr_init(&m_attr);
    pthread_condattr_init(&c_attr);
    pthread_mutex_init(&h->mutex, &m_attr);

    //-------------------------------start mock
    n_vehicles = 10;

    h->vehicles = (vehicle_t*) malloc(n_vehicles * sizeof(struct vehicle_t));
    h->priv_Vehicle = (pthread_cond_t*)malloc(n_vehicles * sizeof(pthread_cond_t));

    for(i=0; i<L; i++)
    {
        h->coda_road[i] = (int*)malloc(n_vehicles * sizeof(int));
    }

    h->next = (int*)malloc(n_vehicles * sizeof(int));
    h->coda_to_start = (int*)malloc(n_vehicles * sizeof(int));

    for(i=0; i<n_vehicles; i++)
    {
        if(i<3)
        {
            temp_type = TRUCK;
            max_speed = 60;
        }
        else if(i<6)
        {
            temp_type = MOTORCYCLE;
            max_speed = 80;
        }
        else
        {
            temp_type = CAR;
            max_speed = 130;
        }

        h->vehicles[i] = createMockedVehicle(temp_type,i,max_speed);

        //Initialize lane and position, which will be changed by the go function
        h->vehicles[i].position.lane = 0;
        h->vehicles[i].position.x_pos = 0;
        h->vehicles[i].position.y_pos = -1;

        pthread_cond_init(&h->priv_Vehicle[i], &c_attr);
        h->next[i] = i+1;
        h->coda_to_start[i] = -1;
    }
    //------------------------------end mock


    for(i=0; i<L; i++)
    {
        //Init private semaphores for each lane
        pthread_cond_init(&h->priv_Lane[i], &c_attr);

        //Init queue and iterator
        for(j=0; j<n_vehicles; j++)
        {
            h->coda_road[i][j] = -1;//INUTILE forse...
        }

        //Init highway 'status' for each portion of the road
        for(k=0; k<L; k++)
        {
            for(l=0; l<ROAD_LENGHT; l++)
            {
                h->road[i][k][l] = -1;
            }
        }

    }

    h->next[n_vehicles-1] = h->waiting_to_start = h->next_vehicle = h->index_coda = 0;

    pthread_condattr_destroy(&c_attr);
    pthread_mutexattr_destroy(&m_attr);

}

//--- Utility functions ---//
void position_switch(int vehicle_id, struct highway_t *h)
{
    int lane_vehicle, x_pos, y_pos, new_y_pos, actual_speed;
    struct position_t pos_vehicle;

    pos_vehicle = h->vehicles[vehicle_id].position;
    lane_vehicle = pos_vehicle.lane;
    x_pos = pos_vehicle.x_pos;
    y_pos = pos_vehicle.y_pos; //-1

    actual_speed = h->vehicles[vehicle_id].actual_speed;

    new_y_pos = (actual_speed > 40) ? y_pos+2 : y_pos+1; //0
    new_y_pos = (actual_speed > 80) ? new_y_pos+2 : new_y_pos;

    if(y_pos != -1)//if the vehicle is already on the road, it is necessary to set the previous position as free (-1)
    {
        h->road[lane_vehicle][x_pos][y_pos] = -1;
    }
    h->road[lane_vehicle][x_pos][new_y_pos] = vehicle_id; //updating the new position in the road's array

    if(h->vehicles[vehicle_id].type == TRUCK)
    {
        h->road[lane_vehicle][x_pos][y_pos+1] = -1;
        h->road[lane_vehicle][x_pos][new_y_pos+1] = vehicle_id;
    }

    h->vehicles[vehicle_id].position.y_pos = new_y_pos;
}

//Check if there is a vehicle in front, and return its id
int check_front(int vehicle_id, struct highway_t *h, position_t pos_vehicle, int lane_to_check)
{
    int i, index_to_check, x_pos, y_pos, lane_vehicle;
    int front_vehicle_id = -1;

    lane_vehicle = lane_to_check;
    x_pos = pos_vehicle.x_pos;
    y_pos = pos_vehicle.y_pos;

    for(i=1; i <= h->vehicles[vehicle_id].min_security_distance+1; i++)
    {
        index_to_check = y_pos+i;
        //if(h->road[0][0][index_to_check] != -1) //It doesn't work in cpp but in c it does... WHY???
        if(h->road[lane_vehicle][x_pos][index_to_check] != -1 && h->road[lane_vehicle][x_pos][index_to_check] != vehicle_id)
        {
            front_vehicle_id = h->road[lane_vehicle][x_pos][index_to_check];
            printf("Check Front: Nella posizione %d, a distanza %d da me,  c'è %d \n", index_to_check, i, h->road[lane_vehicle][x_pos][index_to_check]);
            break; //maybe not safety for RT
        }
    }
    return front_vehicle_id;
}

//Check if a vehicle can start the travel. This is possible when first 3 slots (+ security distance slots) are free
int check_start(int vehicle_id, int y_pos, struct highway_t *h)
{
    int i, index_to_check;
    int can_start = 1;
    printf("Veicolo %d: controllo se posso partire o meno \n", vehicle_id);
    for(i=1; i <= h->vehicles[vehicle_id].min_security_distance+3; i++)
    {
        index_to_check = y_pos+i;
        //if(h->road[0][0][index_to_check] != -1) //It doesn't work in cpp but in c it does... WHY???
        if(h->road[0][0][index_to_check] != -1 && h->road[0][0][index_to_check] != vehicle_id)//DA CONTROLLARE LA SECONDA CONDIZIONE
        {
            can_start = 0;
            printf("In posizione %d c'è già %d \n", index_to_check, h->road[0][0][index_to_check]);
            break; //maybe not safe for RT
        }
        else
        {
            printf("Non c'è nessuno in posizione %d, infatti: %d \n", index_to_check, h->road[0][0][index_to_check]);
        }
    }
    return can_start;
}

int can_overtake(int vehicle_id, struct highway_t *h)
{

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

void graphic_update(int vehicle_id, struct highway_t *h, int movement)
{
    struct position_t pos_vehicle;
    pos_vehicle = h->vehicles[vehicle_id].position;

    printf("Movimento veicolo %d a posizione: (%d, %d) corsia %d, a velocità: %d\n",
           vehicle_id, pos_vehicle.x_pos, pos_vehicle.y_pos, pos_vehicle.lane, h->vehicles[vehicle_id].actual_speed);

    //Apply graphical movement (go on, overtake, turn right etc.) to the mainwindow


}

/* Behaviour
 * 1) Increase speed if necessary
 * 2) Change the vehicle's position in the matrix rappresenting the road
 */
void go(int vehicle_id, struct highway_t *h)//NON BLOCCANTE
{
    int can_start = 1;
    int lane_vehicle, x_pos, y_pos, to_awake;
    struct position_t pos_vehicle;

    pthread_mutex_lock(&h->mutex);

    pos_vehicle = h->vehicles[vehicle_id].position;
    lane_vehicle = pos_vehicle.lane;
    x_pos = pos_vehicle.x_pos;
    y_pos = pos_vehicle.y_pos;

    printf("Veicolo %d: sono a pos: %d \n", vehicle_id, y_pos);

    if(y_pos < 5)//Check that should be done for the first portion of road
    {
        can_start = check_start(vehicle_id, y_pos, h);
    }

    while(!can_start)
    {
        printf("Veicolo %d: sono BLOCCATO per la partenza, devo aspettare...\n", vehicle_id);

        //update variables concerning the waiting
        h->waiting_to_start++;
        h->coda_to_start[h->index_coda] = vehicle_id;
        h->index_coda = h->next[h->index_coda];

        pthread_cond_wait(&h->priv_Vehicle[vehicle_id], &h->mutex);

        can_start = 1;
        if(y_pos < 5)//INUTILE, se y>5 non entra nemmeno nel while
        {
            can_start = check_start(vehicle_id, y_pos, h);
        }
    }

    printf("Veicolo %d: vado avanti\n", vehicle_id);

    if(!(h->vehicles[vehicle_id].speed_limited) && h->vehicles[vehicle_id].actual_speed < h->vehicles[vehicle_id].max_speed)
    {
        h->vehicles[vehicle_id].actual_speed = h->vehicles[vehicle_id].actual_speed + 10;
    }

    //---POSITION SWITCH---//
    position_switch(vehicle_id, h);

    graphic_update(vehicle_id, h, GO_AHEAD);

    if(h->waiting_to_start)//if there is someone waiting to start
    {
        //update variables concerning the waiting and then signal the next vehicle in the waiting queue
        h->waiting_to_start--;
        to_awake = h->coda_to_start[h->next_vehicle];
        printf("Veicolo %d: cerco di svegliare il veicolo %d\n", vehicle_id, to_awake);
        pthread_cond_signal(&h->priv_Vehicle[to_awake]);
        h->next_vehicle = h->next[h->next_vehicle];
    }

    pthread_mutex_unlock(&h->mutex);
}

/* Behaviour
 * 1) Check if there is a vehicle in front and if the distance is critical.
 * 2) In case there is a "critical" situation, it tries to overtake, if it can't it adapts the speed to mantain the security distance
 * and wait to be waken up in order to overtake.
 */
void check_front_distance(int vehicle_id, struct highway_t *h)
{
    int how_near, nearest_vehicle_id, speed_to_adapt;
    int someone_stop_me_to_overtake;
    position_t pos_vehicle;

    pthread_mutex_lock(&h->mutex);

    if(h->vehicles[vehicle_id].actual_speed)
    {

        pos_vehicle = h->vehicles[vehicle_id].position;

        nearest_vehicle_id = check_front(vehicle_id,h, pos_vehicle, pos_vehicle.lane);

        if(nearest_vehicle_id == -1)
        {
            std::cout << "Non c'è nessuno vicino alla mia distanza di sicurezza." << std::endl;
        }
        else //A vehicle is near
        {
            how_near = pos_vehicle.y_pos - h->vehicles[nearest_vehicle_id].position.y_pos;

            std::cout << "A distanza " << how_near << " c'è " << nearest_vehicle_id << ", cerco di superarlo..." << std::endl;

            //Try to overtake
            someone_stop_me_to_overtake = can_overtake(vehicle_id, h);

            if(someone_stop_me_to_overtake != -1)//per ora if, poi while quando sarà implementata la coda di attesa adeguata
            //while(someone_stop_me_to_overtake != -1)
            {
                std::cout << "Non posso superare! Alla mia sinistra c'è " << someone_stop_me_to_overtake
                          << " che occupa lo spazio necessario. Rallento e blocco il sorpasso, in attesa che si liberi..." << std::endl;

                //Adapt the speed
                speed_to_adapt = h->vehicles[nearest_vehicle_id].actual_speed;
                h->vehicles[vehicle_id].actual_speed = speed_to_adapt;
                h->vehicles[nearest_vehicle_id].speed_limited = 1;

                //wait in the correct cond variable,

                //recheck the possibility to overtake
                someone_stop_me_to_overtake = can_overtake(vehicle_id, h);
            }

            //overtake
            h->vehicles[nearest_vehicle_id].speed_limited = 0;

        }
    }
    pthread_mutex_unlock(&h->mutex);
}

void pausetta(int sec)
{
  struct timespec t;
  t.tv_sec = sec;
  //t.tv_sec = 0;
  t.tv_nsec = (rand()%10+1)*1000000;
  nanosleep(&t,NULL);
}

/* ---------------------------------DEBUGGING functions-------------------- */

void mydebugprint()
{

}


/* ---------------------------------------MAIN----------------------------- */

void *go_routine(void *id)
{
    long vehicle_id = (long) id;
    printf("Tento avvio veicolo %d\n", vehicle_id);
    //starting the routine
    for (;;) {
        std::cout << "Go, veicolo" << vehicle_id << std::endl;
        go(vehicle_id,&highway);
        pausetta(0);
    }
}

void *check_front_distance_routine(void *id)
{
    long vehicle_id = (long) id;
    //starting the routine    
    for (;;) {
        printf("Veicolo %d: controllo distanza\n", vehicle_id);
        check_front_distance(vehicle_id,&highway);
        pausetta(0);
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
  pthread_attr_t attr;
  pthread_t p;
  int i;

  QApplication app(argc, argv);
  MainWindow w;
  //w.show();

  //return a.exec();

  /* inizializzo i numeri casuali, usati nella funzione pausetta */
  srand(555);

  pthread_attr_init(&attr); //check commas!!!

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  //Taking user choices
  // &highway->vehicles = veicoli_ritornati_da_menu();

  /* inizializzo il sistema (da modificare una volta tolto il mock presente al suo interno) */
  initHighway(&highway);

  //Create threads
  for(i=0; i<n_vehicles; i++)
  {
      pthread_create(&p, &attr, go_routine, (void *)i);
  }

  for(i=0; i<n_vehicles; i++)
  {
      pthread_create(&p, &attr, check_front_distance_routine,  (void *)i);
  }

  pthread_attr_destroy(&attr);

  /* aspetto 5 secondi prima di terminare tutti quanti */
  sleep(5);

  return 0;
}
