#ifndef VNOVEL_H
#define VNOVEL_H


/* Imports */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Locale tools
#include <locale>
#include <stdio.h>
#include <fcntl.h>


// Base files
#include "base/keywords.h"
#include "base/container_base/container.h"
#include "base/component_base/component.h"
#include "base/vnobject_base/vnobject.h"

// Parser
#include "base/pda/pda.h"
#include "base/pda/pda_string.h"
#include "base/pda/pda_wstring.h"


// Macros for VNovel error messages
#define NO_KEYWORD       -3
#define NO_KEYWORD_ERR_1 L"Cannot create a "
#define NO_KEYWORD_ERR_2 L" using the given keyword "

#define NO_CONT          -3
#define NO_CONT_ERR      L"No Container to store parameter values"

#define NO_COMP          -4
#define NO_COMP_ERR      L"No Component to store parameter values"

#define NO_OBJ           -5
#define NO_OBJ_ERR       L"No VNObject to store parameter values"

#define LOOSE_PARAM      -6
#define LOOSE_PARAM_ERR  L"Parameter doesn't belong anywhere"

#define LOOSE_TXT        -7
#define LOOSE_TXT_ERR    L"Text token doesn't belong anywhere"


/************************************************
 * Visual Novel object
 * 
 * Contents:
 * 	Script file
 * 	Visual novel components
 ************************************************/
class VNovel
{
	private:
		std::string source;            // Script file to build VN from
		
		std::vector<Container*> cont;  // VN components, stored in Container objects
		int prev;                      // Index of previous Container in vector (editing only)
		unsigned int curr;             // Index of current Container in vector
		
		std::vector<std::wstring> err; // Error messages
		
		// Private default constructor
		VNovel() { }
	
	public:
		/* Constructor */
		VNovel(std::string src);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build a Visual Novel */
		
		// Builds VN from script file
		int buildVN();
		
		// Add Container to the list
		void addCont(Container* c)
		{
			this->cont.push_back(c);
		};
		
		// Deactivate Container for editing
		// Save previous Container's index
		void deactivateCont()
		{
			this->prev = this->curr;
			this->curr = this->cont.size();
		};
		
		/* Playback */
		
		// Plays the visual novel
		int playNovel(bool gui)
		{
			
			while( this->curr < this->cont.size() )
			{
				this->prev = this->curr;
				this->curr = this->cont[this->curr]->play(gui);
			}
			
			return 0;
		};
		
		// Reset playback
		void resetPlay()
		{
			this->prev = -1;
			this->curr = 0;
			
			// Reset Container playback
			for(unsigned int i = 0; i < this->cont.size(); i++)
			{
				this->cont[i]->resetPlay();
			}
		};
		
		/* Reporting */
		
		// Number of containers
		unsigned int numCont()
		{
			return this->cont.size();
		};
		
		// Index of currently active Container
		unsigned int getCurr()
		{
			return this->curr;
		};
		
		// Index of previously active Container
		int getPrev()
		{
			return this->prev;
		};
		
		// Container at given index
		Container* getContAt(unsigned int index)
		{
			if(index < this->cont.size())
				return this->cont[index];
			else
				return NULL;
		};
		
		// Display all error messages
		void reportErrors()
		{
			for(unsigned int i = 0; i < this->err.size(); i++)
				std::wcout << L"[Error " << this->err[i] << L"] " << this->err[i] << std::endl;
		};
		
		/* Destructor */
		~VNovel()
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
			for(unsigned int i = 0; i < this->cont.size(); i++)
			{
				delete this->cont[i];
			}
		};
};


/* typedef for generic function used to create/modify any component
 * Used to define type of function in keyword map
 * 
 * Arguments:
 * 	1. Visual Novel object
 * 	2. Place in parameter list to start looking
 * 	3. Other arguments (properties followed by saved data token; parameter list)
 * 
 * Output: Position where function stopped reading arguments (1 before the next argument to be read)
 */
typedef unsigned int (*buildF)(VNovel*, unsigned int, std::vector< std::pair<int, std::wstring> >);


/* Global variables and special definitions */

// Keyword maps
extern std::map<std::wstring, buildF> containers;
extern std::map<std::wstring, buildF> components;
extern std::map<std::wstring, buildF> vnobjects;

// Delimiter vector
extern wchar_t d[];
extern std::vector<wchar_t> delim;

// Ignored characters
extern wchar_t i[];
extern std::vector<wchar_t> ign;


/* Editing tools and helper functions */

// Add elements to keyword maps
int addToContainers(std::wstring kw, buildF b);
int addToComponents(std::wstring kw, buildF b);
int addToVNObjects(std::wstring kw, buildF b);

// Removes escape characters
std::wstring escape(std::wstring token, std::vector<unsigned int> e);

// Strips ignored characters
std::wstring strip(std::wstring token);

// String to integer
int toInt(std::wstring s);

//Set unicode input/output
int setUnicode(bool in, bool out);

// "Press ENTER to continue..."
void enterToContinue();


#endif