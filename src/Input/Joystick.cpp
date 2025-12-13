//
// Created by kaguya on 7/15/24.
//

#include "Joystick.h"
#include <iostream>

namespace Input
{

std::unique_ptr<Joystick> Joystick::instance = nullptr;

Joystick::Joystick()
{
	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i)
	{
		if (glfwJoystickPresent(i))
		{
			joystickId = i;
			std::cout << "\033[36mJoystick " << i << " connected: " << glfwGetJoystickName(i) << std::endl;
			return;
		}
	}
	joystickId = -1;
}

Joystick *Joystick::GetInstance()
{
	if (instance == nullptr)
	{
		instance = std::unique_ptr<Joystick>(new Joystick());
	}
	return instance.get();
}

void Joystick::Update()
{
	if (joystickId == -1)
		return;

	int axesCount;
	const float *axesPtr = glfwGetJoystickAxes(joystickId, &axesCount);
	if (axesPtr)
	{
		for (int i = 0; i < axesCount && i < axes.size(); ++i)
		{
			axes[i] = axesPtr[i];
		}
	}

	int buttonCount;
	const unsigned char *buttonsPtr = glfwGetJoystickButtons(joystickId, &buttonCount);
	if (buttonsPtr)
	{
		for (int i = 0; i < buttonCount && i < buttons.size(); ++i)
		{
			if (buttonsPtr[i] == GLFW_PRESS && !buttons[i].pressed)
			{
				buttons[i].pressed = true;
				if (buttons[i].callback)
				{
					buttons[i].callback();
				}
			}
			else if (buttonsPtr[i] == GLFW_RELEASE)
			{
				buttons[i].pressed = false;
			}
		}
	}
}

Joystick &Joystick::AddCallback(int button, std::function<void()> callback)
{
	if (button >= 0 && button < buttons.size())
	{
		buttons[button].callback = std::move(callback);
	}
	return *this;
}

bool Joystick::GetButtonPress(int button)
{
	if (button >= 0 && button < buttons.size())
	{
		return buttons[button].pressed;
	}
	return false;
}

float Joystick::GetAxisState(int axis)
{
	if (axis >= 0 && axis < axes.size())
	{
		return axes[axis];
	}
	return 0.0f;
}

} // namespace Input
