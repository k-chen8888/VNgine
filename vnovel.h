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

// Add parameter to a frame
void addFrameParam(Frame* f, std::vector<std::pair<int, std::wstring>> params);

// End a frame, closing out all frozen and active components
void endFrame(Frame* f, std::vector<std::pair<int, std::wstring>> params);

// Freeze a component
void freezeComp(Frame* f, std::vector<std::pair<int, std::wstring>> params);

// Unfreeze a component
void unFreezeComp(Frame* f, std::vector<std::pair<int, std::wstring>> params);

// End a component
void endComp(Frame* f, std::vector<std::pair<int, std::wstring>> params);

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
		
		std::vector<Frame*> f;         // VN components, stored in Frame objects
		int curr_frame;                // Index of current frame in vector (-1 means empty vector)
		int current;                   // Index of current frame in vector (playback only)
		
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
		int buildNext(std::vector<pair<int, std::wstring>> params);
		
		/* Playback */
		
		// Plays the visual novel
		int playNovel(bool gui);
		
		/* Reporting */
		
		// Display all error messages
		int reportErrors();
		
		/* Destructor */
		~VNovel();
};


#endif