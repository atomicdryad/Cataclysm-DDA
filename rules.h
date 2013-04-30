enum rules_key {
  RANGED_MULT, DEATH_DELETE, CAR_STABLE,
  SPECIAL_PLACES, MONSTERS_SWAMP, MONSTERS_WORM, EYEBOT_EVENTS,
  MONSTERS_SUPERZOMBIES, MONSTERS_ROBOTS, MONSTERS_ALIENS, MONSTERS_SUPERBUGS, // TODO
  NUM_RULES
};
#define CUSTOMRULES
#ifdef CUSTOMRULES
#include "customrules.h"
#else
// Stub: cut and paste into customrules.h
// later these will be set via rulesets
struct rules_table {
  double rules[NUM_RULES];
  rules_table() {
    rules[RANGED_MULT]=1;		// def=1: 1 = normal range of ~ 10 squares
    rules[DEATH_DELETE]=1;		// def=1: delete character on death, 0: don't delete
    rules[CAR_STABLE]=0;		// def=0: "You fumble with the car's controls" based on skill and car damage (?), 1: cars are always stable
    /* spawn overrides
      def=-1: go with the standard set by OPT_CLASSIC_ZOMBIES
      0: Do NOT spawn, regardless of option
      1: Spawn, regardless of option
    */
    rules[SPECIAL_PLACES]=-1;		// science labs and such
    rules[MONSTERS_SWAMP]=-1;		// giant skeeters(tm)
    rules[MONSTERS_WORM]=-1;		// graboids and junk
    rules[EYEBOT_EVENTS]=-1;		// random floating eyebot attacks
  };
  double& operator[] (rules_key i) { return rules[i]; };
  double& operator[] (int i) { return rules[i]; };
};
#endif
extern rules_table RULES;
//rules_table RULES;