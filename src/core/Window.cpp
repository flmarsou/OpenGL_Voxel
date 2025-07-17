#include "Window.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Window::Init(const u32 width, const u32 height, const i8 *title)
{
	// GLFW: Initializes and configures
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	std::cout << SUCCESS "GLFW initialized successfully" RESET << std::endl;

	// GLFW: Window creation
	this->win = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!this->win)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create the window");
	}
	glfwMakeContextCurrent(this->win);

	std::cout << SUCCESS "Window created successfully" RESET << std::endl;

	// GLAD: Initializes
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	std::cout << SUCCESS "GLAD initialized successfully" RESET << std::endl;
}

// ========================================================================== //
//    Setters                                                                 //
// ========================================================================== //

void	Window::SetWindowTitle(const char *title)
{
	glfwSetWindowTitle(this->win, title);
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

bool	Window::ShouldClose() const
{
	return (glfwWindowShouldClose(this->win));
}

void	Window::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void	Window::SwapBuffers() const
{
	glfwSwapBuffers(this->win);
}

void	Window::PollEvents() const
{
	glfwPollEvents();
}

void	Window::Cleanup()
{
	glfwDestroyWindow(this->win);
	glfwTerminate();
}
