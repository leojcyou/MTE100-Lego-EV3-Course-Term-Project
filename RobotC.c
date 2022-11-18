// Global Constants
const int NO_POW = 0, STD_POW = 50;
const float INOUTENCODERDISTANCE = 20, WHEEL_DIAMETER_CM = 2.3, PINION_DIAMTER_CM = 0.8;
const int BOX_LENGTH_CM = 66, BOX_HEIGHT_CM = 16, SPACE_WIDTH_CM = 2, CHAR_WIDTH_CM = 4, ERASE_MARKER_DIST_CM = 4, EDGE_OFFSET_CM = 5; ;

// Prototype Functions
int maxDays(int year, int month);
bool inputDate(int* dateArray);
void updateDateCharacterArray(int* dateArray, int* dateCharacterArray);
void updateDate(int* dateArray);
int cmToEncoder(float distanceCM, float diameterFactor);
//void calibrationFunction();
void emergencyStop(int dangerDistance, bool &markerDown);
// void eraseAll();
void engageMarker(bool moveMarker, bool &markerDown);
void moveToLine(int lineStartX, int lineStartY, float &currentX,float &currentY, bool &markerDown);
void moveEverything(float xDistance, float yDistance);
void drawLine(int startX, int startY, int endX, int endY, float &currentX,float &currentY);


// Move to main
bool markerDown = false;
float currentX = 0; // in encoder distances
float currentY = 0; // in encoder distances


task main()
{

	//int slash[2][2] = {0, 2, 1, 0};
	//int zero[5][2] = {0, 0, 1, 0, 1, 2, 0, 2, 0, 0};
	//int one[2][2] = {0, 0, 2, 0};
	//int two[6][2] = {0, 0, 1, 0, 1, 1, 0, 1, 0, 2, 1, 2};
	//int three[7][2] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 2, 0, 2};
	//int four[4][2] = {1, 1, 0, 1, 1, 0, 1, 2};
	//int five[6][2] = {1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 0, 2};
	//int six[6][2] = {1, 0, 0, 1, 0, 2, 1, 2, 1, 1, 0, 1};
	//int seven[3][2] = {0, 0, 1, 0, 0, 2};
	//int eight[8][2] = {0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 2, 0, 2, 0, 0};
	//int nine[5][2] = {1, 2, 1, 0, 0, 0, 0, 1, 1, 1};

	//bool dateInput = false, emergencyStatus = false, stopRequired = false;
	//int dateArray[3] = {0,0,0}, dateCharacterArray[8] = {0,0,0,0,0,0,0,0};
}

/////////////////////////////////////////////////////////////////////////////////////
// Functions:
int maxDays(int year, int month)
{
	int maxDate = 31;

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

bool inputDate(int* dateArray)
{
	int startDay = 1, startMonth = 1, startYear = 2022, daysLimit = 0;

	displayString(2, "Enter the year: ", "%s", startYear);
	wait1Msec(1000);
	while (!getButtonPress(buttonEnter))
	{
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight))
		{
			startYear++;
		}
		else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft))
		{
			startYear--;
		}
	}
	displayString(2, "Selected year: ", "%s", startYear);
	wait1Msec(2000);

	displayString(2, "Enter the month: ", "%s", startMonth);
	wait1Msec(1000);
	while (!getButtonPress(buttonEnter))
	{
		if (startMonth > 12 || startMonth < 1)
		{
			startMonth = 1;
		}
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight))
		{
			startMonth++;
		}
		else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft))
		{
			startMonth--;
		}
	}
	displayString(2, "Selected month: ", "%s", startMonth);
	wait1Msec(2000);

	daysLimit = maxDays(startYear, startMonth);

	displayString(2, "Enter the day: ", "%s", startDay);
	wait1Msec(1000);
	while (!getButtonPress(buttonEnter))
	{
		if (startDay > daysLimit || startDay < 1)
		{
			startDay = 1;
		}
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight))
		{
			startDay++;
		}
		else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft))
		{
			startDay--;
		}
	}
	displayString(2, "Enter the day: ", "%s", startDay);
	wait1Msec(2000);

	displayString(2, "Is this the correct date? ", "%s", startYear, "/", "%s",startMonth, "/", "%s",startDay); // IDK if this is the right syntax
	wait1Msec(2000);
	displayString(3, "Press enter to confirm.");
	displayString(4, "Press any other button to restart.");
	wait1Msec(2000);

	if (getButtonPress(buttonEnter))
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

void updateDateCharacterArray(int* dateArray, int* dateCharacterArray)
{
	const int TEN_FACTOR = 10, HUNDRED_FACTOR = 100, THOUSAND_FACTOR = 1000;

	dateCharacterArray[0] = dateArray[0] / THOUSAND_FACTOR;
	dateCharacterArray[1] = (dateArray[0] % THOUSAND_FACTOR) / HUNDRED_FACTOR;
	dateCharacterArray[2] = (dateArray[0] % HUNDRED_FACTOR) / TEN_FACTOR;
	dateCharacterArray[3] = dateArray[0] % TEN_FACTOR;
	dateCharacterArray[4] = dateArray[1] / TEN_FACTOR;
	dateCharacterArray[5] = dateArray[1] % TEN_FACTOR;
	dateCharacterArray[6] = dateArray[2] / TEN_FACTOR;
	dateCharacterArray[7] = dateArray[2] % TEN_FACTOR;
}

void updateDate(int* dateArray)
{
	const int MONTH_LIMIT = 12, RESET = 1;

	int year = dateArray[0];
	int month = dateArray[1];
	int daysLimit = maxDays(year, month);

	dateArray[2]++;

	if (dateArray[2] > daysLimit)
	{
		dateArray[2] = RESET;
		dateArray[1]++;

		if (dateArray[1] > MONTH_LIMIT)
		{
			dateArray[1] = RESET;
			dateArray[0]++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

int cmToEncoder(float distanceCM, float diameterFactor)
{
	return distanceCM * 180 / (diameterFactor * PI);
}

void engageMarker(bool moveMarker, bool &markerDown)
{
	const float TOTAL_ENCODER_DISTANCE = 10;
	const int MOTOR_C_POWER = 20;

	int encoderInitial = nMotorEncoder[motorC];
	int encoderChange = TOTAL_ENCODER_DISTANCE;

	if (moveMarker == markerDown)
	{
		return;
	}

	if (moveMarker)
	{
		encoderChange *= -1; // assuming moving in is negative
	}

	motor[motorC] = MOTOR_C_POWER;
	while (abs(nMotorEncoder[motorC] - encoderInitial) < encoderChange)
	{}
	motor[motorC] = NO_POW;

	markerDown = !markerDown;
}

void moveEverything(float xDistance, float yDistance)
{
	int xEncoderChange = cmToEncoder(xDistance, WHEEL_DIAMETER_CM);
	int yEncoderChange = cmToEncoder(yDistance, PINION_DIAMTER_CM);

	int xEncoderFinal = nMotorEncoder[motorA] + xEncoderChange;
	int yEncoderFinal = nMotorEncoder[motorB] + yEncoderChange;

	float distanceTotal = sqrt(pow(xDistance, 2) + pow(yDistance, 2));
	float xRate = xDistance / distanceTotal;
	float yRate = yDistance / distanceTotal;

	motor[motorA] = STD_POW * xRate;
	motor[motorB] = STD_POW * yRate;

	while (abs(xEncoderFinal - nMotorEncoder[motorA]) < abs(xEncoderChange) && abs(yEncoderFinal - nMotorEncoder[motorB]) < abs(yEncoderChange))
	{}
	motor[motorA] = motor[motorB] = NO_POW;

	currentX += xEncoderChange;
	currentY += yEncoderChange;
}

void moveToLine(int lineStartX, int lineStartY, float &currentX,float &currentY, bool &markerDown)
{
	engageMarker(false, markerDown);
	int xDistance = lineStartX - currentX;
	int yDistance = lineStartY - currentY;

	moveEverything(xDistance, yDistance);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawLine(int startX, int startY, int endX, int endY, float &currentX,float &currentY)
{
	int xDistance = endX - startX;
	int yDistance = endY - startY;

	//int finalX = currentX + xDistance;//??
	//int finalY = currentY + yDistance;

	engageMarker(true, markerDown);

	moveEverything(xDistance, yDistance);

	engageMarker(false, markerDown);
}

void emergencyStop(int dangerDistance, bool &markerDown)
{
	motor[motorA] = motor[motorB] = NO_POW;
	engageMarker(false, markerDown) ;
	time100(T1) = 0;
	while (SensorValue[S3] <= dangerDistance && time100[T1] < 100)
	{}
	moveToLine(0, 0, currentX, currentY, markerDown);
}

//void calibrationFunction(int STD_POW, int BOX_LENGTH_CM, int BOX_HEIGHT_CM, int EDGE_OFFSET_CM, float WHEEL_DIAMETER_CM, float PINION_DIAMTER_CM)
//{
//	// drive forward and backup to offset edge
//	short edgeOffsetEncoder = 0, boxLengthEncoder = 0, boxHeightEncoder = 0;
//	motor[motorA] = STD_POW;
//	while (SensorValue[S3])
//	{}
//	motor[motorA] = 0;
//	edgeOffsetEncoder = cmToEncoder(EDGE_OFFSET_CM, WHEEL_DIAMETER_CM);
//	boxLengthEncoder = cmToEncoder(BOX_LENGTH_CM, WHEEL_DIAMETER_CM);
//	boxHeightEncoder = cmToEncoder(BOX_HEIGHT_CM, PINION_DIAMETER_CM);
//	nMotorEncoder[motorA] = 0;
//	motor[motorA] = -STD_POW;
//	while (abs(nMotorEncoder[motorA]) < edgeOffsetEncoder)
//	{}
//	motor[motorA] = 0;
//	// draw box
//	while (!SensorValue[S3] && !getButtonPress(buttonAny))
//	{
//		moveEverything();
//		drawLine(0, 0, boxHeightEncoder);
//		drawLine();
//		moveEverything();
//		drawLine();
//		drawLine();
//	}
//}
