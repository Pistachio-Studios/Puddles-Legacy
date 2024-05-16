#include "Core/Window.h"
#include "Core/App.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"

#include <pugixml.hpp>
#include <SDL.h>

Window::Window() : Module()
{
	window = NULL;
	screenSurface = NULL;
	name.Create("window");
}

Window::Window(bool startEnabled) : Module(startEnabled)
{
	window = NULL;
	screenSurface = NULL;
	name.Create("window");
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Create window
		// Tip: get the name of the child and the attribute value
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(); // get from config
		bool borderless = config.child("bordeless").attribute("value").as_bool(); // get from config
		bool resizable = config.child("resizable").attribute("value").as_bool(); // get from config
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(); // get from config
		bool screen = config.child("screen").attribute("value").as_int(); // get from config

		width = config.child("resolution").attribute("width").as_int(); //get from config 
		height = config.child("resolution").attribute("height").as_int();; //get from config 
		scale = config.child("resolution").attribute("scale").as_int();; //get from config 

		if (fullscreen == true) flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless == true) flags |= SDL_WINDOW_BORDERLESS;
		if (resizable == true) flags |= SDL_WINDOW_RESIZABLE;
		if (fullscreen_window == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_CENTERED_DISPLAY( screen ), SDL_WINDOWPOS_CENTERED_DISPLAY( screen ), width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface
			//screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint Window::GetScale() const
{
	return scale;
}

void Window::SetFullscreen(bool fullscreen)
{
	LOG("Changing to fullscreen");
	if (fullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	} else {
		SDL_SetWindowFullscreen(window, 0);
	}

	// Save the new setting to the config file
	pugi::xml_document doc;
	if (doc.load_file("config.xml")) {
		pugi::xml_node windowNode = doc.child("config").child("window");
		windowNode.child("fullscreen").attribute("value").set_value(fullscreen);
		if (fullscreen) {
			windowNode.child("borderless").attribute("value").set_value(false);
			windowNode.child("fullscreen_window").attribute("value").set_value(false);
		}
		doc.save_file("config.xml");
	}
}

void Window::SetBorderless(bool borderless)
{
	LOG("Changing to borderless");
	SDL_SetWindowBordered(window, borderless ? SDL_FALSE : SDL_TRUE);

	// Save the new setting to the config file
	pugi::xml_document doc;
	if (doc.load_file("config.xml")) {
		pugi::xml_node windowNode = doc.child("config").child("window");
		windowNode.child("borderless").attribute("value").set_value(borderless);
		if (borderless) {
			windowNode.child("fullscreen").attribute("value").set_value(false);
			windowNode.child("fullscreen_window").attribute("value").set_value(false);
		}
		doc.save_file("config.xml");
	}
}

void Window::SetFullscreenWindow(bool fullscreen_window)
{
	LOG("Changing to fullscreen window");
	if (fullscreen_window) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		SDL_SetWindowFullscreen(window, 0);
	}

	// Save the new setting to the config file
	pugi::xml_document doc;
	if (doc.load_file("config.xml")) {
		pugi::xml_node windowNode = doc.child("config").child("window");
		windowNode.child("fullscreen_window").attribute("value").set_value(fullscreen_window);
		if (fullscreen_window) {
			windowNode.child("fullscreen").attribute("value").set_value(false);
			windowNode.child("borderless").attribute("value").set_value(false);
		}
		doc.save_file("config.xml");
	}
}

void Window::SetResolution(int width, int height)
{
	LOG("Changing resolution");
	SDL_SetWindowSize(window, width, height);

	// Save the new setting to the config file
	pugi::xml_document doc;
	if (doc.load_file("config.xml")) {
		pugi::xml_node resolutionNode = doc.child("config").child("window").child("resolution");
		resolutionNode.attribute("width").set_value(width);
		resolutionNode.attribute("height").set_value(height);
		doc.save_file("config.xml");
	}
}