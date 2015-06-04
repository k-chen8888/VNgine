#ifndef VN_GLOBAL_H
#define VN_GLOBAL_H


// L2 Component definition
#include "component2.h"


// Macros for default files
#define DEFAULT_BG  std::wstring(L"blackscreen.jpg")
#define DEFAULT_BGM std::wstring(L"defaultsong.mp3")
#define DEFAULT_SFX std::wstring(L"defaultsound.mp3")


// Macros for error messages
#define SET_IN         -1
#define SET_IN_ERR     std::string("[Error -1] Cannot set input mode to unicode")

#define SET_OUT        -2
#define SET_OUT_ERR    std::string("[Error -2] Cannot set output mode to unicode")

#define DUP_CONT       -3
#define DUP_CONT_ERR_1 std::wstring(L"[Error -3] Container keyword '")
#define DUP_CONT_ERR_2 std::wstring(L"' already exists in the map")

#define DUP_COMP       -3
#define DUP_COMP_ERR_1 std::wstring(L"[Error -3] Component keyword '")
#define DUP_COMP_ERR_2 std::wstring(L"' already exists in the map")

#define DUP_OBJ        -3
#define DUP_OBJ_ERR_1  std::wstring(L"[Error -3] VNObject keyword '")
#define DUP_OBJ_ERR_2  std::wstring(L"' already exists in the map")


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
typedef unsigned int (*buildComp)(VNovel*, unsigned int, std::vector<std::pair<int, std::wstring>>);


/* Global variables and special definitions */

// Keyword maps
extern std::map<std::wstring, buildComp> containers;
extern std::map<std::wstring, buildComp> components;
extern std::map<std::wstring, buildComp> vnobjects;

// Delimiter vector
extern wchar_t d[];
extern std::vector<wchar_t> delim;
// Definitions for the type of delimiter found at each position
#define CONT_OPEN  1
#define CONT_PARAM 3
#define COMP_OPEN  5
#define COMP_PARAM 7
#define OBJ_OPEN   9
#define OBJ_PARAM  11
#define PARAM_VAL  13
#define TXT_TOKEN  15

// Ignored characters
extern wchar_t i[];
extern std::vector<wchar_t> ignore;


/* Editing tools and helper functions */

// Add elements to keyword maps
void addToContainers(std::wstring kw, buildComp);
void addToComponents(std::wstring kw, buildComp);
void addToVNObjects(std::wstring kw, buildComp);

// Removes escape characters
std::wstring escape(std::wstring token, std::vector<unsigned int> e);

// Strips ignored characters
std::wstring strip(std::wstring token);

// String to integer
int toInt(wstring s);

//Set unicode input/output
int setUnicode(bool in, bool out);


#endif