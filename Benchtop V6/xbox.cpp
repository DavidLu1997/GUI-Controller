#include "xbox.h"
#include <cmath>

Controller::Controller(int number)
{
	controllerNum = number - 1;
}

//Gets raw state
XINPUT_STATE Controller::getState()
{
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	XInputGetState(controllerNum, &controllerState);

	return controllerState;
}

//Checks for connection
bool Controller::isConnected()
{
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	DWORD Result = XInputGetState(controllerNum, &controllerState);

	if (Result == ERROR_SUCCESS)
	{
		return true;
	}

	else
	{
		return false;
	}
}

//Vibrates
void Controller::vibrate(int leftV, int rightV)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = leftV;
	vibration.wRightMotorSpeed = rightV;

	XInputSetState(controllerNum, &vibration);
}

//Updates controller state
void Controller::update()
{
	controllerState = getState();

	leftThumbX = controllerState.Gamepad.sThumbLX;
	leftThumbY = controllerState.Gamepad.sThumbLY;
	rightThumbX = controllerState.Gamepad.sThumbRX;
	rightThumbY = controllerState.Gamepad.sThumbRY;
	leftTrigger = controllerState.Gamepad.bLeftTrigger;
	rightTrigger = controllerState.Gamepad.bRightTrigger;

	a = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	b = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	x = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	y = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	up = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
	down = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
	left = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
	right = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
	start = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
	back = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
	leftThumb = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
	rightThumb = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
	leftButton = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
	rightButton = controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
}

//Returns magnitude normalized by deadzone in the form of number between 0.0 and 1.0
double Controller::deadZone(int x, int y, int type)
{
	int dead = 0;
	int threshold = 0;

	//Checks type
	switch (type)
	{
	case LEFT_THUMB:
		dead = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		threshold = 32767;
		break;
	case RIGHT_THUMB:
		dead = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		threshold = 32767;
		break;
	}

	//Get magnitude
	double magnitude = sqrt((double)x*x + y*y);

	double normalMagnitude = 0;

	//Reduces to 0.0 to 1.0
	if (magnitude > dead)
	{
		if (magnitude > threshold)
			magnitude = threshold;

		magnitude -= dead;

		normalMagnitude = magnitude / (threshold - dead);
	}

	else
	{
		normalMagnitude = 0.0;
	}

	return normalMagnitude;
}

//Returns magnitude normalized by deadzone in the form of number between 0.0 and 1.0
double Controller::deadZone(int v, int type)
{
	int dead = 0;
	int threshold = 0;

	//Checks type
	switch (type)
	{
	case LEFT_THUMB:
		dead = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		threshold = 32767;
		break;
	case RIGHT_THUMB:
		dead = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		threshold = 32767;
		break;
	case TRIGGER:
		dead = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		threshold = 255;
	}

	//Get magnitude

	if(abs(v) >= dead)
		return ((double)abs(v) - dead) / (threshold - dead);
	else
		return 0;
}

//Get left thumb x
int Controller::getLeftThumbX()
{
	return leftThumbX;
}

//Get left thumb y
int Controller::getLeftThumbY()
{
	return leftThumbY;
}

//Get right thumb x
int Controller::getRightThumbX()
{
	return rightThumbX;
}

//Get right thumb y
int Controller::getRightThumbY()
{
	return rightThumbY;
}

//Get left trigger
int Controller::getLeftTrigger()
{
	return leftTrigger;
}

//Get right trigger
int Controller::getRightTrigger()
{
	return rightTrigger;
}

//Get A
bool Controller::isPressedA()
{
	return a;
}

//Get B
bool Controller::isPressedB()
{
	return b;
}

//Get X
bool Controller::isPressedX()
{
	return x;
}

//Get Y
bool Controller::isPressedY()
{
	return y;
}

//Get Up
bool Controller::isPressedUp()
{
	return up;
}

//Get Down
bool Controller::isPressedDown()
{
	return down;
}

//Get Left
bool Controller::isPressedLeft()
{
	return left;
}

//Get Right
bool Controller::isPressedRight()
{
	return right;
}

//Get start
bool Controller::isPressedStart()
{
	return start;
}

//Get back
bool Controller::isPressedBack()
{
	return back;
}

//Get left thumb
bool Controller::isPressedLeftThumb()
{
	return leftThumb;
}

//Get right thumb
bool Controller::isPressedRightThumb()
{
	return rightThumb;
}

//Get left button
bool Controller::isPressedLeftButton()
{
	return leftButton;
}

//Get right button
bool Controller::isPressedRightButton()
{
	return rightButton;
}