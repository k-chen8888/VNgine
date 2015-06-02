#ifndef COMPONENT3_H
#define COMPONENT3_H


// Base files
#include "vn_global.h"


/* Abstract base class that all L3 components must use
 */
class Component3 : public Component2
{
	protected:
		// Modifiers
		//std::map<std::wstring, int> mod;
		
		int freeze; // > -1 implies a freeze after playing this, as well as where to go
		int end;    // > -1 implies the end of parent L2 component after playing this, as well as where to go
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build */
		
		// Add a keyword to the map
		//int addKW(std::wstring kw, buildComp b);
		
		// Set the freeze position
		// virtual void setFreeze(int next);
		
		// Set the end position
		// virtual void setEnd(int next);
		
		// Set a modifier
		// int setModifier(std::wstring k, std::wstring v);
		
		// Create/modify an L3 component
		// Output location where this function stopped reading through params
		// Doesn't actually do anything here
		int buildL3(unsigned int start, std::vector<std::wstring> params)
		{
			return 0;
		}
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwises
		// virtual int play(bool gui);
}


#endif