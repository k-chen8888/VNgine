#ifndef KEYWORDS_H
#define KEYWORDS_H


// Macros for warnings
// Warnings happen when the object is unsure if erratic behavior is intended
// All warnings have the code -1
#define DELIM_WARN_1   L"[Warning] Incorrect delimiter for "
#define DELIM_WARN_2   L" parameters"
#define END_PLAY_WARN  L"[Warning] Reached end of playback"


// Macros for I/O and keyword error messages
#define SET_IN         -1
#define SET_IN_ERR     "[Error -1] Cannot set input mode to unicode"

#define SET_OUT        -2
#define SET_OUT_ERR    "[Error -2] Cannot set output mode to unicode"

#define DUP_CONT       -3
#define DUP_CONT_ERR_1 L"[Error -3] Container keyword '"
#define DUP_CONT_ERR_2 L"' already exists in the map"

#define DUP_COMP       -3
#define DUP_COMP_ERR_1 L"[Error -3] Component keyword '"
#define DUP_COMP_ERR_2 L"' already exists in the map"

#define DUP_OBJ        -4
#define DUP_OBJ_ERR_1  L"[Error -4] VNObject keyword '"
#define DUP_OBJ_ERR_2  L"' already exists in the map"


// General errors that apply to Containers, Components, and VNObjects
#define OPEN_BRACKET  L"["

#define BAD_KEY       -2
#define BAD_KEY_ERR   L" Error -2] Parameter name not found"

#define BAD_VAL       -3
#define BAD_VAL_ERR   L" Error -3] Values in this map must be of type "

#define NO_VAL        -4
#define NO_VAL_ERR    L" Error -4] Tried to modify parameter without giving value"

#define NO_NAME       -5
#define NO_NAME_ERR   L" Error -5] Tried to add value without giving a parameter name"


// Macros for default files
#define DEFAULT_BG    L"blackscreen.jpg"
#define DEFAULT_BGM   L"defaultsong.mp3"
#define DEFAULT_SFX   L"defaultsound.mp3"


// Macros for freeze/unfreeze keywords
#define FREEZE         L"freeze"
#define UNFREEZE       L"unfreeze"


// Definitions for the type of delimiter found at each position
#define CONT_OPEN  1
#define CONT_PARAM 3
#define COMP_OPEN  5
#define COMP_PARAM 7
#define OBJ_OPEN   9
#define OBJ_PARAM  11
#define PARAM_VAL  13
#define TXT_TOKEN  15


/* Add all keywords here */
void addKeywords();


#endif