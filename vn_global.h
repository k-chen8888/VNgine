#ifndef VN_GLOBAL_H
#define VN_GLOBAL_H


// L2 Component definition
#include "component2.h"


// Macros for default files
#define DEFAULT_BG  std::wstring(L"blackscreen.jpg")
#define DEFAULT_BGM std::wstring(L"defaultsong.mp3")
#define DEFAULT_SFX std::wstring(L"defaultsound.mp3")


// Macros for error messages
#define SET_IN        -1
#define SET_IN_ERR    std::string("[Error -1] Cannot set input mode to unicode")

#define SET_OUT       -2
#define SET_OUT_ERR   std::string("[Error -2] Cannot set output mode to unicode")

#define DUP_KEY       -3
#define DUP_KEY_ERR_1 std::wstring(L"[Error -3] Keyword '")
#define DUP_KEY_ERR_2 std::wstring(L"' already exists in the map")


/* typedef for generic function used to create/modify any component
 * Used to define type of function in keyword map
 * 
 * Arguments:
 * 	1. The current frame that the component belongs to
 * 	2. The object that is being edited
 * 	3. Other arguments (properties followed by saved data token)
 * 
 * Procedure (create): Happens when there are no other arguments and second argument is NULL
 * 	1. Create object
 *  2. Create a Component pointer and set it to address of new component
 * 	3. Update active component
 * 
 * Procedure (update):
 *  1. Cast the void* as the correct type
 * 	2. Look for property in component's property map
 *  	a. No property -> text token
 * 	3. Perform update
 * 	4. Update active component only if component is no longer active
 */
typedef void (*buildComp)(Frame*, std::vector<std::pair<int, std::wstring>>);


/* Global variables and special definitions */

// Keyword map
extern std::map<std::wstring, buildComp> keywords;

// Delimiter vector
extern wchar_t d[];
extern std::vector<wchar_t> delim;
// Definitions for the type of delimiter found at each position
#define COMP_OPEN 1
#define F_PARAM   3
#define L2_PARAM  5
#define L3_COMP   7
#define L3_PARAM  11
#define PARAM_VAL 13
#define TXT_TOKEN 15

// Ignored characters
extern wchar_t i[];
extern std::vector<wchar_t> ignore;


/* Edit global variables */

// Add elements to keyword map
void addToKeywords(std::wstring kw, buildComp);

// Check if key exists in map
void checkKeyword(std::wstring kw);

// Removes escape characters
std::wstring escape(std::wstring token, std::vector<unsigned int> e);

// Strips ignored characters
std::wstring strip(std::wstring token);

//Set unicode input/output
int setUnicode(bool in, bool out);


#endif