#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "Debug.h"

enum BUTTON
{
	W,
	A,
	S,
	D,
	E,
	Q,
	SPACE,
	ENTER,
	COUNT
};
enum BUTTONSTATE
{
	UP,
	DOWN,
	PRESSED,
	RELEASED,
};
class Input
{
private:
	Input() {}
	static BUTTONSTATE Button[COUNT];
	static bool Prev[COUNT];
public:
	static void UpdateInputs(GLFWwindow* window, float deltaTime);
	static BUTTONSTATE GetButtonState(BUTTON button);
};

