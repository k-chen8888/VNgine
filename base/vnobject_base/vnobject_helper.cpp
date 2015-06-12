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
#include "vnobject_helper.h"


/* Control functions */

// Add parameters to a VNObject
unsigned int setObjParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	Container* cont = vn->getContAt(vn->getCurr());
	Component* c;
	
	// Is there a valid Container?
	if(cont != NULL)
	{
		c = cont->getContentAt(cont->getCurrent());
		
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