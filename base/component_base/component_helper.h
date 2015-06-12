#ifndef COMPONENT_HELPER_H
#define COMPONENT_HELPER_H


// Base files
#include "../keywords.h"
#include "../../vnovel.h"
#include "../container_base/container.h"
#include "component.h"


/* Control functions */

// Add parameters to a Component
unsigned int setCompParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);


#endif