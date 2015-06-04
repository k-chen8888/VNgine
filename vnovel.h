#ifndef VNOVEL_H
#define VNOVEL_H


// Base files
#include "vn_global.h"
#include "container.h"
#include "component.h"
#include "vnobject.h"


// Parts of a Visual Novel
#include "frame.h"


// Error macros
#define NO_CONT_ERR     std::wstring(L"No Container to store parameter values")
#define NO_CONT         -2

#define NO_COMP_ERR     std::wstring(L"No Component to store parameter values")
#define NO_COMP         -3

#define NO_OBJ_ERR      std::wstring(L"No VNObject to store parameter values")
#define NO_OBJ          -4

#define LOOSE_PARAM_ERR std::wstring(L"Parameter doesn't belong anywhere")
#define LOOSE_PARAM     -5

#define LOOSE_TXT_ERR   std::wstring(L"Text token doesn't belong anywhere")
#define LOOSE_TXT       -6


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
		
		std::vector<Container*> cont;  // VN components, stored in Container objects
		unsigned int curr;             // Index of current Container in vector
		
		std::vector<std::wstring> err; // Error messages
		
		// Private default constructor
		VNovel() { }
	
	public:
		/* Constructor */
		VNovel(std::wstring src)
		{
			this->source = src;
			this->curr = 0;
			
			//Set unicode input/output
			int e = setUnicode(true, true);
			if(e == SET_IN)
			{
				this->err.push_back(SET_IN_ERR);
			}
			if(e == SET_OUT)
			{
				this->err.push_back(SET_OUT_ERR);
			}
		};
		
		/*******************************************
		 * Functions
		 *******************************************/
		
		/* Build a Visual Novel */
		
		// Builds VN from script file
		int buildVN()
		{
			wifstream ifile;
			try
			{
				ifile.open(this->source);
			}
			catch(int e)
			{
				return e; // Failure
			}
			
			wstring line;
			unsigned int lcount;
			while( getline(ifile, line) )
			{
				// PDA
				PDA<wstring> script (line, this->delim);
				
				// Map lookup keywords
				vector<pair<int, wstring>> kwlist;
				
				// Start position of current token
				unsigned int start;
				
				// Locations of escape characters
				vector<unsigned int> esc;
				
				// Pull out and process the tokens
				while(script.getPos() < line.length() && script.getErr() > -1)
				{
					string token = script.readNext();
					int d = script.lastDelim();
					
					if(script.getErr() < -1)
					{
						this->err.push_back( L"[Error:L" + to_wstring(lcount) + L"] Improper formatting\n" );
					}
					else // Found a token?
					{
						if(token.length() > 0)
						{
							// Check if tokens can be added when a new opening delimiter is pushed
							if(d > 0)
							{
								// Check if the token is meaningful (enclosed in delimiters)
								if(script.lastRemoved() > 0)
								{
									// Create the final token and add it to the list
									wstring finaltoken = escape(token, esc);
									if(script.lastRemoved() != TXT_TOKEN)
										finaltoken = strip(finaltoken);
									
									kwlist.push_back( make_pair( script.lastRemoved(), finaltoken ) );
									start = script.getPos();
								}
							}
							
							// Flush stack when the sequence ends
							bool outofkw = script.stackDepth() == 0 && kwlist.size() > 0;
							bool endofline = script.getPos() >= line.length();
							if( outofkw || endofline )
							{
								unsigned int s = 0;
								while(s < kwlist.size())
								{
									// Switch on the delimiter type
									switch( kwlist[0].first )
									{
										// Container
										case CONT_OPEN:
											s = containers[ kwlist[0].second ](this, 1, kwlist);
											break;
										
										// Container parameter
										case CONT_PARAM:
											// Attempt to add Container parameters
											s = containers[ L"cont_param" ](this, 0, kwlist);
											
											// Returns the length of params on error
											if(s == params.size())
											{
												this->err.push_back(NO_CONT_ERR);
												return NO_CONT;
											}
											
											break;
										
										// Component
										case COMP_OPEN:
											s = components[ kwlist[0].second ](this, 1, kwlist);
											break;
										
										// Component parameter
										case COMP_PARAM:
											// Attempt to add Component parameters
											s = components[ L"comp_param" ](this, 0, kwlist);
											
											// Returns the length of params on error
											if(s == params.size())
											{
												this->err.push_back(NO_COMP_ERR);
												return NO_COMP;
											}
											
											break;
										
										// VNObject
										case OBJ_OPEN:
											s = vnobjects[ kwlist[0].second ](this, 1, kwlist);
											break;
										
										// VNObject parameter
										case OBJ_PARAM:
											// Attempt to add Component parameters
											s = vnobjects[ L"obj_param" ](this, 0, kwlist);
											
											// Returns the length of params on error
											if(s == params.size())
											{
												this->err.push_back(NO_OBJ_ERR);
												return NO_OBJ;
											}
											
											break;
										
										// Parameter value (misplaced)
										case PARAM_VAL:
											this->err.push_back(LOOSE_PARAM_ERR);
											return LOOSE_PARAM;
										
										// Text token (misplaced)
										case TXT_TOKEN:
											this->err.push_back(LOOSE_TXT_ERR);
											return TXT_PARAM;
										
										// Everything else
										default:
											break;
									}
									
									s += 1;
								}
								
								// No more keywords
								kwlist.clear();
							}
						}
						else
						{
							if(script.isEsc()) // Mark escape characters
								esc.push_back(script.getPos() - tstart);
						}
					}
				}
				
				lcount += 1;
			}
			
			ifile.close();
			
			return 0;
		};
		
		// Add Container to the list
		void addCont(Container* c)
		{
			this->cont.push_back(c);
		};
		
		// Deactivate Container for editing
		void deactivateCont()
		{
			if(this->curr == this->cont.size() - 1)
			{
				this->curr += 1;
			}
			else
			{
				this->curr = this->cont.size();
			}
		};
		
		/* Playback */
		
		// Plays the visual novel
		int playNovel(bool gui)
		{
			
			return 0;
		};
		
		/* Reporting */
		
		// Number of containers
		unsigned int numCont()
		{
			return this->cont.size();
		}
		
		// Index of currently active Container
		unsigned int getCurr()
		{
			return this->curr;
		};
		
		// Currently active Container
		Container* getActiveCont()
		{
			if(this->curr < this->cont.size())
				return this->cont[this->curr];
			else
				return NULL;
		}
		
		// Container at given index
		Container* getContAt(unsigned int index)
		{
			if(index < this->cont.size())
				return this->cont[index];
			else
				return NULL;
		};
		
		// Display all error messages
		int reportErrors()
		{
			for(int i = 0; i < this->err.length(); i++)
				wcout << this->err[i] << "\n";
			
			return 0;
		};
		
		/* Destructor */
		~VNovel()
		{
			for(int i = 0; i < this->cont.size(); i++)
				delete this->cont[i];
		};
};


#endif