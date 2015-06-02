#ifndef TEXTBOX_H
#define TEXTBOX_H


/* Various TextBox builder functions */

// Master function that accesses the keyword map
void* buildTextBox(Frame f, void* tb, std::vector<string> params);

// Set TextBox height
void* setHeight(Frame f, void* tb, std::vector<string> params);

// Set TextBox color
void* setColor(Frame f, void* tb, std::vector<string> params);

// Add a new line to TextBox (non-member function)
void* add_l(Frame f, void* tb, std::vector<string> params);


// Play contents of textbox (non-member function)
void* playTextBox(void* tb, bool cmd);


// Keyword map
std::map<char, std::map<std::string, std::pair<void*, compPlayback> (*buildComp)(Frame, void*, std::vector<std::wstring> params)>>> textbox_kw;


/************************************************
 * VNovel TextBox
 * 
 * Defines a textbox that represents dialog between characters
 * Textboxes span the entire screen but may have variable heights
 ************************************************/
class TextBox
{
	private:
		std::vector<Text::Text> content;          // Text to be displayed
		std::vector<std::pair<int, int>> sprites; // Index of sprites that will be onscreen when text is displayed
		int current;                              // Current position in content vector
		
		std::map<wstring, int> values;            // Modifiers
		
		// Private default constructor
		TextBox() { }
	
	public:
		/* Constructor */
		TextBox(int h);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Playback */
		
		// Display content
		void showNext();
		
		/* Destructor */
		~Text();
};


#endif