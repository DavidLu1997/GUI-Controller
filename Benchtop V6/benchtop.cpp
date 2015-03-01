#include "benchtop.h"

#define DEBUG true

//Constructor
Benchtop::Benchtop(QWidget *parent)
: QMainWindow(parent)
{
	version = 6.6;

	//Sets up ui
	ui.setupUi(this);

	//Set up config
	config = new ConfigFile("benchtop.inp");

	//Set counts per mm conversion
	//Currently +- a hair
	countmm.push_back(51.5);
	countmm.push_back(51.5);
	countmm.push_back(1000);
	countmmDefault.push_back(51.5);
	countmmDefault.push_back(51.5);
	countmmDefault.push_back(1000);

	//Set copied
	copied = false;

	//Set Delay
	Delay = 100;
	//DelayDefault = 100;

	//Set multiplier
	Multiplier = 10;
	//MultiplierDefault = 10;

	//Set speed
	Speed = 10;
	//SpeedDefault = 10;

	//Set velocity
	velocity = 10000;
	//velocityDefault = 10000;

	//Set acceleration
	acceleration = 10000;
	//accelerationDefault = 10000;

	//Set deceleration
	deceleration = 10000;
	//decelerationDefault = 10000;

	//Set jerk
	jerk = 1000;
	//jerkDefault = 1000;

	//Instructions
	instruction = "Left Thumbstick: X and Y\nRight Thumbstick: Z\nDPAD Right/Left: Change Speed\nDPAD Up: Millimetres\nDPAD Down: Counts\nA: Lambda set\nB: Bregma set\nY: Start/stop recording macro\nX: Record location for macro\nStart: Play macro\nBack: Mechanical home\nRB: Set user home\nLB: Go to user home\nTriggers/Spacebar: STOP";
	
	//Not recording macro
	isRecording = false;

	//Not playing macro
	isPlaying = false;

	//Not panicing
	panicing = false;

	//Create GUI
	createMotors();
	createMacro();
	createButtons();
	createMenu();

	//Create controller
	controller = new Controller();

	//Do not enable button if not connected
	if(!controller->isConnected())
		xboxButton->setEnabled(false);

	//Layout for main GUI
	QHBoxLayout *layout = new QHBoxLayout;

	layout->addWidget(movementBox);
	layout->addWidget(manualBox);

	//Making window
	window = new QWidget;
	window->setLayout(layout);

	//Update displays
	updateMotors();
	updateDisplay();
	adjustSpeed();
	defaultSettings();
	applySettings();

	//Show
	setCentralWidget(window);
	setWindowTitle(tr("Benchtop V%1").arg(version));

	reset();
}

Benchtop::~Benchtop()
{

}

//Update locations
void Benchtop::updateMotors()
{
	//For each motor
	for (int i = 0; i < NumMotors; i++)
	{
		//If in counts
		if (counts->isChecked())
		{
			//Print to buffer integer
			sprintf(buffer, "%d", motorLocationCounts[i]);
		}

		//If in mm
		else
		{
			//Print with 2DP
			sprintf(buffer, "%.2f", motorLocationCounts[i] / countmm[i]);
		}
		
		//Set motor text
		motorCurrent[i]->setText(tr(buffer));
	}

	//Update all displays to be in counts
	if (counts->isChecked())
	{
		lambdaLocation->setText(tr("Lambda: (%1, %2, %3)").arg(lambdaLocCounts[0]).arg(lambdaLocCounts[1]).arg(lambdaLocCounts[2]));
		fromLambda->setText(tr("From Lambda: (%1, %2, %3)").arg(lambdaLocCounts[0] - motorLocationCounts[0]).arg(lambdaLocCounts[1] - motorLocationCounts[1]).arg(lambdaLocCounts[2] - motorLocationCounts[2]));
		bregmaLocation->setText(tr("Bregma: (%1, %2, %3)").arg(bregmaLocCounts[0]).arg(bregmaLocCounts[1]).arg(bregmaLocCounts[2]));
		fromBregma->setText(tr("From Bregma: (%1, %2, %3)").arg(bregmaLocCounts[0] - motorLocationCounts[0]).arg(bregmaLocCounts[1] - motorLocationCounts[1]).arg(bregmaLocCounts[2] - motorLocationCounts[2]));
		userHome->setText(tr("User Home: (%1, %2, %3)").arg(user[0]).arg(user[1]).arg(user[2]));
		//mechanicalHome->setText(tr("Machine Home: (%1, %2, %3)").arg(mech[0]).arg(mech[1]).arg(mech[2]));
	}

	//Update all displays to be in mm
	else
	{
		sprintf(buffer, "Lambda: (%.2f, %.2f, %.2f)", lambdaLocCounts[0] / countmm[0], lambdaLocCounts[1] / countmm[1], lambdaLocCounts[2] / countmm[2]);
		lambdaLocation->setText(tr(buffer));
		sprintf(buffer, "From Lambda: (%.2f, %.2f, %.2f)", (lambdaLocCounts[0] - motorLocationCounts[0]) / countmm[0], (lambdaLocCounts[1] - motorLocationCounts[1]) / countmm[1], (lambdaLocCounts[2] - motorLocationCounts[2]) / countmm[2]);
		fromLambda->setText(tr(buffer));
		sprintf(buffer, "Bregma: (%.2f, %.2f, %.2f)", bregmaLocCounts[0] / countmm[0], bregmaLocCounts[1] / countmm[1], bregmaLocCounts[2] / countmm[2]);
		bregmaLocation->setText(tr(buffer));
		sprintf(buffer, "From Bregma: (%.2f, %.2f, %.2f)", (bregmaLocCounts[0] - motorLocationCounts[0]) / countmm[0], (bregmaLocCounts[1] - motorLocationCounts[1]) / countmm[1], (bregmaLocCounts[2] - motorLocationCounts[2]) / countmm[2]);
		fromBregma->setText(tr(buffer));
		sprintf(buffer, "User Home: (%.2f, %.2f, %.2f)", user[0] / countmm[0], user[1] / countmm[1], user[2] / countmm[2]);
		userHome->setText(tr(buffer));
		//sprintf(buffer, "Machine Home: (%.2f, %.2f, %.2f)", mech[0] / countmm[0], mech[1] / countmm[1], mech[2] / countmm[2]);
		//mechanicalHome->setText(tr(buffer));
	}

	//Macro text update
	if (isPlaying)
		macroStatus->setText(tr("Playing Macro"));
	else if (isRecording)
		macroStatus->setText(tr("Recording Macro"));
	else
		macroStatus->setText(tr(""));

	//Xbox Button
	xboxButton->setEnabled(controller->isConnected());

	//Macro button update
	if(!isRecording)
	{
		macroRecordButton->setEnabled(false);
		macroStopButton->setEnabled(false);
	}

	else if(!panicing)
	{
		macroRecordButton->setEnabled(true);
		macroStopButton->setEnabled(true);
	}

	if(selectedMacro == -1 || isRecording)
	{
		macroPlayButton->setEnabled(false);
	}

	else if(!panicing)
	{
		macroPlayButton->setEnabled(true);
	}

	//Check copied
	if(copied && !counts->isChecked())
	{
		for(int i = 0; i < NumMotors; i++)
		{
			sprintf(buffer, "%.2f", motorTarget[i]->text().toDouble() / countmm[i]);
			motorTarget[i]->setText(tr(buffer));
		}

		copied = false;
	}

	if(!copied && !millimetres->isChecked())
	{
		for(int i = 0; i < NumMotors; i++)
		{
			sprintf(buffer, "%.0f", motorTarget[i]->text().toDouble() * countmm[i]);
			motorTarget[i]->setText(tr(buffer));
		}

		copied = true;
	}

	//Update window
	window->repaint();
	//Sleep(Refresh);
}

//Resets motors to home location
void Benchtop::reset()
{
	//For each motor, move to negative of its current location
	for(int i = NumMotors - 1; i >= 0; i--)
	{
		if(!DEBUG)
			motor[i]->moveCounts(-(motorLocationCounts[i]-mech[i]));
		motorLocationCounts[i] = mech[i];
	}

	//Refresh display
	updateMotors();
}

//Moves left
void Benchtop::leftM()
{
	adjustSpeed();
	move(Left);
}

//Moves right
void Benchtop::rightM()
{
	adjustSpeed();
	move(Right);
}

//Moves up
void Benchtop::upM()
{
	adjustSpeed();
	move(Up);
}

//Moves down
void Benchtop::downM()
{
	adjustSpeed();
	move(Down);
}

//Moves forward
void Benchtop::forwardM()
{
	adjustSpeed();
	move(Forward);
}

//Moves backward
void Benchtop::backwardM()
{
	adjustSpeed();
	move(Backward);
}

//Movement via xbox controller
void Benchtop::xboxM()
{
	if(!controller->isConnected())
		return;

	//Update controller state VERY IMPORTANT
	controller->update();

	//Get out if panicing
	if(panicing)
		return;

	//X

	//Adjust speed to abs of controller X position normalized to 0.0 - 1.0
	speed *= abs(controller->deadZone(controller->getLeftThumbX(), Controller::LEFT_THUMB));

	//If speed is not 0 and x is positive, move down
	if (speed != 0 && controller->getLeftThumbX() > 0)
		move(Down);

	//Otherwise, move up
	else if(speed != 0)
		move(Up);

	//Adjust speed
	adjustSpeed();

	//Y

	//Multiply speed by normalizing Y position
	speed *= abs(controller->deadZone(controller->getLeftThumbY(), Controller::LEFT_THUMB));

	//Move left
	if (speed != 0 && controller->getLeftThumbY() > 0)
		move(Left);

	//Move right
	else if(speed != 0)
		move(Right);

	adjustSpeed();

	//Z

	//Same as X and Y
	speed *= abs(controller->deadZone(controller->getRightThumbY(), Controller::RIGHT_THUMB));

	if (speed != 0 && controller->getRightThumbY() > 0)
		move(Forward);
	else if(speed != 0)
		move(Backward);

	adjustSpeed();

	/*if (controller->isPressedUp())
	{
		speed *= Multiplier;
		move(Forward);
		adjustSpeed();
	}

	if (controller->isPressedDown())
	{
		speed *= Multiplier;
		move(Backward);
		adjustSpeed();
	}*/

	//Adjust Speed down
	if (controller->isPressedLeft())
	{
		//Go through speeds
		for(int i = 0; i < NumSpeeds; i++)
		{
			//If found the one checked
			if(x[i]->isChecked())
			{
				//Uncheck it
				x[i]->setChecked(false);

				//If it is at 0, keep it
				if(i-1 < 0)
					i = 0;

				//Otherwise subtract 1
				else
					i--;

				//Set other as checked
				x[i]->setChecked(true);

				//Get out
				break;
			}
		}
		
		//Update
		adjustSpeed();
		window->repaint();
		Sleep(SpeedDelay);
	}

	//Adjust speed up
	if (controller->isPressedRight())
	{
		//Go through speeds
		for(int i = 0; i < NumSpeeds; i++)
		{
			//If found
			if(x[i]->isChecked())
			{
				//Set to false
				x[i]->setChecked(false);

				//If out of bounds, make it within bounds
				if(i+1 >= NumSpeeds)
					i = NumSpeeds-1;

				//Otherwise add one
				else
					i++;

				//Set to checked
				x[i]->setChecked(true);

				break;
			}
		}
		
		//Update
		adjustSpeed();
		window->repaint();
		Sleep(SpeedDelay);
	}

	//Up: Adjust to mm
	if(controller->isPressedUp())
	{
		millimetres->setChecked(true);
		counts->setChecked(false);

		updateDisplay();
	}

	//Down: Adjust to counts
	if(controller->isPressedDown())
	{
		millimetres->setChecked(false);
		counts->setChecked(true);

		updateDisplay();
	}

	//A: Lambda set
	if (controller->isPressedA())
	{
		setLambda();
	}

	//B: Bregma set
	if (controller->isPressedB())
	{
		setBregma();
	}

	//X: Record macro location
	if (controller->isPressedX())
	{
		macroRecord();
	}

	//Y: Start/stop macro recording
	if (controller->isPressedY())
	{
		if (!isRecording)
			macroStart();
		else
			macroStop();
	}

	//Start: Play Macro
	if (controller->isPressedStart())
	{
		macroPlay();
	}

	//Back: Mechanical home
	if (controller->isPressedBack())
	{
		reset();
	}

	//Left: Go to user home
	if (controller->isPressedLeftButton())
	{
		homeGo();
	}

	//Right: Set to user home
	if (controller->isPressedRightButton())
	{
		homeSet();
	}

	//Left Thumb Click: Select previous macro
	if (controller->isPressedLeftThumb())
	{
		macroUpdate();

		//If no macros
		if(selectedMacro == -1)
		{
			return;
		}

		//If 0, still 0
		else if (selectedMacro == 0)
		{
			selectedMacro = 0;
		}

		//Otherwise go down one
		else
		{
			selectedMacro--;
		}

		//Go through and uncheck everything
		for (int i = 0; i < macroRadio.size(); i++)
		{
			macroRadio[i]->setChecked(false);
		}

		//Check selected
		macroRadio[selectedMacro]->setChecked(true);
	}

	//Right Thumb Click: Select next macro
	if (controller->isPressedRightThumb())
	{
		macroUpdate();

		//If out of bounds, make it in bouns
		if (selectedMacro + 1 >= macroRadio.size())
		{
			selectedMacro = macroRadio.size() - 1;
		}

		//Otherwise add one
		else
		{
			selectedMacro++;
		}

		//Uncheck everything
		for (int i = 0; i < macroRadio.size(); i++)
		{
			macroRadio[i]->setChecked(false);
		}

		//Check selected
		macroRadio[selectedMacro]->setChecked(true);
	}

	//If controller triggers slammed, paic
	if (!panicing && controller->isConnected() && (controller->deadZone(controller->getLeftTrigger(), Controller::TRIGGER) > 0.8 || controller->deadZone(controller->getRightTrigger(), Controller::TRIGGER) > 0.8))
	{
		checkPanic();
	}

	//Update
	updateMotors();
}

//Checks timer, only fires when XBoxButton clicked
void Benchtop::checkTimer()
{
	//If xbox button is checked
	if (xboxButton->isChecked())
	{
		//Fires every delay
		timer->start(Delay);
	}

	//Stop firing timer
	else
	{
		timer->stop();
	}
}

//Sonication
void Benchtop::sonicate()
{
	if(controller->isConnected())
	{
		controller->vibrate();
	}

	macroStatus->setText(tr("SONICATING"));

	window->repaint();

	Sleep(SonicateDelay);

	if(controller->isConnected())
	{
		controller->vibrate(0, 0);
	}
	
	updateMotors();
}

//Update macro window
void Benchtop::macroUpdate()
{
	//Ensure agreement between integer selected and radiobutton checked
	for (int i = 0; i < macro.size(); i++)
	{
		if (macroButtons->button(i)->isChecked())
		{
			selectedMacro = i;
			break;
		}
	}

	//Print out each macro
	std::string temp = "";
	for (int i = 0; i < macro.size(); i++)
	{
		//Print current macro to string
		std::stringstream s;
		s << "Macro " << i+1 << ": ";
		temp += s.str();
		s.str("");

		//Print each location of current macro to string
		for (int j = 0; j < macro[i].size(); j++)
		{
			//If counts, use integers
			if (counts->isChecked())
				s << "(" << macro[i][j][0] << ", " << macro[i][j][1] << ", " << macro[i][j][2] << "); ";

			//Otherwise convert to mm
			else
			{
				s.precision(2);
				s << std::fixed << "(" << macro[i][j][0] / countmm[0] << ", " << macro[i][j][1] / countmm[1] << ", " << macro[i][j][2] / countmm[2] << "); ";
			}

			//Add new line
			if((j+1)% MaxMacros == 0 && j != 0)
			{
				s << std::endl;
			}

			temp += s.str();
			s.str("");
		}

		//Set text for current label
		macroRadio[i]->setText(tr(temp.c_str()));
		temp = "";
	}

	//Update
	window->repaint();
}

//Starts macro recording
void Benchtop::macroStart()
{
	//Set recording
	isRecording = true;

	//Create new button and label
	QRadioButton *tempB = new QRadioButton;
	tempB->setChecked(true);
	macroButtons->addButton(tempB);
	macroButtons->setId(tempB, macroRadio.size());
	macroRadio.push_back(tempB);
	connect(tempB, SIGNAL(clicked()), this, SLOT(macroUpdate()));

	macroLayout->addWidget(tempB);

	//Create new vector
	std::vector< std::vector<int> > temp;
	macro.push_back(temp);

	macroStartButton->setEnabled(false);
	macroPlayButton->setEnabled(false);

	//Update
	updateMotors();
	macroUpdate();
	Sleep(MacroDelay);
}

//Stops macro recording
void Benchtop::macroStop()
{
	//Stop
	isRecording = false;

	macroStartButton->setEnabled(true);
	macroPlayButton->setEnabled(true);

	//Update
	updateMotors();
	Sleep(MacroDelay);
}

//Records macro
void Benchtop::macroRecord()
{
	//If not recording, get out
	if (!isRecording)
		return;

	//Record
	std::vector<int> step;
	macro[macro.size() - 1].push_back(step);
	for (int i = 0; i < NumMotors; i++)
	{
		macro[macro.size() - 1][macro[macro.size() - 1].size() - 1].push_back(motorLocationCounts[i]);
	}

	//Update
	updateMotors();
	macroUpdate();
	Sleep(MacroDelay);
}

//Plays macro
void Benchtop::macroPlay()
{
	if (isRecording || selectedMacro == -1)
		return;

	isPlaying = true;
	updateMotors();

	//Disable buttons
	macroStartButton->setEnabled(false);
	macroRecordButton->setEnabled(false);
	macroStopButton->setEnabled(false);
	updateMotors();

	//Play
	for (int i = 0; i < macro[selectedMacro].size(); i++)
	{
		move(macro[selectedMacro][i]);
		
		sonicate();
	}

	//Update
	isPlaying = false;
	macroStartButton->setEnabled(true);
	macroRecordButton->setEnabled(true);
	macroStopButton->setEnabled(true);
	updateMotors();
	Sleep(MacroDelay);
}

//Set user home
void Benchtop::homeSet()
{
	for (int i = 0; i < NumMotors; i++)
	{
		user[i] = motorLocationCounts[i];
	}

	updateMotors();
}

//Set machine home
void Benchtop::machineSet()
{

	for(int i = 0; i < NumMotors; i++)
	{
		if(motorEnabled[i]->isChecked())
		{
			lambdaLocCounts[i] -= motorLocationCounts[i];
			bregmaLocCounts[i] -= motorLocationCounts[i];
			user[i] -= motorLocationCounts[i];
		}
	}

	for(int i = 0; i < macro.size(); i++)
	{
		for(int j = 0; j < macro[i].size(); j++)
		{
			for(int k = 0; k < macro[i][j].size(); k++)
			{
				if(motorEnabled[k]->isChecked())
					macro[i][j][k] -= motorLocationCounts[k];
			}
		}
	}

	for (int i = 0; i < NumMotors; i++)
	{
		if(motorEnabled[i]->isChecked())
		{
			motorLocationCounts[i] = 0;
			if(!DEBUG)
				motor[i]->reset();
		}
	}

	updateDisplay();
}

//Go to user home
void Benchtop::homeGo()
{
	move(user);
}

//Go to machine home
void Benchtop::machineGo()
{
	move(mech);
}

//Check for key press
void Benchtop::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
	case Qt::Key_Shift:
		panic();
		break;
	}
}
//Checks for panic
void Benchtop::checkPanic()
{
	controller->update();
	//Left/Right trigger: Panic, stop
	if (!panicing && xboxButton->isChecked() && (controller->deadZone(controller->getLeftTrigger(), Controller::TRIGGER) > 0.8 || controller->deadZone(controller->getRightTrigger(), Controller::TRIGGER) > 0.8))
	{
		panic();
	}

	/*else if(panicButton->isChecked())
	{
		panic();
	}*/
}

//Moves motors using buttons
void Benchtop::move(int direction)
{
	//Get out if panicing
	if(panicing)
		return;

	//Set how much to move, direction of movement and axis moved
	int toMove = speed;
	bool moveDirection = false;
	int moved = 0;

	//Set direction and axis based on direction given
	switch (direction)
	{
	case Left:
		moveDirection = false;
		moved = 0;
		break;
	case Right:
		moveDirection = true;
		moved = 0;
		break;
	case Up:
		moveDirection = true;
		moved = 1;
		break;
	case Down:
		moveDirection = false;
		moved = 1;
		break;

	//Multiplier on Z movement
	case Forward:
		moveDirection = true;
		moved = 2;
		toMove *= Multiplier;
		speed *= Multiplier;
		break;

	//Multiplier on Z movement
	case Backward:
		moveDirection = false;
		moved = 2;
		toMove *= Multiplier;
		speed *= Multiplier;
		break;
	}

	//Move for toMove / speed steps
	for (int i = 0; i < toMove / speed; i++)
	{
		//Get out if panicing or motor not enabled
		if (panicing || !motorEnabled[moved]->isChecked())
			return;

		//Move in positive direction
		if (moveDirection)
		{
			if(!DEBUG)
			motor[moved]->moveCounts(speed);
			motorLocationCounts[moved] += speed;
		}

		//Move in negative direction
		else
		{
			if(!DEBUG)
			motor[moved]->moveCounts(-speed);
			motorLocationCounts[moved] -= speed;
		}

		//Check for panicing
		checkPanic();

		updateMotors();
	}

	//Move remaining 
	if (moveDirection)
	{
		if(!DEBUG)
		motor[moved]->moveCounts(toMove % speed);
		motorLocationCounts[moved] += (toMove % speed);
	}

	else
	{
		if(!DEBUG)
		motor[moved]->moveCounts(-(toMove % speed));
		motorLocationCounts[moved] -= (toMove % speed);
	}

	//Update
	updateMotors();
	adjustSpeed();
}

//Moves motors according to targets set
void Benchtop::move()
{
	//Get out if panicing
	if(panicing)
		return;

	//Initialize difference and target
	int difference[NumMotors];
	int target[NumMotors];

	//Set to 0
	for (int i = 0; i < NumMotors; i++)
	{
		difference[i] = 0;
		target[i] = 0;
	}

	//Go through motors
	for (int i = 0; i < NumMotors; i++)
	{
		if(!motorEnabled[i]->isChecked())
			continue;

		//Get target in counts
		if (counts->isChecked())
		{
			target[i] = motorTarget[i]->text().toInt();
			difference[i] = target[i] - motorLocationCounts[i];
		}

		//Get target in mm
		else
		{
			target[i] = motorTarget[i]->text().toDouble() * countmm[i];
			difference[i] = target[i] - motorLocationCounts[i];
		}

		//Z multiplier
		if(i == 2)
		{
			speed *= Multiplier;
		}

		//Number of steps to move in
		for (int j = 0; j < abs(difference[i] / speed); j++)
		{
			//Get out if panicing
			if (panicing)
				return;

			//Move in positive direction
			if (difference[i] >= 0)
			{
				if(!DEBUG)
				motor[i]->moveCounts(speed);
				motorLocationCounts[i] += speed;
			}

			//Move in negative direction
			else
			{
				if(!DEBUG)
				motor[i]->moveCounts(-speed);
				motorLocationCounts[i] -= speed;
			}

			//Check for panic
			checkPanic();

			//Refresh motors if within refresh rate
			if (j % (int)(Refresh) == 0)
				updateMotors();
		}

		//Move remaining
		if (difference[i] >= 0)
		{
			if(!DEBUG)
			motor[i]->moveCounts(abs(difference[i] % speed));
			motorLocationCounts[i] += difference[i] % speed;
		}

		else
		{
			if(!DEBUG)
			motor[i]->moveCounts(-abs(difference[i] % speed));
			motorLocationCounts[i] -= abs(difference[i] % speed);
		}

		motorTarget[i]->setText(tr("0"));

		//Reset speed
		if(i == 2)
			speed /= Multiplier;
	}

	//Update
	updateMotors();
}

//Moves motors to given location
void Benchtop::move(std::vector<int> target)
{
	//Get out if panicing
	if (panicing)
		return;

	//Initialize difference
	std::vector<int> difference(NumMotors, 0);

	//Go through motors
	for (int i = 0; i < NumMotors; i++)
	{
		//Get difference in counts
		difference[i] = target[i] - motorLocationCounts[i];

		//Z multiplier
		if (i == 2)
		{
			speed *= Multiplier;
		}

		//Number of steps to move in
		for (int j = 0; j < abs(difference[i] / speed); j++)
		{
			//Get out if panicing
			if (panicing)
				return;

			//Move in positive direction
			if (difference[i] >= 0)
			{
				if (!DEBUG)
					motor[i]->moveCounts(speed);
				motorLocationCounts[i] += speed;
			}

			//Move in negative direction
			else
			{
				if (!DEBUG)
					motor[i]->moveCounts(-speed);
				motorLocationCounts[i] -= speed;
			}

			//Check for panic
			checkPanic();

			//Refresh motors if within refresh rate
			if (j % (int)(Refresh) == 0)
				updateMotors();
		}

		//Move remaining
		if (difference[i] >= 0)
		{
			if (!DEBUG)
				motor[i]->moveCounts(abs(difference[i] % speed));
			motorLocationCounts[i] += difference[i] % speed;
		}

		else
		{
			if (!DEBUG)
				motor[i]->moveCounts(-abs(difference[i] % speed));
			motorLocationCounts[i] -= abs(difference[i] % speed);
		}

		//Reset Target
		motorTarget[i]->setText(tr("0"));

		//Reset speed
		if (i == 2)
			speed /= Multiplier;
	}

	//Update
	updateMotors();
}

//Adjusts speed of motors
void Benchtop::adjustSpeed()
{
	//Update speed based on ne checked
	for (int i = 0; i < NumSpeeds; i++)
	{
		if (x[i]->isChecked())
		{
			speed = Speed * speedMultiplier[i];
			break;
		}
	}
}

//Copies locations to edit boxes
void Benchtop::copy()
{
	for(int i = 0; i < NumMotors; i++)
	{
		motorTarget[i]->setText(motorCurrent[i]->text());
	}

	copied = counts->isChecked();

	window->repaint();
}

//Sets lambda
void Benchtop::setLambda()
{
	//Update
	updateMotors();

	//Get location in counts
	for (int i = 0; i < NumMotors; i++)
	{
		lambdaLocCounts[i] = motorLocationCounts[i];
	}

	//Print to string
	sprintf(buffer, "Lambda: (%.2f, %.2f, %.2f)", lambdaLocCounts[0] / countmm[0], lambdaLocCounts[1] / countmm[1], lambdaLocCounts[2] / countmm[2]);

	//Print to text
	lambdaLocation->setText(tr(buffer));

	//Update
	updateMotors();
}

//Sets bregma
void Benchtop::setBregma()
{
	//Update
	updateMotors();

	//Set locations
	for (int i = 0; i < NumMotors; i++)
	{
		bregmaLocCounts[i] = motorLocationCounts[i];
	}

	//Print to buffer
	sprintf(buffer, "Bregma: (%.2f, %.2f, %.2f)", bregmaLocCounts[0] / countmm[0], bregmaLocCounts[1] / countmm[1], bregmaLocCounts[2] / countmm[2]);

	//Print to text
	bregmaLocation->setText(tr(buffer));

	//Update
	updateMotors();
}

//Creates movement buttons
void Benchtop::createButtons()
{
	//Button box
	buttonBox = new QGroupBox(tr("Movement Buttons"));

	//Using gridlayout
	QGridLayout *grid = new QGridLayout;

	//Up
	up = new QPushButton(tr("Up (Y)"));
	up->setAutoRepeat(true);
	connect(up, SIGNAL(clicked()), this, SLOT(upM()));
	grid->addWidget(up, 0, 1);

	//Left
	left = new QPushButton(tr("Left (X)"));
	left->setAutoRepeat(true);
	connect(left, SIGNAL(clicked()), this, SLOT(leftM()));
	grid->addWidget(left, 1, 0);

	//Right
	right = new QPushButton(tr("Right (X)"));
	right->setAutoRepeat(true);
	connect(right, SIGNAL(clicked()), this, SLOT(rightM()));
	grid->addWidget(right, 1, 2);

	//Down
	down = new QPushButton(tr("Down (Y)"));
	down->setAutoRepeat(true);
	connect(down, SIGNAL(clicked()), this, SLOT(downM()));
	grid->addWidget(down, 2, 1);

	//Forward
	forward = new QPushButton(tr("Forward (Z)"));
	forward->setAutoRepeat(true);
	connect(forward, SIGNAL(clicked()), this, SLOT(forwardM()));
	grid->addWidget(forward, 3, 2);

	//Backward
	backward = new QPushButton(tr("Backward (Z)"));
	backward->setAutoRepeat(true);
	connect(backward, SIGNAL(clicked()), this, SLOT(backwardM()));
	grid->addWidget(backward, 3, 0);

	//XBOX
	xboxButton = new QCheckBox(tr("XBOX"));
	connect(xboxButton, SIGNAL(clicked()), this, SLOT(checkTimer()));
	grid->addWidget(xboxButton, 4, 0);

	//Panic
	panicButton = new QPushButton(tr("Panic"));
	panicButton->setEnabled(false);
	connect(panicButton, SIGNAL(clicked()), this, SLOT(panic()));
	grid->addWidget(panicButton, 4, 1);

	////Machine go
	//machineGoButton = new QPushButton(tr("Go to Machine Home"));
	//connect(machineGoButton, SIGNAL(clicked()), this, SLOT(machineGo()));
	//grid->addWidget(machineGoButton, 6, 3);

	buttonBox->setLayout(grid);

	//Speed box
	speedBox = new QGroupBox(tr("Speed"));

	QHBoxLayout *speedLayout = new QHBoxLayout;

	speeds = new QButtonGroup;

	//Set speed multipliers
	speedMultiplier.push_back(0.1);
	speedMultiplier.push_back(0.5);
	speedMultiplier.push_back(1);
	speedMultiplier.push_back(5);
	speedMultiplier.push_back(10);

	//Get Speeds
	for (int i = 0; i < NumSpeeds; i++)
	{
		x[i] = new QRadioButton(tr("%1X").arg(speedMultiplier[i]));
		connect(x[i], SIGNAL(toggle()), this, SLOT(adjustSpeed()));
		speeds->addButton(x[i]);
		speedLayout->addWidget(x[i]);
	}

	x[NumSpeeds / 2]->setChecked(true);
	speeds->setExclusive(true);

	speedBox->setLayout(speedLayout);

	//Settings
	createSettings();

	//Instructions
	instructions = new QLabel(tr(instruction.c_str()));

	//Movement
	movementBox = new QGroupBox(tr("Movement"));

	QVBoxLayout *movement = new QVBoxLayout;

	movement->addWidget(buttonBox);
	movement->addWidget(speedBox);
	movement->addWidget(macroBox);

	movementBox->setLayout(movement);

	//movementBox->show();
}

//Creates motors
void Benchtop::createMotors()
{
	//Motors themselves, Y and Z swapped due to motors being swapped
	if(!DEBUG)
	{
		motor[0] = new Motor(1);
		motor[1] = new Motor(3);
		motor[2] = new Motor(2);
	}

	//Initialize vectors

	//Set locations of motors and finds home;
	for (int i = 0; i < NumMotors; i++)
	{
		motorLocationCounts.push_back(0);
		mech.push_back(0);
		lambdaLocCounts.push_back(0);
		bregmaLocCounts.push_back(0);
		user.push_back(0);
		mech.push_back(0);

		//Find home
		if(!DEBUG)
			motor[i]->findHome();
	}

	//Switches
	switchesBox = new QGroupBox(tr("Display"));

	QHBoxLayout *switchesLayout = new QHBoxLayout;

	switches = new QButtonGroup;

	//Millimetres initializing
	millimetres = new QRadioButton(tr("Millimetres"));
	connect(millimetres, SIGNAL(clicked()), this, SLOT(updateDisplay()));
	switches->addButton(millimetres);
	switches->setId(millimetres, 0);
	switchesLayout->addWidget(millimetres);
	millimetres->setChecked(true);

	//Counts initializing
	counts = new QRadioButton(tr("Counts"));
	connect(counts, SIGNAL(clicked()), this, SLOT(updateDisplay()));
	switches->addButton(counts);
	switches->setId(counts, 1);
	switchesLayout->addWidget(counts);

	switches->setExclusive(true);

	switchesBox->setLayout(switchesLayout);

	//Motorbox
	motorBox = new QGroupBox(tr("Motors"));

	//Names
	QFont *font = new QFont;
	font->setPixelSize(32);
	motorName[0] = new QLabel(tr("X"));
	motorName[0]->setFont(*font);
	motorName[1] = new QLabel(tr("Y"));
	motorName[1]->setFont(*font);
	motorName[2] = new QLabel(tr("Z"));
	motorName[2]->setFont(*font);

	//Each Axis
	QGridLayout *motorLayout = new QGridLayout;

	for (int i = 0; i < NumMotors; i++)
	{
		motorEnabled[i] = new QCheckBox;
		motorEnabled[i]->setChecked(true);
		motorCurrent[i] = new QLabel(tr("0"));
		motorCurrent[i]->setFont(*font);
		motorTarget[i] = new QLineEdit(tr("0"));
		motorTarget[i]->setMaximumWidth(MaxWidth);

		motorLayout->addWidget(motorEnabled[i], i, 0);
		motorLayout->addWidget(motorName[i], i, 1, Qt::AlignCenter);
		motorLayout->addWidget(motorCurrent[i], i, 2, Qt::AlignCenter);
		motorLayout->addWidget(motorTarget[i], i, 3);
	}

	machineSetButton = new QPushButton(tr("Reset"));
	connect(machineSetButton, SIGNAL(clicked()), this, SLOT(machineSet()));
	motorLayout->addWidget(machineSetButton, 3, 1);

	copyButton = new QPushButton(tr("Copy"));
	connect(copyButton, SIGNAL(clicked()), this, SLOT(copy()));
	motorLayout->addWidget(copyButton, 3, 2);

	//Move
	updateButton = new QPushButton(tr("Move"));
	connect(updateButton, SIGNAL(clicked()), this, SLOT(move()));
	motorLayout->addWidget(updateButton, 4, 2);

	//Reset
	resetButton = new QPushButton(tr("Home"));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	motorLayout->addWidget(resetButton, 4, 1);

	motorBox->setLayout(motorLayout);

	//Movement and recording box
	manualMovementBox = new QGroupBox(tr("Locations"));

	QGridLayout *manualLayout = new QGridLayout;

	//Lambda
	lambda = new QPushButton(tr("Lambda"));
	connect(lambda, SIGNAL(clicked()), this, SLOT(setLambda()));
	manualLayout->addWidget(lambda, 1, 0);

	//Bregma
	bregma = new QPushButton(tr("Bregma"));
	connect(bregma, SIGNAL(clicked()), this, SLOT(setBregma()));
	manualLayout->addWidget(bregma, 1, 1);

	//Initialize locations
	for (int i = 0; i < NumMotors; i++)
	{
		lambdaLocCounts[i] = 0;
		bregmaLocCounts[i] = 0;
	}

	//Lambda
	lambdaLocation = new QLabel;
	manualLayout->addWidget(lambdaLocation, 2, 0);

	//Bregma
	bregmaLocation = new QLabel;
	manualLayout->addWidget(bregmaLocation, 2, 1);

	//From Lambda
	fromLambda = new QLabel;
	manualLayout->addWidget(fromLambda, 3, 0);

	//From Bregma
	fromBregma = new QLabel;
	manualLayout->addWidget(fromBregma, 3, 1);

	//Home set
	homeSetButton = new QPushButton(tr("Set Home"));
	connect(homeSetButton, SIGNAL(clicked()), this, SLOT(homeSet()));
	manualLayout->addWidget(homeSetButton, 0, 0);

	//Home go
	homeGoButton = new QPushButton(tr("Go Home"));
	connect(homeGoButton, SIGNAL(clicked()), this, SLOT(homeGo()));
	manualLayout->addWidget(homeGoButton, 0, 1);

	//User Home
	userHome = new QLabel;
	manualLayout->addWidget(userHome, 4, 0);

	////Mechanical home
	//mechanicalHome = new QLabel;
	//manualLayout->addWidget(mechanicalHome, 4, 1);

	manualMovementBox->setLayout(manualLayout);

	//Timer
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(xboxM()));

	//Tie it all together in manualBox
	manualBox = new QGroupBox(tr("Motors"));

	QVBoxLayout *layout = new QVBoxLayout;

	layout->addWidget(switchesBox);
	layout->addWidget(motorBox);
	layout->addWidget(manualMovementBox);

	manualBox->setLayout(layout);
	//manualBox->show();
}

void Benchtop::createMacro()
{
	//New window
	macroBox = new QGroupBox(tr("Macros"));

	//New layout
	macroLayout = new QVBoxLayout;

	QHBoxLayout *macros = new QHBoxLayout;

	//Macro start
	macroStartButton = new QPushButton(tr("Start"));
	connect(macroStartButton, SIGNAL(clicked()), this, SLOT(macroStart()));
	macros->addWidget(macroStartButton);

	//Macro stop
	macroStopButton = new QPushButton(tr("Stop"));
	connect(macroStopButton, SIGNAL(clicked()), this, SLOT(macroStop()));
	macros->addWidget(macroStopButton);

	//Macro record
	macroRecordButton = new QPushButton(tr("Record"));
	connect(macroRecordButton, SIGNAL(clicked()), this, SLOT(macroRecord()));
	macros->addWidget(macroRecordButton);

	//Macro play
	macroPlayButton = new QPushButton(tr("Play"));
	connect(macroPlayButton, SIGNAL(clicked()), this, SLOT(macroPlay()));
	macros->addWidget(macroPlayButton);

	macroLayout->addLayout(macros);

	macroStatus = new QLabel(tr(""));
	macroLayout->addWidget(macroStatus);

	selectedMacro = -1;

	macroButtons = new QButtonGroup;

	//Set titles
	macroBox->setLayout(macroLayout);
}

//Creates settings box
void Benchtop::createSettings()
{
	//GridLayout
	QGridLayout *layout = new QGridLayout;

	//Delay
	delayLabel = new QLabel(tr("Polling Delay (ms): "));
	layout->addWidget(delayLabel, 0, 0);
	delayEdit = new QLineEdit(tr("%1").arg(Delay));
	delayEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(delayEdit, 0, 1);

	//Multiplier
	multiplierLabel = new QLabel(tr("Z Axis Multiplier: "));
	layout->addWidget(multiplierLabel, 1, 0);
	multiplierEdit = new QLineEdit(tr("%1").arg(Multiplier));
	multiplierEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(multiplierEdit, 1, 1);

	//Counts per mm
	countmmXLabel = new QLabel(tr("Counts per mm (X): "));
	layout->addWidget(countmmXLabel, 2, 0);
	countmmXEdit = new QLineEdit(tr("%1").arg(countmm[0]));
	countmmXEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(countmmXEdit, 2, 1);

	countmmYLabel = new QLabel(tr("Counts per mm (Y): "));
	layout->addWidget(countmmYLabel, 3, 0);
	countmmYEdit = new QLineEdit(tr("%1").arg(countmm[1]));
	countmmYEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(countmmYEdit, 3, 1);

	countmmZLabel = new QLabel(tr("Counts per mm (Z): "));
	layout->addWidget(countmmZLabel, 4, 0);
	countmmZEdit = new QLineEdit(tr("%1").arg(countmm[2]));
	countmmZEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(countmmZEdit, 4, 1);

	//Speed
	speedLabel = new QLabel(tr("Speed (counts/click)"));
	layout->addWidget(speedLabel, 5, 0);
	speedEdit = new QLineEdit(tr("%1").arg(Speed));
	speedEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(speedEdit, 5, 1);

	//Velocity
	velocityLabel = new QLabel(tr("Velocity (counts/s): "));
	layout->addWidget(velocityLabel, 6, 0);
	velocityEdit = new QLineEdit(tr("%1").arg(velocity));
	velocityEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(velocityEdit, 6, 1);

	//Acceleration
	accelerationLabel = new QLabel(tr("Acceleration (counts/s^2): "));
	layout->addWidget(accelerationLabel, 7, 0);
	accelerationEdit = new QLineEdit(tr("%1").arg(acceleration));
	accelerationEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(accelerationEdit, 7, 1);

	//Deceleration
	decelerationLabel = new QLabel(tr("Deceleration (counts/s^2): "));
	layout->addWidget(decelerationLabel, 8, 0);
	decelerationEdit = new QLineEdit(tr("%1").arg(deceleration));
	decelerationEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(decelerationEdit, 8, 1);

	//Jerk
	jerkLabel = new QLabel(tr("Jerk (counts/s^3): "));
	layout->addWidget(jerkLabel, 9, 0);
	jerkEdit = new QLineEdit(tr("%1").arg(jerk));
	jerkEdit->setMaximumWidth(MaxWidth);
	layout->addWidget(jerkEdit, 9, 1);

	//Apply
	applyButton = new QPushButton(tr("Apply"));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(applySettings()));
	layout->addWidget(applyButton, 10, 0);

	//Default
	defaultButton = new QPushButton(tr("Default"));
	connect(defaultButton, SIGNAL(clicked()), this, SLOT(defaultSettings()));
	layout->addWidget(defaultButton, 10, 1);

	//Set default
	setDefaultButton = new QPushButton(tr("Set Default"));
	connect(setDefaultButton, SIGNAL(clicked()), this, SLOT(setDefaultSettings()));
	layout->addWidget(setDefaultButton, 11, 1);

	//Widget
	settingsWindow = new QWidget();

	QPushButton *ok = new QPushButton(tr("Close"));
	connect(ok, SIGNAL(clicked()), settingsWindow, SLOT(close()));
	layout->addWidget(ok, 11, 0);

	settingsWindow->setLayout(layout);
	settingsWindow->setWindowTitle(tr("Settings"));
}

//Apply settings
void Benchtop::applySettings()
{
	//Delay
	Delay = delayEdit->text().toInt();

	if(Delay < 0)
		Delay = 0;

	timer->setInterval(Delay);

	//Multiplier
	Multiplier = multiplierEdit->text().toInt();

	if(Multiplier < 0)
		Multiplier = 0;

	//Countmm
	countmm[0] = countmmXEdit->text().toDouble();
	if(countmm[0] < 0)
		countmm[0] = 0;

	countmm[1] = countmmYEdit->text().toDouble();
	if(countmm[1] < 0)
		countmm[1] = 0;

	countmm[2] = countmmZEdit->text().toDouble();
	if(countmm[2] < 0)
		countmm[2] = 0;

	//Speed
	Speed = speedEdit->text().toInt();
	if(Speed < 0)
		Speed = 0;

	//Velocity
	velocity = velocityEdit->text().toInt();
	if(velocity < 0)
		velocity = 0;

	//Acceleration
	acceleration = accelerationEdit->text().toInt();
	if(acceleration < 0)
		acceleration = 0;

	//Deceleration
	deceleration = decelerationEdit->text().toInt();
	if(deceleration < 0)
		deceleration = 0;

	//Jerk
	jerk = jerkEdit->text().toInt();
	if(jerk < 0)
		jerk = 0;

	//Set each motor
	if(!DEBUG)
	for(int i = 0; i < NumMotors; i++)
	{
		motor[i]->setVelocity(velocity);
		motor[i]->setAcceleration(acceleration);
		motor[i]->setDeceleration(deceleration);
		motor[i]->setJerk(jerk);
	}

	//Set edits
	delayEdit->setText(tr("%1").arg(Delay));
	multiplierEdit->setText(tr("%1").arg(Multiplier));
	countmmXEdit->setText(tr("%1").arg(countmm[0]));
	countmmYEdit->setText(tr("%1").arg(countmm[1]));
	countmmZEdit->setText(tr("%1").arg(countmm[2]));
	speedEdit->setText(tr("%1").arg(Speed));
	velocityEdit->setText(tr("%1").arg(velocity));
	accelerationEdit->setText(tr("%1").arg(acceleration));
	decelerationEdit->setText(tr("%1").arg(deceleration));
	jerkEdit->setText(tr("%1").arg(jerk));

	//Update
	updateMotors();
}

//Default settings
void Benchtop::defaultSettings()
{
	//Set defaults
	delayEdit->setText(tr("%1").arg(config->read<int>("delay")));
	multiplierEdit->setText(tr("%1").arg(config->read<int>("multiplier")));
	countmmXEdit->setText(tr("%1").arg(config->read<double>("countmmX")));
	countmmYEdit->setText(tr("%1").arg(config->read<double>("countmmY")));
	countmmZEdit->setText(tr("%1").arg(config->read<double>("countmmZ")));
	speedEdit->setText(tr("%1").arg(config->read<int>("speed")));
	velocityEdit->setText(tr("%1").arg(config->read<int>("velocity")));
	accelerationEdit->setText(tr("%1").arg(config->read<int>("acceleration")));
	decelerationEdit->setText(tr("%1").arg(config->read<int>("deceleration")));
	jerkEdit->setText(tr("%1").arg(config->read<int>("jerk")));

	applySettings();
}

//Set default settings
void Benchtop::setDefaultSettings()
{
	//Delay
	config->remove("delay");
	DelayDefault = delayEdit->text().toInt();

	if(DelayDefault < 0)
		DelayDefault = 0;

	config->add("delay", DelayDefault);

	//Multiplier
	config->remove("multiplier");
	MultiplierDefault = multiplierEdit->text().toInt();

	if(MultiplierDefault < 0)
		MultiplierDefault = 0;

	config->add("multiplier", MultiplierDefault);

	//Countmm
	config->remove("countmmX");
	countmmDefault[0] = countmmXEdit->text().toDouble();

	if(countmmDefault[0] < 0)
		countmmDefault[0] = 0;

	config->add("countmmX", countmmDefault[0]);

	config->remove("countmmY");
	countmmDefault[1] = countmmYEdit->text().toDouble();

	if(countmmDefault[1] < 0)
		countmmDefault[1] = 0;

	config->add("countmmY", countmmDefault[1]);

	config->remove("countmmZ");
	countmmDefault[2] = countmmZEdit->text().toDouble();

	if(countmmDefault[2] < 0)
		countmmDefault[2] = 0;

	config->add("countmmZ", countmmDefault[2]);

	//Speed
	config->remove("speed");
	SpeedDefault = speedEdit->text().toInt();

	if(SpeedDefault < 0)
		SpeedDefault = 0;

	config->add("speed", SpeedDefault);

	//Velocity
	config->remove("velocity");
	velocityDefault = velocityEdit->text().toInt();

	if(velocityDefault < 0)
		velocityDefault = 0;

	config->add("velocity", velocityDefault);

	//Acceleration
	config->remove("acceleration");
	accelerationDefault = accelerationEdit->text().toInt();

	if(accelerationDefault < 0)
		accelerationDefault = 0;

	config->add("acceleration", accelerationDefault);

	//Deceleration
	config->remove("deceleration");
	decelerationDefault = decelerationEdit->text().toInt();

	if(decelerationDefault < 0)
		decelerationDefault = 0;

	config->add("deceleration", decelerationDefault);

	//Jerk
	config->remove("jerk");
	jerkDefault = jerkEdit->text().toInt();

	if(jerkDefault < 0)
		jerkDefault = 0;

	config->add("jerk", jerkDefault);

	std::fstream file("benchtop.inp", std::fstream::out);
	file << *config;
	file.close();

	applySettings();
}

//Updating display, updates everything
void Benchtop::updateDisplay()
{
	//Update everything
	updateMotors();
	macroUpdate();
}

//PANIC
void Benchtop::panic()
{
	//Set panic
	panicing = !panicing;

	//IF PANICING, disable everything
	if (panicing)
	{
		//Vibrate if controller connected
		if (controller->isConnected())
			controller->vibrate();

		//DO NOT Reset to home
		//();

		motorTarget[0]->setEnabled(false);
		motorTarget[1]->setEnabled(false);
		motorTarget[2]->setEnabled(false);

		left->setEnabled(false);
		right->setEnabled(false);
		up->setEnabled(false);
		down->setEnabled(false);
		forward->setEnabled(false);
		backward->setEnabled(false);
		resetButton->setEnabled(false);
		updateButton->setEnabled(false);
		macroStartButton->setEnabled(false);
		macroRecordButton->setEnabled(false);
		macroStopButton->setEnabled(false);
		macroPlayButton->setEnabled(false);
		homeSetButton->setEnabled(false);
		homeGoButton->setEnabled(false);
		machineSetButton->setEnabled(false);
		//machineGoButton->setEnabled(false);
		copyButton->setEnabled(false);

		xboxButton->setEnabled(false);
		lambda->setEnabled(false);
		bregma->setEnabled(false);

		applyButton->setEnabled(false);
		defaultButton->setEnabled(false);
		setDefaultButton->setEnabled(false);
		delayEdit->setEnabled(false);
		multiplierEdit->setEnabled(false);
		countmmXEdit->setEnabled(false);
		countmmYEdit->setEnabled(false);
		countmmZEdit->setEnabled(false);
		speedEdit->setEnabled(false);
		velocityEdit->setEnabled(false);
		accelerationEdit->setEnabled(false);
		decelerationEdit->setEnabled(false);
		jerkEdit->setEnabled(false);

		//Only enable panic button
		panicButton->setEnabled(true);

		for (int i = 0; i < NumSpeeds; i++)
		{
			x[i]->setEnabled(false);
		}

		counts->setEnabled(false);
		millimetres->setEnabled(false);

		for (int i = 0; i < macroRadio.size(); i++)
		{
			macroRadio[i]->setEnabled(false);
		}

		//Update
		window->repaint();
		Sleep(PanicDelay);

		//Stop vibration
		if (controller->isConnected())
			controller->vibrate(0, 0);
	}

	//NOT PANICING, enable everything
	else
	{
		motorTarget[0]->setEnabled(true);
		motorTarget[1]->setEnabled(true);
		motorTarget[2]->setEnabled(true);

		left->setEnabled(true);
		right->setEnabled(true);
		up->setEnabled(true);
		down->setEnabled(true);
		forward->setEnabled(true);
		backward->setEnabled(true);
		resetButton->setEnabled(true);
		updateButton->setEnabled(true);
		macroStartButton->setEnabled(true);
		macroRecordButton->setEnabled(true);
		macroStopButton->setEnabled(true);
		macroPlayButton->setEnabled(true);
		homeSetButton->setEnabled(true);
		homeGoButton->setEnabled(true);
		machineSetButton->setEnabled(true);
		//machineGoButton->setEnabled(true);
		copyButton->setEnabled(true);

		xboxButton->setEnabled(true);
		lambda->setEnabled(true);
		bregma->setEnabled(true);

		applyButton->setEnabled(true);
		defaultButton->setEnabled(true);
		setDefaultButton->setEnabled(true);
		delayEdit->setEnabled(true);
		multiplierEdit->setEnabled(true);
		countmmXEdit->setEnabled(true);
		countmmYEdit->setEnabled(true);
		countmmZEdit->setEnabled(true);
		speedEdit->setEnabled(true);
		velocityEdit->setEnabled(true);
		accelerationEdit->setEnabled(true);
		decelerationEdit->setEnabled(true);
		jerkEdit->setEnabled(true);

		for (int i = 0; i < NumSpeeds; i++)
		{
			x[i]->setEnabled(true);
		}

		counts->setEnabled(true);
		millimetres->setEnabled(true);

		for (int i = 0; i < macroRadio.size(); i++)
		{
			macroRadio[i]->setEnabled(true);
		}

		//Disable panic button
		panicButton->setEnabled(false);
	}
}

//Creates menu
void Benchtop::createMenu()
{
	//File menu
	fileMenu = menuBar()->addMenu(tr("File"));

	panicAction = new QAction(tr("Panic"), this);
	panicAction->setShortcut(Qt::Key_Space);
	connect(panicAction, SIGNAL(triggered()), this, SLOT(panic()));

	refreshAction = new QAction(tr("Refresh"), this);
	refreshAction->setShortcut(Qt::Key_F5);
	connect(refreshAction, SIGNAL(triggered()), this, SLOT(updateDisplay()));

	exitAction = new QAction(tr("Exit"), this);
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	fileMenu->addAction(panicAction);
	fileMenu->addAction(refreshAction);
	fileMenu->addAction(exitAction);

	//Settings menu
	settingsMenu = menuBar()->addMenu(tr("Settings"));

	settingsAction = new QAction(tr("Edit Settings"), this);
	connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));

	settingsMenu->addAction(settingsAction);

	//Help menu
	helpMenu = menuBar()->addMenu(tr("Help"));

	//Instructions
	instructionsAction = new QAction(tr("Instructions"), this);
	connect(instructionsAction, SIGNAL(triggered()), this, SLOT(showInstructions()));

	//Create instructions
	instructionsWindow = new QWidget();

	QGridLayout *layout = new QGridLayout;

	layout->addWidget(instructions, 0, 0, Qt::AlignCenter);

	QPushButton *ok = new QPushButton(tr("Close"));
	connect(ok, SIGNAL(clicked()), instructionsWindow, SLOT(close()));
	layout->addWidget(ok, 1, 0);

	instructionsWindow->setLayout(layout);
	instructionsWindow->resize(360, 240);
	instructionsWindow->setWindowTitle(tr("Controller Instructions"));

	//About
	aboutAction = new QAction(tr("About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

	//Create about
	aboutWindow = new QWidget();

	QGridLayout *layout1 = new QGridLayout;

	about = new QLabel(tr("Benchtop V%1\nMade by David Lu\nJuly 18, 2013").arg(version));
	layout1->addWidget(about, 0, 0, Qt::AlignCenter);
	QPushButton *ok1 = new QPushButton(tr("Close"));
	connect(ok1, SIGNAL(clicked()), aboutWindow, SLOT(close()));
	layout1->addWidget(ok1, 1, 0);

	aboutWindow->setLayout(layout1);
	aboutWindow->resize(240, 120);
	aboutWindow->setWindowTitle(tr("About"));

	helpMenu->addAction(instructionsAction);
	helpMenu->addAction(aboutAction);
}

//Show settings
void Benchtop::showSettings()
{
	settingsWindow->show();
}

//Show instructions
void Benchtop::showInstructions()
{
	instructionsWindow->show();
}

//Show about
void Benchtop::showAbout()
{
	aboutWindow->show();
}