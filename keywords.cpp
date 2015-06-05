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
#include "keywords.h"

// Parts of the Visual Novel
#include "vnovel.h"

#include "frame.h"

#include "textbox.h"

#include "text.h"


/* Add all keywords here */
void addKeywords()
{
	/* General */
	addToContainers(L"comment", &ignoreComments)
	
	
	/* Containers */
	addToContainers(L"cont_param", &setContParams);
	
	// Frame
	addToContainers(L"frame", &makeFrame);
	addToContainers(L"endframe", &endFrame);
	
	
	/* Components */
	addToComponents(L"comp_param", &setCompParams);
	
	// TextBox
	addToComponents(L"textbox", &makeTextBox);
	addToComponents(L"endtextbox", &endTextBox);
	
	/* VNObjects */
	addToVNObjects(L"obj_param", &setObjParams);
	
	// Text
	addToVNObjects(L"text", &makeText);
	
};