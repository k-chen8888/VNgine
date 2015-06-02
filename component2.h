#ifndef COMPONENT_H
#define COMPONENT_H


// Base files
#include "vn_global.h"


// typedef for generic function used to create/modify an L3 component
// L2 Component should know what it wants to build based on where output will be stored
// Traverses a parameter string from a starting position and outputs where it ends up
unsigned int (*buildL3Comp)(Component2*, unsigned int, std::vector<std::wstring>);


/* Abstract base class that all L2 components must use
 */
class Component2
{
	protected:
		std::map<std::wstring, int> mod;        // Modifiers
		std::map<std::wstring, buildL3Comp> l3; // Create/modify an L3 component
		
		bool frz;                               // Whether or not this component is ever frozen
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build */
		
		// Add a keyword to the map
		int addKW(std::wstring kw, buildComp b)
		{
			if(checkKeyword(kw) == 0)
			{
				addToKeywords(kw, b);
			}
			else
			{
				this->err.push_back(DUP_KEY_ERR_1 << kw << DUP_KEY_ERR_2);
				return -1;
			}
			
			return 0;
		};
		
		// Set the freeze position
		virtual void setFreeze(int next) = 0;
		
		// Set the end position
		virtual void setEnd(int next) = 0;
		
		// Set a modifier
		int setModifier(std::wstring k, std::wstring v)
		{
			if(this->mod.count(k) == 1)
			{
				this->mod[k] = std::stoi(v);
			}
			else
			{
				return -1;
			}
			
			return 0;
		};
		
		// Create/modify an L3 component
		// Output location where this function stopped reading through params
		virtual int buildL3(unsigned int start, std::vector<std::wstring> params) = 0;
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwise
		virtual int play(bool gui) = 0;
}


#endif