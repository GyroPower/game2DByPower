This project of mine it's for trying to do tilemap editing and make it playable, having entities class and player input 
handling and simple 2d collision detection.

Build it with C++, OpenGL(Using GLAD), ImGui, GLFW, stb_image (for load textures)

To build it use cmake, cmake is the only dependency to install besides having C/C++ compilers and linkers in your machine
Do: cmake -G "Visual Studio 17 2022" -B ./build 

Instead of "Visual Studio 17 2022" can use of course the build system you want (Make, Ninja, etc). In the extern dir are the sources for GLAD, ImGui and stb_image 

All the files, objects, systems need it to do example in sandBox dir (where the game is running) are in game-core
except for the collision detection system (need to restructure and Fix it)

In the future it would be expanded to handle better collisions and integrate it with a collision library of your choice (like box2D), and
probably do 3D render with a 3D renderer and so on, so much work but it's for practicing and fun.

Need to add a license of use but right now it's not a big deal, feel free to clone it and expand it or redo parts, feel free to do backseating but be nice please :).
