#VNgine

###Frames
A <i>frame</i> is the highest-level container in a VN. Frames hold the second-level containers (components) in a vector and play through them as the VN is run

####Member variables

#####Frame elements
```
std::vector<std::string> bg;
```

A list of background images, added in the order they were defined
Supported file types:
- *.jpg
- *.png
- *.gif

Element 0 is a default blackscreen ("blackscreen.jpg")
- This is always drawn underneath

```
std::vector<std::string> bgm;
```

A list of background music files
Supported file types:
- *.mp3
- *.wav

Element 0 is a default song ("defaultsong.mp3")

```
std::vector<std::string> sfx;
```

A list of sound effect files
Supported file types:
- *.mp3
- *.wav

Element 0 is a default sound effect ("defaultsound.mp3")

```
std::vector<std::string> sprites;
```

A list of sprite files
Supported file types:
- *.png (to promote transparent backgrounds)

There are no default sprites

#####VN Components
```
std::vector<std::pair<void*, compPlayback>> comp;
unsigned int current;
```

These variables represent the second-level components contained in a frame


###Second-level Components

A <i>second-level component</i>, or simply <i>component</i>, represents the VN itself; for example, text boxes and options. Each second-level component contains:
- A vector of <i>output components</i>
- A vector containing the next components
  - This allows a second-level component to "freeze" and bounce to many other second-level components
  - The final element in this list is the next component
- An id, which is a pair consisting of its starting and ending location in the file

Each component's source file must also include a generic non-member playback function for the frame to use.

####Playback
Second-level components contain a list of <i>output components</i> that are displayed whenever the playback function is called.

Upon finishing playback, it returns an index for the next component in the list. If there are more output components to be played, it returns -1.