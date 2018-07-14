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
	vehicle.movement_type = GO_AHEAD;

	//Initialize lane and position, which will be changed by the go function
	vehicle.position.lane = 0;
	vehicle.position.x_pos = 0;
	vehicle.position.y_pos = -1;

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

	new_y_pos = (actual_speed > 60) ? y_pos+2 : y_pos+1; //0
	new_y_pos = (actual_speed > 80) ? new_y_pos+1 : new_y_pos;
	new_y_pos = (actual_speed > 120) ? new_y_pos+1 : new_y_pos;

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
	lane_vehicle = pos_vehicle.lane;

	distance_to_check = h->vehicles[vehicle_id].min_security_distance+12;

	//If I'm checking the right lane, I should consider also the possible vehicles behind me
	i = (lane_to_check < lane_vehicle) ? -2 : 1;
	for(i; i <= distance_to_check; i++)
	{
		index_to_check = y_pos+i;
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
