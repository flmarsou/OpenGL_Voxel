#include "config.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

i32	main()
{
	Window		window;
	Renderer	renderer;

	try
	{
		window.Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
		renderer.Init();
	}
	catch (const std::exception &e)
	{
		std::cerr << ERROR << e.what() << RESET << std::endl;
		return (1);
	}

	double	lastTime = glfwGetTime();
	u32		frames = 0;

	while (!window.ShouldClose())
	{
		double	currentTime = glfwGetTime();
		frames++;

		if (currentTime - lastTime >= 1.0)
		{
			double	fps = frames / (currentTime - lastTime);
			frames = 0;
			lastTime = currentTime;

			std::string	title = std::string(WINDOW_TITLE) + " | FPS: " + std::to_string((u32)fps);
			window.SetWindowTitle(title.c_str());
		}

		window.Clear();
		renderer.Render(window.win);
		window.SwapBuffers();
		window.PollEvents();

		// renderer.PrintStatistics();
	}

	renderer.Cleanup();
	window.Cleanup();

	return (0);
}
