#ifndef CONTAINER_H
#define CONTAINER_H


// Base files
#include "../keywords.h"
#include "../../vnovel.h"
#include "../component_base/component.h"


/* Abstract class for a visual novel component container
 */
class Container
{
	protected:
		// Identifying information
		std::wstring type;                       // Class name
		std::wstring name;                       // Name
		int index;                               // Index in list
		
		// Traversal
		int previous;                            // Previously edited position (editing only)
		unsigned int current;                    // Current position being played/edited
		std::vector<Component*> contents;        // Stuff inside this Container
		std::vector<unsigned int> frz;           // Stack of frozen indices (editing only)
		unsigned int ending;                     // Index of the ending that will be reached next
		std::vector< std::pair<int, int> > next; // Index of the next Container to play (the final element in the queue is the true "end" of the Container)
		
		// Error messages
		std::vector< std::pair<int, std::wstring> > err;
	
	public:
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		virtual unsigned int setData(unsigned int start, std::vector< std::pair<int, std::wstring> > params) = 0;
		
		// Tell this Container where to stop and where to go next
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
		
		// Set content as active for editing
		void addActive(Component* c)
		{
			if(c != NULL)
			{
				this->contents.push_back(c);
			}
		};
		
		// Deactivate a content element
		void deactivateContent()
		{
			if(this->current < this->contents.size())
			{
				this->previous = this->current;
				this->current = this->contents.size();
			}
		};
		
		// Freeze current content element
		virtual unsigned int freeze() = 0;
		
		// Unfreeze last frozen content element
		virtual unsigned int unfreeze() = 0;
		
		/* Playback */
		
		// Play the Container
		// Return the next Container to play
		virtual int play(bool gui) = 0;
		
		// Reset playback
		virtual void resetPlay() = 0;
		
		/* Reporting */
		
		// Get name and index
		std::pair<std::wstring, int> getID()
		{
			std::pair<std::wstring, int> out(this->name, this->index);
			return out;
		};
		
		// Get class name
		std::wstring getType()
		{
			return this->type;
		};
		
		// Get index of active content element
		unsigned int getCurrent()
		{
			return this->current;
		};
		
		// Get index of previously active content element
		unsigned int getPrevious()
		{
			return this->previous;
		};
		
		// Retrieve a content element at some index
		Component* getContentAt(unsigned int index)
		{
			if(index < this->contents.size())
				return this->contents[index];
			else
				return NULL;
		};
		
		// Get the number of content elements
		unsigned int getNumContents()
		{
			return this->contents.size();
		};
		
		// Get frozen content element at some index
		Component* getFrz(unsigned int i)
		{
			if(i < this->frz.size())
				return this->contents[this->frz[i]];
			else
				return NULL;
		};
		
		// Get current number of frozen content elements
		unsigned int numFrozen()
		{
			return this->frz.size();
		};
		
		// Display all error messages
		void reportErrors()
		{
			for(unsigned int i = 0; i < this->err.size(); i++)
				std::wcout << "[Error " << this->err[i].first << "] " << this->err[i].second << std::endl;
		};
		
		// Virtual destructor
		virtual ~Container()
		{
			// Automatically report errors before deallocating memory
			if(this->err.size() > 0)
			{
				std::wcout << L"Auto-generated error report" << std::endl;
				this->reportErrors();
				std::wcout << std::endl;
			}
			else
			{
				std::wcout << L"No errors\n" << std::endl;
			}
			
			// Deallocate memory for everything
			std::wcout << L"Removing all Container content..." << std::endl;
		};
};


#endif