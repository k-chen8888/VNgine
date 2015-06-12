/* Imports */

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>

// Class header
#include "component_helper.h"


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
			std::wcout << L"No Component to store parameters in" << std::endl;
		}
	}
	else
	{
		std::wcout << L"No Container; therefore, no place to store Component parameters" << std::endl;
	}
	
	return params.size() - 1;
};