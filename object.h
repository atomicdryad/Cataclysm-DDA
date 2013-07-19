#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <string>
#include <vector>
#include "enums.h"
enum object_type {
  OBJECT_TYPE_NONE, OBJECT_TYPE_INVALID, OBJECT_TYPE_ITEM,
  OBJECT_TYPE_PLAYER, OBJECT_TYPE_NPC, OBJECT_TYPE_MONSTER,
  OBJECT_TYPE_VEHICLE, OBJECT_TYPE_MAP, NUM_OBJECT_TYPES
};
//std::string object_typename[NUM_OBJECT_TYPES]={"none","invalid","item","player","npc","vehicle","-fail-"};
class baseobject {
public:
  object_type objtype;
  baseobject * parentref;
  std::string objstr;
  baseobject();
  baseobject * getparentref();
  virtual void basefunc() = 0;
  virtual std::string getname() { return "(invalid-object)"; };
  ~baseobject();
  point getparentloc();
  //climatezone * getparentenv();
};


class null_object: public virtual baseobject {
public:
  virtual void basefunc() {};
  std::string getname() { return "(null)"; };
  null_object() {
    objstr="(NULL)";
    parentref=static_cast<baseobject*>(this);
    objtype=OBJECT_TYPE_NONE;
  }
};

extern null_object nullobject;

#endif
