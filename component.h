#ifndef COMPONENT_H
#define COMPONENT_H


/* Abstract base class that all L2 components must use
 */
class Component2
{
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
		int setFreeze(int next) = 0;
		
		/* Playback */
		
		// Play the component
		// Return:
		// 	-1 if there are more things to be played
		// 	Index to the next component otherwise
		int play(bool gui) = 0;
}


#endif