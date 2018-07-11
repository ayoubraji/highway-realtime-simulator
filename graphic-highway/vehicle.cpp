#include "vehicle.h"
#include <iostream>
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

void position_switch(int vehicle_id, struct highway_t *h, int lane)
{
	int lane_vehicle, x_pos, y_pos, new_y_pos, actual_speed;
	struct position_t pos_vehicle;

	pos_vehicle = h->vehicles[vehicle_id].position;
	lane_vehicle = pos_vehicle.lane;
	x_pos = pos_vehicle.x_pos;
	y_pos = pos_vehicle.y_pos; //-1

	actual_speed = h->vehicles[vehicle_id].actual_speed;

	new_y_pos = (actual_speed > 40) ? y_pos+2 : y_pos+1; //0
	new_y_pos = (actual_speed > 80) ? new_y_pos+1 : new_y_pos;
	new_y_pos = (actual_speed > 120) ? new_y_pos+2 : new_y_pos;
	new_y_pos = (lane_vehicle != lane) ? new_y_pos+1 : new_y_pos; //when we are changing the lane

	if(y_pos != -1)//if the vehicle is already on the road, it is necessary to set the previous position as free (-1)
	{
		h->road[lane_vehicle][x_pos][y_pos] = -1;
	}
	h->road[lane][x_pos][new_y_pos] = vehicle_id; //updating the new position in the road's array

	if(h->vehicles[vehicle_id].type == TRUCK)
	{
		h->road[lane_vehicle][x_pos][y_pos+1] = -1;
		h->road[lane][x_pos][new_y_pos+1] = vehicle_id;
	}

	h->vehicles[vehicle_id].position.lane = lane;
	h->vehicles[vehicle_id].position.y_pos = new_y_pos;
	printf("PS Movimento veicolo %d a posizione: (%d, %d) corsia %d, a velocità: %d\n",
		   vehicle_id, pos_vehicle.x_pos, h->vehicles[vehicle_id].position.y_pos,
		   h->vehicles[vehicle_id].position.lane, h->vehicles[vehicle_id].actual_speed);
}

//int check_nearby(int vehicle_id, struct highway_t *h, )

//Check if there is a vehicle in front on a specific lane, and return its id
int check_front(int vehicle_id, struct highway_t *h, int lane_to_check, bool for_the_start)
{
	int i, index_to_check, x_pos, y_pos, lane_vehicle, distance_to_check;
	int front_vehicle_id = -1;
	struct position_t pos_vehicle;

	pos_vehicle = h->vehicles[vehicle_id].position;
	//x_pos = pos_vehicle.x_pos;
	x_pos = 0; //should be managed...
	y_pos = pos_vehicle.y_pos; //-1

	//distance_to_check = (for_the_start) ? h->vehicles[vehicle_id].min_security_distance+3 : h->vehicles[vehicle_id].min_security_distance+1;

	distance_to_check = h->vehicles[vehicle_id].min_security_distance+3;

	for(i=1; i <= distance_to_check; i++)
	{
		index_to_check = y_pos+i;
		//if(h->road[0][0][index_to_check] != -1) //It doesn't work in cpp but in c it does... WHY???
		if(h->road[lane_to_check][x_pos][index_to_check] != -1 && h->road[lane_to_check][x_pos][index_to_check] != vehicle_id)
		{
			front_vehicle_id = h->road[lane_to_check][x_pos][index_to_check];

			if(!for_the_start)
			{
				printf("Check Front Veicolo %d: Nella posizione %d, a distanza %d da me,  c'è %d \n", vehicle_id, index_to_check, i, h->road[lane_to_check][x_pos][index_to_check]);
			}
			else
			{
				printf("In posizione %d c'è già %d \n", index_to_check, h->road[lane_to_check][x_pos][index_to_check]);
			}

			break; //maybe not safety for RT
		}
	}
	return front_vehicle_id;
}
