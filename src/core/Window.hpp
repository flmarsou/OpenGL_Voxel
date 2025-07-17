#pragma once

# include "types.hpp"
# include "options.hpp"

# include <iostream>

# include <GLAD/glad.h>
# include <GLFW/glfw3.h>

class	Window
{
	public:
		void	Init(const u32 width, const u32 height, const i8 *title);

		void	SetWindowTitle(const char *title);

		bool	ShouldClose() const;
		void	Clear() const;
		void	SwapBuffers() const;
		void	PollEvents() const;

		void	Cleanup();

		GLFWwindow	*win;
};
