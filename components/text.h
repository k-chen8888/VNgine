#ifndef TEXT_H
#define TEXT_H


/************************************************
 * VNovel Text object
 * 
 * String that is written onto screen by its parent component
 ************************************************/
class Text
{
	private:
		std::string content;     // Text to be displayed
		std::pair<int, int> pos; // (x, y)-position on the screen; use default if (-1, -1)
		
		// Private default constructor
		Text() { }
	
	public:
		/* Constructor */
		Text(std::string c, int x, int y);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Playback */
		
		// Display content on screen
		
		/* Testing */
		
		// Display content on command line
		void seeContent();
		
		/* Destructor */
		~Text();
};


#endif