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
		
		// Traversal
		unsigned int current;                  // Current position being played/edited
		std::vector<VNObject*> contents;       // Stuff inside this Component
		
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