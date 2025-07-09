#pragma once

# include <iostream>
# include <ctime>
# include <iomanip>

# define GREEN	"\e[32m"
# define BLUE	"\e[36m"
# define PURPLE	"\e[35m"
# define RED	"\e[31m"
# define RESET	"\e[0m"

enum	debugType_e
{
	SUCCESS,
	INFO,
	WARNING,
	ERROR,
};

inline void	debug(debugType_e type)
{
	std::time_t	time = std::time(nullptr);
	std::tm		*localTime = std::localtime(&time);

	switch (type)
	{
		case (SUCCESS): std::cout << GREEN; break ;
		case (INFO): std::cout << BLUE; break ;
		case (WARNING): std::cout << PURPLE; break ;
		case (ERROR): std::cout << RED; break ;
		default: break ;
	}

	std::cout << "[" << std::put_time(localTime, "%H:%M:%S") << "] " << RESET;
}
