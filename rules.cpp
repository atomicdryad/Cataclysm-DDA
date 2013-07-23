#include "game.h"
#include "options.h"
#include "rules.h"
#include "output.h"
#include "debug.h"
#include "keypress.h"

#include <stdlib.h>
#include <fstream>
#include <string>

rules_table RULES;
#ifdef CUSTOMRULES
#include "customrules.h"
#endif

void rules_init() {
RULES.add(RANGED_MULT, "RANGED_MULT", 1.0, "Firearms range multiplier", "This blah desc");
RULES.add(DEATH_DELETE, "DEATH_DELETE", 1, "Delete character on death", "desc");
RULES.add(CAR_STABLE, "CAR_STABLE", 0, "Driving stability", "desc");
RULES.add(SPECIAL_PLACES, "SPECIAL_PLACES", -1, "Non-classic locations", "desc");
RULES.add(MONSTERS_SWAMP, "MONSTERS_SWAMP", -1, "Spawn swamp monsters", "desc");
RULES.add(MONSTERS_WORM, "MONSTERS_WORM", -1, "Spawn worms", "desc");
RULES.add(EYEBOT_EVENTS, "EYEBOT_EVENTS", -1, "Spawn eye-bots", "desc");
RULES.add(MONSTERS_SUPERZOMBIES, "MONSTERS_SUPERZOMBIES", 1, "Spawn super zombies", "desc");
RULES.add(VEHICLE_SPEED_MULT, "VEHICLE_SPEED_MULT", 1, "Vehicle speed multiplier", "desc");
RULES.add(REVIVE_BURNMAX, "REVIVE_BURNMAX", 500, "Option name", "desc");
RULES.add(ZOMBIE_DENSITY_GEO, "ZOMBIE_DENSITY_GEO", 0, "Option name", "desc");


#ifdef CUSTOMRULES
apply_custom_ruleset();
#endif
}

bool rules_test_valid(std::string testname) {
//  switch(testname) {
//  }
  return true;
}
