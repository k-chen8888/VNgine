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

// L2 Component definition
#include "component2.h"

// Class header
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */
		
// Set Frame name
void setFrameName(Frame* f, std::wstring n)
{
	int err = f->setName(n);
	if(err < 0)
	{
		wcout << n << " threw error code " << err;
	}
};

// Add a background image
void addBGToFrame(Frame* f, std::wstring bgfile)
{
	int err = f->addBG(bgfile);
	if(err < 0)
	{
		wcout << bgfile << " threw error code " << err;
	}
};

// Add a bgm file
void addBGMToFrame(Frame* f, std::wstring bgmfile)
{
	int err = f->addBGM(bgmfile);
	if(err < 0)
	{
		wcout << bgmfile << " threw error code " << err;
	}
};

// Add a sound effect file
void addSFXToFrame(Frame* f, std::wstring sfxfile)
{
	int err = f->addSFX(sfxfile);
	if(err < 0)
	{
		wcout << sfxfile << " threw error code " << err;
	}
};

// Add a sprite image file
void addSpriteToFrame(Frame* f, std::wstring spritefile)
{
	int err = f->addSprite(spritefile);
	if(err < 0)
	{
		wcout << spritefile << " threw error code " << err;
	}
};


/************************************************
 * Visual Novel Frame
 * 
 * Contents (all vectors):
 * 	Type of component (found in an external map)
 * 	Identifier for component
 * 	Display content
 ************************************************/

/* Constructor */
Frame::Frame(unsigned int loc)
{
	this->index = loc;
	this->current = 0;
	
	
	// Add frame object holders to map
	vector<wstring> bg;      // Background image files
	this->obj[L"bg"] = bg;
	
	vector<wstring> bgm;     // Background music files
	this->obj[L"bgm"] = bgm;
	
	vector<wstring> sfx;     // Sound effects files
	this->obj[L"sfx"] = sfx;
	
	vector<wstring> sprites; // Sprite image files
	this->obj[L"sprite"] = sprites;
	
	
	// Load defaults
	this->obj[L"bg"].push_back(DEFAULT_BG);
	this->obj[L"bgm"].push_back(DEFAULT_BGM);
	this->obj[L"sfx"].push_back(DEFAULT_SFX);
	
	
	// Set control functions
	this->mod[L"name"] = &setFrameName;
	this->mod[L"bg"] = &addBGToFrame;
	this->mod[L"bgm"] = &addBGMToFrame;
	this->mod[L"sfx"] = &addSFXToFrame;
	this->mod[L"sprite"] = &addSpriteToFrame;
	
	
	//Set unicode input/output
	setUnicode(true, true);
};

/*******************************************
 * Functions
 *******************************************/

/* Build */

// Add keywords to the map
int Frame::addKW(wstring kw, buildComp b)
{
	if(checkKeyword(kw) == 0)
	{
		addToKeywords(kw, b);
	}
	else
	{
		this->err.push_back(DUP_KEY_ERR_1 + kw + DUP_KEY_ERR_2);
		return -1;
	}
	
	return 0;
};

// Add non-component data to the frame
void Frame::setData(vector<wstring> params)
{
	wstring p = L"";
	for(int i = 0; i < params.size(); i++)
	{
		if(params[i].first == F_PARAM)
		{
			if(this->mod.count(params[i].second) == 1)
			{
				p = params[i].second;
			}
			else
			{
				wcout << L"No such frame parameter";
			}
		}
		else if(params[i].first == PARAM_VAL)
		{
			if(p.length() > 0)
			{
				this->mod[p](this, params[i].second);
			}
			else
			{
				wcout << L"This parameter value does not belong anywhere in this frame";
			}
		}
		else
		{
			wcout << L"Incorrect delimiter for frame parameters";
		}
	}
};

// Set Frame name
int Frame::setName(wstring n)
{
	if(n.length() > 0)
		this->name = n;
	else
	{
		this->err.push_back(EMPTY_NAME_ERR);
		return EMPTY_NAME;
	}
	
	return 0;
};

// Add a background image
int Frame::addBG(wstring bgfile)
{
	wstring types[] = {L".jpg", L".png", L".gif"};
	vector<wstring> supported(types, types + 5);
	
	if(bgfile.length() > 0)
	{
		for(int i = 0; i < supported.size(); i++){
			if( bgfile.compare( bgfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
			{
				this->obj['bg'].push_back(bgfile);
				return 0;
			}
		}
		
		// Unsupported type error
		this->err.push_back(BAD_BG_ERR + L" (" + bgfile + L")");
		return BAD_BG;
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_BG_ERR);
		return EMPTY_BG;
	}
};

// Add a bgm file
int Frame::addBGM(wstring bgmfile)
{
	wstring types[] = {L".mp3", L".wav"};
	vector<wstring> supported(types, types + 5);
	
	if(bgmfile.length() > 0)
	{
		for(int i = 0; i < supported.size(); i++){
			if( bgmfile.compare( bgmfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
			{
				this->obj['bgm'].push_back(bgmfile);
				return 0;
			}
		}
		
		// Unsupported type error
		this->err.push_back(BAD_BGM_ERR + L" (" + bgmfile + L")");
		return BAD_BGM;
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_BGM_ERR);
		return EMPTY_BGM;
	}
};

// Add a sound effect file
int Frame::addSFX(wstring sfxfile)
{
	wstring types[] = {L".mp3", L".wav"};
	vector<wstring> supported(types, types + 5);
	
	if(sfxfile.length() > 0)
	{
		for(int i = 0; i < supported.size(); i++){
			if( sfxfile.compare( sfxfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
			{
				this->obj['sfx'].push_back(sfxfile);
				return 0;
			}
		}
		
		// Unsupported type error
		this->err.push_back(BAD_SFX_ERR + L" (" + sfxfile + L")");
		return BAD_SFX;
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_SFX_ERR);
		return EMPTY_SFX;
	}
};

// Add a sprite image file
int Frame::addSprite(wstring spritefile)
{
	wstring supported(L".png");
	
	if(spritefile.length() > 0)
	{
		if( spritefile.compare( spritefile.length() - 4, 4, supported ) == 0 ) // Found a supported type
		{
			this->obj['sprite'].push_back(spritefile);
			return 0;
		}
		else
		{
			this->err.push_back(BAD_SPRITE_ERR + L" (" + spritefile + L")");
			return BAD_SPRITE; // Unsupported type error
		}
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_SPRITE_ERR);
		return EMPTY_SPRITE;
	}
};

// Set a component as active for editing
// Also sets frozen component on top of the stack to progress to this one
int Frame::addActiveComponent(Component2* c)
{
	// Active component was never terminated
	if(this->active != NULL)
	{
		this->err.push_back(HAS_ACTIVE_ERR);
		return HAS_ACTIVE_ERR;
		
		this->active.setEnd(this->comp.size());
		this->addComponent();
	}
	
	// Check if freeze index needs to be set
	if(this->frz.size() > 0)
	{
		Component2* frozen = frz.back();
		frozen->setFreeze(c->getLoc());
	}
	
	this->active = c;
	return 0;
};

// Add currently active component to the list
// Component is no longer being edited
int Frame::addComponent()
{
	this->comp.push_back(this->active);
	this->active = NULL;
	return 0;
};

// Freeze a component (no longer being edited)
int Frame::freeze(Component* c)
{
	this->frz.push_back(this->active);
	this->active = NULL;
	return 0;
};

// Unfreeze the last frozen component (the one on the top of the stack) (may be edited again)
int Frame::unfreeze()
{
	// Active component was never terminated
	if(this->active != NULL)
	{
		this->err.push_back(HAS_ACTIVE_ERR);
		return HAS_ACTIVE_ERR;
		
		this->active.setEnd(this->comp.size());
		this->addComponent();
	}
	
	if(this->frz.size() > 0)
	{
		this->active = this->frz.back();
		this->frz.pop_back();
		return 0;
	}
	else
	{
		this->err.push_back(FRZ_EMPTY_ERR);
		return FRZ_EMPTY;
	}
};

/* Playback */

// Play through components
// Output the index of the next frame to jump to
int Frame::play(bool gui)
{
	int out = 0;
	
	// Playback must return the index of the next component to play
	int next = -1; // -1 means keep playing this component

	while(next < 0)
	{
		next = this->comp[this->current]->play(gui);
	}

	// Save the index of the next component
	this->current = next;
	
	// The next frame is how much the output overshoots the number of frames
	if(this->comp.size() <= this->current)
		out = this->current - this->comp.size(); // Calculate next Frame's index
	else
		out = this->index;
	
	return out;
};

// Draws the background using the images in the order specified
int Frame::drawBG(vector<int> order)
{
	return 0;
};
		
/* Reporting */

// Get name and index
pair<wstring, int> getID()
{
	pair<wstring, int> out(this->name, this->index);
	return out;
};

// Check if a component is active for editing
bool Frame::isActiveComponent(Component2* c)
{
	return this->active == c;
};

// Retrieve active component (the one being edited)
Component2* Frame::getActiveComp()
{
	return this->active;
};

// Retrieve a component at some index
void* Frame::getComp(unsigned int index)
{
	return this->comp[index];
};

// Get the number of components
unsigned int Frame::getNumComp();
{
	return this->comp.size();
};

// Get the index of the current component
unsigned int Frame::getCurrent()
{
	return this->current;
};

/* Destructor */
Frame::~Frame()
{
	for(int i = 0; i < this->comp.size(); i++)
		delete this->comp[i];
};