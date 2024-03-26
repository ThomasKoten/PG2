// PG2.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#pragma once
#include <iostream>
#include <chrono>
#include <stack>
#include <random>

#include <GL/glew.h> 
// WGLEW = Windows GL Extension Wrangler (change for different platform) 
// platform specific functions (in this case Windows)
#include <GL/wglew.h> 

#include "App.h"
#include "Model.h"

App app;

void ahoj(int num) {
	for (size_t i = 0; i < num; i++)
	{
		std::cout << "ahoj" << '\n';
	}
}

void getFPS(GLFWwindow* window) {
	double previousTime = glfwGetTime();
	int nbFrames = 0;
	double elapsedTime;

	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		nbFrames++;
		elapsedTime = currentTime - previousTime;
		if (elapsedTime >= 1.0) { // If last prinf() was more than 1 sec ago
			previousTime = currentTime;
			// printf and reset timer
			std::cout << (double(nbFrames) / elapsedTime) << std::endl;
			nbFrames = 0;
		}
	}
}


int main()
{
	if (app.init())
		return app.run();
}