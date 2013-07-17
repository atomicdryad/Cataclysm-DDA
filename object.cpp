#include "object.h"
#include "player.h"
#include "vehicle.h"
#include "output.h"
//extern std::map<int, baseobject*> ids;
#define badint -2147483647
null_object nullobject;//=&new null_object;
//nullobject.parentref=nullobject; // this is intentional; check your iterations
//nullobject.objstr="(NULL)";

  baseobject::baseobject() {
    //printf("OBJ new\n");
      objstr="(OBJECT)";
      parentref=&nullobject;
      objtype=OBJECT_TYPE_NONE;
  };
  baseobject::~baseobject() {
    objtype=OBJECT_TYPE_INVALID;
    parentref=&nullobject;
    //printf("OBJ:rm\n");
  }

baseobject * baseobject::getparentref() {
    if ( parentref == NULL ) {
      nullobject.objstr="(null)";
      nullobject.parentref=&nullobject;
      //return &nullobject;
    }
    return parentref;
  }

point baseobject::getparentloc() {
    int x=badint;
    int y=badint;
    int i=5;
    int otype=OBJECT_TYPE_NONE;
    baseobject * bo=static_cast<baseobject*>(parentref);
    if(bo!=NULL && bo != &nullobject && bo->objtype != OBJECT_TYPE_NONE ) {
        otype=bo->objtype;
//        if(otype != OBJECT_TYPE_NONE) {
            do {
popup(" '%s'[%d] ('%s') -%i-> '%s'[%d] ('%s')",
getname().c_str(),
objtype,
objstr.c_str(),
i,
(bo!=NULL?bo->getname().c_str():"?!"),
otype,
(bo!=NULL?bo->objstr.c_str():"?!")
);
                      otype=bo->objtype;
                i--;
                if(otype==OBJECT_TYPE_VEHICLE) {
                    x=dynamic_cast<vehicle*>(bo)->posx;
                    y=dynamic_cast<vehicle*>(bo)->posy;
                    //break;
                } else if(otype==OBJECT_TYPE_PLAYER) {
                    x=dynamic_cast<player*>(bo)->posx;
                    y=dynamic_cast<player*>(bo)->posy;
                    //break;
                } else if(otype==OBJECT_TYPE_NONE) {
                    break;
                }
/*                if ( bo->parentref == NULL ) {
                    break;   
                } else {
                    bo=bo->parentref;
                }
                otype=bo->objtype;
*/
    //            i--;
  bo=static_cast<baseobject*>(bo->parentref);
  if(bo==NULL) {
    popup("bo=nul");
    return point(x,y);
  }
  if ( bo->parentref == NULL ) {
    popup("pref=nul");
  }

  otype=bo->objtype;
  if( otype==OBJECT_TYPE_NONE ) {
    popup("ot=none");
  }
            } while ( bo != NULL && bo != &nullobject && otype != OBJECT_TYPE_NONE && i > 0);
//        }
    }
    popup(" '%s'[%d] ('%s') -%i-: '%s'[%d] '%s' == %d,%d",
getname().c_str(),
objtype,
objstr.c_str(),
i,(x!=badint?objstr.c_str():"?!"),otype,(bo!=NULL?getname().c_str():"?!"),x,y
);

    return point(x,y);
}
