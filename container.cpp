/* Imports */

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>

// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"

// Class header
#include "container.h"

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Add parameters to a Container
unsigned int setContParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	Container* c = vn->getActiveCont();
	
	if(c != NULL)
	{
		// Set parameters
		return c->setData(start, params);
	}
	else // No Container to store parameters inside
	{
		wcout << L"No Container to set parameters for";
		return params.size() - 1;
	}
};

// Ignores comments
unsigned int ignoreComments(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	return params.size() - 1;
};