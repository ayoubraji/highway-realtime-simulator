/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#include <QApplication>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include "pthread.h"
#include "unistd.h"
#include "semaphore.h"
#include "threads_controller.h"
#include "highway.h"
#include "Graphics/highway_gui.h"

struct highway_t highway;

/* 3 types of THREADS:
 *
 * 1 unique (singleton) thread: NO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *      graphic_thread() always waiting NO!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

void go(int vehicle_id, struct highway_t *h)//BLOCCANTE
{
	int can_start = -1;
	int lane_vehicle, lane_to_switch, x_pos, y_pos;
	struct position_t pos_vehicle;

	pthread_mutex_lock(&h->mutex);

	pos_vehicle = h->vehicles[vehicle_id].position;
	lane_vehicle = pos_vehicle.lane;
	x_pos = pos_vehicle.x_pos;
	y_pos = pos_vehicle.y_pos;

	printf("Veicolo %d: sono in pos: %d \n", vehicle_id, y_pos);

	while(vehicle_id != h->next_vehicle)
	{
		//pthread_cond_signal(&h->priv_Vehicle[h->next_vehicle]);//FORSE TROPPO FORZATO COSI...
		pthread_cond_wait(&h->priv_Vehicle[vehicle_id], &h->mutex);
	}

	if(!h->highway_start)
	{
		h->highway_start = true;
	}

	//if(y_pos == -1)
	if(y_pos < 5)//Check that should be done for the first portion of the road
	{
		can_start = check_front(vehicle_id, h, lane_vehicle, true);

		while(can_start != -1)
		{
			printf("Veicolo %d: sono BLOCCATO per la partenza, devo aspettare...\n", vehicle_id);

			//update variables concerning the waiting
			h->waiting_to_start++;

			h->next_vehicle = 0;
			printf("Veicolo %d: cerco di far partire il veicolo %d\n", vehicle_id, h->next_vehicle);
			pthread_cond_signal(&h->priv_Vehicle[h->next_vehicle]);

			pthread_cond_wait(&h->priv_Vehicle[vehicle_id], &h->mutex);

			h->waiting_to_start--;

			can_start = check_front(vehicle_id, h, lane_vehicle, true);
		}
	}

	printf("Veicolo %d: vado avanti\n", vehicle_id);

	if(!(h->vehicles[vehicle_id].speed_limited) && h->vehicles[vehicle_id].actual_speed < h->vehicles[vehicle_id].max_speed)
	{
		h->vehicles[vehicle_id].actual_speed = h->vehicles[vehicle_id].actual_speed + 10;
	}

	//---POSITION SWITCH---//

	if(h->vehicles[vehicle_id].movement_type == TURN_RIGHT)
	{
		lane_to_switch = lane_vehicle - 1;
	}
	else if(h->vehicles[vehicle_id].movement_type == OVERTAKE)
	{
		lane_to_switch = lane_vehicle + 1;
	}
	else
	{
		lane_to_switch = lane_vehicle;
	}

	position_switch(vehicle_id, h, lane_to_switch);

	graphic_update(vehicle_id, h, h->vehicles[vehicle_id].movement_type);
	h->vehicles[vehicle_id].movement_type = GO_AHEAD;


	h->next_vehicle = h->next[vehicle_id];
	printf("Veicolo %d: cerco di far partire il veicolo %d\n", vehicle_id, h->next_vehicle);
	pthread_cond_signal(&h->priv_Vehicle[h->next_vehicle]);

	pthread_mutex_unlock(&h->mutex);
}


/* Behaviour
 * 1) Check if there is a vehicle in front and if the distance is critical.
 * 2) In case there is a "critical" situation, it tries to overtake, if it can't it adapts the speed to mantain the security distance
 * and wait to be waken up in order to overtake.
 * 3) Try to turn in the right lane
 */

void check_front_distance(int vehicle_id, struct highway_t *h)//MAI BLOCCANTE
{
	int how_close, nearest_vehicle_in_front, nearest_vehicle_in_the_right, speed_to_adapt;
	int someone_stop_me_to_overtake;
	position_t pos_vehicle;

	pthread_mutex_lock(&h->mutex);

	pos_vehicle = h->vehicles[vehicle_id].position;

	//if(pos_vehicle.y_pos != -1)
	if(pos_vehicle.y_pos > 4)//if we are already going
	{

		nearest_vehicle_in_front = check_front(vehicle_id, h, pos_vehicle.lane, false);

		if(nearest_vehicle_in_front == -1)
		{
			printf("Veicolo %d: Non c'è nessuno di fronte a me...\n", vehicle_id);

			if(h->vehicles[vehicle_id].speed_limited)
			{
				h->vehicles[vehicle_id].speed_limited = 0;
			}

			//Check if I can turn right
			if(pos_vehicle.lane > 0)
			{
				nearest_vehicle_in_the_right = check_front(vehicle_id, h, (pos_vehicle.lane-1), false);

				if(nearest_vehicle_in_the_right == -1)//I can turn right
				{
					printf("Veicolo %d: Posso tornare nella corsia a destra...\n", vehicle_id);
					h->vehicles[vehicle_id].movement_type = TURN_RIGHT;
				}
				else
				{
					printf("Veicolo %d: Ho la destra occupata...\n", vehicle_id);
				}
			}
		}
		else if(h->vehicles[vehicle_id].can_overtake
				&& (h->vehicles[nearest_vehicle_in_front].actual_speed < h->vehicles[vehicle_id].actual_speed
					|| h->vehicles[vehicle_id].speed_limited)
				) //A vehicle is near and I'm a car or motorcycle
		{
			how_close = pos_vehicle.y_pos - h->vehicles[nearest_vehicle_in_front].position.y_pos;
			printf("A distanza %d, c'è %d, cerco di superarlo...\n", how_close, nearest_vehicle_in_front);

			//Try to overtake
			someone_stop_me_to_overtake = check_front(vehicle_id, h, (pos_vehicle.lane)+1, false);

			if(someone_stop_me_to_overtake != -1)
			{
				printf("Non posso superare! Alla mia sinistra c'è %d,"
					   "che occupa lo spazio necessario. Rallento e blocco il sorpasso, in attesa che si liberi...\n", someone_stop_me_to_overtake);

				//Adapt the speed
				speed_to_adapt = h->vehicles[nearest_vehicle_in_front].actual_speed;
				h->vehicles[vehicle_id].actual_speed = speed_to_adapt;
				h->vehicles[vehicle_id].speed_limited = 1;
			}
			else
			{
				//overtake
				h->vehicles[vehicle_id].speed_limited = 0;
				h->vehicles[vehicle_id].movement_type = OVERTAKE;

			}
		}
		else //TRUCK
		{
			//adapt my speed only if the vehicle in front of me is going faster than me
			//otherwise it is only only returned in front of me after having overtaken
			if(h->vehicles[nearest_vehicle_in_front].actual_speed < h->vehicles[vehicle_id].actual_speed)
			{
				speed_to_adapt = h->vehicles[nearest_vehicle_in_front].actual_speed;
				h->vehicles[vehicle_id].actual_speed = speed_to_adapt;
				h->vehicles[nearest_vehicle_in_front].speed_limited = 1;
			}

		}
	}

	pthread_mutex_unlock(&h->mutex);
}

void lane_detection(int vehicle_id, struct highway_t *h)//MAI BLOCCANTE
{
	int lane_vehicle, x_pos, y_pos;
	struct position_t pos_vehicle;

	pthread_mutex_lock(&h->mutex);

	pos_vehicle = h->vehicles[vehicle_id].position;
	lane_vehicle = pos_vehicle.lane;

	//1 is the center of the lane, 0 left, 2 right
	x_pos = pos_vehicle.x_pos;
	y_pos = pos_vehicle.y_pos;

	//if x_pos is 0 or 2, so not in the middle of the lane, the position should be corrected
	if(x_pos != 1)
	{
		h->vehicles[vehicle_id].should_correct_alignment = true;
	}

	pthread_mutex_unlock(&h->mutex);
}

void pausetta(int msec)
{
  struct timespec t;
  t.tv_sec = 0;
  //t.tv_nsec = (rand()%10+1)*1000000;
  t.tv_nsec = (msec%1000)*1000000;

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
		printf("Go, veicolo %d\n", vehicle_id);
		go(vehicle_id,&highway);
		//pausetta(100);
		pausetta(160);
		//pausetta(500);
	}
}

void *check_front_distance_routine(void *id)
{
	long vehicle_id = (long) id;
	//starting the routine
	for (;;) {
		printf("Veicolo %d: controllo distanza\n", vehicle_id);
		check_front_distance(vehicle_id,&highway);
		//pausetta(50);
		pausetta(80);
		//pausetta(250);
	}

}

void *lane_detection_routine(void *id)
{
	long vehicle_id = (long) id;
	//starting the routine
	for (;;) {
		printf("Vehicle %d: lane detection routine\n", vehicle_id);
		lane_detection(vehicle_id,&highway);
		//pausetta(200);
		pausetta(200);
	}
}

void initThreads()
{
  pthread_attr_t attr;
  pthread_t p;
  int i;

  // inizializzo i numeri casuali, usati nella funzione pausetta
  srand(555);

  pthread_attr_init(&attr); //check commas!!!

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  //Taking user choices
  // vehicles = veicoli_ritornati_da_menu();
  //initHighway(&highway, vehicles_from_gui);

  // inizializzo il sistema (da modificare una volta tolto il mock presente al suo interno)
  initHighway(&highway);

  //Create threads

  //Vehicle control threads
  for(i=0; i<(highway.n_vehicles); i++)
  {
	  pthread_create(&p, &attr, go_routine, (void *)i);
	  pthread_create(&p, &attr, check_front_distance_routine,  (void *)i);
	  pthread_create(&p, &attr, lane_detection_routine,  (void *)i);
  }

  pthread_attr_destroy(&attr);

  // aspetto 2 secondi prima di terminare tutti quanti
  pausetta(2000);

}
