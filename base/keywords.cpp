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
#include "../vnovel.h"
#include "container_base/container_helper.h"
#include "component_base/component_helper.h"
#include "vnobject_base/vnobject_helper.h"

// Class header
#include "keywords.h"

// Parts of the Visual Novel
#include "../features/features.h"


/* Add all keywords here */
void addKeywords()
{
	/* General */
	addToContainers(L"comment", &ignoreComments);
	
	
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