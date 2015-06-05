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

// Add parameters to a VNObject
unsigned int setObjParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	Container* cont = vn->getActiveCont();
	Component* c;
	
	// Is there a valid Container?
	if(cont != NULL)
	{
		c = cont->getActiveComp();
		
		if(c != NULL)
		{
			if(c->getNumObj() > 0)
			{
				VNObject* v = c->getObjAt(c->getNumObj() - 1);
				
				// Set parameters
				return v->setData(start, params);
			}
			else
			{
				wcout << L"No VNObjects to set parameters for";
			}
		}
		else
		{
			wcout << L"No Component; therefore, no VNObject to store parameters in";
		}
	}
	else
	{
		wcout << L"No Container; therefore, no VNObject to store parameters in";
	}
	
	return params.size() - 1;
};