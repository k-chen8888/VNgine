/* Imports */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>

// Libraries for "Press ENTER to continue..."
#include <limits>

// Base files
#include "keywords.h"
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"

// Parser
#include "pda.h"
#include "pda_string.h"
#include "pda_wstring.h"


/* Global variables */

// Keyword maps
std::map<std::wstring, buildF> containers;
std::map<std::wstring, buildF> components;
std::map<std::wstring, buildF> vnobjects;

// Delimiter vector
wchar_t d[] = {L'\\', L'{', L'}', L'~', L'~', L'[', L']', L':', L':', L'(', L')', L'#', L'#', L'"', L'"', L'\'', L'\''};
std::vector<wchar_t> delim (d, d + sizeof(d)/sizeof(wchar_t));

// Ignored characters
wchar_t i[] = {L' ', L'\t', L'\n', L'\v'};
std::vector<wchar_t> ign(i, i + sizeof(i)/sizeof(wchar_t));


/* Editing tools and helper functions */

// Add elements to keyword maps
int addToContainers(std::wstring kw, buildF b)
{
	if(containers.count(kw) == 1)
	{
		std::wcout << DUP_CONT_ERR_1 << kw << DUP_CONT_ERR_2 << L"\n";
		return DUP_CONT;
	}
	else
	{
		containers[kw] = b;
		return 0;
	}
};

int addToComponents(std::wstring kw, buildF b)
{
	if(containers.count(kw) == 1)
	{
		std::wcout << DUP_COMP_ERR_1 << kw << DUP_COMP_ERR_2 << L"\n";
		return DUP_COMP;
	}
	else
	{
		components[kw] = b;
		return 0;
	}
};

int addToVNObjects(std::wstring kw, buildF b)
{
	if(containers.count(kw) == 1)
	{
		std::wcout << DUP_OBJ_ERR_1 << kw << DUP_OBJ_ERR_2 << L"\n";
		return DUP_OBJ;
	}
	else
	{
		vnobjects[kw] = b;
		return 0;
	}
};

// Removes escape characters
std::wstring escape(std::wstring token, std::vector<unsigned int> e)
{
	std::wstring out;
	unsigned int start = 0;
	
	if(e.size() > 0)
	{
		unsigned int j = 0;
		for(unsigned int i = 0; i < token.length(); i++)
		{
			if(i == e[j])
			{
				// Save the part before the escape character
				std::wstring temp = token.substr(start, e[j] - start);
				out = out + temp;
				
				// Update
				j += 1;
				start = i + 2;
				
				// Handle standard escape characters
				if(start - 1 < token.length())
				{
					switch(token.at(start - 1))
					{
						// Escape character itself
						case L'\\':
							out = out + L"\\";
							j += 1;
							break;
						
						// Backspace
						case L'b':
							out = out + L"\b";
							break;
						
						// Form feed
						case L'f':
							out = out + L"\f";
							break;
						
						// Line feed
						case L'n':
							out = out + L"\n";
							break;
						
						// Carriage return
						case L'r':
							out = out + L"\r";
							break;
						
						// Horizontal tab
						case L't':
							out = out + L"\t";
							break;
						
						// Vertical tab
						case L'v':
							out = out + L"\v";
							break;
						
						// All other cases (include in the next set)
						default:
							break;
					}
				}
				
				// Check if done
				if(j >= e.size())
				{
					i = token.length();
					temp = token.substr(start, i - start);
					out = out + temp;
				}
			}
		}
	}
	else
	{
		return token;
	}
	
	return out;
};

// Strips ignored characters
std::wstring strip(std::wstring token)
{
	std::wstring out;
	
	unsigned int start = 0;
	unsigned int end = 0;
	
	bool start_set = false;
	
	// Go through the token to find the real start and end points
	for(unsigned int i = 0; i < token.length(); i++)
	{
		if(!start_set)
		{
			// Find the start point first
			bool in_ign = false;
			for(unsigned int j = 0; j < ign.size(); j++)
			{
				// in_ign will be true if any of the characters in the vector ign matches the current character
				in_ign = ( (token.at(i) == ign[j]) || in_ign );
			}
			
			// The first character that is not in the ignore list is the real start
			if(!in_ign)
			{
				start = i;
				end = start;
				start_set = true;
			}
		}
		else
		{
			// Then find the end point
			for(unsigned int j = 0; j < ign.size(); j++)
			{
				// Whenever an ignored character is found, set end
				if( token.at(i) == ign[j] )
				{
					end = i - 1;
					j = ign.size();
				}
			}
		}
	}
	
	// Evaluate output
	if(!start_set)
	{
		return L"";
	}
	else
	{
		return token.substr(start, end - start + 1);
	}
};

// String to integer
int toInt(std::wstring s)
{
	int val = 0;
	
	for(unsigned int i = 0; i < s.length(); i++)
	{
		switch(s.at(i))
		{
			case L'0':
				val = val * 10 + 0;
				break;
			
			case L'1':
				val = val * 10 + 1;
				break;
			
			case L'2':
				val = val * 10 + 2;
				break;
			
			case L'3':
				val = val * 10 + 3;
				break;
			
			case L'4':
				val = val * 10 + 4;
				break;
			
			case L'5':
				val = val * 10 + 5;
				break;
			
			case L'6':
				val = val * 10 + 6;
				break;
			
			case L'7':
				val = val * 10 + 7;
				break;
			
			case L'8':
				val = val * 10 + 8;
				break;
			
			case L'9':
				val = val * 10 + 9;
				break;
			
			// Not an integer
			default:
				return -1;
		}
	}
	
	return val;
};

//Set unicode input/output
int setUnicode(bool in, bool out)
{
	if(in)
	{
		int err = _setmode(_fileno(stdin), _O_U16TEXT);
		
		if(err == -1)
		{
			std::cout << SET_IN_ERR;
			return SET_IN;
		}
	}
	
	if(out)
	{
		int err = _setmode(_fileno(stdout), _O_U16TEXT);
		
		if(err == -1)
		{
			std::cout << SET_OUT_ERR;
			return SET_OUT;
		}
	}
	
	return 0;
};

// "Press ENTER to continue..."
void enterToContinue()
{
	std::wcout << "\nPress ENTER to continue...\n";
	std::wcin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );	
};


/************************************************
 * Visual Novel object
 * 
 * Contents:
 * 	Script file
 * 	Visual novel components
 ************************************************/

/* Constructor */
VNovel::VNovel(std::string src)
{
	this->source = src;
	this->prev = -1;
	this->curr = 0;
};

// Builds VN from script file
int VNovel::buildVN()
{
	std::wifstream ifile;
	try
	{
		ifile.open(this->source);
	}
	catch(int e)
	{
		return e; // Failure
	}
	
	std::wstring line;
	unsigned int lcount;
	while( getline(ifile, line) && line.length() > 0)
	{
		// PDA, noisy enabled
		PDA<std::wstring> script (line, delim, false);
		
		// Map lookup keywords
		std::vector< std::pair<int, std::wstring> > kwlist;
		
		// Start position of current token
		unsigned int start = 0;
		
		// Current size of the stack
		unsigned int stack_depth = 0;
		
		// The topmost delimiter on the stack
		unsigned int last_delim = 0;
		
		// Locations of escape characters
		std::vector<unsigned int> esc;
		
		while(script.getPos() < line.length() && script.getErr() > -1)
		{
			// Get next token (PDA automatically increments position by 1)
			std::wstring token = script.readNext();
			
			if( stack_depth < script.stackDepth() )
			{
				// A new delimiter was added -> This must be an opening delimiter
				stack_depth += 1;
				
				// Set start position of next token to be the current position of PDA
				start = script.getPos();
				
				// Remove escape characters
				std::wstring final_token = escape(token, esc);
				esc.clear();
				
				// This token needs to be stripped because TXT_TOKEN cannot contain non-escaped opening delimiters
				final_token = strip(final_token);
				
				// Was a token found?
				if(final_token.length() > 0)
				{
					// Add to list of keywords
					kwlist.push_back( std::make_pair( last_delim, final_token ) );
				}
				
				// Save the new last delimiter
				last_delim = script.lastDelim();
			}
			else if( stack_depth > script.stackDepth() )
			{
				// A delimiter was removed -> a closing delimiter was found
				stack_depth -= 1;
				
				// Set start position of next token to be the current position of PDA
				start = script.getPos();
				
				// Remove escape characters
				std::wstring final_token = escape(token, esc);
				esc.clear();
				
				// Strip this token if it is not a TXT_TOKEN
				if( line.at( script.getPos() - 1 ) == delim[TXT_TOKEN] )
				{
					final_token = strip(final_token);
				}
				
				// Was a token found?
				if(final_token.length() > 0)
				{
					// Add to list of keywords
					kwlist.push_back( std::make_pair( script.lastRemoved(), final_token ) );
				}
				
				// Save the new last delimiter
				last_delim = script.lastDelim();
			}
			else
			{
				// Check for escape characters
				if(script.isEsc())
					esc.push_back(script.getPos() - start -  1);
			}
		}
		
		for(unsigned int k = 0; k < kwlist.size(); k++)
			std::wcout << "(" << kwlist[k].first << ", " << kwlist[k].second << ") ";
		
		std::wcout << "\n";
		
		// Process the tokens found on this line
		for(unsigned int s = 0; s < kwlist.size(); s++)
		{
			// Switch on the delimiter type
			switch( kwlist[s].first )
			{
				// Container
				case CONT_OPEN:
					// Attempt to create a Container
					if( containers.count(kwlist[s].second) == 1 )
					{
						s = containers[ kwlist[s].second ](this, s + 1, kwlist);
					}
					else
					{
						std::wcout << NO_KEYWORD_ERR_1 << "Container" << NO_KEYWORD_ERR_2 << kwlist[s].second << std::endl;
						return NO_KEYWORD;
					}
					
					break;
				
				// Container parameter
				case CONT_PARAM:
					// Attempt to add Container parameters
					s = containers[ L"cont_param" ](this, s, kwlist);
					
					// Returns the length of params on error
					if(s == kwlist.size())
					{
						this->err.push_back(NO_CONT_ERR);
						return NO_CONT;
					}
					
					break;
				
				// Component
				case COMP_OPEN:
					// Attempt to create a Component
					if( components.count(kwlist[s].second) == 1 )
					{
						s = components[ kwlist[s].second ](this, s + 1, kwlist);
					}
					else
					{
						std::wcout << NO_KEYWORD_ERR_1 << "Component" << NO_KEYWORD_ERR_2 << kwlist[s].second << std::endl;
						return NO_KEYWORD;
					}
					
					break;
				
				// Component parameter
				case COMP_PARAM:
					// Attempt to add Component parameters
					s = components[ L"comp_param" ](this, s, kwlist);
					
					// Returns the length of params on error
					if(s == kwlist.size())
					{
						this->err.push_back(NO_COMP_ERR);
						return NO_COMP;
					}
					
					break;
				
				// VNObject
				case OBJ_OPEN:
					// Attempt to create a VNObject
					if( vnobjects.count(kwlist[s].second) == 1 )
					{
						s = vnobjects[ kwlist[s].second ](this, s + 1, kwlist);
					}
					else
					{
						std::wcout << NO_KEYWORD_ERR_1 << "VNObject" << NO_KEYWORD_ERR_2 << kwlist[s].second << std::endl;
						return NO_KEYWORD;
					}
					
					break;
				
				// VNObject parameter
				case OBJ_PARAM:
					// Attempt to add Component parameters
					s = vnobjects[ L"obj_param" ](this, s, kwlist);
					
					// Returns the length of params on error
					if(s == kwlist.size())
					{
						this->err.push_back(NO_OBJ_ERR);
						return NO_OBJ;
					}
					
					break;
				
				// Parameter value (misplaced)
				case PARAM_VAL:
					this->err.push_back(LOOSE_PARAM_ERR);
					return LOOSE_PARAM;
				
				// Text token (misplaced)
				case TXT_TOKEN:
					this->err.push_back(LOOSE_TXT_ERR);
					return LOOSE_TXT;
				
				// Everything else
				default:
					break;
			}
		}
		
		// No more keywords
		kwlist.clear();
		
		// Count number of lines
		lcount += 1;
	}
	
	// Close the file and report success
	ifile.close();
	return 0;
};