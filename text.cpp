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

// Class header
#include "text.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a Text object
unsigned int makeText(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	unsigned int out = start;
	Container* cont = vn->getContAt(vn->getCurr());
	Component* comp;
	Text* text;
	VNObject* v;
	
	bool frz = false;
	
	// Check if there is a Container to store this VNObject inside
	if(cont == NULL)
	{
		wcout << L"No active Container to store this object inside";
		return start;
	}
	
	// Check if there is a Component to store this VNObject in
	comp = cont->getContentAt(cont->getCurrent());
	if(comp == NULL)
	{
		wcout << L"No active Component to store this VNObject inside";
		return start;
	}
	
	// Create the VNObject
	text = new Text(L"");
	v = text;
	
	// Set parameters if there are any
	while(params[i].first > OBJ_OPEN)
	{
		out = v->setData(out, params);
		
		// Handle keyword 'freeze'
		if(params[i].first == PARAM_VAL && params[i].second.compare(FREEZE) == 0)
			frz = true;
	}
	
	// Add this VNObject to the component
	comp->addObj(v);
	
	// Freeze if necessary
	if(frz)
	{
		v->setFreeze(cont->getNumContents());
		cont->freeze();
	}
	
	// End
	return out;
};

// End a Text object... there's no function for this because Text objects are implicitly ended