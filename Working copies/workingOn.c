const int NO_POW = 0, STD_POW = 30;
const float TOTAL_ENCODER_DISTANCE = 1850, WHEEL_DIAMETER_CM = 2.3,
PINION_DIAMETER_CM = 0.65;
const int SAFE_DIST = 5, BOX_LENGTH_CM = 68, BOX_HEIGHT_CM = 16,
SPACE_WIDTH_CM = 2, CHAR_WIDTH_CM = 4,SMALL_DIST = 1, EDGE_OFFSET_CM = 10;



int cmToEncoder(float distanceCM, float diameterFactor);
bool engageMarker(bool moveMarker, bool *markerDown, bool *emergencyStop);
bool moveEverything(float xDistance, float yDistance, bool *emergencyStop);
bool drawLineFunction(int xDistance, int yDistance, bool *markerDown,
bool *emergencyStop);
bool drawCharPart(char direction, int segmentLen, bool *markerDown,
bool *emergencyStop);
void calibrationFunction(bool *markerDown, bool *emergencyStop);

int maxDays(int year, int month);
bool inputDate(int *dateArray);
void updateDateCharacterArray(int *dateArray, int *dateCharacterArray);
void updateDate(int *dateArray);

task main() {
	const int dashIndex = 80;
	float currX = 0;
	float *currentX = &currX;
	float currY = 0;
	float *currentY = &currY;
	bool markDown = false;
	bool *markerDown = &markDown;
	bool dateInputted = false;
	bool eStatus = false;
	int drawingNum = 0, drawingIndex = 0;
	int dateArray[3] = {2022,11, 24};
	int dateCharacterArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	int segmentLen[88] = {
		2,  1, -2, -1, 2, 0, 0,  0,  2, 0,  0,  0, 0,  0,  0,  0, 1,  -1,
		1,  1, 1,  -1, 0, 0, 1,  -1, 1, 1,  -1, 1, 1,  -1, 1,  1, -1, 1,
		-1, 1, 0,  0,  1, 1, -1, 1,  1, -1, 0,  0, 1,  1,  -1, 1, 1,  -1,
		-1, 1, 1,  -1, 2, 0, 0,  0,  0, 0,  1,  2, -1, -2, 1,  1, -1, 1,
		1,  1, -1, -1, 2, 0, 0,  0,  1, 1,  -1, 1, 0,  0,  0,  0};

	char directions[88] = {
		'V', 'H', 'V', 'H', 'V', 'n', 'n', 'n', 'V', 'n', 'n', 'n', 'n', 'n', 'n',
		'n', 'H', 'H', 'V', 'H', 'V', 'H', 'n', 'n', 'H', 'H', 'V', 'H', 'H', 'V',
		'H', 'H', 'V', 'H', 'V', 'V', 'H', 'V', 'n', 'n', 'H', 'V', 'H', 'V', 'H',
		'H', 'n', 'n', 'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V', 'H', 'H', 'V', 'n',
		'n', 'n', 'n', 'n', 'H', 'V', 'H', 'V', 'H', 'V', 'H', 'V', 'V', 'H', 'V',
		'H', 'V', 'n', 'n', 'n', 'h', 'v', 'H', 'v', 'n', 'n', 'n', 'n'};

	/*while (!dateInputted) {
	dateInputted = inputDate(dateArray);
	}
	while(!SensorValue(S3))
	{}
	wait1Msec(3000);*/

	time100(T1) = 0;

	while (eStatus !=true) {
		clearTimer(T1);
		updateDateCharacterArray(dateArray, dateCharacterArray);

		calibrationFunction(markerDown, &eStatus);
		eStatus = moveEverything(-9, 0, &eStatus);
		eStatus = moveEverything(0, 4, &eStatus);

		for (int i = 0; i < 8; i++) {
			drawingNum = dateCharacterArray[i];
			drawingIndex = dateCharacterArray[i] * 8;

			for (int j = 0; j < 8; j++) {
				eStatus = drawCharPart(directions[drawingIndex + j], segmentLen[drawingIndex + j],
				markerDown, &eStatus);
			}

			eStatus = moveEverything(-6, 0, &eStatus);
			eStatus = moveEverything(0, -8, &eStatus);

			if (i == 3 || i == 5) {

				for (int k = 0; k < 8; k++) {
					eStatus = drawCharPart(directions[dashIndex + k], segmentLen[dashIndex + k],
					markerDown, &eStatus);
				}

				eStatus = moveEverything(-6, 0, &eStatus);
				eStatus = moveEverything(0, -8, &eStatus);
			}
		}

		updateDate(dateArray);
		//move to start
		motor[motorA] = -STD_POW;
		while (SensorValue[S3] == 1)
		{}
		motor[motorA] = 0;

		// zeroes encoders
		nMotorEncoder[motorA] = 0;
		motor[motorB] = -STD_POW;
		while (!SensorValue[S2]) {
		}
		eStatus = moveEverything(0, 4, &eStatus);
		motor[motorB] = 0;

		//while (T1 < 1800)
		//{
		//	displayString(1, "in timer");
		//}

	}
	displayString(1, "ending i think");
}

bool engageMarker(bool moveMarker, bool *markerDown, bool *emergencyStop) {
	int motorCPower = -100;
	int encoderInitial = nMotorEncoder[motorC];
	int encoderChange = TOTAL_ENCODER_DISTANCE;

	if (moveMarker != markerDown) {
		if (moveMarker == true) {
			motorCPower *= -1;
		}

		motor[motorC] = motorCPower;
		while (abs(nMotorEncoder[motorC] - encoderInitial) < abs(encoderChange) && SensorValue(S4) > SAFE_DIST)
		{}
		motor[motorC] = NO_POW;
		if(SensorValue(S4)<=SAFE_DIST)
		{
			*emergencyStop = true;
			return true;
		}
		if (*markerDown == true) // this...
		{
			*markerDown = false;
			} else {
			*markerDown = true;
		}
	}
	return false;
}

bool moveEverything(float xDistance, float yDistance, bool *emergencyStop) {
	float xEncoderChange = cmToEncoder(xDistance, WHEEL_DIAMETER_CM);
	float yEncoderChange = cmToEncoder(yDistance, PINION_DIAMETER_CM);

	float xEncoderInitial = nMotorEncoder[motorA];
	float yEncoderInitial = nMotorEncoder[motorB];

	if (yDistance == 0)
		{
		motor[motorA] = STD_POW * (xDistance / abs(xDistance));
		while (abs(nMotorEncoder[motorA] - xEncoderInitial) < abs(xEncoderChange) && SensorValue(S4) > SAFE_DIST)
		{}
		}
		else if (xDistance == 0)
		{
		motor[motorB] = STD_POW * (yDistance / abs(yDistance));
		while (abs(nMotorEncoder[motorB] - yEncoderInitial) < abs(yEncoderChange) && SensorValue(S4) > SAFE_DIST)
		{}
	}
	motor[motorA] = motor[motorB] = NO_POW;
	if(SensorValue(S4)<=SAFE_DIST)
		{
			*emergencyStop = true;
			return true;
		}
		else
		{
			return false;
		}
}

bool drawLineFunction(int xDistance, int yDistance, bool *markerDown,
bool *emergencyStop) {
	*emergencyStop = engageMarker(true, markerDown, emergencyStop);
	*emergencyStop = moveEverything(xDistance, yDistance, emergencyStop);
	*emergencyStop = engageMarker(false, markerDown, emergencyStop);

	if(emergencyStop == true)
	{
	return true;
	}
	else
	{
		return false;
	}
}

bool drawCharPart(char direction, int segmentLen, bool *markerDown,
bool *emergencyStop) {
	// non drawing lines
	if (direction == 'h') {
		if(engageMarker(false, markerDown, emergencyStop) == true)
	{
		return true;
	}
		*emergencyStop = moveEverything(segmentLen * CHAR_WIDTH_CM, 0, emergencyStop);
	}

	if (direction == 'v') {
		if(engageMarker(false, markerDown, emergencyStop) == true)
	{
		return true;
	}
		*emergencyStop = moveEverything(0, segmentLen * CHAR_WIDTH_CM, emergencyStop);
	}

	// drawing lines
	if (direction == 'H') {
		*emergencyStop = drawLineFunction(segmentLen * CHAR_WIDTH_CM, 0, markerDown, emergencyStop);
	}

	if (direction == 'V') {
		*emergencyStop = drawLineFunction(0, segmentLen * CHAR_WIDTH_CM, markerDown, emergencyStop);
	}
}

void calibrationFunction(bool *markerDown, bool *emergencyStop) {
	short edgeOffsetEncoder = cmToEncoder(EDGE_OFFSET_CM, WHEEL_DIAMETER_CM),
	boxLengthEncoder = cmToEncoder(BOX_LENGTH_CM, WHEEL_DIAMETER_CM);
	// edge check
	motor[motorA] = -STD_POW;
	while (SensorValue[S3] == 1) {
	}
	motor[motorA] = 0;

	// zeroes encoders
	nMotorEncoder[motorA] = 0;
	motor[motorB] = -STD_POW;
	while (!SensorValue[S2]) {
	}
	*emergencyStop = moveEverything(0, 4, emergencyStop);
	motor[motorB] = 0;

	// zeroes encoders
	nMotorEncoder[motorA] = 0;
	motor[motorA] = STD_POW;
	while (abs(nMotorEncoder[motorA]) < edgeOffsetEncoder + boxLengthEncoder) {
	}
	motor[motorA] = 0;

	nMotorEncoder[motorA] = 0;

	*emergencyStop = drawLineFunction(-BOX_LENGTH_CM, 0, markerDown,
	emergencyStop); // draws top line of box from left to right
	*emergencyStop = drawLineFunction(0, BOX_HEIGHT_CM, markerDown,
	emergencyStop); // draws right side line from top to bottom
	*emergencyStop = drawLineFunction(BOX_LENGTH_CM, 0, markerDown,
	emergencyStop); // draws bottom line from right to left
	*emergencyStop = drawLineFunction(0, -BOX_HEIGHT_CM, markerDown,
	emergencyStop); // draws left side line from bottom to top

	nMotorEncoder[motorA] = nMotorEncoder[motorB] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int cmToEncoder(float distanceCM, float diameterFactor) {
	return distanceCM * 180 / (diameterFactor * PI);
}

int maxDays(int year, int month) {
	int maxDate = 31;

	if (month == 2) {
		if (year % 4 == 0) {
			maxDate = 29;
			} else {
			maxDate = 28;
		}
		} else if (month == 4 || month == 6 || month == 9 || month == 11) {
		maxDate = 30;
	}
	return maxDate;
}

bool inputDate(int *dateArray) {
	int startDay = 1, startMonth = 1, startYear = 2022, daysLimit = 0;

	displayString(2, "Enter the year: %d", startYear);
	for (int i = 0; i < 10000000000; i++) {
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight)) {
			startYear += 1;
			eraseDisplay();
			displayString(2, "Enter the year: %d", startYear);
			} else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft)) {
			startYear -= 1;
			eraseDisplay();
			displayString(2, "Enter the year: %d", startYear);
			} else if (getButtonPress(buttonEnter)) {
			eraseDisplay();
			break;
		}
		wait1Msec(200);
	}
	displayString(2, "Selected year: %d", startYear);
	wait1Msec(1000);
	eraseDisplay();

	displayString(2, "Enter the month: %d", startMonth);
	for (int i = 0; i < 10000000000; i++) {
		if (startMonth > 12 || startMonth < 1) {
			startMonth = 1;
			eraseDisplay();
			displayString(2, "Enter the month: %d", startMonth);
		}
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight)) {
			startMonth += 1;
			eraseDisplay();
			displayString(2, "Enter the month: %d", startMonth);
			} else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft)) {
			startMonth -= 1;
			eraseDisplay();
			displayString(2, "Enter the month: %d", startMonth);
			} else if (getButtonPress(buttonEnter)) {
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
	for (int i = 0; i < 10000000000; i++) {
		if (startDay > daysLimit || startDay < 1) {
			startDay = 1;
			eraseDisplay();
			displayString(2, "Enter the day: %d", startDay);
		}
		if (getButtonPress(buttonUp) || getButtonPress(buttonRight)) {
			startDay += 1;
			eraseDisplay();
			displayString(2, "Enter the day: %d", startDay);
			} else if (getButtonPress(buttonDown) || getButtonPress(buttonLeft)) {
			startDay -= 1;
			eraseDisplay();
			displayString(2, "Enter the day: %d", startDay);
			} else if (getButtonPress(buttonEnter)) {
			eraseDisplay();
			break;
		}
		wait1Msec(200);
	}
	displayString(2, "Enter the day: %d", startDay);
	wait1Msec(1000);
	eraseDisplay();

	displayString(2, "Is this the correct date?");
	displayString(3, "%d %d %d", startYear, startMonth, startDay);
	displayString(4, "Press enter to confirm.");
	displayString(5, "Press any other button to restart.");
	while (!getButtonPress(buttonAny)) {
	}
	eraseDisplay();

	if (getButtonPress(buttonEnter)) {
		dateArray[0] = startYear;
		dateArray[1] = startMonth;
		dateArray[2] = startDay;

		return true;
		} else {
		return false;
	}
}

void updateDateCharacterArray(int *dateArray, int *dateCharacterArray) {
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

void updateDate(int *dateArray) {
	const int MONTH_LIMIT = 12, RESET = 1;

	int year = dateArray[0];
	int month = dateArray[1];
	int daysLimit = maxDays(year, month);

	dateArray[2]++;

	if (dateArray[2] > daysLimit) {
		dateArray[2] = RESET;
		dateArray[1]++;

		if (dateArray[1] > MONTH_LIMIT) {
			dateArray[1] = RESET;
			dateArray[0]++;
		}
	}
}
