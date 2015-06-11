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

// Class header
#include "textbox.h"

// Content VNObject headers
#include "text.h"


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	unsigned int out = start;
	Container* cont = vn->getContAt(vn->getCurr());
	TextBox* tb;
	Component* comp;
	
	// Check if there is a place to store new TextBox
	if(cont == NULL)
	{
		std::wcout << L"Nowhere to store this component" << std::endl;
		return start;
	}
	
	// Check if another Component is active
	if(cont->getCurrent() < cont->getNumContents())
	{
		std::wcout << L"Cannot add new content while something is still active" << std::endl;
		return start;
	}
	
	// Make and add the new TextBox
	tb = new TextBox(cont->getCurrent());
	comp = tb;
	
	// Add any other parameters
	out = comp->setData(out, params);

	// End
	cont->addActive(comp);
	return out;
};

// End a TextBox
unsigned int endTextBox(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params)
{
	Container* cont = vn->getContAt(vn->getCurr());
	Component* comp;
	Text* txt;
	VNObject* v;
	
	bool unfrz = false;
	
	if(cont != NULL)
	{
		comp = cont->getContentAt(cont->getCurrent());
		
		// End this component for good
		if(comp != NULL)
		{
			// Use object if there is one
			if(comp->getNumObj() > 0)
			{
				v = comp->getObjAt(comp->getCurrent() - 1);
			}
			else
			{
				// Implicitly create an object and use it
				txt = new Text(L"");
				v = txt;
				comp->addObj(v);
			}
			
			// Check if a component will be unfrozen afterwards
			if(params[start].first == PARAM_VAL && params[start].second.compare(UNFREEZE) == 0)
			{
				if(cont->numFrozen() > 0)
				{
					// Go to the next element to be unfrozen
					v->setFreeze( cont->getFrz(cont->numFrozen() - 1)->getID() );
					
					unfrz = true;
				}
				else
				{
					// Error; ignore keyword
					std::wcout << L"Did not expect " << UNFREEZE << L" here; ignoring keyword" << std::endl;
					
					// Go to the end of the Container's content list
					v->setEnd(cont->getNumContents());
				}
			}
			else
			{
				// Go to the end of the Container's content list
				v->setEnd(cont->getNumContents());
			}
			
			// Implicitly set a pause at the end of a TextBox
			std::vector< std::pair<int, std::wstring> > pause = params;
			pause.push_back(std::make_pair(PARAM_VAL, L"p"));
			vnobjects[L"obj_param"](vn, 0, pause);
			
			// Component is no longer active for editing
			cont->deactivateContent();
			
			// Is there something to unfreeze?
			if(unfrz)
			{
				// Unfreeze the frozen Component
				cont->unfreeze();
			}
		}
		else
		{
			std::wcout << L"No TextBox to end" << std::endl;
		}
	}
	else
	{
		std::wcout << L"No Container; therefore, no TextBox to end" << std::endl;
	}
	
	// On end, ignore the rest of params
	return params.size() - 1;
};