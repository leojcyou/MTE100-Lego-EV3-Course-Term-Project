/*
Need to change emergency condition codes so enough parameters ar epassing
char -> arrays need to be sorted
i must sleep

*/
// Global Constants
const int NO_POW = 0, STD_POW = 50;
const float INOUTENCODERDISTANCE = 20, WHEEL_DIAMETER_CM = 2.3, PINION_DIAMETER_CM = 0.8, ERASER_HEIGHT_CM = 0, ERASER_WIDTH_CM = 0;
const int BOX_LENGTH_CM = 66, BOX_HEIGHT_CM = 16, SPACE_WIDTH_CM = 2, CHAR_WIDTH_CM = 4, ERASE_MARKER_DIST_CM = 4, EDGE_OFFSET_CM = 5, TIME_INTERVAL = 300;

// Prototype Functions
int maxDays(int year, int month);
bool inputDate(int* dateArray);
void updateDateCharacterArray(int* dateArray, int* dateCharacterArray);
void updateDate(int* dateArray);

int cmToEncoder(float distanceCM, float diameterFactor);
void engageMarker(bool moveMarker, bool*markerDown, bool* emergencyCondition); //implement EC
void moveEverything(float xDistance, float yDistance, float* currentX, float* currentY, bool* emergencyCondition); //implement EC
void moveToLine(int lineStartX, int lineStartY, float* currentX, float* currentY, bool* markerDown, bool* emergencyCondition);

void emergencyStop(int dangerDistance, float* currentX, float* currentY, bool* markerDown);
void drawLineFunction(int startX, int startY, int endX, int endY, float* currentX,float* currentY, bool* markerDown);

//URGENTLY NEEDS TO BE DONE
void calibrationFunction(float* currentX, float* currentY, bool* markerDown, bool* emergencyCondition); //needs to be finalised //implement EC
void eraseAll(float* currentX, float* currentY, bool* markerDown, bool* emergencyCondition); //needs to be finalised //implement EC

typedef struct
{
	char relNum;
	int numArray[8][2] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//not sure if this is correct initialisation

} numArrays;


task main()
{
	/*
	int slash[2][2] = {1, 0, 0, 2};
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
	*/

	struct numArrays slash = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};

	struct numArrays zero = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays one = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays two = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays three = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays four = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays five = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays six = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays seven = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays eight = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};
	struct numArrays nine = {"/",{1,0,0,2,0,2,0,2,0,2,0,2,0,2,0,2}};

	int dateArray[3] = {0,0,0};
	int dateCharacterArray[8] = {0,0,0,0,0,0,0,0};

	bool dateInput = false, emergencyStatus = false, stopRequired = false, markerDown = false, emergencyCondition = false;
	float currentX = 0, currentY = 0, prevX = 0, prevY = 0; 
	
	while(!dateInput)
	{
		dateInput = inputDate(dateArray);
	}
	
	calibration();
	time100[T1]=0;
	
	while(!emergencyCondition)
	{
		updateDateCharacterArray(dateArray, dateCharacterArray);

		

		for(int dateArrayIndex = 0; dateArrayIndex < 8; dateArrayIndex++)
		{
			moveToLine();

			for(int charArrayIndex = 0; charArrayIndex< ; charArrayIndex++)
			{
				drawLine();
			}
			
			if(dateArrayIndex == 1 || dateArrayIndex == 3)
			{
				moveToLine();
				drawLine();
			}
		}

		moveToLine();
		while(time100[T1]<TIME_INTERVAL* && !getButtonPress(buttonAny) && !SensorValue(S3) < BOX_HEIGHT_CM)
		{}
		if (getButtonPress(buttonAny) || SensorValue(S3) < BOX_HEIGHT_CM)
		{
			emergencyCondition = true;
		}
		else
		{
			clearTimer[T1];
			updateDate(dateArray);
			eraseAll(currentX, currentY, markerDown, emergencyCondition);
		}
	}

	if (emergencyCondition)
	{
		emergencyStop(SensorValue[S3], currentX, currentY, markerDown)
	}
	else
	{
		eraseAll(currentX, currentY, markerDown, emergencyCondition); //what if emergency here?
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

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

	displayString(2, "Is this the correct date? ", "%s", startYear, "/", "%s",startMonth, "/", "%s",startDay); 
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

void engageMarker(bool moveMarker, bool* markerDown, bool* emergencyCondition)
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

void moveEverything(float xDistance, float yDistance, float* currentX, float* currentY, bool* emergencyCondition)
{
	int xEncoderChange = cmToEncoder(xDistance, WHEEL_DIAMETER_CM);
	int yEncoderChange = cmToEncoder(yDistance, PINION_DIAMETER_CM);

	int xEncoderFinal = nMotorEncoder[motorA] + xEncoderChange;
	int yEncoderFinal = nMotorEncoder[motorB] + yEncoderChange;

	float distanceTotal = sqrt(pow(xDistance, 2) + pow(yDistance, 2));
	float xRate = xDistance / distanceTotal;
	float yRate = yDistance / distanceTotal;

	motor[motorA] = STD_POW * xRate;
	motor[motorB] = STD_POW * yRate;

	while (abs(xEncoderFinal - nMotorEncoder[motorA]) < abs(xEncoderChange)*& abs(yEncoderFinal - nMotorEncoder[motorB]) < abs(yEncoderChange))
	{}
	motor[motorA] = motor[motorB] = NO_POW;

	currentX += xEncoderChange;
	currentY += yEncoderChange;
}

void moveToLine(int lineStartX, int lineStartY, float* currentX, float* currentY, bool* markerDown)
{
	engageMarker(false, markerDown);
	int xDistance = lineStartX - currentX;
	int yDistance = lineStartY - currentY;

	moveEverything(xDistance, yDistance, currentX, currentY);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawLineFunction(int startX, int startY, int endX, int endY, float* currentX,float* currentY, bool* markerDown)
{
	int xDistance = endX - startX;
	int yDistance = endY - startY;

	engageMarker(true, markerDown);

	moveEverything(xDistance, yDistance, currentX, currentY);

	engageMarker(false, markerDown);
}

void emergencyStop(int dangerDistance, float* currentX, float* currentY, bool* markerDown)
{
	motor[motorA] = motor[motorB] = motor[motorC] = NO_POW;
	engageMarker(false, markerDown);
	time100[T2] = 0;
	while (SensorValue[S3] <= dangerDistance && time100[T2] < 20)
	{}
	moveToLine(0, 0, currentX, currentY, markerDown);
}

void calibrationFunction(float* currentX, float* currentY, bool* markerDown, bool* emergencyCondition)
{
	// drive forward and backup to where erase function is set to start (edge offset distance from right corner)
	short edgeOffsetEncoder = 0, boxLengthEncoder = 0, boxHeightEncoder = 0;

	motor[motorA] = STD_POW;
	while (SensorValue[S3])
	{}
	motor[motorA] = 0;

	edgeOffsetEncoder = cmToEncoder(EDGE_OFFSET_CM, WHEEL_DIAMETER_CM);
	boxLengthEncoder = cmToEncoder(BOX_LENGTH_CM, WHEEL_DIAMETER_CM);
	boxHeightEncoder = cmToEncoder(BOX_HEIGHT_CM, PINION_DIAMETER_CM);

	//Sets x position to 0
	nMotorEncoder[motorA] = 0;

	motor[motorA] = -STD_POW;
	while (abs(nMotorEncoder[motorA]) < edgeOffsetEncoder)
	{}
	motor[motorA] = 0;

	nMotorEncoder[motorA] = 0;

	//Sets y position to 0

  	// erase the area
  	eraseAll();
  	// draw box
	while (!SensorValue[S3] && !getButtonPress(buttonAny))
	{
		drawLineFunction(0, 0,);
		drawLineFunction();
		drawLineFunction();
		drawLineFunction();
    	moveEverything();
	}

	float currentX = 0, currentY = 0; 
}

void eraseAll(float* currentX, float* currentY, bool* markerDown, bool* emergencyCondition)
{
  //moves the robot to the origin minus distance len
  float distAcross = BOX_LENGTH_CM - 2*(SMALL_DIST + ERASER_WIDTH_CM + BETWEEN_DIST);
  const float MULTIPLIER = 0.75;
  int NUM_SWIPES = BOX_HEIGHT_CM/(ERASER_HEIGHT_CM*MULTIPLIER) + 1;
  
  for(int i=0; i < NUM_SWIPES; i++)
  {
    //moves down by the height of the eraser and back the horizontal distance
    moveEverything(distAcross, ERASER_HEIGHT_CM*MULTIPLIER);

    //drawline of distAcross
    drawLineFunction(currentX, currentY, endX, endY, currentX, currentY, markerDown); 
  }  
  
  //moves up by the height of the box 
  moveEverything(0, BOX_HEIGHT_CM); 
}