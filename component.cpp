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

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Add parameters to a Component
unsigned int setCompParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	Container* cont = vn->getActiveCont();
	Component* c = cont->getActiveComp();
	
	if(c == NULL) // No Component to store parameters inside
	{
		return params.size();
	}
	
	// Set parameters
	for(int i = start; i < params.size(); i++)
	{
		switch(params[i].first)
		{
			// Component parameter
			case COMP_PARAM:
				i = c->setData(start, params);
				break;
			
			// Value for Component parameter, but cannot be handled
			case PARAM_VAL:
				wcout << L"Cannot handle Component parameter value '" << params[i].second << L"' without knowing where it belongs\n";
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
addToComponents(L"comp_param", &setCompParams);