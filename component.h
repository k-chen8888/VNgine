#ifndef COMPONENT_H
#define COMPONENT_H


/* Control functions */

// Add parameters to a Component
unsigned int setCompParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);


/* Abstract class for a visual novel component
 */
class Component
{
	protected:
		// Identifying information
		int index                                       // Index in list
		
		// Modifier map (keywords -> integers)
		std::map<std::wstring, int> mod;
		
		// Traversal
		unsigned int current;                         // Current position being played/edited
		std::vector<VNObject*> contents;              // Stuff inside this Component
		unsigned int ending;                          // Index of the next Component to play
		std::vector<std::pair<int, Component*>> next; // Next Component to play (the final element in the queue is the true "end" of the Component)
		
		// Error messages
		std::vector<std::wstring> err;
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		virtual unsigned int setData(unsigned int start, std::vector<std::pair<int, std::wstring>> params) = 0;
		
		// Tell this Component where to stop and where to go next
		void setNext(int stop_pt, Component* next)
		{
			if(stop_pt > -1)
				next.push_back( std::make_pair(stop_pt, next) );
		};
		
		/* Playback */
		
		// Play the Component
		// Return index to the next Component
		virtual int play(bool gui) = 0;
		
		// Reset playback
		virtual void resetPlay() = 0;
		
		/* Reporting */
		
		// Get index of active VNObject
		unsigned int getCurrent()
		{
			return this->current;
		}
		
		// Check if there is a VNObject active for editing
		bool hasActive()
		{
			if(this->current < this->contents.size())
				return this->active == this->contents[this->current];
			else
				return false;
		};
		
		// Retrieve active VNObject (the one being played/edited)
		VNObject* getActiveObj()
		{
			if(this->current < this->contents.size())
				return this->contents[this->current];
			else
				return NULL;
		};
		
		// Retrieve a VNObject at some index
		VNObject* getObj(unsigned int index)
		{
			if(index < this->contents.size())
				return this->contents[index];
			else
				return NULL;
		};
		
		// Get the number of Components
		unsigned int getNumObj()
		{
			return this->contents.size();
		};
};


#endif