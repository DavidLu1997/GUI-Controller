#ifndef XBOX_H
#define XBOX_H

#include <Windows.h>
#include <XInput.h>

#pragma comment(lib, "XInput.lib")

class Controller
{
public:
	//Constructor
	Controller(int number = 1);

	//Connection checker
	bool isConnected();

	//Vibrator
	void vibrate(int leftV = 65535, int rightV = 65535);

	//Updater
	void update();
	
	//Calculate deadzone
	double deadZone(int x, int y, int type);
	double deadZone(int v, int type);

	//Analog getters
	int getLeftThumbX();
	int getLeftThumbY();
	int getRightThumbX();
	int getRightThumbY();
	int getLeftTrigger();
	int getRightTrigger();

	//Digital getters
	bool isPressedA();
	bool isPressedB();
	bool isPressedX();
	bool isPressedY();
	bool isPressedUp();
	bool isPressedDown();
	bool isPressedLeft();
	bool isPressedRight();
	bool isPressedStart();
	bool isPressedBack();
	bool isPressedLeftThumb();
	bool isPressedRightThumb();
	bool isPressedLeftButton();
	bool isPressedRightButton();

	enum{ LEFT_THUMB = 0, RIGHT_THUMB = 1, TRIGGER = 2 };

private:

	//States
	XINPUT_STATE getState();
	XINPUT_STATE controllerState;
	int controllerNum;

	//Analog states
	int leftThumbX;
	int leftThumbY;
	int rightThumbX;
	int rightThumbY;
	int leftTrigger;
	int rightTrigger;

	//Digital states
	bool a;
	bool b;
	bool x;
	bool y;
	bool up;
	bool down;
	bool left;
	bool right;
	bool start;
	bool back;
	bool leftThumb;
	bool rightThumb;
	bool leftButton;
	bool rightButton;
};

#endif