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
#include "frame.h"

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a Frame
unsigned int makeFrame(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	unsigned int out = start;
	Frame* f;
	Container* c;
	Container* prev_frame;
	
	// Check if previous Frame was closed out properly (prevents ID collisions from vn->getCurr())
	int prev = -1;
	if(vn->getCurr() < vn->numCont())
	{
		// Clean up the mess
		// Save the index of the previous Frame
		prev = endFrame(vn, start, params) - (params.size() - 1);
	}
	// Get the previously closed Frame back
	if (prev > -1)
		prev_frame = vn->getContAt(prev);
	
	// Create a new frame
	if(params[out].first == L'\'')
	{
		f = new Frame(params[out].second, vn->getCurr());
		out += 1;
	}
	else
	{
		f = new Frame(L"", vn->getCurr());
	}
	c = f;
	
	// Set ending of prev_frame to reference this Frame
	if(prev_frame != NULL)
	{
		prev_frame->setNext(0, c->getID().second);
	}
	
	// Set any other parameters if they exist
	out = c->setData(out, params);
	
	// Add Container to VNovel and end
	vn->addCont(c);
	return out;
};

// Add a background image
void addBGToFrame(Container* f, std::wstring bgfile)
{
	int err = f->addBG(bgfile);
	if(err < 0)
		wcout << L"'" << bgfile << L"' threw error code " << err;
};

// Add a bgm file
void addBGMToFrame(Container* f, std::wstring bgmfile)
{
	int err = f->addBGM(bgmfile);
	if(err < 0)
		wcout << L"'" << bgmfile << L"' threw error code " << err;
};

// Add a sound effect file
void addSFXToFrame(Container* f, std::wstring sfxfile)
{
	int err = f->addSFX(sfxfile);
	if(err < 0)
		wcout << L"'" << sfxfile << L"' threw error code " << err;
};

// Add a sprite image file
void addSpriteToFrame(Container* f, std::wstring spritefile)
{
	int err = f->addSprite(spritefile);
	if(err < 0)
		wcout << L"'" << spritefile << L"' threw error code " << err;
};

// End a Frame, closing out all frozen and active content
unsigned int endFrame(VNovel* vn, unsigned int start, vector<pair<int, wstring>> params)
{
	Container* curr_frame = vn->getActiveCont();
	Component* uf;
	
	// End the last existing Frame for good
	if(curr_frame != NULL)
	{
		int numfrz = curr_frame->unfreeze();
		uf = curr_frame->getActiveComp();
		
		// Check for and end any frozen Components
		while(uf != NULL)
		{
			// Add to this Component's next list
			uf->setNext(curr_frame->getCurrent(), -1);
			if(numfrz > 0)
			{
				// Set jump to the next component that will be unfrozen
				Component* f = curr_frame->lastFrozen();
				uf->setNext(curr_frame->getCurrent(), f->getID());
			}
			
			// Continue unfreezing
			numfrz = curr_frame->unfreeze();
			uf = curr_frame->getActiveComp();
		}
		
		// End of Frame is the current position
		curr_frame->setNext(curr_frame->getCurrent(), -1);
		vn->deactivateCont();
	}
	
	// On end, ignore the rest of params
	// Also add on this Frame's index
	return curr_frame->getID().second + params.size() - 1;
};