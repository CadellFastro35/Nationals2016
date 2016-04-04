/*
 * @file robot.h
 *
 * @brief Collection of files that make up VEX ALL PROSe.
 * Robot data structure and prototypes.
 * Creation of motors to be used by robots.
 * LCD Menus with autonomous selection.
 * This is stricly a BETA release and is not the final version.
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

#ifndef ROBOT_H_
#define ROBOT_H_

#include <NDAPI.h>	//NDA API

//alliances
#define RED_ALLIANCE  1	//red alliance
#define BLUE_ALLIANCE 2	//blue alliance

//starting tiles
#define POS_1 1	//first position
#define POS_2 2	//second position

//robot modes
#define SKILLS 0
#define COMPETITION 1

//controller type
#define DRIVER  1	//the main driver controller
#define PARTNER 2	//the partner driver controller

//motors
Motor motor1;	//motor on port 1
Motor motor2;	//motor on port 2
Motor motor3;	//motor on port 3
Motor motor4;	//motor on port 4
Motor motor5;	//motor on port 5
Motor motor6;	//motor on port 6
Motor motor7;	//motor on port 7
Motor motor8;	//motor on port 8
Motor motor9;	//motor on port 9
Motor motor10;	//motor on port 10

//robot data structure
struct{
	int alliance;		//the robot's alliance
	int startPos;		//the robot's starting position
	bool skills;		//flag for if the robot is in a skills challenge or not
	bool record;		//flag for if the robot is in the recording state or not
	LCD lcd;			//the robot's LCD screen
	int liftPos;		//the robot's current lift position
	double liftConst;	//the robot's lift constant for PID, default is 0.7

	//motor systems
	MotorSystem rightDrive;		//robot's right drive
	MotorSystem leftDrive;		//robot's left drive
	MotorSystem intake;			//robot's intake motor system
	MotorSystem lift;			//robot's lift motor system
	MotorSystem PTO;

	//sensors
	Sensor rightDriveSensor; 	//robot's right drive
	Sensor leftDriveSensor;		//robot's left drive sensor
	Sensor liftSensor;			//robot's lift sensor
	Sensor turnSensor;			//robot's turn sensor
	Sensor puncherEncoder; 		//puncher encoder
	Sensor wheelEncoder;		//wheel encoder
	Sensor puncherDetector;		//puncher line follower
	Sensor wheelDetector;		//wheel line follower
} Robot;

/* Generic robot functions */

void robot_init();	//initialize the robot

//getter methods
int robot_getAlliance();	//retrieve the robot's alliance
int robot_getStartPos();	//retrieve the robot's starting position
bool robot_getSkills();		//retrieve the robot's skills state
int robot_getLiftPos();		//retrieve the robot's lift position
bool robot_isRecording();	//determine if the robot is in recording mode
double robot_getLiftConst();//get the PID lift constant value

//lcd methods
void robot_lcdMenu();	//lcd selection menu

//drive methods
void robot_joyDrive(unsigned int controller);							//control robot's drive via the vexNET joystick
void robot_setDrive(int velocity);										//set both the right and left drives to the same motor velocities
void robot_setDriveSplit(int left, int right);							//set the robot's drive velocities independently
void robot_stop();														//set the velocity of the drive to zero
void robot_setDriveFor(int velocity, unsigned int time);				//run drive for a certain amount of time at a certain velocity
void robot_setDriveForSplit(int left, int right, unsigned int time);	//run drive for a certain amount of time independently

//lift methods
void robot_liftToPosition(int pos);	//go to the specified position

//intake methods
void robot_intakeIn();		//set the robot's intake to in
void robot_intakeOut();		//set the robot's intake to out
void robot_intakeStop();	//stop the robot's intake
void robot_setLiftConst();	//set the PID lift constant value

//autonomous methods
void robot_record(unsigned long int time);	//record the value of the motor ports for 15 seconds
void robot_replay();						//playback the value of all motor ports

void robot_driverControl();		//controls all robot's functions from joystick

#endif /* ROBOT_H_ */
