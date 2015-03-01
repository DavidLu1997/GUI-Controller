#include "motor.h"
#include <cstdio>

#define CheckError checkError(err);

Motor::Motor(u8 Axis)
{
	//Presets
	acceleration = 10000;
	deceleration = 10000;
	velocity = 10000;
	jerk = 1000;
	zHome = 10000;
	currentPosition = 0;
	targetPosition = 0;

	//Board
	boardID = 1;
	axis = Axis;
	csr = 0;

	//Set mode
	err = flex_set_op_mode(boardID, axis, NIMC_ABSOLUTE_POSITION);
	CheckError;

	//Check if the board is at power up reset condition
	err = flex_read_csr_rtn(boardID, &csr);
	CheckError;

    if (csr & NIMC_POWER_UP_RESET ){
		flex_initialize_controller (boardID, "7350");
	}
}

//Get current location in counts
int Motor::getCurrentCounts()
{
	return currentPosition;
}

//Move motor in counts
void Motor::moveCounts(int counts)
{
	targetPosition = currentPosition + counts;
	currentPosition += counts;

	//Set Velocity
	err = flex_load_velocity(boardID, axis, velocity, 0xFF);
	CheckError;

	//Set acceleration
	err = flex_load_acceleration(boardID, axis, NIMC_ACCELERATION, acceleration, 0xFF);
	CheckError;

	//Set deceleration
	err = flex_load_acceleration(boardID, axis, NIMC_DECELERATION, deceleration, 0xFF);
	CheckError;

	//Set jerk
	err = flex_load_scurve_time(boardID, axis, jerk, 0xFF);
	CheckError;

	//Load position
	err = flex_load_target_pos(boardID, axis, targetPosition, 0xFF);
	CheckError;

	//Start move
	err = flex_start(boardID, axis, 0);
	CheckError;

	do
	{
		axisStatus = 0;

		//Check if move complete
		err = flex_check_move_complete_status(boardID, axis, 0, &moveComplete);
		CheckError;

		// Check for error
		err = flex_read_axis_status_rtn(boardID, axis, &axisStatus);
		CheckError;

		//Read communication
		err = flex_read_csr_rtn(boardID, &csr);
		CheckError;

		//Check errors if any
		if (csr & NIMC_MODAL_ERROR_MSG)
		{
			err = csr & NIMC_MODAL_ERROR_MSG;
			CheckError;
		}
	}
	//Exit on move complete or error
	while (!moveComplete && !(axisStatus & NIMC_FOLLOWING_ERROR_BIT) && !(axisStatus & NIMC_AXIS_OFF_BIT));
}

//Checks for error
void Motor::checkError(i32 err)
{

	// Check to see if there were any Modal Errors
	if (csr & NIMC_MODAL_ERROR_MSG){
		do{
			//Get the command ID, resource and the error code of the modal
			//	error from the error stack on the board
			flex_read_error_msg_rtn(boardID, &commandID, &resourceID, &errorCode);
			displayError(errorCode, commandID, resourceID);

			//Read the Communication Status Register
			flex_read_csr_rtn(boardID, &csr);

		} while (csr & NIMC_MODAL_ERROR_MSG);
	}
	else		// Display regular error 
		displayError(err, 0, 0);
}

//Finds home
void Motor::findHome()
{
	axisBitmap = (1 << axis);
	err = flex_find_reference(boardID, NIMC_AXIS_CTRL, axisBitmap, NIMC_FIND_HOME_REFERENCE);
	CheckError;
}

//Reset position, resets current position to 0
void Motor::reset()
{
	flex_reset_pos(boardID, axis, 0, 0, 0xFF);
	currentPosition = 0;
}

//Print error to display
void Motor::displayError(i32 errorCode, u16 commandID, u16 resourceID)
{
	printf("Error: %d %d %d", errorCode, commandID, resourceID);
}

//Set velocity
void Motor::setVelocity(int v)
{
	velocity = v;
}

//Set acceleration
void Motor::setAcceleration(int a)
{
	acceleration = a;
}

//Set deceleration
void Motor::setDeceleration(int d)
{
	deceleration = d;
}

//Set jerk
void Motor::setJerk(int j)
{
	jerk = j;
}