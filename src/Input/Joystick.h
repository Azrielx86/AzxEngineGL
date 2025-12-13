//
// Created by kaguya on 7/15/24.
//

#ifndef SHADERPLAYGROUND_JOYSTICK_H
#define SHADERPLAYGROUND_JOYSTICK_H

#include <functional>
#include <memory>
#include <array>
#include <unordered_map>

#include <GLFW/glfw3.h>

namespace Input
{

class Joystick
{
  private:
	typedef struct _button
	{
		bool pressed = false;
		std::function<void()> callback = nullptr;
	} Button;

	std::array<Button, 15> buttons;
	std::array<float, 6> axes;
	static std::unique_ptr<Joystick> instance;
	int joystickId;
	Joystick();

  public:
	Joystick(Joystick &) = delete;
	Joystick operator=(Joystick &) = delete;
	static Joystick *GetInstance();
	void Update();
	Joystick &AddCallback(int button, std::function<void()> callback);
	[[nodiscard]] bool GetButtonPress(int button);
	[[nodiscard]] float GetAxisState(int axis);
};

} // namespace Input

#endif // SHADERPLAYGROUND_JOYSTICK_H
