// PG2.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.

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
#include <opencv2\opencv.hpp>

App app;

void getFPS(GLFWwindow* window) {
	double previousTime = glfwGetTime();
	int frameCount = 0;
	double elapsedTime;

	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		frameCount++;
		elapsedTime = currentTime - previousTime;
		if (elapsedTime >= 1.0) { // If last prinf() was more than 1 sec ago
			previousTime = currentTime;
			// printf and reset timer
			std::cout << (double(frameCount) / elapsedTime) << std::endl;
			frameCount = 0;
		}
	}
}


int main()
{
	// Initialize the application
	if (app.init()) {
		app.report();
		// Run the application
		return app.run();
	}
	else {
		std::cerr << "Failed to initialize the application." << std::endl;
		return -1;
	}
}