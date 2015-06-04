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
#include "frame.h"

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Add parameters to a Container
unsigned int setContParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	Container* c = vn->getActiveCont();
	
	if(c == NULL) // No Container to store parameters inside
	{
		return params.size();
	}
	
	// Set parameters
	for(int i = start; i < params.size(); i++)
	{
		switch(params[i].first)
		{
			// Container parameter
			case CONT_PARAM:
				i = c->setData(start, params);
				break;
			
			// Value for Container parameter, but cannot be handled
			case PARAM_VAL:
				wcout << L"Cannot handle Container parameter value '" << params[i].second << L"' without knowing where it belongs\n";
				break;
			
			// Insignificant symbol here
			default:
				return i;
		}
	}
	
	// Finished processing all parameters
	return params.size() - 1;
};


// Add helper keywords
addToContainers(L"cont_param", &setContParams);