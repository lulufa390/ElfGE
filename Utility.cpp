//
// Created by 张程易 on 29/10/2017.
//

#define STB_IMAGE_IMPLEMENTATION

#include "Utility.h"

int Utility::SCREEN_WIDTH = 1024;
int Utility::SCREEN_HEIGHT = 768;
double Utility::deltaTime = 0.0;
GLFWwindow* Utility::window = nullptr;

std::string Utility::WINDOW_NAME = "ElfGameEngine";
std::string Utility::ASSETS_PREFIX = "../Assets/";