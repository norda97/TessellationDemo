#pragma once

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#define CHANGE_CONSOLE_ATTRIBUTE(attribute) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attribute)
#elif
#define CHANGE_CONSOLE_ATTRIBUTE(attribute)
#endif

#define LOG_ERROR(errorMsg, ...) CHANGE_CONSOLE_ATTRIBUTE(12); printf("LOG_ERROR: "); printf(errorMsg, __VA_ARGS__); printf("\n"); CHANGE_CONSOLE_ATTRIBUTE(15);
#define LOG_INFO(infoMsg, ...) CHANGE_CONSOLE_ATTRIBUTE(10); printf("LOG_INFO: "); printf(infoMsg, __VA_ARGS__); printf("\n"); CHANGE_CONSOLE_ATTRIBUTE(15);