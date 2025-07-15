#include "options.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "World.hpp"

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

	while (!window.ShouldClose())
	{
		window.Clear();

		renderer.Render(window.win);

		window.SwapBuffers();
		window.PollEvents();

		renderer.PrintStatistics();
	}

	renderer.Cleanup();
	window.Cleanup();

	return (0);
}
