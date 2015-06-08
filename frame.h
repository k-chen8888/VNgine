#ifndef FRAME_H
#define FRAME_H


// Base files
#include "vnovel.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


// Macros for warnings
// Warnings happen when the object is unsure if erratic behavior is intended
// All warnings have the code -1
#define BAD_DELIM_WARN   std::wstring(L"[Warning] Incorrect delimiter for Frame parameters")
#define END_PLAY_WARN    std::wstring(L"[Warning] Reached end of playback")


// Macros for error messages
// Errors happen when the object is sure that behavior is wrong
#define BAD_PARAM        -2
#define BAD_PARAM_ERR    std::wstring(L"[Frame Error -2] Parameter name not found")
#define FLOATING_VAL     -3
#define FLOATING_VAL_ERR std::wstring(L"[Frame Error -3] Tried to add value without giving a parameter name")

#define EMPTY_BG         -4
#define EMPTY_BG_ERR     std::wstring(L"[Frame Error -4] No background image file given")
#define BAD_BG           -5
#define BAD_BG_ERR       std::wstring(L"[Frame Error -5] Background image file of unsupported type")

#define EMPTY_BGM        -6
#define EMPTY_BGM_ERR    std::wstring(L"[Frame Error -6] No BGM file given")
#define BAD_BGM          -7
#define BAD_BGM_ERR      std::wstring(L"[Frame Error -7] BGM file of unsupported type")

#define EMPTY_SFX        -8
#define EMPTY_SFX_ERR    std::wstring(L"[Frame Error -8] No sound effect file given")
#define BAD_SFX          -9
#define BAD_SFX_ERR      std::wstring(L"[Frame Error -9] Sound effect file of unsupported type")

#define EMPTY_SPRITE     -10
#define EMPTY_SPRITE_ERR std::wstring(L"[Frame Error -10] No sprite file given")
#define BAD_SPRITE       -11
#define BAD_SPRITE_ERR   std::wstring(L"[Frame Error -11] Sprite file of unsupported type")

#define CANNOT_FRZ       -12
#define CANNOT_FRZ_ERR   std::wstring(L"[Frame Error -12] No content to freeze")
#define FRZ_EMPTY        -13
#define FRZ_EMPTY_ERR    std::wstring(L"[Frame Error -13] Unable to unfreeze component; no frozen content found")


/* Control functions */

// Create a Frame
unsigned int makeFrame(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);

// End a Frame, closing out all frozen and active Components
unsigned int endFrame(VNovel* vn, unsigned int start, std::vector< std::pair<int, std::wstring> > params);


/************************************************
 * Visual Novel Frame
 * Container-type VN component (holds objects of type Component)
 ************************************************/
class Frame : public Container
{
	private:
		// Map that defines all possible frame parameters and provides a way to add and store them
		// wstring -> pair (supported types, file names)
		std::map< std::wstring, std::pair< std::vector<std::wstring>, std::vector<std::wstring> > > mod;
		
		// Private default constructor
		Frame() { }
	
	public:
		/* Constructor */
		Frame(std::wstring n, int i);
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		unsigned int setData(unsigned int start, std::vector< std::pair<int, std::wstring> > params)
		{
			std::wstring p = L"";
			
			// Go through parameters and fill in data
			for(int i = start; i < params.size(); i++)
			{
				if(params[i].first == CONT_PARAM)
				{
					if(this->mod.count(params[i].second) == 1)
					{
						p = params[i].second;
					}
					else
					{
						this->err.push_back(std::make_pair(BAD_PARAM, BAD_PARAM_ERR));
						return i;
					}
				}
				else if(params[i].first == PARAM_VAL)
				{
					if(p.length() > 0)
					{
						// Grab supported file types
						std::vector<std::wstring> supported = this->mod[p].first;
						
						// Attempt to add new file to list
						std::wstring filename = params[i].second;
						if(filename.length() > 0)
						{
							for(int i = 0; i < supported.size(); i++){
								if( filename.compare( filename.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
								{
									this->mod[p].second.push_back(filename);
									return 0;
								}
							}
							
							// Unsupported type error
						}
						else
						{
							// Empty string error
						}
					}
					else
					{
						this->err.push_back(std::make_pair(FLOATING_VAL, FLOATING_VAL_ERR));
						return i;
					}
				}
				else
				{
					this->err.push_back(std::make_pair(-1, BAD_DELIM_WARN));
					return i;
				}
			}
			
			return params.size() - 1;
		};
		
		// Freeze content and return how many things are frozen
		unsigned int freeze()
		{
			if(this->current < this->contents.size())
			{
				this->frz.push_back(this->current);
				this->deactivateContent();
			}
			else
			{
				this->err.push_back(std::make_pair(CANNOT_FRZ, CANNOT_FRZ_ERR));
			}
			
			return this->frz.size();
		};
		
		// Unfreeze content and return how many things are frozen
		unsigned int unfreeze()
		{
			if(this->frz.size() > 0)
			{
				if(this->current < this->contents.size())
				{
					// Cannot unfreeze while there's an active Component
				}
				else
				{
					// Reactivate frozen component
					this->current = this->frz.back();
					this->frz.pop_back();
				}
			}
			else
			{
				this->err.push_back(std::make_pair(FRZ_EMPTY, FRZ_EMPTY_ERR));
			}
			
			return this->frz.size();
		};
		
		/* Playback */
		
		// Play through components (to GUI if gui == true, otherwise, to a command line)
		// Output the index of the next Frame to jump to
		int play(bool gui)
		{
			while(this->current < this->contents.size())
			{
				this->current = this->contents[this->current]->play(gui);
			}
			
			if(this->ending < this->next.size())
			{
				// Intentionally terminated playback?
				if(this->ending >= this->next.size() - 1)
					this->err.push_back(std::make_pair(-1, END_PLAY_WARN));
				
				// Update ending and return the next Frame to play
				this->ending += 1;
				return this->next[this->ending - 1].second;
			}
			
			return -1;
		};
		
		// Reset playback
		void resetPlay()
		{
			this->current = 0;
			this->ending = 0;
			
			for(int i = 0; i < this->contents.size(); i++)
			{
				this->contents[i]->resetPlay();
			}
		};
		
		/* Destructor */
		~Frame()
		{
			for(int i = 0; i < this->contents.size(); i++)
			{
				delete this->contents[i];
			}
		};
};


#endif