#include "Window.h"

Window::Window() : window(nullptr), context(nullptr)
{
}

Window::~Window()
{
	OnDestroy();
}

//Call to create a window and initialize libraries.
bool Window::OnCreate(std::string name_, int width_, int height_)
{
	//Initialize SDL and error check.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Debug::FatalError("Failed to initialize SDL", "Window.cpp", __LINE__);
		return false;
	}

	//Set width and height.
	width = width_;
	height = height_;

	//Set attributes that need to be set before window creation.
	SetPreAttributes();

	//Create window. Set cast name from string to c_str, then set window position (centered), dimensions (defined when called), and flag (connect window to OpenGL).
	window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	//If window failed to create, end program and print error.
	if (!window)
	{
		Debug::FatalError("Failed to create window", "Window.cpp", __LINE__);
		return false;
	}

	//Set the SDL OpenGL context for the window, then set post attributes.
	context = SDL_GL_CreateContext(window);
	SetPostAttributes();

	//Initialize GLEW then error check initialization.
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Debug::FatalError("Failed to initialize GLEW", "Window.cpp", __LINE__);
		return false;
	}

	//Enable GL depth, then print OpenGl version to console.
	glEnable(GL_DEPTH_TEST);
	
	Debug::Info("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)), "Window.cpp", __LINE__);
	//std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;

	Debug::Info("Window created", "Window.cpp", __LINE__);
	return true;
}

//Call to destroy window and clear memory used.
void Window::OnDestroy()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}

//Set attributes that must be set before window creation.

void Window::SetPreAttributes()
{
	//Set the SDL/GL profile to CORE so deprecated functions are disallowed.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//Sets OpenGl version to 4.5
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	//Turn double buffering on to allow two buffers which will improve performance.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Set the swap interval to update synchronized with the vertical retrace. This makes frames being drawn look smooth on the display rather than choppy.
	SDL_GL_SetSwapInterval(1); 
	//Set GLEW experimental to true before initialization. This fixes many bugs found when using GL functions.
	glewExperimental = GL_TRUE;
}

//Set attributes that must be set after window creation.
//Set depth buffer to 32 bits minimum.
void Window::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
}
