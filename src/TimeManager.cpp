//
// Created by tohka on 9/9/25.
//

#include "TimeManager.h"

void TimeManager::UpdateDeltaTime(const double newTime)
{
	currentTime = newTime;
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

double TimeManager::GetDeltaTime() const
{
	return deltaTime;
}