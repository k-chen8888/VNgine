#ifndef CONTAINER_H
#define CONTAINER_H


// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


/* Control functions */

// Add parameters to a Container
unsigned int setContParams(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);

// Add contents to a container
unsigned int nonContData(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);


/* Abstract class for a visual novel component container
 */
template <typename T>
class Container
{
	protected:
		// Identifying information
		std::wstring name;                            // Name
		int index;                                    // Index in list
		
		// Traversal
		unsigned int current;                         // Current position being played/edited
		std::vector<T*> contents;                     // Stuff inside this Container
		std::vector<T*> frz;                          // Stack of frozen T (editing only)
		unsigned int ending;                          // Index of the next Container to play
		std::vector<std::pair<int, Container*>> next; // Next Container to play (the final element in the queue is the true "end" of the Container)
		
		// Error messages
		std::vector<std::wstring> err;
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		virtual unsigned int setData(unsigned int start, std::vector<std::pair<int, std::wstring>> params) = 0;
		
		// Tell this Container where to stop and where to go next
		// When given no Container*, assume there is no next container to jump to (NULL)
		// When given a Container*, set the pair at the end of the vector jump to this Container*
		void setNext(int stop_pt, Container* n)
		{
			if(n == NULL)
			{
				if(stop_pt > -1)
				{
					next.push_back( std::make_pair(stop_pt, n) );
				}
			}
			else // Set the next container to jump to only if it exists
			{
				if(this->next.size() > 0)
				{
					this->next[this->next.size() - 1].second = n;
				}
			}
		};
		
		// Add content
		virtual unsigned int setContent(unsigned int start, std::vector<std::pair<int, std::wstring>> params) = 0;
		
		/* Playback */
		
		// Play the Container
		// Return the next Container to play
		virtual int play(bool gui) = 0;
}


#endif