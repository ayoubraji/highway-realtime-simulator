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

void go_refactor(int vehicle_id, struct highway_t *h)//NON BLOCCANTE
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

	while(vehicle_id != h->next_pre_starting)
	{
		pthread_cond_signal(&h->priv_Vehicle[h->next_pre_starting]);//FORSE TROPPO FORZATO COSI...

		pthread_cond_wait(&h->priv_Vehicle[vehicle_id], &h->mutex);
	}

	//h->next_pre_starting = -1;

	if(!h->highway_start)
	{
		h->highway_start = true;
	}

	if(y_pos < 5)//Check that should be done for the first portion of road
	{
		can_start = check_front(vehicle_id, h, lane_vehicle, true);

		while(can_start != -1)
		{
			printf("Veicolo %d: sono BLOCCATO per la partenza, devo aspettare...\n", vehicle_id);

			//update variables concerning the waiting
			h->waiting_to_start++;

			h->next_pre_starting = 0;
			printf("Veicolo %d: cerco di far partire il veicolo %d\n", vehicle_id, h->next_pre_starting);
			pthread_cond_signal(&h->priv_Vehicle[h->next_pre_starting]);

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


	h->next_pre_starting = h->next[vehicle_id];
	printf("Veicolo %d: cerco di far partire il veicolo %d\n", vehicle_id, h->next_pre_starting);
	pthread_cond_signal(&h->priv_Vehicle[h->next_pre_starting]);

	pthread_mutex_unlock(&h->mutex);
}

void go(int vehicle_id, struct highway_t *h)//NON BLOCCANTE
{
	int can_start = -1;
	int lane_vehicle, x_pos, y_pos, to_awake;
	int lane_to_switch;
	struct position_t pos_vehicle;

	pthread_mutex_lock(&h->mutex);

	pos_vehicle = h->vehicles[vehicle_id].position;
	lane_vehicle = pos_vehicle.lane;
	x_pos = pos_vehicle.x_pos;
	y_pos = pos_vehicle.y_pos;

	if(!h->highway_start)
	{
		h->highway_start = true;
	}

	printf("Veicolo %d: sono in pos: %d \n", vehicle_id, y_pos);

	if(!h->waiting_to_start && y_pos > 8 && h->all_gone)
	{
		if(vehicle_id != h->next_pre_starting)
		{
			pthread_cond_wait(&h->priv_Vehicle[vehicle_id], &h->mutex);
		}
		else
		{
			h->next_pre_starting = -1;
		}

	}

	if(y_pos < 5)//Check that should be done for the first portion of road
	{
		can_start = check_front(vehicle_id, h, lane_vehicle, true);

		while(can_start != -1)
		{
			printf("Veicolo %d: sono BLOCCATO per la partenza, devo aspettare...\n", vehicle_id);

			//update variables concerning the waiting
			h->waiting_to_start++;
			//h->coda_to_start[h->index_coda] = vehicle_id;
			//h->index_coda = h->next[h->index_coda];

			pthread_cond_wait(&h->priv_Vehicle[vehicle_id], &h->mutex);

			h->waiting_to_start--;

			if(y_pos < 5)//INUTILE, se y>5 non entra nemmeno nel while
			{
				can_start = check_front(vehicle_id, h, lane_vehicle, true);
			}
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

	if(h->waiting_to_start && h->vehicles[vehicle_id].position.y_pos > 4)//if there is someone waiting to start and I've passed the 'security' portion of the road
	{
		//update variables concerning the waiting and then signal the next vehicle in the waiting queue
		//to_awake = h->coda_to_start[h->next_vehicle];
		to_awake = h->next[vehicle_id];
		printf("Veicolo %d: cerco di far partire il veicolo %d\n", vehicle_id, to_awake);
		pthread_cond_signal(&h->priv_Vehicle[to_awake]);
		//h->next_vehicle = h->next[h->next_vehicle];
	}
	else if(h->vehicles[vehicle_id].position.y_pos > 4)
	{

		to_awake = h->next[vehicle_id];

		if(h->all_gone)
		{
			h->next_pre_starting = to_awake;
		}
		else if(!h->all_gone && !h->waiting_to_start)//ultimo ad entrare in strada
		{
			h->all_gone = true;
			h->next_pre_starting = vehicle_id;
		}

		printf("Veicolo %d: cerco di svegliare il veicolo %d\n", vehicle_id, to_awake);
		pthread_cond_signal(&h->priv_Vehicle[to_awake]);

	}

	if(h->blocking_someone[vehicle_id] > 0)//risveglio chi è bloccato nel sorpasso per colpa mia
	{
		pthread_cond_broadcast(&h->priv_blocked_for_vehicle[vehicle_id]);
		printf("Veicolo %d: cerco di svegliare i veicoli che stavo bloccando %d\n", vehicle_id);
	}

	pthread_mutex_unlock(&h->mutex);
}


/* Behaviour
 * 1) Check if there is a vehicle in front and if the distance is critical.
 * 2) In case there is a "critical" situation, it tries to overtake, if it can't it adapts the speed to mantain the security distance
 * and wait to be waken up in order to overtake.
 * 3) Try to turn in the right lane
 */

void check_front_distance(int vehicle_id, struct highway_t *h)//mai bloccante
{
	int how_close, nearest_vehicle_in_front, nearest_vehicle_in_the_right, speed_to_adapt;
	int someone_stop_me_to_overtake;
	position_t pos_vehicle;
	//nearest_vehicle_in_front = nearest_vehicle_in_the_right = -1;

	pthread_mutex_lock(&h->mutex);

	pos_vehicle = h->vehicles[vehicle_id].position;

	if(pos_vehicle.y_pos > 4)//if we are already going //NON FUNZIONA MI SA...
	{

		nearest_vehicle_in_front = check_front(vehicle_id, h, pos_vehicle.lane, false);

		if(nearest_vehicle_in_front == -1)
		{
			printf("Veicolo %d: Non c'è nessuno di fronte a me...\n", vehicle_id);

			if(h->vehicles[vehicle_id].speed_limited)
			{
				h->vehicles[nearest_vehicle_in_front].speed_limited = 0;
			}

			//Check if I can turn right
			if(pos_vehicle.lane > 0)
			{
				nearest_vehicle_in_the_right = check_front(vehicle_id, h, (pos_vehicle.lane-1), false);

				if(nearest_vehicle_in_the_right == -1)//I can turn right
				{
					printf("Veicolo %d: Posso tornare nella corsia a destra...\n", vehicle_id);
					h->vehicles[vehicle_id].movement_type = TURN_RIGHT;

					//---POSITION SWITCH---//
					//position_switch(vehicle_id, h, (pos_vehicle.lane)-1);

					//graphic_update(vehicle_id, h, TURN_RIGHT);//probabilmente non serve indicare se è un sorpasso, o forse sì per l'animazione...

				}
				else
				{
					printf("Veicolo %d: Ho la destra occupata...\n", vehicle_id);
				}
			}
		}
		else if(h->vehicles[vehicle_id].can_overtake
				&& h->vehicles[nearest_vehicle_in_front].actual_speed < h->vehicles[vehicle_id].actual_speed) //A vehicle is near and I'm a car or motorcycle
		{
			how_close = pos_vehicle.y_pos - h->vehicles[nearest_vehicle_in_front].position.y_pos;
			printf("A distanza %d, c'è %d, cerco di superarlo...\n", how_close, nearest_vehicle_in_front);

			//Try to overtake
			someone_stop_me_to_overtake = check_front(vehicle_id, h, (pos_vehicle.lane)+1, false);

			if(someone_stop_me_to_overtake != -1)
			//while(someone_stop_me_to_overtake != -1)
			{
				printf("Non posso superare! Alla mia sinistra c'è %d,"
					   "che occupa lo spazio necessario. Rallento e blocco il sorpasso, in attesa che si liberi...\n", someone_stop_me_to_overtake);

				//Adapt the speed
				speed_to_adapt = h->vehicles[nearest_vehicle_in_front].actual_speed;
				h->vehicles[vehicle_id].actual_speed = speed_to_adapt;
				h->vehicles[nearest_vehicle_in_front].speed_limited = 1;
			}
			else
			{
				//overtake
				h->vehicles[nearest_vehicle_in_front].speed_limited = 0;
				h->vehicles[vehicle_id].movement_type = OVERTAKE;

				//---POSITION SWITCH---//
				//position_switch(vehicle_id, h, (pos_vehicle.lane)+1);

				//registro il cambio di posizione
				//che verrà applicato dal thread della gui
				//graphic_update(vehicle_id, h, OVERTAKE);//probabilmente non serve indicare se è un sorpasso, o forse sì per l'animazione...

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

void gui_update(struct highway_t *h)//mai bloccante
{
	pthread_mutex_lock(&h->mutex);

	/*if(!h->fist_gui)
	{
		QApplication app();
		HighwayGui& gui = HighwayGui::getInstance();
		gui.show();
		gui.displayMainMenu();
	}*/

	//Waiting if the user hasn't already chosen to start
	//or if there are no updates
	if(!h->highway_start || !h->gui_new_update)
	{
		h->gui_is_waiting = true;
		pthread_cond_wait(&h->priv_Gui, &h->mutex);
		h->gui_is_waiting = false;
	}

	//There is a new gui update
	h->gui_new_update = false;

	HighwayGui& gui = HighwayGui::getInstance();

	QCoreApplication::postEvent(&gui, new QEvent(QEvent::UpdateRequest),
								Qt::LowEventPriority);
	QMetaObject::invokeMethod(&gui, "moveVehicle");

	if(h->blocked_by_the_gui)
	{
		pthread_cond_signal(&h->priv_blocked_waiting_gui);
	}

	pthread_mutex_unlock(&h->mutex);
}

void pausetta(int msec)
{
  struct timespec t;
  //t.tv_sec = sec;
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

void *gui_routine(void *id)
{

	printf("Avvio la gui \n");

	//QApplication app(argc, argv);
	static int arg = 0;
	static char *b[2];
	b[0] = "a";

	//QApplication app(arg,b);
	//QApplication *app = new QApplication(arg, b);
	//app->setParent(this);

	//HighwayGui& gui = HighwayGui::getInstance();
	//gui.show();
	//gui.displayMainMenu();

	//starting the routine
	for (;;) {
		//printf("Gui routine... \n");
		//gui_update(&highway, gui);
		gui_update(&highway);
		//go_ordinato(vehicle_id,&highway);
		pausetta(100);
		//app.processEvents();
	}
	//return app->exec();
}

void *go_routine(void *id)
{
	long vehicle_id = (long) id;
	printf("Tento avvio veicolo %d\n", vehicle_id);
	//starting the routine
	for (;;) {
		printf("Go, veicolo %d\n", vehicle_id);
		//go(vehicle_id,&highway);
		go_refactor(vehicle_id,&highway);
		//go_ordinato(vehicle_id,&highway);
		//pausetta(100);
		pausetta(200);
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
		pausetta(100);
		//pausetta(250);
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

void initGuiThread()
{
  pthread_attr_t attr;
  pthread_t p;
  //int i;

  //highway_gui = gui;
  //gui = guii;

  // inizializzo i numeri casuali, usati nella funzione pausetta
  srand(555);

  pthread_attr_init(&attr); //check commas!!!

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  //Taking user choices
  // vehicles = veicoli_ritornati_da_menu();
  //initHighway(&highway, vehicles_from_gui);

  // inizializzo il sistema (da modificare una volta tolto il mock presente al suo interno)
  //initHighway(&highway, gui);
  initHighway(&highway);

  //GUI thread's routine
  pthread_create(&p, &attr, gui_routine, NULL);

  pthread_attr_destroy(&attr);

  // aspetto 5 secondi prima di terminare tutti quanti

}

void initVehiclesThreads()
{
  pthread_attr_t attr;
  pthread_t p;
  int i;

  // inizializzo i numeri casuali, usati nella funzione pausetta
  srand(555);

  pthread_attr_init(&attr); //check commas!!!

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  //Vehicle control threads
  for(i=0; i<(highway.n_vehicles); i++)
  {
	  pthread_create(&p, &attr, go_routine, (void *)i);
	  pthread_create(&p, &attr, check_front_distance_routine,  (void *)i);
  }

  pthread_attr_destroy(&attr);

  // aspetto 5 secondi prima di terminare tutti quanti

}


void initThreads()
{
  pthread_attr_t attr;
  pthread_t p;
  int i;

  //highway_gui = gui;
  //gui = guii;

  // inizializzo i numeri casuali, usati nella funzione pausetta
  srand(555);

  pthread_attr_init(&attr); //check commas!!!

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  //Taking user choices
  // vehicles = veicoli_ritornati_da_menu();
  //initHighway(&highway, vehicles_from_gui);

  // inizializzo il sistema (da modificare una volta tolto il mock presente al suo interno)
  //initHighway(&highway, gui);
  initHighway(&highway);

  //Create threads

  //GUI thread
  //pthread_create(&p, &attr, gui_routine, NULL);

  //Vehicle control threads
  for(i=0; i<(highway.n_vehicles); i++)
  {
	  pthread_create(&p, &attr, go_routine, (void *)i);
	  pthread_create(&p, &attr, check_front_distance_routine,  (void *)i);
  }

  pthread_attr_destroy(&attr);

  // aspetto 5 secondi prima di terminare tutti quanti
  pausetta(5000);

}
