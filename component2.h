#ifndef COMPONENT_H
#define COMPONENT_H


// Base files
#include "vn_global.h"


// typedef for generic function used to create/modify an L3 component
// L2 Component should know what it wants to build based on where output will be stored
// Traverses a parameter string from a starting position and outputs where it ends up
unsigned int (*buildL3Comp)(Component2*, unsigned int, std::vector<std::pair<int, std::wstring>>);


/* Abstract base class that all L2 components must use
 */
class Component2
{
	protected:
		std::map<std::wstring, int> mod;        // Modifiers
		std::map<std::wstring, buildL3Comp> l3; // Create/modify an L3 component
		
		std::vector<int> next;                  // Index for possible next components/frames to play
	
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
				// Special case: boolean values
				if(v == L"+1")
				{
					this->mod[k] = (this->mod[k] + 1) % 2;
					return 0;
				}
				
				// Perform conversion to integer and store
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
				
				this->mod[k] = val;
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
		// Return index to the next component
		virtual int play(bool gui) = 0;
}


#endif