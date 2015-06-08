#ifndef CONTAINER_HELPER_H
#define CONTAINER_HELPER_H


// Base files
#include "keywords.h"
#include "vnovel.h"


/* Control functions */

// Add parameters to a Container
unsigned int setContParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);

// Ignores comments
unsigned int ignoreComments(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);


#endif