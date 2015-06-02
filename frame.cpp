/* Imports */

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>

// Class header
#include "vnovel.h"

// Parser (pushdown automata)
#include "pda_wstring.h"

// Namespaces
using namespace std;


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
	
	// Load defaults
	this->bg.push_back(DEFAULT_BG);
	this->bgm.push_back(DEFAULT_BGM);
	this->sfx.push_back(DEFAULT_SFX);
	
	//Set unicode input/output
	setUnicode(true, true);
};

/*******************************************
 * Functions
 *******************************************/

/* Build */

// Add keywords to the map
int addKW(std::wstring kw, buildComp b)
{
	if(checkKeyword(kw) == 0)
	{
		addToKeywords(kw, b);
	}
	else
	{
		this->err.push_back(DUP_KEY_ERR_1 << kw << DUP_KEY_ERR_2);
		return -1;
	}
	
	return 0;
};

// Set Frame name
int setName(std::wstring n)
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
				this->bg.push_back(bgfile);
				return 0;
			}
		}
		
		// Unsupported type error
		this->err.push_back(BAD_BG_ERR);
		return BAD_BG;
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_BG_ERR);
		return EMPTY_BG;
	}
};

// Add a bgm file
int Frame::addBGM(std::wstring bgmfile)
{
	wstring types[] = {L".mp3", L".wav"};
	vector<wstring> supported(types, types + 5);
	
	if(bgmfile.length() > 0)
	{
		for(int i = 0; i < supported.size(); i++){
			if( bgmfile.compare( bgmfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
			{
				this->bgm.push_back(bgmfile)
				return 0;
			}
		}
		
		// Unsupported type error
		this->err.push_back(BAD_BGM_ERR);
		return BAD_BGM;
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_BGM_ERR);
		return EMPTY_BGM;
	}
};

// Add a sound effect file
int Frame::addSFX(std::wstring sfxfile)
{
	wstring types[] = {L".mp3", L".wav"};
	vector<wstring> supported(types, types + 5);
	
	if(sfxfile.length() > 0)
	{
		for(int i = 0; i < supported.size(); i++){
			if( sfxfile.compare( sfxfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
			{
				this->sfx.push_back(sfxfile);
				return 0;
			}
		}
		
		// Unsupported type error
		this->err.push_back(BAD_SFX_ERR);
		return BAD_SFX;
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_SFX_ERR);
		return EMPTY_SFX;
	}
};

// Add a sprite image file
int Frame::addSprite(std::wstring spritefile)
{
	wstring supported(L".png");
	
	if(string.length() > 0)
	{
		if( spritefile.compare( spritefile.length() - 4, 4, supported ) == 0 ) // Found a supported type
		{
			this->sprites.push_back(spritefile);
			return 0;
		}
		else
		{
			this->err.push_back(BAD_SPRITE_ERR);
			return BAD_SPRITE; // Unsupported type error
		}
	}
	else // Empty string error
	{
		this->err.push_back(EMPTY_SPRITE_ERR);
		return EMPTY_SPRITE;
	}
};

// Add a component
int Frame::addComponent(std::pair<void*, compPlayback> c)
{
	this->comp.push_back(c);
	return 0;
};

/* Playback */

// Play through components
// Output the index of the next frame to jump to
unsigned int Frame::play(bool gui)
{
	unsigned int out = 0;
	
	if(gui) // GUI display
	{
		
	}
	else // Command line display
	{
		// Playback must return the index of the next component to play
		int next = -1; // -1 means keep playing this component
		
		while(next == -1)
		{
			next = this->comp[this->current].second(this->comp[this->current].first, false, true);
		}
		
		// Save the index of the next component
		this->current = next;
	}
	
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

// Retrieve a component at some index
void* getComp(unsigned int index)
{
	return this->comp[index];
};

// Get the number of components
unsigned int getNumComp();
{
	return this->comp.size();
};

// Get the index of the current component
unsigned int getCurrent()
{
	return this->current;
};

/* Destructor */
Frame::~Frame()
{
	for(int i = 0; i < this->comp.size(); i++)
		delete this->comp[i];
};