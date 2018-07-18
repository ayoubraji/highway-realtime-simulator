/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#include "vehicle.h"
#include <iostream>

//#define ROAD_LENGHT 5000
#define ROAD_LENGHT 2500

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
	//1 is the center of the lane, 0 left, 2 right, 3 lane overpassed (so the system failed)
	vehicle.position.x_pos = 1;

	vehicle.position.y_pos = -1;

	vehicle.position.travel_completed = false;

	vehicle.should_correct_alignment = false;

	vehicle.to_be_tracked = false;

	return vehicle;
}

void position_switch(int vehicle_id, struct highway_t *h, int lane)
{
	int lane_vehicle, x_pos, y_pos, new_x_pos, new_y_pos, actual_speed, random_int;
	struct position_t pos_vehicle;
	int frequency;

	frequency = (h->rare_frequency) ? 500 : 200;
	pos_vehicle = h->vehicles[vehicle_id].position;
	lane_vehicle = pos_vehicle.lane;
	x_pos = pos_vehicle.x_pos;
	y_pos = pos_vehicle.y_pos; //-1 initially

	//random between 1 and the frequency
	random_int = rand() % frequency + 1;
	if(random_int < 50)
	{
		//Simulating a wrong lateral movement on the lane
		//if it is in 1 or 3 and the vehicle hasn't detected the disalignment,
		//the system have failed
		switch(x_pos)
		{
		case 0:
			new_x_pos = 3;
			break;
		case 2:
			new_x_pos = 3;
			break;
		case 1:
			new_x_pos = (random_int > 25) ? 0 : 2;
		}
	}
	else //no wrong lateral movement
	{
		new_x_pos = x_pos;
	}

	if(h->vehicles[vehicle_id].should_correct_alignment)//correcting the alignment on the lane
	{
		new_x_pos = 1;
		h->vehicles[vehicle_id].should_correct_alignment = false;
	}

	actual_speed = h->vehicles[vehicle_id].actual_speed;

	//calculating the new y position
	new_y_pos = (actual_speed > 60) ? y_pos+2 : y_pos+1; //0
	new_y_pos = (actual_speed > 80) ? new_y_pos+1 : new_y_pos;
	new_y_pos = (actual_speed > 120) ? new_y_pos+1 : new_y_pos;

	//The vehicle have completed the travel
	if(new_y_pos >= ROAD_LENGHT)
	{
		new_y_pos = ROAD_LENGHT;
		h->vehicles[vehicle_id].position.travel_completed = true;
	}

	//if the vehicle is already on the road, it is necessary to set the previous position as free (-1)
	if(y_pos != -1)
	{
		h->road[lane_vehicle][x_pos][y_pos] = -1;
	}

	//updating the new position in the road's array
	h->road[lane][new_x_pos][new_y_pos] = vehicle_id;

	//Trucks occupy 2 slots of the array
	if(h->vehicles[vehicle_id].type == TRUCK)
	{
		h->road[lane_vehicle][x_pos][y_pos+1] = -1;

		if(!h->vehicles[vehicle_id].position.travel_completed)
		{
			h->road[lane][new_x_pos][new_y_pos+1] = vehicle_id;
		}
	}

	h->vehicles[vehicle_id].position.lane = lane;
	h->vehicles[vehicle_id].position.x_pos = new_x_pos;
	h->vehicles[vehicle_id].position.y_pos = new_y_pos;
}

//Check if there is a vehicle in front on a specific lane, and return its id
int check_front(int vehicle_id, struct highway_t *h, int lane_to_check, bool for_the_start)
{
	int i, j, index_to_check, to_add, x_pos, y_pos, lane_vehicle, distance_to_check;
	struct position_t pos_vehicle;
	int front_vehicle_id = -1;
	bool exit_for = false;

	pos_vehicle = h->vehicles[vehicle_id].position;
	x_pos = pos_vehicle.x_pos;
	y_pos = pos_vehicle.y_pos;
	lane_vehicle = pos_vehicle.lane;

	to_add = (lane_to_check < lane_vehicle) ? 15 : 12;
	distance_to_check = h->vehicles[vehicle_id].min_security_distance+to_add;


	//If I'm checking the right lane, I should consider also the possible vehicles behind me
	i = (lane_to_check < lane_vehicle) ? -2 : 1;

	for(i; !exit_for && i <= distance_to_check; i++)
	{
		index_to_check = y_pos+i;
		for(j = 0; j < 4; j++)
		{
			if(h->road[lane_to_check][j][index_to_check] != -1 && h->road[lane_to_check][j][index_to_check] != vehicle_id)
			{
				front_vehicle_id = h->road[lane_to_check][j][index_to_check];

				//It is already too much verbose...
				/*if(!for_the_start)
				{
					printf("Check Front Vehicle %d: Nella posizione %d, a distanza %d da me,  c'è %d \n", vehicle_id, index_to_check, i, h->road[lane_to_check][x_pos][index_to_check]);
				}
				else
				{
					printf("In posizione %d c'è già %d \n", index_to_check, h->road[lane_to_check][j][index_to_check]);
				}*/

				exit_for = true;
			}
		}
	}
	return front_vehicle_id;
}
