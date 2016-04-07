/*
 * @file usercontrol.c
 *
 * @brief Contains the method for which the client will
 * 		   put in their code to control their robot via
 * 		   the vexNET Joystick.
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

#include "main.h"
#include "NDAPI.h"
#include "robot.h"
/**
 * Insert all joystick commands here and any other functions
 * that will be used to control the robot during the Operator
 * Control Period. This method will be run in a loop with a delay.
 * Therefore there is no need to insert a loop or a delay in this method.
 * This is sexy af. Bask in its glory.
 */
int mode = 0;
int wheelSetSpeed = 80;
void userControl(){
	robot_joyDrive(DRIVER);	//control drive from joystick

	bool intake = joystickGetDigital(1, 6, JOY_UP);
	bool outtake = joystickGetDigital(1, 6, JOY_DOWN);
	bool flywheelToggle = joystickGetDigital(1, 8, JOY_DOWN);
	bool puncherToggle = joystickGetDigital(1, 8, JOY_UP);
	bool rapidfire = joystickGetDigital(1, 8, JOY_RIGHT);
	bool speedUp = joystickGetDigital(1, 7, JOY_UP);
	bool speedDown = joystickGetDigital(1, 7, JOY_DOWN);
	bool flywheel = joystickGetDigital(1, 8, JOY_LEFT);
	bool puncher = joystickGetDigital(1, 8, JOY_LEFT);
	bool presetHigh = joystickGetDigital(1, 7, JOY_RIGHT);
	bool presetLow = joystickGetDigital(1, 7, JOY_LEFT);
	bool bandIntake = joystickGetDigital(1, 5, JOY_UP);
	bool bandOuttake = joystickGetDigital(1, 5, JOY_DOWN);

	if(sensor_getValue(Robot.puncherEncoder) >= 360) //checks puncher encoder value
		sensor_reset(&Robot.puncherEncoder); //resets after each rotation

	int wheelVelocity = sensor_getValue(Robot.wheelEncoder);
	sensor_reset(&Robot.wheelEncoder);

	if(Robot.skills == false)
	{
		if(lcd_buttonPressed(Robot.lcd) == 0){
			lcdPrint(uart2, 1, "setSpeed: %d", wheelSetSpeed);
			lcdPrint(uart2, 2, "encoderSpeed: %d", sensor_getValue(Robot.wheelEncoder));
		}
		else if(lcd_buttonPressed(Robot.lcd) != 0){
			lcdPrint(uart2, 1, "Main: %f", (double)(powerLevelMain()/1000));
			lcdPrint(uart2, 2, "Expander: %f", (double)(analogRead(1)/1000));
		}
	}
	else{
		lcd_print(&Robot.lcd, TOP, "AutonSelected:");
		lcd_print(&Robot.lcd, BOTTOM, "SKILLS");
	}



	//PTO:
	if(mode == 0){ 			//if in puncher mode
		digitalWrite(12, LOW);
		if(puncher) //manual mode
			motorSystem_setVelocity(&Robot.PTO, 127);
		else
			motorSystem_stop(&Robot.PTO); //end manual mode
	}
	else if(mode == 1){ //if in flywheel mode
		digitalWrite(12, HIGH);
		if(rapidfire){ //rapid fire mode
			if(wheelVelocity >= 0.518*wheelSetSpeed+30.7){
				motorSystem_setVelocity(&Robot.intake, 127);
				motorSystem_setVelocity(&Robot.PTO, wheelSetSpeed);
			}
			else if(wheelVelocity < 0.518*wheelSetSpeed+30.7 && sensor_getValue(Robot.wheelDetector) >= 900){
				motorSystem_setVelocity(&Robot.intake, 127);
				motorSystem_setVelocity(&Robot.PTO, 127);
			}
			else if(wheelVelocity < 0.518*wheelSetSpeed+30.7 && sensor_getValue(Robot.wheelDetector) < 900){
				motorSystem_stop(&Robot.intake);
				motorSystem_setVelocity(&Robot.PTO, 127);
			}
		}
		else{
			motorSystem_stop(&Robot.PTO);
			motorSystem_stop(&Robot.intake);
		} //end rapid fire mode

		if(flywheel) //manual fire mode
			motorSystem_setVelocity(&Robot.PTO, wheelSetSpeed);
		else
			motorSystem_stop(&Robot.PTO);
	}
	//PTO

	if(intake) //intake operations
		motorSystem_setVelocity(&Robot.intake, 127);
	else if(outtake)
		motorSystem_setVelocity(&Robot.intake, -50);
	else
		motorSystem_stop(&Robot.intake);

	if(bandIntake)
		motor_setVelocity(&motor10, 127);
	else if(bandOuttake)
		motor_setVelocity(&motor10, -50);
	//end of intake operations

	if(speedUp && wheelSetSpeed < 127) //adjustable wheel speed
		wheelSetSpeed++;
	else if(speedDown && wheelSetSpeed > 0)
		wheelSetSpeed--;
	//end adjustable wheel speed

	if(presetHigh) //wheel presets
		wheelSetSpeed = 110;
	else if(presetLow)
		wheelSetSpeed = 80;

	if(flywheelToggle){ //choose between puncher and flywheel
		mode = 1;
		motorSystem_setVelocity(&Robot.PTO, 50);
	}
	else if(puncherToggle){
		motorSystem_setVelocity(&Robot.PTO, 30);
		mode = 0;
	}
}
