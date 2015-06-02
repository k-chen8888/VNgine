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

// Freeze a component
Component2* freezeComp(Frame* f, Component2* comp, vector<wstring> params)
{
	if(comp != NULL) // Found a component to freeze
	{
		f->freeze(comp);
	}
	
	return NULL;
};

// Unfreeze a component
Component2* unFreezeComp(Frame* f, Component2* comp, vector<wstring> params)
{
	int err = f->unfreeze();
	
	return NULL;
};

// End a component
Component2* endComp(Frame* f, Component2* comp, vector<wstring> params)
{
	if(comp != NULL) // Found a component to end
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
VNovel::VNovel(wstring src, vector<wchar_t> d)
{
	this->source = src;
	this->curr_frame = -1;
	
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
	this->addKW("Frame", NULL);             // Build a new frame
	this->addKW("freeze", &freezeComp);     // Freeze current active component inside frame
	this->addKW("unfreeze", &unFreezeComp); // Freeze current active component inside frame
	this->addKW("endcomp", &endComp);       // End a component
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
		
		// Index of previous delimiter
		unsigned int prev = 0;
		
		// Index of current delimiter
		unsigned int curr = 0;
		
		// Map lookup keywords
		vector<wstring> kwstack;
		
		// Locations of escape characters
		vector<unsigned int> esc;
		
		// Pull out and process the tokens
		while(script.getPos() < line.length() && script.getErr() > -1)
		{
			string token = this->strip( script.readNext() );
			
			if(script.getErr() > -1)           // Continue if there are no errors
			{
				if(curr != script.lastDelim()) // Update previous value only if the current opening delimiter has changed
					prev = script.lastRemoved();
				curr = script.lastDelim();
				
				// Validity check
				if(script.getErr() < -1)
				{
					this->err.push_back( L"[Error:L" + to_wstring(lcount) +"] Improper formatting\n" );
				}
				else
				{
					// Work on actual tokens that are not comments	
					if(token.length() > 0 && token != "comment")
					{
						// Add token to the stack
						token = this->escape(token, esc);
						kwstack.push_back(token);
						
						if(stack.getPos() == line.length() ||
						   this->delim[curr] == '{' ||
						   this->delim[curr] == '['
						  )                      // Flush the stack on reaching the end of a line or beginning of a separate L2 delimiter
						{
							if(prev > 0)         // Closing delimiter was found
							{
								int err_code = this->buildNext(prev, kwstack);
								
								if(err_code < 0) // Report any errors
									this->err.push_back( L"[Error:L" + to_wstring(lcount) +"] Invalid token \"" + token + "\" threw code " + to_wstring(err_code) + "\n" );
							}
						}
					}
					else
					{
						if(script.isEsc())       // Mark escape characters
							esc.push_back(script.getPos());
					}
				}
			}
			else
			{
				this->err.push_back("[Error:L" + to_wstring(lcount) + "] Parser exception");
			}
		}
		
		lcount += 1;
	}
	
	ifile.close();
	
	return 0;
};

// Builds the next VN component or update an existing one
int VNovel::buildNext(int d, vector<wstring> params)
{
	// Special case for making a frame
	if(params[0] == L"Frame")
	{
		Frame temp = new Frame(0);
		this->curr_frame += 1;
		
		this->f[this->curr_frame].push_back(temp);
		
		return 0;
	}
	else
	{
		if(this->curr_frame == -1) // No Frame to store this component inside
		{
			this->err.push_back(NO_FRAME_ERR);
			return NO_FRAME;
		}
	}
	
	// Frame parameters
	if(this->delim[d] == '~')
	{
		if(this->curr_frame == -1) // No Frame found
		{
			this->err.push_back(NO_FRAME_ERR);
			return NO_FRAME;
		}
		
		// Add to Frame
		this->f[this->curr_frame].setData(params);
	}
	
	// General case
	if( this->f[this->curr_frame]->getActiveComp() != NULL || keywords.count(params[0]) == 1 )
	{
		Component2* data = keywords[params[0]]( this->f[this->curr_frame], this->f[this->curr_frame].getActiveComp(), params );
	}
	else // Nowhere to store this new data
	{
		this->err.push_back(NO_COMP_ERR);
		return NO_COMP;
	}
	
	return 0;
};

// Set ignored characters
void VNovel::setIgnore(std::vector<wchar_t> i)
{
	this->ignore = i;
};

// Strips ignored characters
wstring VNovel::strip(wstring token)
{
	wstring out;
	
	// Starting ignored characters
	bool change = true;
	bool ig = false;
	int i = 0;
	while(change && i < token.length())
	{
		ig = false;
		for(int j = 0; j < this->ignore.size(); j++)
		{
			if(token.at(i) == this->ignore[j])
			{
				ig = true;
				j = this->ignore.size();
			}
		}
		
		if(ig)
			i += 1;
		else
			change = false;
	}
	
	// Strip out the starting ignored characters
	if(i < token.length())
	{
		if(i == 0)
			out = token;
		else
			out = token.substr(i, token.length() - i);
	}
	else // Entire token to be ignored
	{
		return out;
	}
	
	// Ending ignored characters
	change = true;
	int i = out.length() - 1;
	while(change && i > -1)
	{
		ig = false;
		for(int j = 0; j < this->ignore.size(); j++)
		{
			if(token.at(i) == this->ignore[j])
			{
				ig = true;
				j = this->ignore.size();
			}
		}
		
		if(ig)
			i -= 1;
		else
			change = false;
	}
	
	// Strip out the ending ignored characters
	if(i > 0)
	{
		if(i == out.length() - 1)
			return out;
		else
			out = out.substr(0, i + 1);
	}
	
	return out;
};

// Removes escape characters
wstring VNovel::escape(wstring token, vector<unsigned int> e)
{
	wstring out;
	int start = 0;
	
	if(e.size() > 0)
	{
		int j = 0;
		for(int i = 0; i < token.length(); i++)
		{
			if(i == e[j])
			{
				wstring temp = token.substr(start, e[j] - start);
				out = out + temp;
				
				// Update
				j += 1;
				if(j >= e.size())
					i = token.length();
			}
		}
	}
	else
	{
		return token;
	}
	
	return out;
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