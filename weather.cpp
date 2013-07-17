#include <vector>
#include <sstream>
#include "options.h"
#include "game.h"
#include "weather.h"
#include "weather_const.h"
#include "calendar.h"
#define PLAYER_OUTSIDE (g->m.is_outside(g->u.posx, g->u.posy) && g->levz >= 0)
#define THUNDER_CHANCE 50
#define LIGHTNING_CHANCE 600

void weather_effect::glare(game *g)
{
 if (PLAYER_OUTSIDE && g->is_in_sunlight(g->u.posx, g->u.posy) && !g->u.is_wearing("sunglasses"))
  g->u.infect(DI_GLARE, bp_eyes, 1, 2, g);
}

void weather_effect::wet(game *g)
{
 if (!g->u.is_wearing("coat_rain") && !g->u.has_trait(PF_FEATHERS) &&
     g->u.warmth(bp_torso) < 20 && PLAYER_OUTSIDE && one_in(2))
  g->u.add_morale(MORALE_WET, -1, -30);
// Put out fires and reduce scent
 for (int x = g->u.posx - SEEX * 2; x <= g->u.posx + SEEX * 2; x++) {
  for (int y = g->u.posy - SEEY * 2; y <= g->u.posy + SEEY * 2; y++) {
   if (g->m.is_outside(x, y)) {
    field *fd = &(g->m.field_at(x, y));
    if (fd->type == fd_fire)
     fd->age += 15;
    if (g->scent(x, y) > 0)
     g->scent(x, y)--;
   }
  }
 }
}

void weather_effect::very_wet(game *g)
{
 if (!g->u.is_wearing("coat_rain") && !g->u.has_trait(PF_FEATHERS) &&
     g->u.warmth(bp_torso) < 50 && PLAYER_OUTSIDE)
  g->u.add_morale(MORALE_WET, -1, -60);
// Put out fires and reduce scent
 for (int x = g->u.posx - SEEX * 2; x <= g->u.posx + SEEX * 2; x++) {
  for (int y = g->u.posy - SEEY * 2; y <= g->u.posy + SEEY * 2; y++) {
   if (g->m.is_outside(x, y)) {
    field *fd = &(g->m.field_at(x, y));
    if (fd->type == fd_fire)
     fd->age += 45;
    if (g->scent(x, y) > 0)
     g->scent(x, y)--;
   }
  }
 }
}

void weather_effect::thunder(game *g)
{
 very_wet(g);
 if (one_in(THUNDER_CHANCE)) {
  if (g->levz >= 0)
   g->add_msg("You hear a distant rumble of thunder.");
  else if (!g->u.has_trait(PF_BADHEARING) && one_in(1 - 3 * g->levz))
   g->add_msg("You hear a rumble of thunder from above.");
 }
}

void weather_effect::lightning(game *g)
{
thunder(g);
}

void weather_effect::light_acid(game *g)
{
    wet(g);
    if (int(g->turn) % 10 == 0 && PLAYER_OUTSIDE)
    {
        if (!g->u.is_wearing("coat_rain") || !one_in(3))
        {
            g->add_msg("The acid rain stings, but is mostly harmless for now...");
            if (one_in(20) && (g->u.pain < 10)) {g->u.pain++;}
        }
        else
        {
            g->add_msg("Your raincoat protects you from the acidic drizzle.");
        }
    }
}

void weather_effect::acid(game *g)
{
    if (PLAYER_OUTSIDE)
    {
        if (!g->u.is_wearing("coat_rain") || !one_in(10))
        {
            g->add_msg("The acid rain burns!");
            if (one_in(8)&&(g->u.pain < 100)) {g->u.pain += 3 * rng(1, 3);}
        }
        else
        {
            g->add_msg("Your raincoat protects you from the acid rain");
        }
    }

 if (g->levz >= 0) {
  for (int x = g->u.posx - SEEX * 2; x <= g->u.posx + SEEX * 2; x++) {
   for (int y = g->u.posy - SEEY * 2; y <= g->u.posy + SEEY * 2; y++) {
    if (!g->m.has_flag(diggable, x, y) && !g->m.has_flag(noitem, x, y) &&
        g->m.move_cost(x, y) > 0 && g->m.is_outside(x, y) && one_in(400))
     g->m.add_field(g, x, y, fd_acid, 1);
   }
  }
 }
 for (int i = 0; i < g->z.size(); i++) {
  if (g->m.is_outside(g->z[i].posx, g->z[i].posy)) {
   if (!g->z[i].has_flag(MF_ACIDPROOF))
    g->z[i].hurt(1);
  }
 }
 very_wet(g);
}


// Script from wikipedia:
// Current time
// The current time is hour/minute Eastern Standard Time
// Local conditions
// At 8 AM in Falls City, it was sunny. The temperature was 60 degrees, the dewpoint 59,
// and the relative humidity 97%. The wind was west at 6 miles (9.7 km) an hour.
// The pressure was 30.00 inches (762 mm) and steady.
// Regional conditions
// Across eastern Nebraska, southwest Iowa, and northwest Missouri, skies ranged from
// sunny to mostly sunny. It was 60 at Beatrice, 59 at Lincoln, 59 at Nebraska City, 57 at Omaha,
// 59 at Red Oak, and 62 at St. Joseph."
// Forecast
// TODAY...MOSTLY SUNNY. HIGHS IN THE LOWER 60S. NORTHEAST WINDS 5 TO 10 MPH WITH GUSTS UP TO 25 MPH.
// TONIGHT...MOSTLY CLEAR. LOWS IN THE UPPER 30S. NORTHEAST WINDS 5 TO 10 MPH.
// MONDAY...MOSTLY SUNNY. HIGHS IN THE LOWER 60S. NORTHEAST WINDS 10 TO 15 MPH.
// MONDAY NIGHT...PARTLY CLOUDY. LOWS AROUND 40. NORTHEAST WINDS 5 TO 10 MPH.

// 0% – No mention of precipitation
// 10% – No mention of precipitation, or isolated/slight chance
// 20% – Isolated/slight chance
// 30% – (Widely) scattered/chance
// 40% or 50% – Scattered/chance
// 60% or 70% – Numerous/likely
// 80%, 90% or 100% – No additional modifiers (i.e. "showers and thunderstorms")

std::string weather_forecast(game *g, radio_tower tower)
{
    std::stringstream weather_report;

    // Current time
    weather_report << "The current time is " << g->turn.print_time() << " Eastern Standard Time.  ";

    // Local conditions
    weather_report << "At " << g->turn.print_time(true);

    city *closest_city = &g->cur_om->cities[g->cur_om->closest_city(point(tower.x, tower.y))];
    if (closest_city)
    {
        weather_report << " in " << closest_city->name;
    }
    weather_report << ", it was " << weather_data[g->weather].name << ".  ";
    weather_report << "The temperature was " << print_temperature(g->temperature);

    //weather_report << ", the dewpoint ???, and the relative humidity ???.  ";
    //weather_report << "The wind was <direction> at ? mi/km an hour.  ";
    //weather_report << "The pressure was ??? in/mm and steady/rising/falling.";

    // Regional conditions (simulated by chosing a random range containing the current conditions).
    // Adjusted for weather volatility based on how many weather changes are coming up.
    //weather_report << "Across <region>, skies ranged from <cloudiest> to <clearest>.  ";
    // TODO: Add fake reports for nearby cities

    // TODO: weather forecast
    // forecasting periods are divided into 12-hour periods, day (6-18) and night (19-5)
    // Accumulate percentages for each period of various weather statistics, and report that
    // (with fuzz) as the weather chances.
    int weather_proportions[NUM_WEATHER_TYPES] = {0};
    signed char high = 0;
    signed char low = 0;
    calendar start_time = g->turn;
    int period_start = g->turn.hours();
    // TODO wind direction and speed
    for( std::list<weather_segment>::iterator period = g->future_weather.begin();
         period != g->future_weather.end(); period++ )
    {
        int period_deadline = period->deadline.hours();
        signed char period_temperature = period->temperature;
        weather_type period_weather = period->weather;
        bool start_day = period_start >= 6 && period_start <= 18;
        bool end_day = period_deadline >= 6 && period_deadline <= 18;

        high = std::max(high, period_temperature);
        low = std::min(low, period_temperature);

        if(start_day != end_day) // Assume a period doesn't last over 12 hrs?
        {
            weather_proportions[period_weather] += end_day ? 6 : 18 - period_start;
            int weather_duration = 0;
            int predominant_weather;
            std::string day;
            if( g->turn.days() == period->deadline.days() )
            {
                if( start_day )
                {
                    day = "Today";
                }
                else
                {
                    day = "Tonight";
                }
            }
            else
            {
                day = start_time.day_of_week();
                if( !start_day )
                {
                    day += " Night";
                }
            }
            for( int i = WEATHER_CLEAR; i < NUM_WEATHER_TYPES; i++)
            {
                if( weather_proportions[i] > weather_duration)
                {
                    weather_duration = weather_proportions[i];
                    predominant_weather = i;
                }
            }
            // Print forecast
            weather_report << day << "..." << weather_data[predominant_weather].name << ".  ";
            weather_report << "Highs of " << print_temperature(high) << ".  Lows of " << print_temperature(low) << ".  ";

            low = period_temperature;
            high = period_temperature;
            weather_proportions[period_weather] += end_day ? 6 : 18 - period_start;
        } else {
            weather_proportions[period_weather] += period_deadline - period_start;
        }
        start_time = period->deadline;
        period_start = period_deadline;
    }
    return weather_report.str();
}

std::string print_temperature(float fahrenheit, int decimals)
{
    std::stringstream ret;

    ret.precision(decimals);
    ret << std::fixed;

    if(OPTIONS[OPT_USE_CELSIUS])
    {
        ret << ((fahrenheit-32) * 5 / 9) << "C";
    }
    else
    {
        ret << fahrenheit << "F";
    }

    return ret.str();

}

////////////////////////////////////////////////////
// helpful helper functions

// stub, until there's more than wet/very wet.
// returns 0, 300, 600 (since we use 10unit per min for rot anyway, and morale is -30 / -60)
int wtype_to_hourly_precip (const weather_type wt) {
  if (wt == WEATHER_DRIZZLE ) {
    return 300;
  } else if ( wt ==  WEATHER_RAINY || wt == WEATHER_THUNDER || wt == WEATHER_LIGHTNING ) {
    return 600;
  // todo; bucket of melted snow?
  } else {
    return 0;
  }
}

// return sunlight, modified by weather. Absolutely no moonlight.
int true_sunlight( const calendar& cal, const weather_type wtype ) {
   calendar sunrise_time = cal.sunrise(), sunset_time = cal.sunset();
   //int wsun[NUM_WEATHER_TYPES]={0,0,20,-20,-30,-40,-50,-30,-40,-30,-30,-55};

   int wmod=weather_data[wtype].light_modifier;

   int mins = 0, sunrise_mins = 0, sunset_mins = 0;
   mins = cal.minutes_past_midnight();
   sunrise_mins = sunrise_time.minutes_past_midnight();
   sunset_mins = sunset_time.minutes_past_midnight();

   int ret = 0;
   if (mins > sunset_mins + TWILIGHT_MINUTES || mins < sunrise_mins) { 
     return ret;
   } else if ( mins >= sunrise_mins && mins <= sunrise_mins + TWILIGHT_MINUTES) {
     double percent = double(mins - sunrise_mins) / TWILIGHT_MINUTES;
     ret = int( double(DAYLIGHT_LEVEL + wmod ) * percent );
   } else if ( mins >= sunset_mins && mins <= sunset_mins + TWILIGHT_MINUTES) {
     double percent = double(mins - sunset_mins) / TWILIGHT_MINUTES;
     ret = int( double(DAYLIGHT_LEVEL + wmod ) * (1. - percent) );
   } else {
     ret = DAYLIGHT_LEVEL;
   }
   return ( ret < 0 ? 0 : ret );
}

// generate struct for hourly weather log
hourly_weather_ent mk_weather_log_ent(int temp, weather_type weathertype, int hour) {
    calendar cal(hour*600);
    hourly_weather_ent ent;
    ent.temperature = temp;
    ent.rot = get_hourly_rotpoints_at_temp(temp);
    ent.rain = wtype_to_hourly_precip(weathertype); // todo; partial
    ent.acid = ( weathertype == WEATHER_ACID_RAIN ? 600 : (
        weathertype == WEATHER_ACID_DRIZZLE ? 300 :
        0 )
    );
    ent.sun = true_sunlight( cal, weathertype );
    return ent;
}

// iterate backwards through hourly weather log and populate quick-lookup fields
void weather_log_cumulative_update( std::map <int, hourly_weather_ent> &wlog ) {
  for(int hour = wlog.size()-1, ot=0, rt=0, pt=0, at=0, st=0; hour >= 0; hour--) {
    ot += 600;
    rt += wlog[hour].rot;
    pt += wlog[hour].rain; // todo: calculate evaporation per some-unit^2 surface
    at += wlog[hour].acid;
    st += wlog[hour].sun;
    wlog[hour].rot_since = rt;
    wlog[hour].rain_since = pt;
    wlog[hour].acid_since = at;
    wlog[hour].sun_since = st;
    //popup_nowait("%d %d %d",hour,wlog[hour].rot,rt);
  }
}

// return logged temperature at turn/600, or 65 if hour's temperature isn't logged.
int game::get_temp_at_hour( const int hour ) {
  std::map <int, hourly_weather_ent>::iterator ent = weather_log.find(hour);
  return (ent != weather_log.end() ? weather_log[ hour ].temperature : 65 );
}

// calc rot at turn/600, or 600 if hour isn't logged.
int game::calc_rot_at_hour( const int hour ) {
  std::map <int, hourly_weather_ent>::iterator ent = weather_log.find(hour);
  if ( ent == weather_log.end() ) return 600;
  return get_hourly_rotpoints_at_temp ( weather_log[ hour ].temperature );
}

// get cached rot at turn/600, or return 600
int game::get_rot_at_hour( const int hour ) {
  std::map <int, hourly_weather_ent>::iterator ent = weather_log.find(hour);
  if (ent == weather_log.end()) return 600;
  return weather_log[ hour ].rot;
}

// get cached accumulative rot since turn
int game::get_rot_since( const int start_turn ) {
  bool do_rem = true;            // full value, or just every full hour up to this?
  int hrot=0,remrotstart=0,remrotnow=0, start_rem=0, now_rem=0;

  const int now_temp=(int)temperature;         // game->temperature;
  const int now_h=int(turn)/600; // game->turn.hour_param;

  int start_h=(start_turn/600);

  if(do_rem==true) {
    now_rem=int(turn) % 600;       // game->turn.minute;
    start_rem=start_turn % 600;
  }

  if ( now_h > 0 && start_h < now_h ) {
    std::map <int, hourly_weather_ent>::iterator ent = weather_log.find(start_h);
    if ( ent == weather_log.end() ) { // shouldn't happen
      ent=weather_log.upper_bound(start_h+1);
      if ( ent == weather_log.end() || ent->first >= now_h ) { // hrrrrm
        hrot=(now_h-start_h) * defrot;
        return hrot; // we'll just walk away now >.>
      } else {
        hrot=(start_h - ent->first) * defrot;
        start_h=ent->first;
        hrot+=weather_log[start_h].rot_since;
        remrotstart=get_rot_at_hour(start_h);
      }
    } else {
      hrot=weather_log[start_h].rot_since;
      remrotstart=get_rot_at_hour(start_h);
    }
  } else {
    now_rem=int(turn)-start_turn;
    remrotnow=get_hourly_rotpoints_at_temp(now_temp);
    hrot=0;
  }
  if ( do_rem == true && start_rem > 0 ) {
    remrotstart=( ( get_rot_at_hour(start_h) * start_rem ) / 600 );
    hrot+=remrotstart;
    remrotnow=( ( remrotnow * now_rem ) / 600 );
    hrot+=remrotnow;

  }
  return hrot;
}
