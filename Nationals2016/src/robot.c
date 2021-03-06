/*
 * @file robot.c
 *
 * @brief Implementation of robot methods.
 *        LCD Menus with autonomous selection.
 *        This is stricly a BETA release and is not the final version.
 *
 * Copyright (C) 2016  Jordan M. Kieltyka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <robot.h>
#include <main.h>

/*
 * Initialize all of the motors for the robot.
 *
 * @param driveType The type of drive the robot will be
 * 					utilizing.
 */
void robot_init(){
	Robot.liftConst = 0.7;					//sed default value for PID lift constant
	motor1 = motor_init(PORT_1, false);		//initialize motor on port 1
	motor2 = motor_init(PORT_2, false);		//initialize motor on port 2
	motor3 = motor_init(PORT_3, false);		//initialize motor on port 3
	motor4 = motor_init(PORT_4, false);		//initialize motor on port 4
	motor5 = motor_init(PORT_5, false);		//initialize motor on port 5
	motor6 = motor_init(PORT_6, false);		//initialize motor on port 6
	motor7 = motor_init(PORT_7, false);		//initialize motor on port 7
	motor8 = motor_init(PORT_8, false);		//initialize motor on port 8
	motor9 = motor_init(PORT_9, false);		//initialize motor on port 9
	motor10 = motor_init(PORT_10, false);	//initialize motor on port 10
}

/*
 * Retrieve the robot's alliance.
 *
 * @return Robot's alliance.
 */
int robot_getAlliance(){
	return Robot.alliance;
}

/*
 * Retrieve the robot's starting position.
 *
 * @return Robot's starting position.
 */
int robot_getStartPos(){
	return Robot.startPos;
}

/*
 * Retrieve the robot's skills state
 *
 * @return Robot's starting position.
 */
bool robot_getSkills(){
	return Robot.skills;
}

/*
 * Retrieve the robot's current lift position.
 *
 * @return The robot's current lift position.
 */
int robot_getLiftPos(){
	return Robot.liftPos;
}

/*
 * Retrieve if the robot is currently in record mode.
 *
 * @return If the robot is in record mode or not.
 */
bool robot_isRecording(){
	return Robot.record;
}

/*
 * Retrieve the robot's PID lift constant value.
 *
 * @return Robot's PID lift constant value.
 */
double robot_getLiftConst(){
	return Robot.liftConst;
}

/*
 * Display battery voltages.
 * Alliance color selection menu.
 * Starting positions selection menu.
 */
void robot_lcdMenu(){

	lcd_backLight(&Robot.lcd, ON);	//turn on the robot's lcd backlight

	//display battery status
	while(lcd_buttonPressed(Robot.lcd) == 0){
		lcd_clear(&Robot.lcd);																	//clear lcd
		lcdPrint(Robot.lcd.port, TOP, "Primary: %1.2f V", (double)(powerLevelMain()/1000));		//display main battery voltage
		lcdPrint(Robot.lcd.port, BOTTOM, "Backup: %1.2f V", (double)(powerLevelBackup()/1000));	//display backup battery voltage
	}

	lcd_waitForRelease(Robot.lcd);	//wait for the button to be released before proceeding
	Robot.record = false;			//set the defualt record to false

	//select a mode
	while(!robot_isRecording() && !lcd_buttonIsPressed(Robot.lcd, LCD_BTN_RIGHT)){
		lcd_centerPrint(&Robot.lcd, TOP, "Select Mode");	//print lcd prompt
		lcd_print(&Robot.lcd, BOTTOM, "REC         COMP");	//print lcd prompt

		//recording selected
		if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_LEFT))
			Robot.record = true;

		//competition selected
		else if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_RIGHT))
			Robot.record = false;
	}

	lcd_waitForRelease(Robot.lcd);	//wait for the button to be released before proceeding
	Robot.skills = false;			//set the defualt skills to false

	//select a mode
	while(!robot_getSkills() && !lcd_buttonIsPressed(Robot.lcd, LCD_BTN_RIGHT)){
		lcd_centerPrint(&Robot.lcd, TOP, "Select Mode");	//print lcd prompt
		lcd_print(&Robot.lcd, BOTTOM, "SKILLS      COMP");	//print lcd prompt

		//competition selected
		if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_LEFT))
			Robot.skills = true;

		//skills selected
		else if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_RIGHT))
			Robot.skills = false;
	}

	lcd_waitForRelease(Robot.lcd);	//wait for the button to be released before proceeding

	//select competition settings
	if(!robot_getSkills()){
		Robot.alliance = NULL;			//set default alliance to null

		//select an alliance
		while(robot_getAlliance() == NULL){
			lcd_centerPrint(&Robot.lcd, TOP, "Select Alliance");	//print lcd prompt
			lcd_print(&Robot.lcd, BOTTOM, "RED         BLUE");		//print lcd prompt

			//red alliance selected
			if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_LEFT))
				Robot.alliance = RED_ALLIANCE;

			//blue alliance selected
			else if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_RIGHT))
				Robot.alliance = BLUE_ALLIANCE;
		}

		lcd_waitForRelease(Robot.lcd);	//wait for the button to be released before proceeding
		Robot.startPos = NULL;			//set default starting position to null

		//select a starting position
		while(robot_getStartPos() == NULL){
			lcd_centerPrint(&Robot.lcd, TOP, "Select Position");	//print lcd prompt
			lcd_print(&Robot.lcd, BOTTOM, "POS 1      POS 2");		//print lcd prompt

			//red alliance selected
			if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_LEFT))
				Robot.startPos = POS_1;

			//blue alliance selected
			else if(lcd_buttonIsPressed(Robot.lcd, LCD_BTN_RIGHT))
				Robot.startPos = POS_2;
		}

		lcd_waitForRelease(Robot.lcd);	//wait for the button to be released before proceeding
	}
	lcd_clear(&Robot.lcd);			//clear the lcd screen
}

/*
 *	Control robot's drive via the vexNET joystick
 *
 *	@param controller The joystick that will be controlling the drive.
 */
void robot_joyDrive(unsigned int controller){

	//used for dead zoning joystick
	if(abs(joystickGetAnalog(controller, 2)) > 10)
		motorSystem_setVelocity(&Robot.rightDrive, joystickGetAnalog(controller, 2));	//set robot's right drive velocity
	else
		motorSystem_stop(&Robot.rightDrive);	//stop robot's right drive

	//used for dead zoning joystick
	if(abs(joystickGetAnalog(controller, 3)) > 10)
		motorSystem_setVelocity(&Robot.leftDrive, joystickGetAnalog(controller, 3));	//set robot's left drive velocity
	else
		motorSystem_stop(&Robot.leftDrive);	//stop robot's left drive
}

/*
 *	Set both of the robot's drives to the desired
 *	velocity.
 *
 *	@param velocity The desired velocity for the left and right drive.
 */
void robot_setDrive(int velocity){
	robot_setDriveSplit(velocity, velocity);	//set the right and left drive to the desired velocity
}

/*
 * Set the desired velocity for the left and right drive.
 *
 * @param left The desired velocity for the left drive.
 * @param right The desired velocity for the right drive.
 */
void robot_setDriveSplit(int left, int right){
	motorSystem_setVelocity(&Robot.leftDrive, left);		//set the left drive velocity
	motorSystem_setVelocity(&Robot.rightDrive, right);	//set the right drive velocity
}

/*
 * Set the robot's drive train velocities to zero.
 */
void robot_stop(){
	robot_setDrive(0);	//set both right and left drive to zero
}

/*
 * Set the robot's drives for a desired amount of time then stop.
 *
 * @param velocity The desired drive velocity.
 * @param time The amount of time in ms for the drive to run.
 */
void robot_setDriveFor(int velocity, unsigned int time){
	robot_setDriveForSplit(velocity, velocity, time);
}

/*
 * Set the robot's drives independenty for a desired amount of time
 * then stop.
 *
 * @param left The desired velocity of the left drive.
 * @param right The desired velocity of the right drive.
 * @param time The amount of time in ms for the drives to run.
 */
void robot_setDriveForSplit(int left, int right, unsigned int time){
	robot_setDriveSplit(left, right);	//set the right and left drive to the desired velocities
	delay(time);						//pause for the desired amount of time
	robot_stop();						//stop the drive
}

/*
 * Have the robot's lift go to the desired position
 *
 * @param pos The desired lift position.
 */
void robot_liftToPosition(int pos){

	//it is the autonomous period
	if(isAutonomous())
		motorSystem_setTillPID(&Robot.lift, &Robot.liftSensor, robot_getLiftConst(), pos);

	//it is op control period
	else{
		//update motor in PID loop until sensor target value is near
		if(abs(sensor_getValue(Robot.liftSensor)) != pos || (pos - sensor_getValue(Robot.liftSensor)) * robot_getLiftConst() < 10)
			motorSystem_setVelocity(&Robot.lift, (pos - sensor_getValue(Robot.liftSensor)));

		//target position has been reached
		else
			motorSystem_stop(&Robot.lift);	//stop motor
	}
}

/**
 * Set the PID lift constant value for the
 * desired lift behaviour.
 */
void robot_setLiftConst(double value){
	Robot.liftConst = value;
}

/*
 * Set the robot's intake to the on state.
 */
void robot_intakeIn(){
	motorSystem_setVelocity(&Robot.intake, 127);
}

/*
 * Set the robot's intake to the out state.
 */
void robot_intakeOut(){
	motorSystem_setVelocity(&Robot.intake, -127);
}

/*
 * Turn off the robot's intake.
 */
void robot_intakeStop(){
	motorSystem_stop(&Robot.intake);
}

/*
 * Convert a digit value to the ASCII equivelant.
 *
 * @param dgt A number between 0 and 9.
 * @return The ASCII value of the digit.
 */
char intToChar(int dgt){
	switch(dgt){
	default:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	}
}

/*
 * Convert and ASCII value from '0' to '9' to its integer equivelant.
 *
 * @param chr The ACII character to be converted.
 * @return The integer value of the ASCII character.
 */
int charToInt(char chr){
	switch(chr){
	default:
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	}
}

/*
 * Write the motor velocity value + 127 to the desired
 * file.
 *
 * @param file The file to write too.
 * @param motor The motor port whose velocity is being taken.
 */
void writeMotorValue(FILE* file, int port){
	int velocity = motorGet(port) + 127;
	fputc(intToChar(velocity/100), file);
	velocity %= 100;
	fputc(intToChar(velocity/10), file);
	velocity %= 10;
	fputc(intToChar(velocity), file);
}

/*
 * Retrieve a motor value from the desired file.
 *
 * @param The velocity from the file.
 */
int readMotorValue(FILE* file){
	int velocity = 0;
	velocity = charToInt(fgetc(file))*100;
	velocity += charToInt(fgetc(file))*10;
	return (velocity + charToInt(fgetc(file)) - 127);
}

/*
 * Write the state of the digital port to the desired
 * file.
 */
void writeDigitalPortValue(FILE* file, int port){
	fputc(intToChar(digitalRead(port)), file);
}

/*
 * Retrieve the digital port value from a file.
 *
 * @param The file being read from,
 * @return The numerical state of the digital port.
 */
int readDigitalPortValue(FILE* file){
	return charToInt(fgetc(file));
}

/*
 * Record the robots movements for a set amount of time.
 *
 * @param time The amount of time in milliseconds that should
 * 			   be recorded.
 */
void robot_record(unsigned long int time){

	FILE* file = NULL;	//initialize file pointer

	//skills challenge autonomous
	if(robot_getSkills())
			file = fopen("sk.txt", "w");

	//red alliance autonomous at starting position 1
	else if(robot_getAlliance() == RED_ALLIANCE && robot_getStartPos() == POS_1)
		file = fopen("r1.txt" , "w");

	//red alliance autonomous at starting position 2
	else if(robot_getAlliance() == RED_ALLIANCE && robot_getStartPos() == POS_2)
		file = fopen("r2.txt" , "w");

	//blue alliance autonomous at starting position 1
	else if(robot_getAlliance() == BLUE_ALLIANCE && robot_getStartPos() == POS_1)
		file = fopen("b1.txt" , "w");

	//blue alliance autonomous at starting position 2
	else if(robot_getAlliance() == BLUE_ALLIANCE && robot_getStartPos() == POS_2)
		file = fopen("b2.txt" , "w");

	//countdown timer
	lcd_centerPrint(&Robot.lcd, TOP, "Recording in:");
	for(int i = 10; i > 0; i--){
		lcd_clearLine(&Robot.lcd, BOTTOM);
		lcdPrint(Robot.lcd.port, BOTTOM, "%d seconds", i);
		delay(1000);
	}

	lcd_centerPrint(&Robot.lcd, TOP, "Recording");	    //print to lcd
	lcd_centerPrint(&Robot.lcd, BOTTOM, "IN PROGRESS");	//print to lcd

	//read motor values until record time is reached
	unsigned int counter = 0;
	if(file != NULL)
		while(counter < time){
			userControl();	//do normal drive functions

			//write motor values
			for(int i = PORT_1; i <= PORT_10; i++)
				writeMotorValue(file, i);

			//write sensor values
			for(int i = DGTL_1; i <= DGTL_12; i++)
				writeDigitalPortValue(file, i);

			//delay(5);		//standard delay
			counter += 26;	//increase counter
		}

	robot_stop();										//stop all motors
	fclose(file);										//close the file stream
	lcd_centerPrint(&Robot.lcd, TOP, "Recording");	    //print to lcd
	lcd_centerPrint(&Robot.lcd, BOTTOM, "COMPLETED");	//print to lcd
}

/*
 * Replay the robots movements for a certain
 * alliance and position.
 */
void robot_replay(){
	FILE* file = NULL; //initialize file pointer

	//skills challenge autonomous
	if(robot_getSkills())
		file = fopen("sk.txt", "r");

	//red alliance autonomous at starting position 1
	else if(robot_getAlliance() == RED_ALLIANCE && robot_getStartPos() == POS_1)
		file = fopen("r1.txt" , "r");

	//red alliance autonomous at starting position 2
	else if(robot_getAlliance() == RED_ALLIANCE && robot_getStartPos() == POS_2)
		file = fopen("r2.txt" , "r");

	//blue alliance autonomous at starting position 1
	else if(robot_getAlliance() == BLUE_ALLIANCE && robot_getStartPos() == POS_1)
		file = fopen("b1.txt" , "r");

	//blue alliance autonomous at starting position 2
	else if(robot_getAlliance() == BLUE_ALLIANCE && robot_getStartPos() == POS_2)
		file = fopen("b2.txt" , "r");

	//continue to feed motor values until the end of the file
	if(file != NULL)
		while(!feof(file)){

			//set motor velocities
			for(int i = PORT_1; i <= PORT_10; i++)
				motorSet(i, readMotorValue(file));

			//set digital pin statuses
			for(int i = DGTL_1; i <= DGTL_12; i++)
				digitalWrite(i, readDigitalPortValue(file));

			delay(26);
		}
	robot_stop();	//stop all motors
}
