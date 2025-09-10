//
// Created by tohka on 9/9/25.
//

#ifndef OPENGLPLAYGROUND_TIMEMANAGER_H
#define OPENGLPLAYGROUND_TIMEMANAGER_H

class TimeManager
{
	double deltaTime = 0;
	double currentTime = 0;
	double lastTime = 0;
public:
	void UpdateDeltaTime(double newTime);
	double GetDeltaTime() const;
};

#endif // OPENGLPLAYGROUND_TIMEMANAGER_H
