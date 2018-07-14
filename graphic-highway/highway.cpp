#include "highway.h"
#include <iostream>
#include <QApplication>

void initHighway(struct highway_t *h)//DA MODIFICARE quando non saranno mockati qua dentro ma in vehicles tramite gui
{

	int i,j,k,l;
	pthread_mutexattr_t m_attr;
	pthread_condattr_t c_attr;
	int max_speed, temp_type;

	pthread_mutexattr_init(&m_attr);
	pthread_condattr_init(&c_attr);
	pthread_mutex_init(&h->mutex, &m_attr);

	//-------------------------------start mock
	h->n_vehicles = 50;

	h->vehicles = (vehicle_t*) malloc(h->n_vehicles * sizeof(struct vehicle_t));
	h->priv_Vehicle = (pthread_cond_t*)malloc(h->n_vehicles * sizeof(pthread_cond_t));
	h->next = (int*)malloc(h->n_vehicles * sizeof(int));

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

		h->vehicles[i] = createVehicle(temp_type,max_speed);

		pthread_cond_init(&h->priv_Vehicle[i], &c_attr);
		h->next[i] = i+1;

	}
	//------------------------------end mock

	for(i=0; i<LANES; i++)
	{

		//Init highway 'status' for each portion of the road
		for(k=0; k<LANES; k++)
		{
			for(l=0; l<ROAD_LENGHT; l++)
			{
				h->road[i][k][l] = -1;
			}
		}

	}

	h->next[h->n_vehicles-1] = h->waiting_to_start = h->next_vehicle = 0;
	h->highway_start = false;

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


}
