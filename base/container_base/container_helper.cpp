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
#include "container_helper.h"


/* Control functions */

// Add parameters to a Container
unsigned int setContParams(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	Container* c = vn->getContAt(vn->getCurr());
	
	if(c != NULL)
	{
		// Set parameters
		return c->setData(start, params);
	}
	else // No Container to store parameters inside
	{
		std::wcout << L"No Container to set parameters for" << std::endl;
		return params.size() - 1;
	}
};

// Ignores comments
unsigned int ignoreComments(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	return params.size() - 1;
};