#ifndef BENCHTOP_H
#define BENCHTOP_H

#include <QtGui/QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QTimer>
#include <QBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdio>
#include "ui_benchtop.h"
#include "Motor.h"
#include "xbox.h"
#include "ConfigFile.h"
#include <Windows.h>
#include <fstream>
#include <iomanip>

class Benchtop : public QMainWindow
{
	Q_OBJECT

public:
	Benchtop(QWidget *parent = 0);
	~Benchtop();

private slots:

	//Resets to 'home' position
	void reset();

	//Moves motors according to target set
	void move();

	//Checks speed and resets it to what is selected
	void adjustSpeed();

	//Sets lambda location
	void setLambda();

	//Sets bregma location
	void setBregma();

	//***********SLOTS FOR DIRECTIONAL MOVEMENT*************
	void leftM();
	void rightM();
	void upM();
	void downM();
	void forwardM();
	void backwardM();

	//Xbox movement
	void xboxM();

	//Timer to check Xbox controller
	void checkTimer();

	//Updates display in motor group box
	void updateMotors();

	//**************SLOTS FOR MACROS***************
	void macroStart();
	void macroStop();
	void macroRecord();
	void macroPlay();
	void macroUpdate();

	//Sets user home position
	void homeSet();

	//Go to user home position
	void homeGo();

	//Sets mechanical home
	void machineSet();

	//Go to mechanical home
	void machineGo();

	//Updates display of home locations, etc.
	void updateDisplay();

	//Checks if motors should panic
	void checkPanic();

	//Panics
	void panic();

	//Applys settings
	void applySettings();
	void defaultSettings();
	void setDefaultSettings();

	//Copy
	void copy();

	//Show widgets
	void showSettings();
	void showInstructions();
	void showAbout();

protected:
	//For panic, DOES NOT WORK
	void keyPressEvent(QKeyEvent *e);

private:

	//Move with a direction, called from previous move function
	void move(int direction);

	//Moves to location given
	void move(std::vector<int> target);

	//************CREATES ALL GUI COMPONENTS***********************
	void createButtons();
	void createMotors();
	void createMacro();
	void createSettings();

	//Sonication
	void sonicate();

	Ui::BenchtopClass ui;

	//******************USER CONTROL***************
	
	//Xbox poll delay
	QLabel *delayLabel;
	QLineEdit *delayEdit;
	int Delay;
	int DelayDefault;
	
	//Z multiplier
	QLabel *multiplierLabel;
	QLineEdit *multiplierEdit;
	int Multiplier;
	int MultiplierDefault;

	//Counts per mm
	QLabel *countmmXLabel;
	QLineEdit *countmmXEdit;
	QLabel *countmmYLabel;
	QLineEdit *countmmYEdit;
	QLabel *countmmZLabel;
	QLineEdit *countmmZEdit;
	std::vector<double> countmm;
	std::vector<double> countmmDefault;

	//Speed in counts per click
	QLabel *speedLabel;
	QLineEdit *speedEdit;
	int Speed;
	int SpeedDefault;

	//Velocity in counts/s
	QLabel *velocityLabel;
	QLineEdit *velocityEdit;
	int velocity;
	int velocityDefault;

	//Acceleration in counts/s^2
	QLabel *accelerationLabel;
	QLineEdit *accelerationEdit;
	int acceleration;
	int accelerationDefault;

	//Deceleration in counts/s^2
	QLabel *decelerationLabel;
	QLineEdit *decelerationEdit;
	int deceleration;
	int decelerationDefault;

	//Jerk in counts/s^3
	QLabel *jerkLabel;
	QLineEdit *jerkEdit;
	int jerk;
	int jerkDefault;

	//Button
	QPushButton *applyButton;
	QPushButton *defaultButton;
	QPushButton *setDefaultButton;

	//File
	ConfigFile *config;

	//*******************ENUMS*******************

	//Number of motors
	enum { NumMotors = 3 };

	//Number of speeds
	enum { NumSpeeds = 5 };

	//Directions
	enum { Left = 1, Right = 2, Up = 3, Down = 4, Forward = 5, Backward = 6 };

	//Delay for xbox controller scanning, how many movements per refresh, buffer size for display components, delay between macro selection
	enum { Refresh = 1, Buffer = 200, MacroDelay = 500, SonicateDelay };

	//Speed of motors at counts per click, velocity of motor in counts per second, multiplier on Z axis movement, mandatory pause after panic, delay between changing speeds
	enum { PanicDelay = 1000, SpeedDelay = 200};

	//Millimetres and counts
	enum { MM = false, COUNT = true};

	//Maximum width of edits
	enum { MaxWidth = 100};

	//Maximum macros per line
	enum {MaxMacros = 3};

	//Version
	double version;

	//Instructions for xbox
	std::string instruction;

	//Motors themselves
	Motor *motor[NumMotors];

	//Xbox controller
	Controller *controller;
	QLabel *instructions;

	//Location of motors in counts
	std::vector<int> motorLocationCounts;

	//Buffer for conversion
	char buffer[Buffer];

	//Panicing
	bool panicing;

	QWidget *window;

	//Boxes for sorting purposes
	QGroupBox *movementBox;
	QGroupBox *buttonBox;
	QGroupBox *motorBox;
	QGroupBox *manualBox;
	QGroupBox *axes[NumMotors];
	QGroupBox *speedBox;
	QGroupBox *manualMovementBox;
	QGroupBox *switchesBox;
	QGroupBox *macroBox;

	//Motor labels and targets, all go into respective axes
	QLabel *motorCurrent[NumMotors];
	QLabel *motorName[NumMotors];
	QLineEdit *motorTarget[NumMotors];
	QPushButton *copyButton;
	bool copied;


	//Buttons for move, and reset (to home)
	QPushButton *left;
	QPushButton *right;
	QPushButton *up;
	QPushButton *down;
	QPushButton *forward;
	QPushButton *backward;
	QPushButton *resetButton;
	QPushButton *updateButton;
	QPushButton *macroStartButton;
	QPushButton *macroRecordButton;
	QPushButton *macroStopButton;
	QPushButton *macroPlayButton;
	QPushButton *homeSetButton;
	QPushButton *homeGoButton;
	QPushButton *machineSetButton;
	QPushButton *machineGoButton;
	QPushButton *panicButton;
	QPushButton *refreshButton;

	//Which motors are on
	QCheckBox *motorEnabled[NumMotors];

	//Check boxes
	QCheckBox *xboxButton;
	QTimer *timer;

	//Buttons to record lambda/bregma
	QPushButton *lambda;
	QPushButton *bregma;

	//Labels and storage for lambda/bregma
	QLabel *lambdaLocation;
	QLabel *fromLambda;
	std::vector<int> lambdaLocCounts;
	QLabel *bregmaLocation;
	QLabel *fromBregma;
	std::vector<int> bregmaLocCounts;

	//Speed
	QButtonGroup *speeds;
	QRadioButton *x[NumSpeeds];
	std::vector<double> speedMultiplier;
	int speed;

	//Counts or mm
	QButtonGroup *switches;
	QRadioButton *counts;
	QRadioButton *millimetres;

	//Macro storage
	bool isRecording;
	std::vector< std::vector < std::vector<int> > > macro;
	QButtonGroup *macroButtons;
	QVBoxLayout *macroLayout;
	QLabel *macroStatus;
	bool isPlaying;
	int selectedMacro;
	std::vector<QRadioButton*> macroRadio;

	//Mechanical home
	std::vector<int> mech;
	QLabel *mechanicalHome;

	//User home
	std::vector<int> user;
	QLabel *userHome;

	//Menu
	QMenu *fileMenu;
	QMenu *settingsMenu;
	QMenu *helpMenu;
	QAction *refreshAction;
	QAction *exitAction;
	QAction *settingsAction;
	QAction *instructionsAction;
	QAction *aboutAction;
	QAction *panicAction;
	QWidget *settingsWindow;
	QWidget *instructionsWindow;
	QWidget *aboutWindow;
	QLabel *about;
	void createMenu();
};

#endif // BENCHTOP_H
