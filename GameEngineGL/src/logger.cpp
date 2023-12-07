#include "logger.h"

void log_info(std::string msg) {
	std::cout << "I : " << msg << std::endl;
}

void log_warn(std::string msg) {
	std::cout << "W : " << msg << std::endl;
}

void log_error(std::string msg) {
	std::cout << "E : " << msg << std::endl;
}

void log_debug(std::string msg) {
	std::cout << "D : " << msg << std::endl;
}