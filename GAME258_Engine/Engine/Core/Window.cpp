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
		std::cout << "Failed to initialize SDL" << std::endl;
		return false;
	}

	//Set width and height.
	width = width_;
	height = height_;

	//Set attributes that need to be set before window creation.
	SetPreAttributes();

	//Create window. Set cast name from string to c_str, then set window position (centered), dimensions (defined when called), and flag (connect window to opengl).
	window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	//If window failed to create, end program and print error.
	if (!window)
	{
		std::cout << "Window failed to create" << std::endl;
		return false;
	}

	//Set the SDL OpenGL context for the window, then set post attributes.
	context = SDL_GL_CreateContext(window);
	SetPostAttributes();

	//Initialize glew then error check initialization.
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	//Enable gl depth, then print OpenGl version to console.
	glEnable(GL_DEPTH_TEST);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << std::endl;

	std::cout << "Window Created" << std::endl;
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
//Sets OpenGl version to 4.5
void Window::SetPreAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //Ask about this
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Ask about this
	SDL_GL_SetSwapInterval(1); //Ask about this
	glewExperimental = GL_TRUE;
}

//Set attributes that must be set after window creation.
//Set depth buffer to 32 bits.
void Window::SetPostAttributes()
{
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32); //Ask about this.
}
