#ifndef COMPONENT_H
#define COMPONENT_H


/* Abstract class for a visual novel component
 */
class Component
{
	protected:
		// Traversal
		unsigned int current;                           // Current position being played/edited
		std::vector<VNObject*> contents;                // Stuff inside this Component
		unsigned int ending;                            // Index of the next Component to play
		std::vector<std::pair<int i, Component*>> next; // Next Component to play (the final element in the queue is the true "end" of the Component)
		
		// Error messages
		std::vector<std::wstring> err;
	
	public:
		/* Build and edit */
		
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
}


#endif