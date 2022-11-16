#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

//emergency stop should be added move the robot to the side before turning off

/*
Function Divying:
Lucas: Fix Move so that x and y goes at the same time - done i think
Julia: Erase all (remebre the directionyou need to go in (work together)) ; Check for erased box - filled in box at far corner (at xsize and y size) will need timer
Hannah: You fix calibration - draw perimeter box, create emrgency stop procedure(erase, return to start and dont redraw box)
Leo: Emergency stop and move to initial position - hand hits. update (date) function
*/

/*
outside loop:
file has text version of all dates starting at current date
read data from file into array
number array

[10122022]
[1, 0, 1, 2, 2, 0, 2, 2]

inside loop:

for character in length(string)
  drawChar(string[char])
  if character == 1 or character == 3
    drawChar(slash)

NEED:
- drawChar()
- a way to 

*/

/* Conventions:
camel case for variables, functions, arrays
CAPS for constants
 
Sensors:
 S1 touch right
 S2 touch left

Motors:
 A - tracks
 B - up and down
 C - in and out

using xy coordinates

*/

//Declarations to avoid errors
const float PI = 3.141592;
int motorA = 0;
int motorB = 0;
int motorC = 0;
int S1 = 0;
int SensorValue [1] = {0};
int motor[1] = {0};
int nMotorEncoder[1] = {0};



// Prototype Functions

void calibrationFunction(int STDPOWER, int EDGEOFFSETCM, int WHEELDIAMETERCM);
void drawLine(int startX, int startY, int endX, int endY, float & currentX, float & currentY, float WHEELDIAMETERCM, float PINIONDIAMTERCM, int STDPOWER);
void moveToLine(int lineStartX, int lineStartY, float & currentX, float & currentY, float WHEELDIAMETERCM, float PINIONDIAMTERCM, int STDPOWER);
void moveX(int power, float distanceCM, float WHEELDIAMETERCM);
void moveY(int power, float distanceCM, float WHEELDIAMETERCM);
float cmToDegrees(float distanceCM);

// relocate to main
float currentX = 0; // in encoder distances
float currentY = 0; // in encoder distances

bool markerDown = false;


//task main()
int main()
{
    const float INOUTENCODERDISTAsNCE = 20; // need to decide on this
    const int STDPOWER = 50;
    const int BOXLENGTHCM = 50; // need to decide on this
    const int BOXHEIGHTCM = 20; // need to decide on this
    const float WHEELDIAMETERCM = 2.3; // need to determine this
    const float PINIONDIAMTERCM = 0.8; // need to determine this
    const int EDGEOFFSETCM = 5; // need to decide on this
    
    // character arrays - ig these are all constants as well
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
    
    // dont need if using manually created arrays
//    ifstream FileIn ("eight.txt");
//
//    while(!FileIn)
//    {
//        cout << "File failed to load" << endl;
//        return EXIT_FAILURE;
//    }
//    int character[8][2] = {0};
//    int count = 0;
//    while(FileIn)
//    {
//        FileIn >> character[count][0] >> character[count][1];
//        count ++ ;
//    }
    
        
    
    calibrationFunction(STDPOWER, EDGEOFFSETCM, WHEELDIAMETERCM);
    
//  process of drawing a letter - obviously still needs to be turned into a function
    moveToLine(nine[0][0], nine[0][1], currentX, currentY, WHEELDIAMETERCM, PINIONDIAMTERCM, STDPOWER);
    
    //define previous XY variables for use later
    int prevX = nine[0][0];
    int prevY = nine[0][1];
    
    // need to fix - the five below needs to update depending on the length of the array for each character
    for(int i = 1; i < 5; i++)
    {
      // need to have a way to pass 'nine' (the character array) into the function that needs to be created
        drawLine(prevX, prevY, nine[i][0], nine[i][1], currentX, currentY, WHEELDIAMETERCM, PINIONDIAMTERCM, STDPOWER);
        moveToLine(nine[i][0], nine[i][1], currentX, currentY, WHEELDIAMETERCM, PINIONDIAMTERCM, STDPOWER);
        prevX = nine[i][0];
        prevY = nine[i][1];
    }

    
//    FileIn.close(); not needed as above
    return EXIT_SUCCESS;
}

// Functions

// convert cm to degrees
int cmToDegrees(float distanceCM, float WHEELDIAMETERCM)
{
    return distanceCM * 180/(WHEELDIAMETERCM*PI);
}

//both axis movement
void move(int STDPOWER, float xDistance, float yDistance, float WHEELDIAMETERCM, float PINIONDIAMETERCM)
{
    int xEncoderChange = cmToDegrees(xDistance, WHEELDIAMETERCM);
    int yEncoderChange = cmToDegrees(yDistance, PINIONDIAMETER);

    int xEncoderFinal = nMotorEncoder[motorA] + xEncoderChange;
    int yEncoderFinal = nMotorEncoder[motorB] + yEncoderChange;

    // total distance (hypotenuse)
    int distanceTotal = pow((pow(xDistance, 2) + pow(yDistance, 2)), 0.5);
    
    // rates at which to move in each axis so that diagonal lines can be drawn
    int xRate = xDistance/distanceTotal;
    int yRate = yDistance/distanceTotal;
    
    
    motor[motorA] = STDPOWER*xRate;
    motor[motorB] = STDPOWER*yRate;

    while (abs(xEncoderFinal - nMotorEncoder[motorA]) < abs(xEncoderChange) && abs(yEncoderFinal - nMotorEncoder[motorB]) < abs(yEncoderChange))
    {
    }

    motor[motorA] = 0;
    motor[motorB] = 0;

    currentX += xEncoderChange;
    currentY += yEncoderChange;

}

//Drives to the end of the board and then back a set distance (5cm atm)
//Must place robot in correct orientation so that it drives towards lookers right
void calibrationFunction(int STDPOWER, int EDGEOFFSETCM, int WHEELDIAMETERCM) {
    motor[motorA] = STDPOWER;
    while (SensorValue[S1] == 1){
    }
    motor[motorA] = 0;

    int edgeOfBoardEncoder = nMotorEncoder[motorA];

    //erase board
    // if not end statement, bool end var, pass as parameters
    move(STDPOWER, cmToDegrees(EDGEOFFSETCM), 0, WHEELDIAMETERCM, PINIONDIAMETERCM); //determine which way is positive,
  //draw a boox
}

//puts the marker against the board
void putMarkerDown(bool & markerDown) // need to figure out which direction we need to turn the motor in to determine what sign power to use
{
    const float TOTALENCODERDISTANCE = 10; // number of encoder degrees required to move to move the marker from closest to the board to farthest away from the board - determine this experimentally
    const int MOTORCPOWER = 20; // specific motor power for this motor (slower than the others)
    
    int encoderInitial = nMotorEncoder[motorC]; // inital encoder position
    int encoderChange = (-1 * markerDown) * TOTALENCODERDISTANCE; // gets the right direction for the movement - probably not working yet
    
    motor[motorC] = MOTORCPOWER; // turn on the motor
    
    //abs to accomodate movement in postive and negative directions
    while(abs(nMotorEncoder[motorC] - encoderInitial) < encoderChange)
    {
    }
    
    //stop motor
    motor[motorC] = 0;
    
    // change marker down state - could be done with *-1 i think but not sure about bool multiplications rules in c
    if(markerDown == true)
    {
        markerDown = false;
    }
    else if(markerDown == false)
    {
        markerDown = true;
    }
}


// takes start XY, end XY and current XY and moves marker to the start of the line then draws the line and lifts the marker
void drawLine(int startX, int startY, int endX, int endY, float & currentX, float & currentY, float WHEELDIAMETERCM, float PINIONDIAMTERCM, int STDPOWER)
{
    // total distance to move in each axis
    int xDistance = endX - startX;
    int yDistance = endY - startY;
    
    // final position for each axis after movement
    int finalX = currentX + xDistance;
    int finalY = currentY + yDistance;
    
    // total distance (hypotenuse)
    int distanceTotal = pow((pow(xDistance, 2) + pow(yDistance, 2)), 0.5);
    
    // rates at which to move in each axis so that diagonal lines can be drawn
    int xRate = xDistance/distanceTotal;
    int yRate = yDistance/distanceTotal;
    
    
    // put the marker on the whiteboard
    markerDown = true;
    putMarkerDown(markerDown);
    
    //moves the X and Y motors at their respective rates
    move(STDPOWER, xDistance, yDistance, WHEELDIAMETERCM, PINIONDIAMETER); 
    
    //lift the marker off the whiteboard
    markerDown = false;
    putMarkerDown(markerDown);
}

    
// moves to start of line
void moveToLine(int lineStartX, int lineStartY, float & currentX, float & currentY, float WHEELDIAMETERCM, float PINIONDIAMTERCM, int STDPOWER)
{
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
    move(STDPOWER, xDistance, yDistance, WHEELDIAMETERCM, PINIONDIAMETER); 
  
  //spatial shit
}
