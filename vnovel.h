#ifndef VNOVEL_H
#define VNOVEL_H


// Base files
#include "vn_global.h"

// L2 Component definition
#include "component2.h"


// Macros for error messages
#define NO_FRAME     -2
#define NO_FRAME_ERR std::wstring(L"[Error -2] No Frame available to store data")

#define NO_COMP      -3
#define NO_COMP_ERR  std::wstring(L"[Error -3] No component available to store data")


/* Control functions */

// Freeze a component
Component2* freezeComp(Frame* f, Component2* comp, std::vector<std::wstring> params);

// Unfreeze a component
Component2* unFreezeComp(Frame* f, Component2* comp, std::vector<std::wstring> params);

// End a component
Component2* endComp(Frame* f, Component2* comp, std::vector<std::wstring> params)

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
		std::wstring source;           // Script file to build VN from
		std::vector<wchar_t> ignore;   // Clean these out of the front and back of stored strings
		
		std::vector<Frame*> f;          // VN components, stored in Frame objects
		int curr_frame;                // Index of current frame in vector (-1 means empty vector)
		
		std::vector<std::wstring> err; // Error messages
		
		// Private default constructor
		VNovel() { }
	
	public:
		/* Constructor */
		VNovel(std::wstring src);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build a Visual Novel */
		
		// Add keywords to the map
		int addKW(std::wstring kw, buildComp b);
		
		// Builds VN from script file
		int buildVN();
		
		// Builds the next VN component or update an existing one
		int buildNext(int d, std::wstring compname);
		
		// Set ignored characters
		void setIgnore(std::vector<wchar_t> i);
		
		// Strips ignored characters
		std::wstring strip(std::wstring token);
		
		// Removes escape characters
		std::wstring escape(std::wstring token, std::vector<unsigned int> e);
		
		/* Reporting */
		
		// Display all error messages
		int reportErrors();
		
		/* Destructor */
		~VNovel();
};


#endif