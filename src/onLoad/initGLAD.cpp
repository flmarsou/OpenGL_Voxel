#include "onLoad.hpp"

bool	initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		debug(ERROR);
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return (false);
	}

	debug(SUCCESS);
	std::cout << "GLAD initialized successfully" << std::endl;

	return (true);
}
