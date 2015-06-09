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
wchar_t d[] = {L'\\', L'{', L'}', L'~', L'~', L'[', L']', L':', L':', L'(', L')', L'#', L'#', L'\'', L'\'', L'"', L'"'};
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
	int start = 0;
	
	if(e.size() > 0)
	{
		int j = 0;
		for(int i = 0; i < token.length(); i++)
		{
			if(i == e[j])
			{
				// Save the part before the escape character
				std::wstring temp = token.substr(start, e[j] - start);
				out = out + temp;
				
				// Update
				j += 1;
				start = i + 1;
				
				// Handle standard escape characters
				if(start < token.length())
				{
					switch(token.at(start))
					{
						// Escape character itself
						case L'\\':
							out = out + L"\\";
							start += 1;
							j += 1;
							break;
						
						// Backspace
						case L'b':
							out = out + L"\b";
							start += 1;
							break;
						
						// Form feed
						case L'f':
							out = out + L"\f";
							start += 1;
							break;
						
						// Line feed
						case L'n':
							out = out + L"\n";
							start += 1;
							break;
						
						// Carriage return
						case L'r':
							out = out + L"\r";
							start += 1;
							break;
						
						// Horizontal tab
						case L't':
							out = out + L"\t";
							start += 1;
							break;
						
						// Vertical tab
						case L'v':
							out = out + L"\v";
							start += 1;
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
	
	// Starting ignored characters
	bool change = true;
	bool ig = false;
	int i = 0;
	while(change && i < token.length())
	{
		ig = false;
		for(int j = 0; j < ign.size(); j++)
		{
			if(token.at(i) == ign[j])
			{
				ig = true;
				j = ign.size();
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
	i = out.length() - 1;
	while(change && i > -1)
	{
		ig = false;
		for(int j = 0; j < ign.size(); j++)
		{
			if(token.at(i) == ign[j])
			{
				ig = true;
				j = ign.size();
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

// String to integer
int toInt(std::wstring s)
{
	int val = 0;
	int t_val = 0;
	int pos = 0;
	while(pos < s.length())
	{
		switch(s.at(pos))
		{
			case L'0':
				t_val = 0;
				break;
			
			case L'1':
				t_val = 1;
				break;
			
			case L'2':
				t_val = 2;
				break;
			
			case L'3':
				t_val = 3;
				break;
			
			case L'4':
				t_val = 4;
				break;
			
			case L'5':
				t_val = 5;
				break;
			
			case L'6':
				t_val = 6;
				break;
			
			case L'7':
				t_val = 7;
				break;
			
			case L'8':
				t_val = 8;
				break;
			
			case L'9':
				t_val = 9;
				break;
			
			// Not an integer
			default:
				return -1;
		}
		
		val = val * 10 + t_val;
		pos += 1;
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
	//Set unicode input/output
	int e = setUnicode(true, true);
	if(e == SET_IN)
	{
		std::cout << SET_IN_ERR;
		this->err.push_back(L"Must terminate");
	}
	if(e == SET_OUT)
	{
		std::cout << SET_OUT_ERR;
		this->err.push_back(L"Must terminate");
	}
	
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
	while( getline(ifile, line) )
	{
		// PDA
		PDA<std::wstring> script (line, delim);
		
		// Map lookup keywords
		std::vector< std::pair<int, std::wstring> > kwlist;
		
		// Start position of current token
		unsigned int start;
		
		// Locations of escape characters
		std::vector<unsigned int> esc;
		
		// Pull out and process the tokens
		while(script.getPos() < line.length() && script.getErr() > -1)
		{
			std::wstring token = script.readNext();
			int d = script.lastDelim();
			
			if(script.getErr() < -1)
			{
				this->err.push_back( L"[Error:L" + std::to_wstring(lcount) + L"] Improper formatting\n" );
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
							std::wstring finaltoken = escape(token, esc);
							if(script.lastRemoved() != TXT_TOKEN)
								finaltoken = strip(finaltoken);
							
							kwlist.push_back( std::make_pair( script.lastRemoved(), finaltoken ) );
							start = script.getPos();
						}
					}
					
					// Flush stack when the sequence ends
					bool outofkw = script.stackDepth() == 0 && kwlist.size() > 0;
					bool endofline = script.getPos() >= line.length();
					if( outofkw || endofline )
					{
						unsigned int s = 0;
						while(s < kwlist.size())
						{
							// Switch on the delimiter type
							switch( kwlist[0].first )
							{
								// Container
								case CONT_OPEN:
									s = containers[ kwlist[0].second ](this, 1, kwlist);
									break;
								
								// Container parameter
								case CONT_PARAM:
									// Attempt to add Container parameters
									s = containers[ L"cont_param" ](this, 0, kwlist);
									
									// Returns the length of params on error
									if(s == kwlist.size())
									{
										this->err.push_back(NO_CONT_ERR);
										return NO_CONT;
									}
									
									break;
								
								// Component
								case COMP_OPEN:
									s = components[ kwlist[0].second ](this, 1, kwlist);
									break;
								
								// Component parameter
								case COMP_PARAM:
									// Attempt to add Component parameters
									s = components[ L"comp_param" ](this, 0, kwlist);
									
									// Returns the length of params on error
									if(s == kwlist.size())
									{
										this->err.push_back(NO_COMP_ERR);
										return NO_COMP;
									}
									
									break;
								
								// VNObject
								case OBJ_OPEN:
									s = vnobjects[ kwlist[0].second ](this, 1, kwlist);
									break;
								
								// VNObject parameter
								case OBJ_PARAM:
									// Attempt to add Component parameters
									s = vnobjects[ L"obj_param" ](this, 0, kwlist);
									
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
							
							s += 1;
						}
						
						// No more keywords
						kwlist.clear();
					}
				}
				else
				{
					if(script.isEsc()) // Mark escape characters
						esc.push_back(script.getPos() - start);
				}
			}
		}
		
		lcount += 1;
	}
	
	ifile.close();
	
	return 0;
};