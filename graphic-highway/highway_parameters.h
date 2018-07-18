/*
	Ayoub Raji
	Project of Real Time Embedded Systems course
*/

#ifndef HIGHWAY_PARAMETERS_H
#define HIGHWAY_PARAMETERS_H

//Used to pass the user parameters of the custom choice dialog
struct highway_parameters_t
{
	int cars_perc, trucks_perc, motorcycles_perc;
	bool rare_frequency, frequent_frequency;
	int vehicles_number, vehicle_to_track;
};

#endif // HIGHWAY_PARAMETERS_H
