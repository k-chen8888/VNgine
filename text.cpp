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
#include "text.h"

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a Text object
unsigned int makeText(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	unsigned int out = start;
	Text* t;
	Component* c;
	Container* last_cont = vn->getActiveCont();
	
	if(last_cont == NULL)
	{
		wcout << L"Nowhere to add this object";
		return start;
	}
	else
	{
		c = last_cont
	}
};

// End a Text object... there's no function for this because Text objects are implicitly ended