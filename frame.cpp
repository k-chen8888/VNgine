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
	Container* last_frame;
	
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
	
	// Check if previous frame was closed out properly
	if(vn->getCurr() < vn->numCont())
	{
		// Clean up the mess
		// Don't save the output; still working on params
		endFrame(vn, start, params);
	}
	last_frame = vn->getContAt(vn->getCurr() - 1);
	
	// Set ending of last_frame to reference this frame
	if(last_frame != NULL)
	{
		last_frame->setNext(0, c);
	}
	
	// Set any other parameters if they exist
	for(int i = out; i < params.size(); i++)
	{
		switch(params[i].first)
		{
			// Frame parameter
			case F_PARAM:
				i = c->setData(start, params);
				break;
			
			// Value for frame parameter, but cannot be handled
			case PARAM_VAL:
				wcout << L"Cannot handle frame parameter value '" << params[i].second << L"' without knowing what it is\n";
				break;
			
			// Insignificant symbol here
			default:
				out = i;
				break;
		}
	}
	
	// Add Container to list and end
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

// End a Frame, closing out all frozen and active Components
unsigned int endFrame(VNovel* vn, unsigned int start, vector<pair<int, wstring>> params)
{
	Container* curr_frame = vn->getActiveCont();
	
	// End the last existing frame for good
	if(curr_frame != NULL)
	{
		// End of frame is the current position (which should be the number of components at the very end)
		curr_frame->setNext(curr_frame->getCurrent(), NULL);
		vn->deactivateCont();
	}
	
	// On end, ignore the rest of params
	return params.size() - 1;
};


// Add keywords to map
addToKeywords(L"frame", &makeFrame);
addToKeywords(L"endframe", &endFrame);