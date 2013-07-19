#ifndef _WEATHER_CONST_H_
#define _WEATHER_CONST_H_
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>

#define defrot 600

extern std::vector<int> rot_chart;

std::vector<int> calc_rot_array(const int cap);
int calc_hourly_rotpoints_at_temp (const int temp);
int get_hourly_rotpoints_at_temp (const int temp);
#endif // _WEATHER_CONST_H_
