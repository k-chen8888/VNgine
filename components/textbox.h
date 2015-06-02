#ifndef TEXTBOX_H
#define TEXTBOX_H


// Base files
#include "vn_global.h"

// L2 Component definition
#include "component2.h"


// Possible TextBox colors
enum TextBoxColor = {WHITE, BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};


// Macros for default modifier values
#define DEFAULT_HEIGHT    100
#define DEFAULT_COLOR     WHITE
#define HIGHLIGHT_SPEAKER 1


/* Various TextBox builder functions */

// Master function that accesses the keyword maps
Component2* buildTextBox(Frame* f, Component2* tb, std::vector<string> params);

// Builds the L3 components for this component (the only one that can be built is Text)
// Output location where this function stopped reading through params
unsigned int buildText(TextBox* tb, unsigned int start, std::vector<std::wstring> params);

// Builds an L3 component that represents the end of the L2 component
unsigned int buildEnd(TextBox* tb, unsigned int start, std::vector<std::wstring> params);


/************************************************
 * VNovel TextBox
 * 
 * Defines a textbox that represents dialog between characters
 * Textboxes span the entire screen but may have variable heights
 * 
 * Uses abstract base class Component
 ************************************************/
class TextBox : public Component2
{
	private:
		unsigned int index;                       // Identifying index
		
		std::vector<Text::Text> content;          // Text to be displayed
		std::vector<std::pair<int, int>> sprites; // Index of sprites that will be onscreen when text is displayed (limit 2 at a time)
		unsigned int current;                     // Current position in content vector
		
		// Private default constructor
		TextBox() { }
	
	public:
		/* Constructor */
		TextBox(unsigned int loc);
		
		/*******************************************
		 * Functions
		 * 
		 * Must include all virtual functions
		 *******************************************/
		
		/* Build */
		
		// Create/modify an L3 component
		// Output location where this function stopped reading through params
		int buildL3(unsigned int start, std::vector<std::wstring> params)
		{
			if(this->l3.count(params[start]) == 1)
			{
				return this->l3[params[start]](this, start + 1, params);
			}
			else
			{
				return start;
			}
		};
		
		// Append to content vector
		void addText(Text t)
		{
			this->content.push_back(t);
		};
		
		// Set the freeze position
		void setFreeze(int next)
		{
			Text t(-1, -1);
			t->setFreeze(next);
			this->hasfrz = true;
		};
		
		// Set the end position
		void setEnd(int next)
		{
			Text t(-1, -1);
			t.setEnd(next);
		};
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwise
		int play(bool gui)
		{
			
		};
		
		/* Reporting */
		
		unsigned int getLoc()
		{
			return this->index;
		};
		
		// Check if this TextBox is ever frozen
		bool hasFreeze()
		{
			return this->frz;
		}
		
		// Check if this TextBox has an end component
		bool hasEnd()
		{
			Text t = this->content.back();
			return t.isEnd();
		};
		
		/* Destructor */
		~Text();
};


#endif