#ifndef VEHICLE_H
#define VEHICLE_H

#include "highway.h"
//vehicles type
enum VEHICLES{CAR, MOTORCYCLE, TRUCK};
#define CAR 0
#define MOTORCYCLE 1
#define TRUCK 2

//Inside a lane, the vehicol has position (x,y)
struct position_t
{
	int lane;
	int x_pos;
	int y_pos;
	bool travel_completed;
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
	int movement_type;
	bool should_correct_alignment;
	bool to_be_tracked;
};

//methods definition
struct vehicle_t createVehicle(int type, int max_speed);

void position_switch(int vehicle_id, struct highway_t *h, int lane);

int check_front(int vehicle_id, struct highway_t *h, int lane_to_check, bool for_the_start);


#endif // VEHICLE_H
