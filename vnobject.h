#ifndef VNOBJECT_H
#define VNOBJECT_H


/* Abstract class for a visual novel object inside a component
 */
class VNObject
{
	protected:
		// Traversal
		
		// Error messages
		std::vector<std::wstring> err;
	
	public:
		// Play the VNObject
		// Return index to the next Component
		virtual int play(bool gui) = 0;
}


#endif