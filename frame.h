#ifndef FRAME_H
#define FRAME_H


// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"

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


// typedef for control functions
void (*addObj)(Frame*, std::wstring);


/* Control functions */

// Create a frame
unsigned int makeFrame(VNovel* vn, unsigned int start, std::vector<std::pair<int, std::wstring>> params);

// Add a background image
void addBG(Frame* f, std::wstring bgfile);

// Add a bgm file
void addBGM(Frame* f, std::wstring bgmfile);

// Add a sound effect file
void addSFX(Frame* f, std::wstring sfxfile);

// Add a sprite image file
void addSprite(Frame* f, std::wstring spritefile);


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
				if(params[i].first == F_PARAM)
				{
					if(this->mod.count(params[i].second) == 1)
					{
						p = params[i].second;
					}
					else
					{
						wcout << L"No such frame parameter";
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
						wcout << L"No specified location for given parameter value in this frame";
						return i;
					}
				}
				else
				{
					wcout << L"[Warning] Incorrect delimiter for frame parameters";
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
		
		// Set a component as active for editing
		void addActiveComp(Component* c)
		{
			if(c != NULL)
			{
				this->contents.push_back(c);
			}
		};
		
		// Deactivate a component for editing
		void deactivateComp()
		{
			if(this->current == this->contents.size() - 1)
			{
				this->current += 1;
			}
			else
			{
				this->current = this->contents.size();
			}
		}
		
		/* Playback */
		
		// Play through components (to GUI if gui == true, otherwise, to a command line)
		// Output the index of the next frame to jump to
		int play(bool gui)
		{
			// Reset traversal
			this->current = 0;
			this->ending = 0;
			
		};
		
		/* Reporting */
		
		// Get name and index
		std::pair<std::wstring, int> getID()
		{
			std::pair<std::wstring, int> out(this->name, this->index);
			return out;
		};
		
		// Get index of active component
		unsigned int getCurrent()
		{
			return this->current;
		}
		
		// Check if a component is active for editing
		bool isActive(Component* c)
		{
			if(this->current < this->contents.size())
				return this->active == this->contents[this->current];
			else
				return false;
		};
		
		// Retrieve active component (the one being played/edited)
		Component* getActiveComp()
		{
			if(this->current < this->contents.size())
				return this->contents[this->current];
			else
				return NULL;
		};
		
		// Retrieve a component at some index
		Component* getComp(unsigned int index)
		{
			if(index < this->contents.size())
				return this->contents[index];
			else
				return NULL;
		};
		
		// Get the number of components
		unsigned int getNumComp()
		{
			return this->contents.size();
		};
		
		/* Destructor */
		~Frame()
		{
			// Nothing to do here
		};
};


#endif