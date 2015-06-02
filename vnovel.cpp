/* Imports */

#include <iostream>
#include <string>
#include <vector>
#include <map>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>

// Parser (pushdown automata)
#include "pda_wstring.h"

// Class header
#include "vnovel.h"

// Base files
#include "vn_global.h"

// Sub-Component definitions
#include "component2.h"
#include "component3.h"

// VN Components
#include "frame.h"
#include "textbox.h"
#include "text.h"

// Namespaces
using namespace std;


/* Control functions */

// Add parameter to a frame
void addFrameParam(Frame* f, Component2* comp, vector<wstring> params)
{
	if(f != NULL)
	{
		f->setData(params);
	}
	
	return NULL;
}

// End a frame, closing out all frozen and active components
void endFrame(Frame* f, vector<pair<int, wstring>> params)
{
	// Clean up active component
	endComp(f, params);
	
	// Clean up frozen components
	while(f->frz.size() > 0)
	{
		unFreezeComp(f, params);
		endComp(f, params);
	}
};

// Freeze a component
void freezeComp(Frame* f, vector<pair<int, wstring>> params)
{
	// Freeze active component if there is one
	f->freeze();
	
	return NULL;
};

// Unfreeze a component
void unFreezeComp(Frame* f, Component2* comp, vector<pair<int, wstring>> params)
{
	// Unfreeze component if there is a frozen component
	f->unfreeze();
	
	return NULL;
};

// End a component
void endComp(Frame* f, Component2* comp, vector<pair<int, wstring>> params)
{
	Component2* comp = f->getActiveComp();
	if(comp != NULL)
	{
		comp->setEnd(f->getNumComp());
	}
	
	return NULL;
};

/************************************************
 * Visual Novel object
 * 
 * Contents:
 * 	Script file
 * 	Visual novel components
 ************************************************/

/* Constructor */
VNovel::VNovel(wstring src)
{
	this->source = src;
	this->curr_frame = -1;
	this->current = 1;
	
	//Set unicode input/output
	int e = setUnicode(true, true);
	if(e == SET_IN)
	{
		this->err.push_back(SET_IN_ERR);
	}
	if(e == SET_OUT)
	{
		this->err.push_back(SET_OUT_ERR);
	}
	
	// Add keywords
	this->addKW(L"f_param", &addFrameParam); // Add frame parameters
	this->addKW(L"endframe");
	this->addKW(L"freeze", &freezeComp);     // Freeze current active component inside frame
	this->addKW(L"unfreeze", &unFreezeComp); // Freeze current active component inside frame
	this->addKW(L"endcomp", &endComp);       // End a component
};

/*******************************************
 * Functions
 *******************************************/
 
/* Build a Visual Novel */

// Add keywords to the map
int addKW(std::wstring kw, buildComp b)
{
	if(checkKeyword(kw) == 0)
	{
		addToKeywords(kw, b);
	}
	else
	{
		err.push_back(DUP_KEY_ERR_1 + kw + DUP_KEY_ERR_2);
		return -1;
	}
	
	return 0;
};

// Builds VN from script file
int VNovel::buildVN()
{
	wifstream ifile;
	try
	{
		ifile.open(this->source);
	}
	catch(int e)
	{
		return e; // Failure
	}
	
	wstring line;
	unsigned int lcount;
	while( getline(ifile, line) )
	{
		// PDA
		PDA<wstring> script (line, this->delim);
		
		// Map lookup keywords
		vector<pair<int, wstring>> kwlist;
		
		// Start position of current token
		unsigned int start;
		
		// Locations of escape characters
		vector<unsigned int> esc;
		
		// Pull out and process the tokens
		while(script.getPos() < line.length() && script.getErr() > -1)
		{
			string token = script.readNext();
			int d = script.lastDelim();
			
			if(script.getErr() < -1)
			{
				this->err.push_back( L"[Error:L" + to_wstring(lcount) + L"] Improper formatting\n" );
			}
			else // Found a token?
			{
				if(token.length() > 0)
				{
					// Check if tokens can be added when a new opening delimiter is pushed
					if(d > 0)
					{
						// Check if the token is meaningful (enclosed in delimiters)
						if(script.lastRemoved() > 0)
						{
							// Create the final token and add it to the list
							wstring finaltoken = escape(token, esc);
							if(script.lastRemoved() != TXT_TOKEN)
								finaltoken = strip(finaltoken);
							
							pair<int, wstring> t( script.lastRemoved(), finaltoken );
							kwlist.push_back(t);
							start = script.getPos();
						}
					}
					
					// Flush stack when the sequence ends
					bool outofkw = script.stackDepth() == 0 && kwlist.size() > 0;
					bool endofline = script.getPos() >= line.length();
					if( outofkw || endofline )
					{
						this->buildNext(kwlist);
						kwlist.clear();
					}
				}
				else
				{
					if(script.isEsc()) // Mark escape characters
						esc.push_back(script.getPos() - tstart);
				}
			}
		}
		
		lcount += 1;
	}
	
	ifile.close();
	
	return 0;
};

// Builds the next VN component or update an existing one
int VNovel::buildNext(vector<pair<int, wstring>> params)
{
	pair<int, wstring> data = params[0];
	params.erase(params.begin() + 0);
	
	// Switch on the delimiter type
	switch( data.first )
	{
		// Frame or L2 Component
		case COMP_OPEN:
			if(data.second == L"Frame")
			{
				this->curr_frame += 1;
				Frame temp = new Frame(this->curr_frame);
				
				this->f[this->curr_frame].push_back(temp);
				
				return 0;
			}
			else
			{
				keywords[ params[i].second ](this->f[this->curr_frame], params);
			}
			
			break;
		
		// Frame parameter
		case F_PARAM:
			if(this->curr_frame == -1) // No Frame to store this component inside
			{
				this->err.push_back(NO_FRAME_ERR);
				return NO_FRAME;
			}
			else
			{
				keywords[ L"f_param" ](this->f[this->curr_frame], params);
			}
			
			break;
		
		// L2_PARAM, L3_COMP, L3_PARAM, PARAM_VAL and TXT_TOKEN handled in L2/L3 functions
		
		// Everything else
		default:
			break;
	}
	
	return 0;
};

/* Playback */

// Plays the visual novel
int playNovel(bool gui)
{
	while(i < this->f.size())
	{
		this->current = this->f[this->current]->play(gui);
	}
	
	return 0;
};

/* Reporting */

// Display all error messages
int reportErrors()
{
	for(int i = 0; i < this->err.length(); i++)
		wcout << this->err[i] << "\n";
	
	return 0;
};

/* Destructor */
VNovel::~VNovel()
{
	for(int i = 0; i < this->f.size(); i++)
		delete this->f[i];
};