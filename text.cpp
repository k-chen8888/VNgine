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
	Container* cont = vn->getActiveCont();
	Component* comp;
	Text* t;
	VNObject* v;
	
	// Check if there is a Container to store this VNObject inside
	if(cont == NULL)
	{
		wcout << L"No active Container to store this object inside";
		return start;
	}
	
	// Check if there is a Component to store this VNObject in
	c = last_cont->getContentAt(last_cont->getCurrent());
	if(c == NULL)
	{
		wcout << L"No active Component to store this VNObject inside";
		return start;
	}
	
	// Create the VNObject
	t = new Text(L"");
	v = t;
	
	// Set parameters if there are any
	out = v->setData(out, params);
	
	// Add this VNObject to the component
	c->addObj(v);
	
	// End
	return out;
};

// End a Text object... there's no function for this because Text objects are implicitly ended