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
#include "container.h"
#include "component.h"
#include "vnobject.h"

// Class header
#include "textbox.h"

// Visual Novel
#include "vnovel.h"

// Namespaces
using namespace std;


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params)
{
	
};

// End a TextBox, closing out all frozen and active VNObjects
unsigned int endTextBox(VNovel* vn, unsigned int start, vector<pair<int, wstring>> params)
{
	
};


// Add keywords to map
addToKeywords(L"textbox", &makeTextBox);
addToKeywords(L"endtextbox", &endTextBox);