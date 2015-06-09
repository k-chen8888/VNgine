#ifndef VNOBJECT_H
#define VNOBJECT_H


// Base files
#include "keywords.h"


/* Abstract class for a visual novel object inside a component
 */
class VNObject
{
	protected:
		// Identifying information
		std::wstring type;                     // Class name
		
		// Modifier map (keywords -> integers)
		std::map<std::wstring, int> mod;
		
		// Traversal
		int freeze = -1;                       // A jump to the next Component if > -1
		int end = -1;                          // A jump to the next Component if > -1
		
		// Error messages
		std::vector< std::pair<int, std::wstring> > err;
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		virtual unsigned int setData(unsigned int start, std::vector< std::pair<int, std::wstring> > params) = 0;
		
		// Set a freeze point
		void setFreeze(int next)
		{
			if(next > -1)
				this->freeze = next;
		}
		
		// Set an end point
		void setEnd(int next)
		{
			if(next > -1)
				this->end = next;
		}
		
		/* Playback */
		
		// Play the VNObject
		// Return index to the next Component (or -1 to continue within the same Component)
		virtual int play(bool gui) = 0;
		
		/* Reporting */
		
		// Is there a freeze after this VNObject?
		bool isFreeze()
		{
			return this->freeze > -1;
		};
		
		// Does the component end after this VNObject?
		bool isEnd()
		{
			return this->end > -1;
		};
};


#endif