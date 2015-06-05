#ifndef TEXTBOX_H
#define TEXTBOX_H


// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);

// End a TextBox, closing out all frozen and active VNObjects
unsigned int endTextBox(VNovel* vn, unsigned int start, vector<pair<int, wstring>> params);


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
						wcout << L"No specified location for given parameter value in this TextBox Component";
						break;
					
					// All other delimiters
					default:
						return i;
				}
			}
			
			return params.size() - 1;
		};
		
		/* Playback */
		
		// Play the Component
		// Return index to the next Component
		int play(bool gui)
		{
			int out = -1;
			
			// Play current component until output changes
			while(out == -1)
			{
				out = this->contents[this->current]->play(gui);
			}
			
			return out;
		};
		
		// Reset playback
		void resetPlay()
		{
			this->current = 0;
			this->ending = 0;
		};
		
		/* Destructor */
		~TextBox()
		{
			for(int i = 0; i < this->contents.size(); i++)
			{
				delete this->contents[i];
			}
		};
};


#endif