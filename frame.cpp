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
	Container* cont;
	
	// Check if previous Container was closed out properly
	if(vn->getCurr() < vn->numCont())
	{
		wcout << L"Open Container preventing creation of new Frame";
		return start;
	}
	
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
	cont = f;
	
	// Set ending of previous Container to reference this Frame
	cont->setNext(vn->getPrev(), vn->getCurr());
	
	// Set any other parameters if they exist
	out = cont->setData(out, params);
	
	// Add Container to VNovel and end
	vn->addCont(cont);
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
	
	// End the last existing Frame for good
	if(curr_frame != NULL)
	{
		// Cannot handle things that aren't Frames, so return -1 + params.size() - 1
		if(curr_frame->getType().compare(L"Frame") != 0)
		{
			wcout << L"Open Container is not a Frame";
			return params.size() - 2;
		}
		
		// Cannot close Frame if there is still something frozen or active, so return -1 + params.size() - 1
		if(curr_frame->lastFrozen() != NULL)
		{
			wcout << L"Cannot close Frame if there is still something frozen";
			return params.size() - 2;
		}
		if(curr_frame->getCurrent() < curr_frame->getNumContents())
		{
			wcout << L"Cannot close Frame if there is still something active";
			return params.size() -2;
		}
		
		// End of Frame is the current position
		curr_frame->setNext(curr_frame->getCurrent(), -1);
		vn->deactivateCont();
	}
	
	// On end, ignore the rest of params
	return params.size() - 1;
};