#ifndef TEXTBOX_H
#define TEXTBOX_H


// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


/************************************************
 * Visual Novel TextBox
 * Component that contains and displays Text objects
 ************************************************/
class TextBox : public Component
{
	private:
		// Private default constructor
		TextBox() { }
	
	public:
		/* Constructor */
		TextBox(int loc)
		{
			//Set unicode input/output
			setUnicode(true, true);
			
			// Set identifiers
			this->index = loc;
			
			// Add modifiers to map
			
			// Color
			this->mod[L"r"] = 255;
			this->mod[L"g"] = 255;
			this->mod[L"b"] = 255;
			this->mod[L"alpha"] = 100;
			
			// Height
			this->mod[L"height"] = 200;
		};
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		unsigned int setData(unsigned int start, std::vector<std::pair<int, std::wstring>> params)
		{
			for(int i = start; i < params.size(); i++)
			{
				switch(params[i].first)
				{
					// Component parameter delimiter
					case COMP_PARAM:
						if(params[i + 1].first == PARAM_VAL)
						{
							// Attempt to generate integer
							int val = toInt(params[i + 1].second);
							if(val == -1)
							{
								// Report invalid parameter value error
							}
							else
							{
								if(this->mod.count(params[i].second) == 1)
								{
									this->mod[params[i].second] = val;
								}
								else
								{
									// Report key not found error
								}
							}
							
							i += 1;
						}
						else
						{
							// Report no value for parameter error
						}
						
						break;
					
					// Floating parameter value delimiter (skip)
					case COMP_PARAM:
						wcout << L"No specified location for given parameter value in this frame";
						break;
					
					// All other delimiters
					default:
						return i;
				}
			}
			
			return params.size() - 1;
		}
		
		// Add content
		unsigned int setContent(unsigned int start, std::vector<std::pair<int, std::wstring>> params)
		{
			
		};
}


#endif