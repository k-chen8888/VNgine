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
#include "frame.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a Frame
unsigned int makeFrame(VNovel* vn, unsigned int start, vector< pair<int, wstring> > params)
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

// End a Frame, closing out all frozen and active content
unsigned int endFrame(VNovel* vn, unsigned int start, vector< pair<int, wstring> > params)
{
	Container* curr_frame;
	
	// End the last existing Frame for good
	if(vn->getContAt(vn->getCurr()) != NULL)
	{
		// Cannot handle things that aren't Frames, so return -1 + params.size() - 1
		if(vn->getContAt(vn->getCurr())->getType().compare(L"Frame") == 0)
		{
			curr_frame = vn->getContAt(vn->getCurr());
		}
		else
		{
			wcout << L"Open Container is not a Frame";
			return params.size() - 2;
		}
		
		// Cannot close Frame if there is still something frozen or active, so return -1 + params.size() - 1
		if(curr_frame->numFrozen() > 0)
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


// Class constructor
Frame::Frame(std::wstring n, int i)
{
	//Set unicode input/output
	setUnicode(true, true);
	
	// Set identifiers
	this->type = L"Frame";
	this->name = n;
	this->index = i;
	
	// Add frame object holders to map
	vector<wstring> vbg;
	vector<wstring> vbgm;
	vector<wstring> vsfx;
	vector<wstring> vsprites;
	
	std::wstring types_bg[] = {L".jpg", L".png", L".gif"};
	std::vector<std::wstring> supported_bg(types_bg, types_bg + 3);
	
	std::wstring types_bgm[] = {L".mp3", L".wav"};
	std::vector<std::wstring> supported_bgm(types_bgm, types_bgm + 2);
	
	std::wstring types_sfx[] = {L".mp3", L".wav"};
	std::vector<std::wstring> supported_sfx(types_sfx, types_sfx + 2);
	
	std::wstring types_sprites[] = {L".png"};
	std::vector<std::wstring> supported_sprites(types_sprites, types_sprites + 1);
	
	this->mod.insert( make_pair( L"bg", make_pair(supported_bg, vbg ) ) );                // Background image files
	this->mod.insert( make_pair( L"bgm", make_pair( supported_bgm, vbgm ) ) );            // Background music files
	this->mod.insert( make_pair( L"sfx", make_pair( supported_sfx, vsfx ) ) );            // Sound effects files
	this->mod.insert( make_pair( L"sprite", make_pair( supported_sprites, vsprites ) ) ); // Sprite image files
	
	// Default editing/traversal parameters
	this->previous = -1;
	this->current = 0;
	this->ending = 0;
};