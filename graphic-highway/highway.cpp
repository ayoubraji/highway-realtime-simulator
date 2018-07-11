#include "highway.h"
#include <iostream>
#include <QApplication>

//void initHighway(struct highway_t *h, HighwayGui* gui)
void initHighway(struct highway_t *h)//DA MODIFICARE quando non saranno mockati qua dentro ma in vehicles tramite gui
{

	int i,j,k,l;
	pthread_mutexattr_t m_attr;
	pthread_condattr_t c_attr;
	int max_speed, temp_type;

	//HighwayGui& gui = HighwayGui::getInstance();
	//gui.displayMainMenu();

	pthread_mutexattr_init(&m_attr);
	pthread_condattr_init(&c_attr);
	pthread_mutex_init(&h->mutex, &m_attr);

	//-------------------------------start mock
	h->n_vehicles = 50;

	h->vehicles = (vehicle_t*) malloc(h->n_vehicles * sizeof(struct vehicle_t));
	h->priv_Vehicle = (pthread_cond_t*)malloc(h->n_vehicles * sizeof(pthread_cond_t));
	h->priv_blocked_for_vehicle = (pthread_cond_t*)malloc(h->n_vehicles * sizeof(pthread_cond_t));

	//Init gui condition variable
	pthread_cond_init(&h->priv_Gui, &c_attr);
	pthread_cond_init(&h->priv_blocked_waiting_gui, &c_attr);

	for(i=0; i<LANES; i++)
	{
		h->coda_road[i] = (int*)malloc(h->n_vehicles * sizeof(int));
	}

	h->next = (int*)malloc(h->n_vehicles * sizeof(int));
	h->coda_to_start = (int*)malloc(h->n_vehicles * sizeof(int));
	h->blocking_someone = (int*)malloc(h->n_vehicles * sizeof(int));

	for(i=0; i<h->n_vehicles; i++)
	{
		if(i<15)
		{
			temp_type = TRUCK;
			max_speed = 60;
		}
		else if(i<23)
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
		h->blocking_someone[i] = 0;

	}
	//------------------------------end mock

	for(i=0; i<LANES; i++)
	{
		//Init private semaphores for each lane
		pthread_cond_init(&h->priv_Lane[i], &c_attr);

		//Init queue and iterator
		for(j=0; j<h->n_vehicles; j++)
		{
			h->coda_road[i][j] = -1;//INUTILE forse...
		}

		//Init highway 'status' for each portion of the road
		for(k=0; k<LANES; k++)
		{
			for(l=0; l<ROAD_LENGHT; l++)
			{
				h->road[i][k][l] = -1;
			}
		}

	}

	h->next[h->n_vehicles-1] = h->waiting_to_start = h->next_vehicle = h->next_to_start = h->next_vehicle_to_go = h->index_coda = 0;
	h->gui_is_waiting = h->blocked_by_the_gui = h->fist_gui = false;

	pthread_condattr_destroy(&c_attr);
	pthread_mutexattr_destroy(&m_attr);

}

void graphic_update(int vehicle_id, struct highway_t *h, int movement)
{
	struct position_t pos_vehicle;
	pos_vehicle = h->vehicles[vehicle_id].position;

	printf("Movimento veicolo %d a posizione: (%d, %d) corsia %d, a velocità: %d\n",
		   vehicle_id, pos_vehicle.x_pos, pos_vehicle.y_pos, pos_vehicle.lane, h->vehicles[vehicle_id].actual_speed);

	HighwayGui& gui = HighwayGui::getInstance();

	QCoreApplication::postEvent(&gui, new QEvent(QEvent::UpdateRequest),
								Qt::LowEventPriority);
	QMetaObject::invokeMethod(&gui, "moveVehicle", Q_ARG(int, vehicle_id),
							  Q_ARG(int, pos_vehicle.lane),
							  Q_ARG(int, pos_vehicle.x_pos),
							  Q_ARG(int, pos_vehicle.y_pos));

	//Apply graphical movement (go on, overtake, turn right etc.) to the mainwindow

	//HighwayGui& gui = HighwayGui::getInstance();
	//gui.displayMainMenu();
	//gui.moveVehicle();
	//h->gui.moveVehicle();

}
