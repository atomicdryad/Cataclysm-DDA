#ifndef _RULES_H_
#define _RULES_H_
#include <vector>
#include <map>
#include <list>
#include <string>

enum rules_key {
  RANGED_MULT, DEATH_DELETE, CAR_STABLE,
  SPECIAL_PLACES, MONSTERS_SWAMP, MONSTERS_WORM, EYEBOT_EVENTS,
  MONSTERS_SUPERZOMBIES, MONSTERS_ROBOTS, MONSTERS_ALIENS, MONSTERS_SUPERBUGS, // TODO
  MONSTERS_DND_REJECTS, VEHICLE_SPEED_MULT, REVIVE_BURNMAX, USE_OLD_SPAWNMETHOD, CIRCLEDIST,
  NUM_RULES
};
/*
//#ifdef CUSTOMRULES
//#include "customrules.h"
//#else
// Stub: cut and paste into customrules.h
// Compile with: PROFILE=-DCUSTOMRULES=1 make
// later these will be set via rulesets
struct orule {
   rules_key key;
   double var;
   double default;
   std::string txtid;
   std::string title;
   std::string desc;
};
///// cut here /////
struct orules_table {
  double rules[NUM_RULES];
  rules_table() {
    rules[RANGED_MULT]=1;		// def=1: 1 = normal range of ~ 10 squares
    rules[DEATH_DELETE]=1;		// def=1: delete character on death, 0: don't delete
    rules[CAR_STABLE]=0;		// def=0: "You fumble with the car's controls" based on skill and car damage (?), 1: cars are always stable
//     spawn overrides
//      def=-1: go with the standard set by OPT_CLASSIC_ZOMBIES
//      0: Do NOT spawn, regardless of option
//      1: Spawn, regardless of option
//    
    rules[SPECIAL_PLACES]=-1;		// science labs and such
    rules[MONSTERS_SWAMP]=-1;		// giant skeeters(tm)
    rules[MONSTERS_WORM]=-1;		// graboids and junk
    rules[EYEBOT_EVENTS]=-1;		// random floating eyebot attacks
    rules[MONSTERS_SUPERZOMBIES]=-1;
    rules[VEHICLE_SPEED_MULT]=1;
    rules[REVIVE_BURNMAX]=500;
    rules[USE_OLD_SPAWNMETHOD]=0;
    rules[CIRCLEDIST]=0;
  };
  double& operator[] (rules_key i) { return rules[i]; };
  double& operator[] (int i) { return rules[i]; };
};
*/
struct gamerule {
    std::string txtkey;
    double defaultval;
    std::string name;
    std::string desc;
};

struct rules_table {
    double val[NUM_RULES];
    gamerule ruleentries[NUM_RULES];
    std::map <std::string, int> txtkey;

    void add(int rid, std::string txt, double def, std::string nam, std::string des) {
        ruleentries[rid].txtkey=txt;
        ruleentries[rid].defaultval=def;
        ruleentries[rid].name=nam;
        ruleentries[rid].desc=des;
        txtkey[txt]=rid;
        val[rid]=def;
    };

    double& operator[] (rules_key i) { return val[i]; };
    double& operator[] (int i) { return val[i]; };
    
};

///// cut here /////
//#endif
extern rules_table RULES;
void rules_init();
#endif
