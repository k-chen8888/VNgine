#ifndef FRAME_H
#define FRAME_H


// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


// Macros for error messages
// Errors happen when the object is sure that behavior is wrong
#define BAD_PARAM        -1
#define BAD_PARAM_ERR    std::wstring(L"[Frame Error -1] Parameter name not found")
#define FLOATING_VAL     -2
#define FLOATING_VAL_ERR std::wstring(L"[Frame Error -2] Tried to add value without giving a parameter name")

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

#define CANNOT_FRZ       -11
#define CANNOT_FRZ_ERR   std::wstring(L"[Frame Error -11] No content to freeze")
#define FRZ_EMPTY        -12
#define FRZ_EMPTY_ERR    std::wstring(L"[Frame Error -12] Unable to unfreeze component; no frozen content found")


// Macros for warnings
// Warnings happen when the object is unsure if erratic behavior is intended
#define BAD_DELIM_WARN   std::wstring(L"[Warning] Incorrect delimiter for frame parameters")
#define END_PLAY_WARN    std::wstring(L"[Warning] Reached end of playback")
#define HAS_ACTIVE_WARN  std::wstring(L"[Warning] Tried to add an active frame, but there was already one in existance (compensating by auto-ending frame)")


// typedef for control functions
void (*addObj)(Frame*, std::wstring);


/* Control functions */

// Create a Frame
unsigned int makeFrame(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);

// Add a background image
void addBG(Frame* f, std::wstring bgfile);

// Add a bgm file
void addBGM(Frame* f, std::wstring bgmfile);

// Add a sound effect file
void addSFX(Frame* f, std::wstring sfxfile);

// Add a sprite image file
void addSprite(Frame* f, std::wstring spritefile);

// End a Frame, closing out all frozen and active Components
unsigned int endFrame(VNovel* vn, unsigned int start, vector<pair<int, wstring>> params);


/************************************************
 * Visual Novel Frame
 * Container-type VN component (holds objects of type Component)
 ************************************************/
template <>
class Frame : public Container<Component>
{
	private:
		// Map that defines all possible frame parameters and provides a way to add and store them
		std::map<std::wstring, std::pair<std::vector<std::wstring>, addObj>> mod;
		
		// Private default constructor
		Frame() { }
	
	public:
		/* Constructor */
		Frame(std::wstring n, int i)
		{
			//Set unicode input/output
			setUnicode(true, true);
			
			// Set identifiers
			this->name = n;
			this->index = i;
			
			// Add frame object holders to map
			this->mod[L"bg"].second = &addBG;         // Background image files
			this->mod[L"bgm"].second = &addBGM;       // Background music files
			this->mod[L"sfx"].second = &addSFX;       // Sound effects files
			this->mod[L"sprite"].second = &addSprite; // Sprite image files
			
			// Playback control
			this->current = 0;
			this->ending = 0;
		};
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build and edit */
		
		// Fill in parameters
		unsigned int setData(unsigned int start, std::vector<std::pair<int, std::wstring>> params)
		{
			wstring p = L"";
			
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
						this->err.push_back(BAD_PARAM_ERR);
						return i;
					}
				}
				else if(params[i].first == PARAM_VAL)
				{
					if(p.length() > 0)
					{
						this->mod[p](this, params[i].second);
					}
					else
					{
						this->err.push_back(FLOATING_VAL_ERR);
						return i;
					}
				}
				else
				{
					this->err.push_back(BAD_DELIM_WARN);
					return i;
				}
			}
			
			return params.size() - 1;
		};
		
		// Add a background image
		int addBG(std::wstring bgfile)
		{
			std::wstring types[] = {L".jpg", L".png", L".gif"};
			std::vector<std::wstring> supported(types, types + 3);
			
			if(bgfile.length() > 0)
			{
				for(int i = 0; i < supported.size(); i++){
					if( bgfile.compare( bgfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
					{
						this->obj['bg'].first.push_back(bgfile);
						return 0;
					}
				}
				
				// Unsupported type error
				this->err.push_back(BAD_BG_ERR + L" (" + bgfile + L")");
				return BAD_BG;
			}
			else // Empty string error
			{
				this->err.push_back(EMPTY_BG_ERR);
				return EMPTY_BG;
			}
		};
		
		// Add a bgm file
		int addBGM(std::wstring bgmfile)
		{
			std::wstring types[] = {L".mp3", L".wav"};
			std::vector<std::wstring> supported(types, types + 2);
			
			if(bgmfile.length() > 0)
			{
				for(int i = 0; i < supported.size(); i++){
					if( bgmfile.compare( bgmfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
					{
						this->obj['bgm'].first.push_back(bgmfile);
						return 0;
					}
				}
				
				// Unsupported type error
				this->err.push_back(BAD_BGM_ERR + L" (" + bgmfile + L")");
				return BAD_BGM;
			}
			else // Empty string error
			{
				this->err.push_back(EMPTY_BGM_ERR);
				return EMPTY_BGM;
			}
		};
		
		// Add a sound effect file
		int addSFX(std::wstring sfxfile);
		{
			std::wstring types[] = {L".mp3", L".wav"};
			std::vector<std::wstring> supported(types, types + 2);
			
			if(sfxfile.length() > 0)
			{
				for(int i = 0; i < supported.size(); i++){
					if( sfxfile.compare( sfxfile.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
					{
						this->obj['sfx'].first.push_back(sfxfile);
						return 0;
					}
				}
				
				// Unsupported type error
				this->err.push_back(BAD_SFX_ERR + L" (" + sfxfile + L")");
				return BAD_SFX;
			}
			else // Empty string error
			{
				this->err.push_back(EMPTY_SFX_ERR);
				return EMPTY_SFX;
			}
		};
		
		// Add a sprite image file
		int addSprite(std::wstring spritefile)
		{
			wstring supported(L".png");
			
			if(spritefile.length() > 0)
			{
				if( spritefile.compare( spritefile.length() - 4, 4, supported ) == 0 ) // Found a supported type
				{
					this->obj['sprite'].first.push_back(spritefile);
					return 0;
				}
				else
				{
					this->err.push_back(BAD_SPRITE_ERR + L" (" + spritefile + L")");
					return BAD_SPRITE; // Unsupported type error
				}
			}
			else // Empty string error
			{
				this->err.push_back(EMPTY_SPRITE_ERR);
				return EMPTY_SPRITE;
			}
		};
		
		// Freeze content and return how many things are frozen
		unsigned int freeze()
		{
			if(this->current < this->contents.size())
			{
				this->frz.push_back(this->contents[this->current]);
				this->contents.pop_back();
				this->deactivateComp();
			}
			else
			{
				this->err.push_back(CANNOT_FRZ_ERR);
			}
			
			return this->frz.size();
		};
		
		// Unfreeze content and return how many things are frozen
		unsigned int unfreeze()
		{
			if(this->frz.size() > 0)
			{
				Component* frozen = this->frz.back();
				this->current = frozen->getID();
				this->frozen.pop_back();
			}
			else
			{
				this->err.push_back(FRZ_EMPTY_ERR);
			}
			
			return this->frz.size();
		};
		
		/* Playback */
		
		// Play through components (to GUI if gui == true, otherwise, to a command line)
		// Output the index of the next frame to jump to
		int play(bool gui)
		{
			while(this->current < this->contents.size()
			{
				this->current = this->contents[this->current]->play(gui);
			}
			
			if(this->ending < this->next.size())
			{
				// Intentionally terminated playback?
				if(this->ending >= this->next.size() - 1)
					this->err.push_back(END_PLAY_WARN);
				
				this->ending += 1;
				return this->next[this->ending].first;
			}
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