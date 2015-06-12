#ifndef FRAME_H
#define FRAME_H


// Base files
#include "../../../vnovel.h"


// Macros for error messages
// Errors happen when the object is sure that behavior is wrong
#define EMPTY_FILE       -4
#define EMPTY_FILE_ERR_1 L"[Frame Error -4] No file name given for map["
#define EMPTY_FILE_ERR_2 L"]"

#define UNSUP_FILE       -5
#define UNSUP_FILE_ERR_1 L"[Frame Error -5] Unsupported file given for map["
#define UNSUP_FILE_ERR_2 L"], "

#define CANNOT_FRZ       -6
#define CANNOT_FRZ_ERR   L"[Frame Error -6] No content to freeze"
#define FRZ_EMPTY        -7
#define FRZ_EMPTY_ERR    L"[Frame Error -7] Unable to unfreeze component; no frozen content found"


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
			for(unsigned int i = start; i < params.size(); i++)
			{
				switch(params[i].first)
				{
					case CONT_PARAM:
						if(this->mod.count(params[i].second) == 1)
						{
							p = params[i].second;
						}
						else
						{
							this->err.push_back(std::make_pair(BAD_KEY, OPEN_BRACKET + this->type + BAD_KEY_ERR));
						}
						
						break;
					
					case PARAM_VAL:
						if(p.length() > 0)
						{
							// Grab supported file types
							std::vector<std::wstring> supported = this->mod[p].first;
							
							// Attempt to add new file to list
							std::wstring filename = params[i].second;
							if(filename.length() > 0)
							{
								for(unsigned int i = 0; i < supported.size(); i++)
								{
									if( filename.compare( filename.length() - 4, 4, supported[i] ) == 0 ) // Found a supported type
									{
										this->mod[p].second.push_back(filename);
									}
									else
									{
										// Unsupported type error
										this->err.push_back(std::make_pair(UNSUP_FILE, UNSUP_FILE_ERR_1 + p + UNSUP_FILE_ERR_2 + filename));
									}
								}
							}
							else
							{
								// Empty string error
								this->err.push_back(std::make_pair(EMPTY_FILE, EMPTY_FILE_ERR_1 + p + EMPTY_FILE_ERR_2));
							}
						}
						else
						{
							this->err.push_back(std::make_pair(NO_NAME, OPEN_BRACKET + this->type + NO_NAME_ERR));
						}
						
						break;
					
					default:
						this->err.push_back(std::make_pair(-1, DELIM_WARN_1 + this->type + DELIM_WARN_2));
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
		
		// Play through Components (to GUI if gui == true, otherwise, to a command line)
		// Output the index of the next Frame to jump to
		int play(bool gui)
		{
			while(this->current < this->contents.size())
			{
				int prev = this->current;
				this->current = this->contents[this->current]->play(gui);
				
				if(this->ending < this->next.size())
				{
					// Check if Component that was just played was an ending point
					if(prev == this->next[this->ending].first)
					{
						// Update ending
						this->ending += 1;
						
						// Intentionally terminated playback?
						if(this->ending >= this->next.size() - 1)
							this->err.push_back(std::make_pair(-1, END_PLAY_WARN));
						
						// Return the next Container to play
						return this->next[this->ending - 1].second;
					}
				}
			}
			
			return -1;
		};
		
		// Reset playback
		void resetPlay()
		{
			this->current = 0;
			this->ending = 0;
			
			for(unsigned int i = 0; i < this->contents.size(); i++)
			{
				this->contents[i]->resetPlay();
			}
		};
		
		/* Destructor */
		~Frame()
		{
			for(unsigned int i = 0; i < this->contents.size(); i++)
				delete this->contents[i];
		};
};


#endif