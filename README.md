# glfwpp
**glfwpp** is a C++17 Visual Studio 2017 Solution that represents the contributor's learning
of the [OpenGL 4.5 API](https://www.khronos.org/registry/OpenGL-Refpages/gl4/)
which generally follows the chapters in the book [OpenGL SuperBible 7th Edition](http://www.openglsuperbible.com/);
 see [Amazon UK](https://www.amazon.co.uk/OpenGL-Superbible-Comprehensive-Tutorial-Reference/dp/0672337479/ref=sr_1_1?ie=UTF8&qid=1543245854&sr=8-1&keywords=opengl+superbible+comprehensive+tutorial+and+reference)  
The code in this repository is loosely based on the [example code](http://www.openglsuperbible.com/example-code/)
presented in this book and the sole contributor of this repository sincerely thank the
authors of the book for their inspiration. Some resources from the example code will also have been used.  
The [glfwpp] project and some others are, and will be, solely developed by the contributor.

**Note: This repository is under adhoc development and is not currently documented.**

The VS Solution currently comprises 5 projects:
### glfwpp
This is a static library project and is a partial implementation of a C++ wrapper for the [GLFW 3.2.1 library](http://www.glfw.org/)
and the [OpenGL 4.5 API](https://www.khronos.org/registry/OpenGL-Refpages/gl4/). It will be further developed as needed.  
Dependencies: [oglLimitsAndSettings]; [glad](https://glad.dav1d.de/); [glm](https://glm.g-truc.net/0.9.9/index.html)

### instancedCubes
Uses the [glfwpp] project to demo the instancing of a cube.

### oglLimitsAndSettings
This is a DLL project that retrieves and captures the values of a subset of the parameters as documented in [glGet](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGet.xhtml).
It uses a [def](https://msdn.microsoft.com/en-us/library/d91k01sh.aspx) file to export a C++ map of said OpenGL parameters as keys to their runtime values.  
The [glfwpp] project loads this dll dynamically to get a copy of this map. This then can be accessed by clients of [glfwpp].

### printOpenGLlimits
This is a simple console program to print out the values captured by [oglLimitsAndSettings].

### simpleTexture
Uses the [glfwpp] project to demo a simple texture.


##### Notes
Version of Visual Studio 2017 Professional: 15.9.2<br/>
[The latest Visual C++ redistributables.](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)

