#ifndef COMPONENT3_H
#define COMPONENT3_H


// Base files
#include "vn_global.h"


/* Abstract base class that all L3 components must use
 */
class Component3
{
	protected:
		std::map<std::wstring, int> mod; // Modifiers
		
		int freeze;                      // > -1 implies a freeze after playing this, as well as where to go
		int end;                         // > -1 implies the end of parent L2 component after playing this, as well as where to go
		
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
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwise
		virtual int play(bool gui) = 0;
}


#endif