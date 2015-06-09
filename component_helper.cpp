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
#include "keywords.h"
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"



/* Control functions */

// Add parameters to a Component
unsigned int setCompParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	Container* cont = vn->getContAt(vn->getCurr());
	Component* c;
	
	// Is there a valid Container?
	if(cont != NULL)
	{
		c = cont->getContentAt(cont->getCurrent());
		
		if(c != NULL)
		{
			// Set parameters
			return c->setData(start, params);
		}
		else
		{
			std::wcout << L"No Component to store parameters in";
		}
	}
	else
	{
		std::wcout << L"No Container; therefore, no place to store Component parameters";
	}
	
	return params.size() - 1;
};