/****MOTOR CLASS****/
//Drives a motor on a 1D axis
//Written by David Lu
//July 10, 2014

#ifndef MOTOR_H
#define MOTOR_H

#include "flexmotn.h"
#include <stdlib.h>

#pragma comment(lib, "FlexMS32.lib")
#pragma comment(lib, "nimotion.lib")

class Motor
{
public:
	//Constructor
	Motor(u8 Axis);

	//Moves motor (counts)
	void moveCounts(int counts);

	//Find home
	void findHome();

	//Gets current position in counts
	int getCurrentCounts();

	//Reset position
	void reset();

	//User adjustable
	void setVelocity(int v);
	void setAcceleration(int a);
	void setDeceleration(int d);
	void setJerk(int j);

private:

	//Settings
	int acceleration;
	int deceleration;
	int velocity;
	int jerk;
	int zHome;

	//Error checking
	void checkError(i32 err);
	void displayError(i32 errorCode, u16 commandID, u16 resourceID);
	i32 err;

	//User adjustable
	int targetPosition;
	int currentPosition;

	//Board handling
	u8 boardID;
	u8 axis;
	u16 csr;
	u16 axisStatus;
	u16 moveComplete;
	u16 axisBitmap;

	//Error handling
	u16 commandID;
	u16 resourceID;
	i32 errorCode;
};

#endif