#include "highway.h"
#include <iostream>
#include <QApplication>


void initStandardHighway(struct highway_t *h)//DA MODIFICARE quando non saranno mockati qua dentro ma in vehicles tramite gui
{

	int i,j,k,l;
	pthread_mutexattr_t m_attr;
	pthread_condattr_t c_attr;
	int max_speed, temp_type;
	int cars, trucks, motorcycles;

	cars = 10;
	trucks = 15;
	motorcycles = 25;

	pthread_mutexattr_init(&m_attr);
	pthread_condattr_init(&c_attr);
	pthread_mutex_init(&h->mutex, &m_attr);

	//-------------------------------start mock
	h->n_vehicles = 50;
	h->vehicle_to_track = 23;
	h->rare_frequency = true;


	h->vehicles = (vehicle_t*) malloc(h->n_vehicles * sizeof(struct vehicle_t));
	h->priv_Vehicle = (pthread_cond_t*)malloc(h->n_vehicles * sizeof(pthread_cond_t));
	h->next = (int*)malloc(h->n_vehicles * sizeof(int));

	for(i=0; i<h->n_vehicles; i++)
	{
		if(i<=trucks)
		{
			temp_type = TRUCK;
			max_speed = 60;
		}
		else if(i<=motorcycles)
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

		if(i == h->vehicle_to_track)
		{
			h->vehicles[i].to_be_tracked = true;
		}

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

void initCustomHighway(struct highway_t *h, struct highway_parameters_t parameters)//DA MODIFICARE quando non saranno mockati qua dentro ma in vehicles tramite gui
{

	int i,j,k,l;
	pthread_mutexattr_t m_attr;
	pthread_condattr_t c_attr;
	int max_speed, temp_type;
	int cars, trucks, motorcycles;

	cars = parameters.vehicles_number * parameters.cars_perc / 100;
	trucks = parameters.vehicles_number * parameters.trucks_perc / 100;
	motorcycles = parameters.vehicles_number * parameters.motorcycles_perc / 100;

	h->vehicle_to_track = parameters.vehicle_to_track;
	h->rare_frequency = parameters.rare_frequency;

	pthread_mutexattr_init(&m_attr);
	pthread_condattr_init(&c_attr);
	pthread_mutex_init(&h->mutex, &m_attr);

	//-------------------------------start mock
	h->n_vehicles = parameters.vehicles_number;

	h->vehicles = (vehicle_t*) malloc(h->n_vehicles * sizeof(struct vehicle_t));
	h->priv_Vehicle = (pthread_cond_t*)malloc(h->n_vehicles * sizeof(pthread_cond_t));
	h->next = (int*)malloc(h->n_vehicles * sizeof(int));

	for(i=0; i<h->n_vehicles; i++)
	{
		if(i<=trucks)
		{
			temp_type = TRUCK;
			max_speed = 60;
		}
		else if(i<=motorcycles+trucks)
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

		if(i == h->vehicle_to_track)
		{
			h->vehicles[i].to_be_tracked = true;
		}

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
							  Q_ARG(int, pos_vehicle.y_pos),
							  Q_ARG(bool,h->vehicles[vehicle_id].to_be_tracked));


}
