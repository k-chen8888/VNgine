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
	Component* c;
	
	// Is there a valid Container?
	if(cont != NULL)
	{
		c = cont->getActiveComp();
		
		if(c != NULL)
		{
			// Set parameters
			return c->setData(start, params);
		}
		else
		{
			wcout << L"No Component to store parameters in";
		}
	}
	else
	{
		wcout << L"No Container; therefore, no place to store Component parameters";
	}
	
	return params.size() - 1;
};