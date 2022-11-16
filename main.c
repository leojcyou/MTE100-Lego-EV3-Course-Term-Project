#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

//Declarations to avoid errors
const float PI = M_PI;
int motorA = 0;
int motorB = 0;
int motorC = 0;
int S1 = 0;
int SensorValue [1] = {0};
int motor[1] = {0};
int nMotorEncoder[1] = {0};

//Global Constants
const int NO_POW = 0;
const int STD_POW = 50;

// Prototype Functions
int maxDays(int year, int month);
bool inputDate(int & dateArray);
void updateDateCharacterArray(int & dateArray, int & dateCharacterArray);
void updateDate(int & dateArray);
int cmToEncoder(float distanceCM, float WHEEL_DIAMETER_CM);
void calibrationFunction(int STD_POW, int EDGE_OFFSET_CM, int WHEEL_DIAMETER_CM);
//void emergencyStop();
//void eraseAll();
//return to rest position function needed
//void checkEraseMarks();
void engageMarker(bool moveMarker, bool & markerDown);
void moveToLine(int lineStartX, int lineStartY, float & currentX, float & currentY, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM);
void move(float xDistance, float yDistance, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM);
void drawLine(int startX, int startY, int endX, int endY, float & currentX, float & currentY, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM, int STD_POW);

//Move to main
bool markerDown = false
float currentX = 0; // in encoder distances
float currentY = 0; // in encoder distances

//task main()
int main()
{
	//const float INOUTENCODERDISTANCE = 20; // need to decide on this
	const int BOX_LENGTH_CM = 50; // need to decide on this
	const int BOX_HEIGHT_CM = 20; // need to decide on this
	const float WHEEL_DIAMETER_CM = 2.3; // need to determine this
	const float PINION_DIAMTER_CM = 0.8; // need to determine this
	const int EDGE_OFFSET_CM = 5; // need to decide on this

	// character arrays - ig these are all constants as well // WE COULD HAVE THESE IN A FILE SO WE DONT NEED TO DECLARE CONSTANTS
	int slash[2][2] = {0, 2, 1, 0};
	int zero[5][2] = {0, 0, 1, 0, 1, 2, 0, 2, 0, 0};
	int one[2][2] = {0, 0, 2, 0};
	int two[6][2] = {0, 0, 1, 0, 1, 1, 0, 1, 0, 2, 1, 2};
	int three[7][2] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 2, 0, 2};
	int four[4][2] = {1, 1, 0, 1, 1, 0, 1, 2};
	int five[6][2] = {1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 0, 2};
	int six[6][2] = {1, 0, 0, 1, 0, 2, 1, 2, 1, 1, 0, 1};
	int seven[3][2] = {0, 0, 1, 0, 0, 2};
	int eight[8][2] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 2, 0, 2, 0, 0};
	int nine[5][2] = {1, 2, 1, 0, 0, 0, 0, 1, 1, 1};
	//Or I think it would be beneficial to implement a struct for these arrays so that we can equate numbers to specific arrays when drawing

	//Regular Initialisations
	bool dateInput = false, emergencyStatus = false, stopRequired = false;
	int dateArray[3] = {0}, dateCharacterArray[8] = {0};


	//MAIN SEQUENCE STARTS
	while(!dateInput)
	{
		dateInput = inputDate(dateArray);
	}

	calibration(EDGE_OFFSET_CM, WHEEL_DIAMETER_CM);

	//  process of drawing a letter - obviously still needs to be turned into a function
	moveToLine(nine[0][0], nine[0][1], currentX, currentY, WHEEL_DIAMETER_CM, PINION_DIAMTER_CM);

	while() //need to find a way to actively check this |confused about the grid system
	{

		updateDateCharacterArray(dateArray, dateCharacterArray); //place this at the start of the loop (before drawing date but after receiving an updated or initial date)
		int prevX = nine[0][0];
		int prevY = nine[0][1];

		// need to fix - the five below needs to update depending on the length of the array for each character
		for(int i = 1; i < 5; i++)
		{
			// need to have a way to pass 'nine' (the character array) into the function that needs to be created
			drawLine(prevX, prevY, nine[i][0], nine[i][1], currentX, currentY, WHEEL_DIAMETER_CM, PINION_DIAMTER_CM);
			moveToLine(nine[i][0], nine[i][1], currentX, currentY, WHEEL_DIAMETER_CM, PINION_DIAMTER_CM);
			prevX = nine[i][0];
			prevY = nine[i][1];
		}
		//define previous XY variables for use later

	}

	if(emergencyStatus)
	{
		emergencyStop(BOX_HEIGHT_CM);
	}

	return EXIT_SUCCESS;
}

// Functions:
int maxDays(int year, int month)
{
	maxDate = 31;

	if (month == 2)
	{
		if (year % 4 == 0)
		{
			maxDate = 29;
		}
		else
		{
			maxDate = 28;
		}
	}

	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		maxDate = 30;
	}

	return maxDate;
}

bool inputDate(int & dateArray)
{ //Function to collect start date from user
	int startDay = 1, startMonth = 1, startYear = 2022, daysLimit = 0;

	displayString(2,"Enter the year: " , "%s" , startYear);
	wait1MSec(1000);
	while(!getButtonPress(buttonEnter))
	{
		if (getButtonPress(buttonUp)||getButtonPress(buttonRight))
		{
			startYear++;
		}
		else if (getButtonPress(buttonDown)||getButtonPress(buttonLeft))
		{
			startYear--;
		}
	}
	displayString(2,"Selected year: " , "%s" , startYear);
	wait1MSec(2000);

	displayString(2,"Enter the month: " , "%s" , startMonth);
	wait1MSec(1000);
	while(!getButtonPress(buttonEnter))
	{
		if (startMonth > 12 || startMonth < 1)
		{
			startMonth = 1;
		}
		if (getButtonPress(buttonUp)||getButtonPress(buttonRight))
		{
			startMonth++;
		}
		else if (getButtonPress(buttonDown)||getButtonPress(buttonLeft))
		{
			startMonth--;
		}
	}
	displayString(2,"Selected month: " , "%s" , startMonth);
	wait1MSec(2000);

	daysLimit = maxDays(startYear, startMonth);

	displayString(2,"Enter the day: " , "%s" , startDay)
	wait1MSec(1000);
	while(!getButtonPress(buttonEnter))
	{
		if (startDay > daysLimit || startDay < 1)
		{
			startDay = 1;
		}
		if (getButtonPress(buttonUp)||getButtonPress(buttonRight))
		{
			startDay++;
		}
		else if (getButtonPress(buttonDown)||getButtonPress(buttonLeft))
		{
			startDay--;
		}
	}
	displayString(2,"Enter the day: " , "%s" , startDay);
	wait1MSec(2000);

	displayString(2,"Is this the correct date? ", "%s", startYear, "/", "%s", startMonth, "/","%s",startDay); //IDK if this is the right syntax
	wait1MSec(2000);
	displayString(3, "Press enter to confirm.");
	displayString(4,"Press any other button to restart.");
	wait1MSec(2000);

	if(getButtonPress(buttonEnter))
	{
		dateArray[0] = startYear;
		dateArray[1] = startMonth;
		dateArray[2] = startDay;

		return true;
	}
	else
	{
		return false;
	}
}

void updateDateCharacterArray(int & dateArray, int & dateCharacterArray)
{
	const int TEN_FACTOR = 10, HUNDRED_FACTOR = 100, THOUSAND_FACTOR = 1000;

	dateCharcterArray[0] = dateArray[0] / THOUSAND_FACTOR;
	dateCharcterArray[1] = (dateArray[0] % THOUSAND_FACTOR) / HUNDRED_FACTOR;
	dateCharcterArray[2] = (dateArray[0] % HUNDRED_FACTOR) / TEN_FACTOR;
	dateCharcterArray[3] = dateArray[0] % TEN_FACTOR;
	dateCharcterArray[4] = dateArray[1] / TEN_FACTOR;
	dateCharcterArray[5] = dateArray[1] % TEN_FACTOR;
	dateCharcterArray[6] = dateArray[2] / TEN_FACTOR;
	dateCharcterArray[7] = dateArray[2] % TEN_FACTOR;
}

void updateDate(int & dateArray)
{
	const int MONTH_LIMIT = 12, RESET = 1;
	int dateLimit = 0;

	year = dateArray[0];
	month = dateArray[1];
	day = dateArray[2];
	daysLimit = maxDays(year, month)

	dateArray[2] ++;

	if(dateArray[2] > dateLimit)
	{
		dateArray[2] = RESET;
		dateArray[1] ++;
		if(dateArray[1] > MONTH_LIMIT)
		{
			dateArray[1] = RESET;
			dateArray[0]++;
		}
	}
}

int cmToEncoder(float distanceCM, float WHEEL_DIAMETER_CM)
{
	return distanceCM * 180/(WHEEL_DIAMETER_CM*PI);
}

void calibration(int EDGE_OFFSET_CM, int WHEEL_DIAMETER_CM) 
{ //Drives to the end of the board and then back a set distance (5cm atm); Must place robot in correct orientation so that it drives towards lookers right

	// ensure marker is up
	motor[motorA] = STD_POW;
	while (SensorValue[S1])
	{}
	motor[motorA] = NO_POW;

	int edgeOfBoardEncoder = nMotorEncoder[motorA];

	//erase board
	// if not end statement, bool end var, pass as parameters
	move(STD_POW, cmToEncoder(EDGE_OFFSET_CM), 0, WHEEL_DIAMETER_CM, PINION_DIAMTER_CM); //determine which way is positive,
	//draw a box
}

void emergencyStop(int dangerDistance)
{
	motor[motorA] = motor[motorB] = NO_POW;
	engageMarker(false, markerDown)
	time100(T1) = 0;
	while(SensorValue[S3] <= dangerDistance)
	{}
	//return to starting position
	return EXIT_FAILURE;
}

void engageMarker(bool moveMarker, bool & markerDown)
{ //puts the marker against the board: need to figure out which direction we need to turn the motor in to determine what sign power to use
	const float TOTAL_ENCODER_DISTANCE = 10; // number of encoder degrees required to move to move the marker from closest to the board to farthest away from the board - determine this experimentally
	const int MOTOR_C_POWER = 20;

	int encoderInitial = nMotorEncoder[motorC];
	int encoderChange = TOTAL_ENCODER_DISTANCE;

	if (moveMarker == markerDown)
	{
		return;
	}

	if (moveMarker)
	{
		encoderChange *= -1;  //assuming moving in is negative
	}

	motor[motorC] = MOTOR_C_POWER;
	while(abs(nMotorEncoder[motorC] - encoderInitial) < encoderChange)
	{}
	motor[motorC] = NO_POW;

	markerDown = !markerDown;
}

void moveToLine(int lineStartX, int lineStartY, float & currentX, float & currentY, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM)
{ // moves to start of line
	// total distance to move in each axis
	int xDistance = lineStartX - currentX;
	int yDistance = lineStartY - currentY;

	// final position for each axis after movement
	int finalX = currentX + xDistance;
	int finalY = currentY + yDistance;

	// total distance (hypotenuse)
	int distanceTotal = pow((pow(xDistance, 2) + pow(yDistance, 2)), 0.5);

	// rates at which to move in each axis to minimize time
	int xRate = xDistance/distanceTotal;
	int yRate = yDistance/distanceTotal;

	//moves the X and Y motors at their respective rates
	move(STD_POW, xDistance, yDistance, WHEEL_DIAMETER_CM, PINIONDIAMETER);

	//spatial shit
}

void move(float xDistance, float yDistance, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM)
{ //both axis movement
	int xEncoderChange = cmToEncoder(xDistance, WHEEL_DIAMETER_CM);
	int yEncoderChange = cmToEncoder(yDistance, PINION_DIAMTER_CM);

	int xEncoderFinal = nMotorEncoder[motorA] + xEncoderChange;
	int yEncoderFinal = nMotorEncoder[motorB] + yEncoderChange;

	// finding hypotenuse distance and move rates for diagonal lines
	int distanceTotal = pow((pow(xDistance, 2) + pow(yDistance, 2)), 0.5);
	float xRate = xDistance/distanceTotal;
	float yRate = yDistance/distanceTotal;

	motor[motorA] = STD_POW*xRate;
	motor[motorB] = STD_POW*yRate;

	while (abs(xEncoderFinal - nMotorEncoder[motorA]) < abs(xEncoderChange) && abs(yEncoderFinal - nMotorEncoder[motorB]) < abs(yEncoderChange))
	{}
	motor[motorA] = motor[motorB] = NO_POW;

	currentX += xEncoderChange;
	currentY += yEncoderChange;
}

void drawLine(int startX, int startY, int endX, int endY, float & currentX, float & currentY, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM)
{ // takes start XY, end XY and current XY and moves marker to the start of the line then draws the line and lifts the marker
	// total distance to move in each axis
	int xDistance = endX - startX;
	int yDistance = endY - startY;

	// final position for each axis after movement
	int finalX = currentX + xDistance;
	int finalY = currentY + yDistance;

	// put the marker on the whiteboard
	engageMarker(true, markerDown);

	//moves the X and Y motors at their respective rates
	move(STD_POW, xDistance, yDistance, WHEEL_DIAMETER_CM, PINION_DIAMTER_CM);

	//lift the marker off the whiteboard
	engageMarker(false, markerDown);
}