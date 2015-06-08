#ifndef COMPONENT_H
#define COMPONENT_H


// Base files
#include "keywords.h"
#include "vnobject.h"


/* Abstract class for a visual novel component
 */
class Component
{
	protected:
		// Identifying information
		std::wstring type;                     // Class name
		unsigned int index;                    // Index in list
		
		// Modifier map (keywords -> integers)
		std::map<std::wstring, int> mod;
		
		// Traversal
		int previous;                          // Previously edited position (editing only)
		unsigned int current;                  // Current position being played/edited
		std::vector<VNObject*> contents;       // Stuff inside this Component
		unsigned int ending;                   // Index of the ending that will be reached next
		std::vector<std::pair<int, int>> next; // Index of the next Container to play (the final element in the queue is the true "end" of the Container)
		
		// Error messages
		std::vector< std::pair<int, std::wstring> > err;
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		virtual unsigned int setData(unsigned int start, std::vector< std::pair<int, std::wstring> > params) = 0;
		
		// Add a VNObject
		void addObj(VNObject* v)
		{
			this->contents.push_back(v);
			this->current += 1;
		};
		
		// Tell this Component where to stop and where to go next
		// -1 implies that there is nothing left to jump to
		void setNext(int stop_pt, int n)
		{
			if(n < 0)
			{
				if(stop_pt > -1)
				{
					this->next.push_back( std::make_pair(stop_pt, n) );
				}
			}
			else // Set next Container's index for the last ending in the list
			{
				if(this->next.size() > 0)
				{
					this->next[this->next.size() - 1].second = n;
				}
			}
		};
		
		// Deactivate a content element
		void deactivateContent()
		{
			if(this->current < this->contents.size() - 1)
			{
				this->previous = this->current;
				this->current = this->contents.size();
			}
		};
		
		/* Playback */
		
		// Play the Component
		// Return index to the next Component
		virtual int play(bool gui) = 0;
		
		// Reset playback
		virtual void resetPlay() = 0;
		
		/* Reporting */
		
		// Get identifying index
		unsigned int getID()
		{
			return this->index;
		};
		
		// Get class name
		std::wstring getType()
		{
			return this->type;
		}
		
		// Get traversal position
		unsigned int getCurrent()
		{
			return this->current;
		};
		
		// Retrieve a VNObject at some index
		VNObject* getObjAt(unsigned int index)
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