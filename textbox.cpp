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
#include "textbox.h"

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	unsigned int out = start;
	TextBox* t;
	Component* c;
	Container* last_cont = vn->getActiveCont();
	Component* last_comp;
	
	// Create a TextBox
	if(last_cont == NULL)
	{
		wcout << L"Nowhere to store this component";
		return start;
	}
	else
	{
		// Check if previous TextBox was closed out properly
		if(last_cont->getCurrent() < last_cont->getNumComp())
		{
			// Clean up the mess
			// Don't save the output; still working on params
			endTextBox(vn, start, params);
		}
		last_comp = last_cont->getContAt(last_cont->getCurrent() - 1);
		
		t = new TextBox(last_cont->getCurrent());
		c = t;
		
		// Set ending of last_comp to reference this TextBox
		if(last_comp != NULL)
		{
			last_comp->setNext(0, c);
		}
		
		// Set any other parameters if they exist
		out = c->setData(out, params);
		
		// Add TextBox to Container and end
		last_cont->addComp(c);
		return out;
	}
};

// End a TextBox
unsigned int endTextBox(VNovel* vn, unsigned int start, vector<pair<int, wstring>> params)
{
	Container* cont = vn->getActiveCont();
	Component* c;
	
	if(cont != NULL)
	{
		c = cont->getActiveComp();
		
		// End this component for good
		if(c != NULL )
		{
			// Assume that the next component is at the end, and set again if the situation changes
			VNObject* v = c->getObjAt(c->getCurrent() - 1);
			v->setEnd( cont->getCurrent() );
			
			// Implicitly set a pause at the end of a TextBox
			pair<int, wstring> p[] = {make_pair(OBJ_PARAM, L"p")};
			vector<pair<int, wstring>> pause(p, p + sizeof(p)/sizeof(pair<int, wstring>));
			vnobjects[L"obj_param"](vn, 0, pause);
			
			// Component is no longer active for editing
			cont->deactivateContent();
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
	return params.size() - 1;
};