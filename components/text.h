#ifndef TEXT_H
#define TEXT_H


// Base files
#include "vn_global.h"

// L3 Component definition
#include "component3.h"


// Possible Text colors
enum TextColor = {WHITE, BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};


// Macros for default modifier values
#define XPOS     10
#define YPOS     10
#define COLOR    1
#define FONTSIZE 12
#define PAUSE    0


/************************************************
 * VNovel Text object
 * 
 * String that is written onto screen by its parent component
 ************************************************/
class Text : public Component3
{
	private:
		std::wstring content; // Text to be displayed
		
		// Private default constructor
		Text() { }
	
	public:
		/* Constructor */
		Text(int x, int y);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		// Set the freeze position
		void setFreeze(int next)
		{
			if(next > -1)
				this->freeze = next;
		};
		
		// Set the end position
		void setEnd(int next)
		{
			if(next > -1)
				this->end = next;
		};
		
		// Add content
		void addContent(std::wstring c)
		{
			if(c.length() > 0)
				content = c;
		}
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwise
		int play(bool gui)
		{
			if(gui)
			{
				
			}
			else
			{
				if(this->content.length() > 0)
				{
					// Display content
					wcout << this->content;
					
					// Ask to press enter
					if(this->mod[L"p"] == 1)
					{
						wcout << "\nPress enter to continue...\n";
						wcin.ignore( numeric_limits <streamsize> ::max(), '\n' );
					}
				}
				
				// Jump to the next component on a freeze
				if(this->freeze > -1)
				{
					return this->freeze;
				}
				
				// Jump to the next component on component end
				if(this->end > -1)
				{
					return this->end;
				}
			}
			
			return -1;
		};
		
		/* Reporting */
		
		// Check if this is an end component
		bool isEnd()
		{
			return this->end > -1;
		}
		
		/* Destructor */
		~Text();
};


#endif