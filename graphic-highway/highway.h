#ifndef HIGHWAY_H
#define HIGHWAY_H

#include "pthread.h"
#include "semaphore.h"
#include "vehicle.h"
#include "Graphics/highway_gui.h"

//Highway lanes
#define LANES 4

//Road lenght
//Lo si può vedere anche come numero di caselle,
//dove ogni veicolo occupa n caselle
#define ROAD_LENGHT 50000

//Movements
enum MOVEMENTS{GO_AHEAD, OVERTAKE, TURN_RIGHT};
#define GO_AHEAD 10
#define OVERTAKE 11
#define TURN_RIGHT 12

/* struttura condivisa */
struct highway_t
{
    //array containing all the vehicles
    struct vehicle_t *vehicles;

	//HighwayGui& gui = HighwayGui::getInstance();

    pthread_mutex_t mutex;
	pthread_cond_t *priv_Vehicle;
	pthread_cond_t priv_Gui;
	pthread_cond_t *priv_blocked_for_vehicle;
	pthread_cond_t priv_blocked_waiting_gui;
    pthread_cond_t priv_Lane[LANES];//forse non serve

	//vehicles quantity... da cambiare e rendere dinamico
	int n_vehicles;

    //Array that represents each portion of the highway
    //NULL (or -1) if there is no vehicle in the [L][L][ROAD_LENGTH] position
    //the vehicle id otherwise
    int road[LANES][LANES][ROAD_LENGHT];

    //queue containing vehicles that are waiting to start
	int *coda_to_start, *blocking_someone;
	//int *coda_vehicles;

    //numeber of waiting vehicles
	int waiting_to_start, next_vehicle, next_to_start, next_vehicle_to_go, index_coda;
	int next_pre_starting = -1;

    //queue of blocked vechicles
    int *coda_road[LANES]; //for each lane L, it tells if the N vehicle is blocking someone /and how many vehicles\(not necessary this last thing)
    //if it is blocking some vehicles, these vehicles will wait on the priv_Vehicle[N],
    //and then they will be waked up by the N vehicle

    //int coda_road[L][ROAD_LENGHT];

    //queue iterator
    int *next;

	//da spostare l'inizializzazione in initHi...
	bool highway_start = false;
	bool all_gone = false;
	bool gui_is_waiting;
	bool gui_new_update;
	bool blocked_by_the_gui;
	bool fist_gui;

};

void initHighway(struct highway_t *h);
//void initHighway(struct highway_t *h, HighwayGui *gui);

void graphic_update(int vehicle_id, struct highway_t *h, int movement);

#endif // HIGHWAY_H
