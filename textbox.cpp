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
#include "textbox.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, vector< pair<int, wstring> > params)
{
	unsigned int out = start;
	Container* cont = vn->getActiveCont();
	TextBox* t;
	Component* c;
	
	// Check if there is a place to store new TextBox
	if(cont == NULL)
	{
		wcout << L"Nowhere to store this component";
		return start;
	}
	
	// Check if another Component is active
	if(cont->getCurrent() < cont->getNumContents())
	{
		wcout << L"Cannot add new content while something is still active";
		return start;
	}
	
	// Make and add the new TextBox
	t = new TextBox(cont->getCurrent());
	c = t;
	
	// Set ending of previous Container to reference this Frame
	c->setNext(vn->getPrev(), vn->getCurr());
	
	// Add any other parameters
	out = c->setData(out, params);

	// End
	cont->addObj(c);
	return out;
};

// End a TextBox
unsigned int endTextBox(VNovel* vn, unsigned int start, vector< pair<int, wstring> > params)
{
	Container* cont = vn->getActiveCont();
	Component* c;
	
	if(cont != NULL)
	{
		c = cont->getActiveComp();
		
		// End this component for good
		if(c != NULL )
		{
			// Set the ending
			c->setNext(c->getCurrent(), -1);
			
			// Implicitly set a pause at the end of a TextBox
			if(c->getNumContents() > 0)
			{
				pair<int, wstring> p[] = {make_pair(OBJ_PARAM, L"p")};
				vector<pair<int, wstring>> pause(p, p + sizeof(p)/sizeof(pair<int, wstring>));
				vnobjects[L"obj_param"](vn, 0, pause);
			}
			
			// Component is no longer active for editing
			c->deactivateContent();
		}
		else
		{
			wcout << L"No TextBox to end";
		}
	}
	else
	{
		wcout << L"No Container; therefore, no TextBox to end";
	}
	
	// On end, ignore the rest of params
	// Also add on this Component's index
	return c->getID() + params.size() - 1;
};