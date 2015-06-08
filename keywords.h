#ifndef KEYWORDS_H
#define KEYWORDS_H


// Macros for global error messages
#define SET_IN         -1
#define SET_IN_ERR    "[Error -1] Cannot set input mode to unicode"

#define SET_OUT        -2
#define SET_OUT_ERR    "[Error -2] Cannot set output mode to unicode"

#define DUP_CONT       -3
#define DUP_CONT_ERR_1 L"[Error -3] Container keyword '"
#define DUP_CONT_ERR_2 L"' already exists in the map"

#define DUP_COMP       -3
#define DUP_COMP_ERR_1 L"[Error -3] Component keyword '"
#define DUP_COMP_ERR_2 L"' already exists in the map"

#define DUP_OBJ        -3
#define DUP_OBJ_ERR_1  L"[Error -3] VNObject keyword '"
#define DUP_OBJ_ERR_2  L"' already exists in the map"


// Macros for default files
#define DEFAULT_BG     L"blackscreen.jpg"
#define DEFAULT_BGM    L"defaultsong.mp3"
#define DEFAULT_SFX    L"defaultsound.mp3"


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