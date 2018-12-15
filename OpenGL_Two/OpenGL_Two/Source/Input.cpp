#include "Input.h"

BUTTONSTATE Input::Button[COUNT];
bool Input::Prev[COUNT];

void Input::UpdateInputs(GLFWwindow* window, float deltaTime)
{
	//W key
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Input::Button[W] = DOWN;

		if (Prev == false)
		{
			Input::Button[W] = PRESSED;
		}
		Prev[W] = true;
	}
	else 
	{
		Input::Button[W] = UP;
		if (Prev[W] == true)
		{
			Input::Button[W] = RELEASED;
		}
		Prev[W] = false;
	}

	//A key
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Input::Button[A] = DOWN;

		if (Prev == false)
		{
			Input::Button[A] = PRESSED;
		}
		Prev[A] = true;
	}
	else
	{
		Input::Button[A] = UP;
		if (Prev[A] == true)
		{
			Input::Button[A] = RELEASED;
		}
		Prev[A] = false;
	}

	//S key
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Input::Button[S] = DOWN;

		if (Prev == false)
		{
			Input::Button[S] = PRESSED;
		}
		Prev[S] = true;
	}
	else
	{
		Input::Button[S] = UP;
		if (Prev[S] == true)
		{
			Input::Button[S] = RELEASED;
		}
		Prev[S] = false;
	}

	//D key
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Input::Button[D] = DOWN;

		if (Prev == false)
		{
			Input::Button[D] = PRESSED;
		}
		Prev[D] = true;
	}
	else
	{
		Input::Button[D] = UP;
		if (Prev[D] == true)
		{
			Input::Button[D] = RELEASED;
		}
		Prev[D] = false;
	}

	//E key
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		Input::Button[E] = DOWN;

		if (Prev == false)
		{
			Input::Button[E] = PRESSED;
		}
		Prev[E] = true;
	}
	else
	{
		Input::Button[E] = UP;
		if (Prev[E] == true)
		{
			Input::Button[E] = RELEASED;
		}
		Prev[E] = false;
	}

	//Q key
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		Input::Button[Q] = DOWN;

		if (Prev == false)
		{
			Input::Button[Q] = PRESSED;
		}
		Prev[Q] = true;
	}
	else
	{
		Input::Button[Q] = UP;
		if (Prev[Q] == true)
		{
			Input::Button[Q] = RELEASED;
		}
		Prev[Q] = false;
	}

	//debug
	/*switch (Button[BUTTON::W])
	{
		case BUTTONSTATE::DOWN:
			Debug::Log("Button DOWN");
			break;
		case BUTTONSTATE::PRESSED:
			Debug::Log("Button PRESSED");
			break;
		case BUTTONSTATE::RELEASED:
			Debug::Log("Button RELEASED");
			break;
		default:
			break;
	}*/
}

BUTTONSTATE Input::GetButtonState(BUTTON button)
{
	return Button[button];
}
