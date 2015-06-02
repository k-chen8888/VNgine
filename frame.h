#ifndef VNOVEL_H
#define VNOVEL_H


// Base files
#include "vn_global.h"

// L2 Component definition
#include "component2.h"


// Macros for error messages
#define EMPTY_NAME       -2
#define EMPTY_NAME_ERR   std::wstring(L"[Frame Error -2] No name given")

#define EMPTY_BG         -3
#define EMPTY_BG_ERR     std::wstring(L"[Frame Error -3] No background image file given")
#define BAD_BG           -4
#define BAD_BG_ERR       std::wstring(L"[Frame Error -4] Background image file of unsupported type")

#define EMPTY_BGM        -5
#define EMPTY_BGM_ERR    std::wstring(L"[Frame Error -5] No BGM file given")
#define BAD_BGM          -6
#define BAD_BGM_ERR      std::wstring(L"[Frame Error -6] BGM file of unsupported type")

#define EMPTY_SFX        -7
#define EMPTY_SFX_ERR    std::wstring(L"[Frame Error -7] No sound effect file given")
#define BAD_SFX          -8
#define BAD_SFX_ERR      std::wstring(L"[Frame Error -8] Sound effect file of unsupported type")

#define EMPTY_SPRITE     -9
#define EMPTY_SPRITE_ERR std::wstring(L"[Frame Error -9] No sprite file given")
#define BAD_SPRITE       -10
#define BAD_SPRITE_ERR   std::wstring(L"[Frame Error -10] Sprite file of unsupported type")

#define HAS_ACTIVE       -11
#define HAS_ACTIVE_ERR   std::wstring(L"[Frame Error -11] Tried to add an active frame, but there was already one in existance (compensating by auto-ending frame)")


#define FRZ_EMPTY        -12
#defint FRZ_EMPTY_ERR    std::wstring(L"[Frame Error -12] Unable to unfreeze component; no frozen components found")

/* Control functions */


/************************************************
 * Visual Novel Frame
 * 
 * Contents (all vectors):
 * 	Type of component (found in an external map)
 * 	Identifier for component
 * 	Display content
 ************************************************/
class Frame
{
	private:
		std::wstring name;                    // Identifying name
		unsigned int index;                   // Identifying index
		
		// Frame objects
		std::map<std::wstring, std::vector<std::wstring>> obj;
		
		// Editing variables
		Component2* active;                   // Component being edited
		std::vector<Component2*> frz;         // Vector (stack) containing frozen components and their corresponding playback function
		
		// Playback variables
		std::vector<Component2*> comp;        // Vector containing the component
		unsigned int current;                 // Index of current component being played
		
		std::vector<std::wstring> err;        // Error messages
		
		// Private default constructor
		Frame() { }
	
	public:
		/* Constructor */
		Frame(unsigned int loc);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build */
		
		// Add keywords to the map
		int addKW(std::wstring kw, buildComp b);
		
		// Add non-component data to the frame
		int setData(std::vector<std::wstring> params);
		
		// Set Frame name
		int setName(std::wstring n);
		
		// Add a background image
		int addBG(std::wstring bgfile);
		
		// Add a bgm file
		int addBGM(std::wstring bgmfile);
		
		// Add a sound effect file
		int addSFX(std::wstring sfxfile);
		
		// Add a sprite image file
		int addSprite(std::wstring spritefile);
		
		// Set a component as active for editing
		int addActiveComponent(Component2* c);
		
		// Add currently active component to the list
		// Component is no longer being edited
		int addComponent();
		
		// Freeze a component (no longer being edited)
		int freeze(Component2* c);
		
		// Unfreeze the last frozen component (the one on the top of the stack) (may be edited again)
		int unfreeze();
		
		/* Playback */
		
		// Play through components (to GUI if gui == true, otherwise, to a command line)
		// Output the index of the next frame to jump to
		unsigned int play(bool gui);
		
		// Draws the background using the images in the order specified
		int drawBG(std::vector<int> order);
		
		/* Reporting */
		
		// Get name and index
		std::pair<std::wstring, int> getID();
		
		// Check if a component is active for editing
		bool isActiveComponent(Component2* c);
		
		// Retrieve active component (the one being edited)
		Component2* getActiveComp();
		
		// Retrieve a component at some index
		Component2* getComp(unsigned int index);
		
		// Get the number of components
		unsigned int getNumComp();
		
		// Get the index of the current component
		unsigned int getCurrent();
		
		/* Destructor */
		~Frame();
};


#endif