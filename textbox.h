#ifndef TEXTBOX_H
#define TEXTBOX_H


// Base files
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


// Macros for warnings
// Warnings happen when the object is unsure if erratic behavior is intended
// All warnings have the code -1
#define BAD_DELIM_WARN   std::wstring(L"[Warning] Incorrect delimiter for TextBox parameters")
#define END_PLAY_WARN    std::wstring(L"[Warning] Reached end of playback")


// Macros for error messages
// Errors happen when the object is sure that behavior is wrong
#define BAD_PARAM        -2
#define BAD_PARAM_ERR    std::wstring(L"[TextBox Error -2] Parameter name not found")

#define FLOATING_VAL     -3
#define FLOATING_VAL_ERR std::wstring(L"[TextBox Error -3] Tried to add value without giving a parameter name")
#define BAD_VAL          -4
#define BAD_VAL_ERR      std::wstring(L"[TextBox Error -4] Only integer parameter values are allowed")
#define MISSING_VAL      -5
#define MISSING_VAL_ERR  std::wstring(L"[TextBox Error -5] Invoked a parameter name without giving it a value")


/* Control functions */

// Create a TextBox
unsigned int makeTextBox(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);

// End a TextBox, closing out all frozen and active VNObjects
unsigned int endTextBox(VNovel* vn, unsigned int start,  std::vector< std::pair<int,  std::wstring> > params);


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
			this->type = L"TextBox";
			this->index = loc;
			
			// Default editing/traversal parameters
			this->previous = -1;
			this->current = 0;
			
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
		unsigned int setData(unsigned int start, std::vector< std::pair<int, std::wstring> > params)
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
								this->err.push_back(std::make_pair(BAD_VAL, BAD_VAL_ERR));
							}
							else
							{
								if(this->mod.count(params[i].second) == 1)
								{
									this->mod[params[i].second] = val;
								}
								else
								{
									this->err.push_back(std::make_pair(BAD_PARAM, BAD_PARAM_ERR));
								}
							}
							
							i += 1;
						}
						else
						{
							this->err.push_back(std::make_pair(MISSING_VAL, MISSING_VAL_ERR));
						}
						
						break;
					
					// Floating parameter value delimiter (skip)
					case PARAM_VAL:
						this->err.push_back(std::make_pair(FLOATING_VAL, FLOATING_VAL_ERR));
						break;
					
					// All other delimiters
					default:
						this->err.push_back(std::make_pair(-1, BAD_DELIM_WARN));
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
			while(this->current < this->contents.size())
			{
				// Play and move on to the next content
				this->contents[this->current]->play(gui);
				this->current += 1;
				
				// Check for an ending point
				if(this->current - 1 == this->next[this->ending].first)
				{
					out = this->next[this->ending].second;
					this->ending += 1;
					return out;
				}
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