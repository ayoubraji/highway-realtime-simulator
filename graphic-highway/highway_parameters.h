#ifndef HIGHWAY_PARAMETERS_H
#define HIGHWAY_PARAMETERS_H

struct highway_parameters_t
{
	int cars_perc, trucks_perc, motorcycles_perc;
	bool rare_frequency, frequent_frequency;
	int vehicles_number, vehicle_to_track;
};

#endif // HIGHWAY_PARAMETERS_H
