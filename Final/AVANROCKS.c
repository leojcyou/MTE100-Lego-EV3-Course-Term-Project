const int NO_POW = 0, STD_POW = 30;
const float TOTAL_ENCODER_DISTANCE = 1850, WHEEL_DIAMETER_CM = 2.3, PINION_DIAMETER_CM = 0.65;
const int BOX_LENGTH_CM = 68, BOX_HEIGHT_CM = 16, SPACE_WIDTH_CM = 2, CHAR_WIDTH_CM = 4, SMALL_DIST = 1, EDGE_OFFSET_CM = 10, SAFE_DIST = 5, TIME_INTERVAL = 2400;

int cmToEncoder(float distanceCM, float diameterFactor);
bool sensorIsOn();
bool engageMarker(bool moveMarker, bool* markerDown);
bool moveEverything(float xDistance, float yDistance);
bool drawLineFunction(int xDistance, int yDistance, bool* markerDown);
bool drawCharPart(char direction, int segmentLen, bool* markerDown);
bool calibrationFunction(bool* markerDown);

int maxDays(int year, int month);
bool inputDate(int* dateArray);
void updateDateCharacterArray(int* dateArray, int* dateCharacterArray);
void updateDate(int* dateArray);


task main()
{
	const int dashIndex = 80;
	bool markDown = false;
	bool* markerDown = &markDown;
	bool dateInputted = false;
	bool nonEmergencyExit = false;
	int drawingNum=0, drawingIndex=0;
	int dateArray[3] = {0,0,0};
	int dateCharacterArray[8] = {0,0,0,0,0,0,0,0};

	int segmentLen[88] = {2, 1, -2, -1, 2, 0, 0, 0,
		2, 0, 0, 0, 0, 0, 0, 0,
		1, -1, 1, 1, 1, -1, 0, 0,
		1, -1, 1, 1, -1, 1, 1, -1,
		1, 1, -1, 1, -1, 1, 0, 0,
		1, 1, -1, 1, 1, -1, 0, 0,
		1, 1, -1, 1, 1, -1, -1, 1,
		1, -1, 2, 0, 0, 0, 0, 0,
		1, 2, -1, -2, 1, 1, -1, 1,
		1, 1, -1, -1, 2, 0, 0, 0,
		1, 1, -1, 1, 0, 0, 0, 0};

	char directions[88] = {'V', 'H', 'V', 'H', 'V', 'n', 'n', 'n',
		'V', 'n', 'n', 'n', 'n', 'n', 'n', 'n',
		'H', 'H', 'V', 'H', 'V', 'H', 'n', 'n',
		'H', 'H', 'V', 'H', 'H', 'V', 'H', 'H',
		'V', 'H', 'V', 'V', 'H', 'V', 'n', 'n',
		'H', 'V', 'H', 'V', 'H', 'H', 'n', 'n',
		'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V',
		'H', 'H', 'V', 'n', 'n', 'n', 'n', 'n',
		'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V',
		'V', 'H', 'V', 'H', 'V', 'n', 'n', 'n',
		'h', 'v', 'H', 'v', 'n', 'n', 'n', 'n'};


	while(!dateInputted)
	{
		dateInputted = inputDate(dateArray);
	}

	while(!SensorValue(S3))
	{}
	wait1Msec(2000);

	time100(T1) = 0;

	while(nonEmergencyExit==false)
	{
		clearTimer(T1);
		updateDateCharacterArray(dateArray, dateCharacterArray);

		if (!calibrationFunction(markerDown)) return;
		if (!moveEverything(-(2*CHAR_WIDTH_CM+SMALL_DIST), 0)) return;
		if (!moveEverything(0,CHAR_WIDTH_CM)) return;

		for(int i=0; i < 8; i++)
		{
			drawingNum = dateCharacterArray[i];
			drawingIndex = dateCharacterArray[i]*8;

			for(int j = 0; j<8; j++)
			{
				if (!drawCharPart(directions[drawingIndex+j], segmentLen[drawingIndex+j], markerDown)) return;
			}

			if (!moveEverything(-(SPACE_WIDTH_CM+CHAR_WIDTH_CM),0)) return;
			if (!moveEverything(0,-(2*CHAR_WIDTH_CM))) return;

			if(i==3 || i==5)
			{

				for(int k = 0; k<8; k++)
				{
					if (!drawCharPart(directions[dashIndex+k], segmentLen[dashIndex+k], markerDown)) return;
				}

				if (!moveEverything(-(SPACE_WIDTH_CM+CHAR_WIDTH_CM),0)) return;
				if (!moveEverything(0,-(2*CHAR_WIDTH_CM))) return;
			}
		}

		motor[motorA] = -STD_POW;
		while (SensorValue[S3] == 1)
		{
			if(sensorIsOn()) return;
		}
		motor[motorA] = 0;

		//zeroes encoders
		nMotorEncoder[motorA] = 0;
		motor[motorB] = -STD_POW;
		while (!SensorValue[S2])
		{
			if (sensorIsOn()) return;
		}
		motor[motorB] = 0;

		motor[motorB] = STD_POW;
		wait1Msec(250);
		motor[motorB] = 0;

		motor[motorA] = STD_POW;
		wait1Msec(2000);
		motor[motorA]= 0;

		while(time100[T1] < TIME_INTERVAL) // for the real version this number would be 24 hours
		{
			if (sensorIsOn()) return;
			if(getButtonPress(buttonEnter))
			{
				nonEmergencyExit = true;
			}
		}
		wait1Msec(2000);
		updateDate(dateArray);
	}
}

bool sensorIsOn()
{
  return SensorValue[S4] < SAFE_DIST;
}

bool engageMarker(bool moveMarker, bool* markerDown)
{
	int motorCPower = -100;
	int encoderInitial = nMotorEncoder[motorC];
	int encoderChange = TOTAL_ENCODER_DISTANCE;

	if (moveMarker != *markerDown)
	{
		if (moveMarker == true)
		{
			motorCPower *= -1;
		}

		motor[motorC] = motorCPower;
		while (abs(nMotorEncoder[motorC]-encoderInitial) < abs(encoderChange))
		{
      if (sensorIsOn())
        return false;
    }
		motor[motorC] = NO_POW;

		if(*markerDown == true)//this...
		{*markerDown = false;}
		else
		{*markerDown = true;}
	}

  return true;
}

bool moveEverything(float xDistance, float yDistance)
{
	float xEncoderChange = cmToEncoder(xDistance, WHEEL_DIAMETER_CM);
	float yEncoderChange = cmToEncoder(yDistance, PINION_DIAMETER_CM);

	float xEncoderInitial = nMotorEncoder[motorA];
	float yEncoderInitial = nMotorEncoder[motorB];

	if (yDistance == 0)
	{
		motor[motorA] = STD_POW *(xDistance/abs(xDistance));
		while(abs(nMotorEncoder[motorA]-xEncoderInitial) < abs(xEncoderChange))
		{
      if (sensorIsOn())
        return false;
    }
	}
	else if (xDistance == 0)
	{
		motor[motorB] = STD_POW *(yDistance/abs(yDistance));
		while(abs(nMotorEncoder[motorB]-yEncoderInitial) < abs(yEncoderChange))
		{
      if (sensorIsOn())
        return false;
    }
	}

	motor[motorA] = motor[motorB] = NO_POW;

  return true;
}

bool drawLineFunction(int xDistance, int yDistance, bool* markerDown)
{
	if (!engageMarker(true, markerDown)) return false;
	if (!moveEverything(xDistance, yDistance)) return false;
	if (!engageMarker(false, markerDown)) return false;

	return true;
}

bool drawCharPart(char direction, int segmentLen, bool* markerDown)
{
	//non drawing lines
	if(direction == 'h')
	{
		if (!moveEverything(segmentLen*CHAR_WIDTH_CM, 0)) return false;
	}

	if(direction == 'v')
	{
		if (!moveEverything(0, segmentLen*CHAR_WIDTH_CM)) return false;
	}

	// drawing lines
	if(direction == 'H')
	{
		if (!drawLineFunction(segmentLen*CHAR_WIDTH_CM, 0, markerDown)) return false;
	}

	if(direction == 'V')
	{
		if (!drawLineFunction(0, segmentLen*CHAR_WIDTH_CM, markerDown)) return false;
	}

	return true;
}

bool calibrationFunction(bool* markerDown)
{
	short edgeOffsetEncoder = cmToEncoder(EDGE_OFFSET_CM, WHEEL_DIAMETER_CM), boxLengthEncoder = cmToEncoder(BOX_LENGTH_CM, WHEEL_DIAMETER_CM);
	//edge check
	motor[motorA] = -STD_POW;
	while (SensorValue[S3] == 1)
	{}
	motor[motorA] = 0;

	//zeroes encoders
	nMotorEncoder[motorA] = 0;
	motor[motorB] = -STD_POW;
	while (!SensorValue[S2])
	{}
	if (!moveEverything(0,CHAR_WIDTH_CM)) return false;
	motor[motorB] = 0;

	motor[motorA] = STD_POW;
	while (abs(nMotorEncoder[motorA]) < edgeOffsetEncoder + boxLengthEncoder)
	{}
	motor[motorA] = 0;

	nMotorEncoder[motorA] = 0;

	if (!drawLineFunction(-BOX_LENGTH_CM, 0, markerDown)) return false;  //draws top line of box from left to right)
	if (!drawLineFunction(0, BOX_HEIGHT_CM, markerDown)) return false;  //draws right side line from top to bottom)
	if (!drawLineFunction(BOX_LENGTH_CM, 0, markerDown)) return false;  //draws bottom line from right to left)
	if (!drawLineFunction(0, -BOX_HEIGHT_CM, markerDown)) return false;  //draws left side line from bottom to top)

	nMotorEncoder[motorA] = nMotorEncoder[motorB] = 0;

	return true;
}


// no in-out
//







////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int cmToEncoder(float distanceCM, float diameterFactor)
{return distanceCM * 180 / (diameterFactor * PI);}

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

	displayString(2, "Enter the year: %d", startYear);
	for(int i = 0; i < 10000000000; i++)
	{
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight))
		{
			startYear += 1;
			eraseDisplay();
			displayString(2, "Enter the year: %d", startYear);
		}
		else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft))
		{
			startYear -= 1;
			eraseDisplay();
			displayString(2, "Enter the year: %d", startYear);
		}
		else if (getButtonPress(buttonEnter))
		{
			eraseDisplay();
			break;
		}
		wait1Msec(200);
	}
	displayString(2, "Selected year: %d", startYear);
	wait1Msec(1000);
	eraseDisplay();

	displayString(2, "Enter the month: %d", startMonth);
	for(int i = 0; i < 10000000000; i++)
	{
		if (startMonth > 12 || startMonth < 1)
		{
			startMonth = 1;
			eraseDisplay();
			displayString(2, "Enter the month: %d", startMonth);
		}
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight))
		{
			startMonth += 1;
			eraseDisplay();
			displayString(2, "Enter the month: %d", startMonth);
		}
		else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft))
		{
			startMonth -= 1;
			eraseDisplay();
			displayString(2, "Enter the month: %d", startMonth);
		}
		else if (getButtonPress(buttonEnter))
		{
			eraseDisplay();
			break;
		}
		wait1Msec(200);
	}
	displayString(2, "Selected month: %d", startMonth);
	wait1Msec(1000);
	eraseDisplay();

	daysLimit = maxDays(startYear, startMonth);

	displayString(2, "Enter the day: %d", startDay);
	for(int i = 0; i < 10000000000; i++)
	{
		if (startDay > daysLimit || startDay < 1)
		{
			startDay = 1;
			eraseDisplay();
			displayString(2, "Enter the day: %d", startDay);
		}
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight))
		{
			startDay += 1;
			eraseDisplay();
			displayString(2, "Enter the day: %d", startDay);
		}
		else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft))
		{
			startDay -= 1;
			eraseDisplay();
			displayString(2, "Enter the day: %d", startDay);
		}
		else if (getButtonPress(buttonEnter))
		{
			eraseDisplay();
			break;
		}
		wait1Msec(200);
	}
	displayString(2, "Selected day: %d", startDay);
	wait1Msec(1000);
	eraseDisplay();

	displayString(2, "Is this the correct date?");
	displayString(3, "%d %d %d", startYear,startMonth,startDay);
	displayString(4, "Press enter to confirm.");
	displayString(5, "Press any other button to restart.");
	while(!getButtonPress(buttonAny))
	{}
	eraseDisplay();

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
