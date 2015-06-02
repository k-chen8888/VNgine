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
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwise
		virtual int play(bool gui) = 0;
}


#endif