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
wchar_t d[] = {'\\', '{', '}', '~', '~', '[', ']', '(', ')', '\'', '\'', '"', '"'};
vector<wchar_t> delim (d, d + sizeof(d)/sizeof(wchar_t));


/* Edit common properties and global variables */

// Add elements to keyword map
void addToKeywords(std::wstring kw, buildComp b)
{
	keywords[kw] = buildComp;
};

// Check if key exists in map
void checkKeyword(std::wstring kw)
{
	return keywords.count(kw);
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