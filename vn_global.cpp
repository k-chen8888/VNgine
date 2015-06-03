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

// Keyword map
map<wstring, buildComp> keywords;

// Delimiter vector
wchar_t d[] = {L'\\', L'{', L'}', L'~', L'~', L'[', L']', L':', L':', L'(', L')', L'#', L'#', L'\'', L'\'', L'"', L'"'};
vector<wchar_t> delim (d, d + sizeof(d)/sizeof(wchar_t));

// Ignored characters
wchar_t i[] = {L' ', L'\t', L'\n', L'\v'};
std::vector<wchar_t> ignore(i, i + sizeof(i)/sizeof(wchar_t));


/* Edit common properties and global variables */

// Add elements to keyword map
void addToKeywords(std::wstring kw, buildComp b)
{
	if(keywords.count(kw) == 1)
	{
		wcout << DUP_KEY_ERR_1 << kw << DUP_KEY_ERR_2 << L"\n";
	}
	else
	{
		keywords[kw] = buildComp;
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
						case '\\':
							out = out + L"\\";
							start += 1;
							j += 1;
							break;
						
						// Backspace
						case 'b':
							out = out + L"\b";
							start += 1;
							break;
						
						// Form feed
						case 'f':
							out = out + L"\f";
							start += 1;
							break;
						
						// Line feed
						case 'n':
							out = out + L"\n";
							start += 1;
							break;
						
						// Carriage return
						case 'r':
							out = out + L"\r";
							start += 1;
							break;
						
						// Horizontal tab
						case 't':
							out = out + L"\t";
							start += 1;
							break;
						
						// Vertical tab
						case 'v':
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

//Set unicode input/output
int setUnicode(bool in, bool out)
{
	if(in)
	{
		int err = _setmode(_fileno(stdin), _O_U16TEXT);
		
		if(err == -1)
			return SET_IN;
	}
	
	if(out)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
		
		if(err == -1)
			return SET_OUT;
	}
	
	return 0;
}