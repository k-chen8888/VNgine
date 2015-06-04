#ifndef VNOBJECT_H
#define VNOBJECT_H


/* Control functions */

// Add parameters to a VNObject
unsigned int setObjParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);


/* Abstract class for a visual novel object inside a component
 */
class VNObject
{
	protected:
		// Traversal
		int freeze; // Freeze the component if this is > -1 (also tells which Component to go to)
		int end;    // End the component if this is > -1 (also tells which Component to go to)
		
		// Modifier map (keywords -> integers)
		std::map<std::wstring, int> mod;
		
		// Error messages
		std::vector<std::wstring> err;
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		virtual unsigned int setData(unsigned int start, std::vector<std::pair<int, std::wstring>> params) = 0;
		
		// Set freeze point
		void setFreeze(int next)
		{
			if(next > -1)
				this->freeze = next;
		};
		
		// Set end point
		void setEnd(int next)
		{
			if(next > -1)
				this->end = next;
		};
		
		/* Playback */
		
		// Play the VNObject
		// Return index to the next Component (or -1 to continue within the same Component)
		virtual int play(bool gui) = 0;
};


#endif