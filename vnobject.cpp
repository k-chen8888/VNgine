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
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


/* Control functions */

// Add parameters to a VNObject
unsigned int setObjParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	Container* cont = vn->getActiveCont();
	Component* comp;
	
	// Is there a valid Container?
	if(cont != NULL)
	{
		comp = cont->getActiveComp();
		
		if(comp != NULL)
		{
			if(comp->getNumObj() > 0)
			{
				VNObject* v = comp->getObjAt(comp->getNumObj() - 1);
				
				// Set parameters
				return v->setData(start, params);
			}
			else
			{
				std::wcout << L"No VNObjects to set parameters for";
			}
		}
		else
		{
			std::wcout << L"No Component; therefore, no VNObject to store parameters in";
		}
	}
	else
	{
		std::wcout << L"No Container; therefore, no VNObject to store parameters in";
	}
	
	return params.size() - 1;
};