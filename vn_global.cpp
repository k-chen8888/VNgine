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

// Namespaces
using namespace std;


/* Global variables */

// Keyword maps
std::map<std::wstring, buildComp> containers;
std::map<std::wstring, buildComp> components;
std::map<std::wstring, buildComp> vnobjects;

// Delimiter vector
wchar_t d[] = {L'\\', L'{', L'}', L'~', L'~', L'[', L']', L':', L':', L'(', L')', L'#', L'#', L'\'', L'\'', L'"', L'"'};
vector<wchar_t> delim (d, d + sizeof(d)/sizeof(wchar_t));

// Ignored characters
wchar_t i[] = {L' ', L'\t', L'\n', L'\v'};
std::vector<wchar_t> ignore(i, i + sizeof(i)/sizeof(wchar_t));


/* Editing tools and helper functions */

// Add elements to keyword maps
int addToContainers(std::wstring kw, buildComp b)
{
	if(containers.count(kw) == 1)
	{
		wcout << DUP_CONT_ERR_1 << kw << DUP_CONT_ERR_2 << L"\n";
		return DUP_CONT;
	}
	else
	{
		containers[kw] = buildComp;
		return 0;
	}
};

void addToComponents(std::wstring kw, buildComp b)
{
	if(containers.count(kw) == 1)
	{
		wcout << DUP_COMP_ERR_1 << kw << DUP_COMP_ERR_2 << L"\n";
		return DUP_COMP;
	}
	else
	{
		components[kw] = buildComp;
		return 0;
	}
};

void addToVNObjects(std::wstring kw, buildComp b)
{
	if(containers.count(kw) == 1)
	{
		wcout << DUP_OBJ_ERR_1 << kw << DUP_OBJ_ERR_2 << L"\n";
		return DUP_OBJ;
	}
	else
	{
		vnobjects[kw] = buildComp;
		return 0;
	}
};

// Removes escape characters
wstring escape(std::wstring token, std::vector<unsigned int> e)
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
				// Save the part before the escape character
				wstring temp = token.substr(start, e[j] - start);
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
wstring strip(wstring token)
{
	wstring out;
	
	// Starting ignored characters
	bool change = true;
	bool ig = false;
	int i = 0;
	while(change && i < token.length())
	{
		ig = false;
		for(int j = 0; j < ignore.size(); j++)
		{
			if(token.at(i) == ignore[j])
			{
				ig = true;
				j = ignore.size();
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
		for(int j = 0; j < ignore.size(); j++)
		{
			if(token.at(i) == ignore[j])
			{
				ig = true;
				j = ignore.size();
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
int toInt(wstring s)
{
	int val = 0;
	int t_val = 0;
	int pos = 0;
	while(pos < v.length())
	{
		switch(v.at(pos))
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
			cout << SET_IN_ERR;
			return SET_IN;
		}
	}
	
	if(out)
	{
		int err = _setmode(_fileno(stdout), _O_U16TEXT);
		
		if(err == -1)
		{
			cout << SET_OUT_ERR;
			return SET_OUT;
		}
	}
	
	return 0;
}