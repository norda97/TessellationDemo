#pragma once

#include <iostream>

#define LOG_ERROR(errorMsg, ...) printf("LOG_ERROR: "); printf(errorMsg, __VA_ARGS__); printf("\n");
#define LOG_INFO(infoMsg, ...) printf("LOG_INFO: "); printf(infoMsg, __VA_ARGS__); printf("\n");