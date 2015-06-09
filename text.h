#ifndef TEXT_H
#define TEXT_H


// Base files
#include "keywords.h"
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


/* Control functions */

// Create a Text object
unsigned int makeText(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);

// End a Text object... there's no function for this because Text objects are implicitly ended


/************************************************
 * Visual Novel Text
 * VNObject that contains and displays wstring text
 ************************************************/
class Text : public VNObject
{
	private:
		std::wstring content;
		
		// Private default constructor
		Text() { };
	
	public:
		/* Constructor */
		Text(std::wstring txt)
		{
			// Set unicode input/output
			setUnicode(true, true);
			
			// Add data
			this->content = txt;
			
			// Add modifiers to map
			
			// Color
			this->mod[L"r"] = 255;
			this->mod[L"g"] = 255;
			this->mod[L"b"] = 255;
			this->mod[L"alpha"] = 100;
			
			// Position
			this->mod[L"x"] = 0;
			this->mod[L"y"] = 0;
			
			// 1 puts a shadow over a non-speaking sprite
			this->mod[L"highlight"] = 1;
			
			// 1 pauses and waits for the user to continue
			this->mod[L"p"] = 0;
			
			// Font
			this->mod[L"size"] = 12;
			
			// Indices of speaker and spectator in sprite list (in Frame)
			// -1 implies no speaker/spectator
			this->mod[L"speaker"] = -1;
			this->mod[L"spectator"] = -1;
			
			// Indices of Frame background and bgm
			// -1 uses Frame defaults
			this->mod[L"framebg"] = -1;
			this->mod[L"framebgm"] = -1;
			
			// Index of Frame SFX to use
			// -1 uses no SFX
			this->mod[L"framesfx"] = -1;
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
					// VNObject parameter delimiter
					case OBJ_PARAM:
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
							if(params[i].second.compare(L"p") == 0)
							{
								this->mod[L"p"] = 1;
							}
							else
							{
								// Report no value for parameter error
							}
						}
						
						break;
					
					// Floating parameter value delimiter (skip, unless it freezes this VNObject)
					case COMP_PARAM:
						if(params[i].second.compare(FREEZE) == 0)
						{
							// Handle freezing on the outside
							return i;
						}
						else
						{
							std::wcout << L"No specified location for given parameter value in this Text object";
						}
						
						break;
					
					// Content
					case TXT_TOKEN:
						this->content = params[i].second;
						break;
					
					// All other delimiters
					default:
						return i;
				}
			}
		};
		
		/* Playback */
		
		// Play the VNObject
		int play(bool gui)
		{
			if(gui)
			{
				
			}
			else
			{
				std::wcout << content;
				
				// Wait to continue if needed
				if(this->mod[L"p"] == 1)
				{
					enterToContinue();
				}
			}
			
			// Freeze and jump
			if(this->freeze > -1)
				return this->freeze;
			
			// End and jump
			if(this->end > -1)
				return this->end;
			
			// Continue within component
			return -1;
		};
		
		/* Destructor */
		~Text()
		{
			// Nothing to do here
		};
};


#endif