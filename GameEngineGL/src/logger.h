#pragma once

#include <iostream>

/*
  A simple logger util. Will eventually write to a file but for now just simple logs to console
*/

void log_info(std::string msg);
void log_warn(std::string msg);
void log_error(std::string msg);
void log_debug(std::string msg);