#ifndef VNOBJECT_HELPER_H
#define VNOBJECT_HELPER_H


// Base files
#include "../keywords.h"
#include "../../vnovel.h"
#include "../container_base/container.h"
#include "../component_base/component.h"
#include "vnobject.h"


/* Control functions */

// Add parameters to a VNObject
unsigned int setObjParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);


#endif