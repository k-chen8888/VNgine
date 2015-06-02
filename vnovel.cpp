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

// Base files
#include "vn_global.h"

// VN Components
#include "frame.h"

// Namespaces
using namespace std;


/* Control functions */

// Freezing a component
std::pair<void*, compPlayback> freezeComp(Frame f, void* comp, std::vector<std::wstring> params)
{
	std::pair<void*, compPlayback> out;
	
	if(comp == NULL) // No component to freeze
	{
		return out;
	}
	
	return out;
};

// Unfreezing a component
std::pair<void*, compPlayback> unFreezeComp(Frame f, void* comp, std::vector<std::wstring> params)
{
	std::pair<void*, compPlayback> out;
	
	return out;
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
	this->addKW("freeze", &freezeComp);     // Freeze current active component inside frame
	this->addKW("unfreeze", &unFreezeComp); // Freeze current active component inside frame
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
		vector<string> kwstack;
		
		// Locations of escape characters
		vector<unsigned int> esc;
		
		// Pull out and process the tokens
		while(script.getPos() < line.length() && script.getErr() > -1)
		{
			string token = this->strip( script.readNext() );
			if(curr != script.lastDelim()) // Update previous value only if the current opening delimiter has changed
				prev = script.lastRemoved();
			curr = script.lastDelim();
			
			// Validity check
			if(script.getErr() < -1)
			{
				this->err.push_back( "[Error:L" + to_string(lcount) +"] Improper formatting\n" );
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
								this->err.push_back( "[Error:L" + to_string(lcount) +"] Invalid token \"" + token + "\" threw code " + to_string(err_code) + "\n" );
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
		
		lcount += 1;
	}
	
	ifile.close();
	
	return 0;
};

// Builds the next VN component or update an existing one
int VNovel::buildNext(int d, vector<wstring> params)
{
	vector<wstring> p;
	if(this->delim[d] == '{')
	{
		for(int i = 1; i < params.size(); i++)
			p.push_back(params[i]);
	}
	else
	{
		p = params;
	}
	
	// Special case for making a frame
	if(params[0] == "Frame")
	{
		Frame temp(0);
		this->curr_frame += 1;
		
		this->f[this->curr_frame].push_back(temp);
		
		return 0;
	}
	else
	{
		if(this->curr_frame == -1) // No Frame to store this component inside
		{
			this->err.push_back("[Error -2] No Frame to store component");
			return -2;
		}
	}
	
	// Frame parameters
	if(this->delim[d] == '~')
	{
		
	}
	else
	{
		if(this->curr_frame == -1) // No Frame found
		{
			this->err.push_back("[Error -3] No Frame to hold parameters");
			return -3;
		}
	}
	
	// General case
	if(p.size() == 0) // Make new component
	{
		keywords[params[0]](this->f[this->curr_frame], NULL, p);
	}
	else              // Pass in existing component
	{
		if( this->f[this->curr_frame].getNumComp() > 0)
		{
			pair<void*, buildComp> data = keywords[this->delim[d]][params[0]]( this->f[this->curr_frame], this->f[this->curr_frame].getComp( this->f[this->curr_frame].getNumComp() - 1 ), p );
			this->f[this->curr_frame].addComponent(data);
		}
		else
		{
			this->err.push_back("[Error -4] No component to store data");
			return -4;
		}
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