#include "weather_const.h"

std::vector<int> rot_chart=calc_rot_array(200);

/////////////////////////////////////

// calculate how much food rots per hour, based on 10 = 1 minute of decay @ 65 F
// IRL this tends to double every 10c a few degrees above freezing, but past a certian
// point the rate decreases until even extremophiles find it too hot. Here we just stop
// further acceleration at 105 F. This should only need to run once.
int calc_hourly_rotpoints_at_temp(const int temp) {
  const int base=600;       // default temp = 65, so generic->rotten() assumes 600 decay points per hour
  const int dropoff=38;     // ditch our fancy equation and do a linear approach to 0 rot at 31f
  const int cutoff=105;     // stop torturing the player at this temperature, which is
  const int cutoffrot=3540; // ..almost 6 times the base rate. bacteria hate the heat too

  const int dsteps=dropoff - 32;
  const int dstep=(35.91*pow(2.0,(float)dropoff/16) / dsteps);

  if ( temp < 32 ) {
    return 0;
  } else if ( temp > cutoff ) {
    return cutoffrot;
  } else if ( temp < dropoff ) {
    return ( ( temp - 32 ) * dstep );
  } else {
    return int((35.91*pow(2.0,(float)temp/16))+0.5);
  }
}

// generate vector based on above function
std::vector<int> calc_rot_array(const int cap) {
  std::vector<int> ret;
  for (int i = 0; i < cap; i++  ) {
    ret.push_back(calc_hourly_rotpoints_at_temp(i));
  }
  return ret;
}

// fetch value from rot_chart for temperature
int get_hourly_rotpoints_at_temp (const int temp) {
  if ( temp < 0 || temp < -1 ) return 0;
  if ( temp > 150 ) return 3540;
  return rot_chart[temp];
}
