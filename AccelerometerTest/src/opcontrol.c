/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
int analogCalibrate(unsigned char channel);
int analogReadCalibrated(unsigned char channel);
int analogReadCalibratedHR(unsigned char channel);
int joystickGetAnalog(unsigned char joystick, unsigned char axis);

int rightlever;
int leftlever;
double accelm;

void update(){
	rightlever = joystickGetAnalog(1,3);
	leftlever = joystickGetAnalog(1,2);
	accelm = analogRead(1);
}

void drive(){
 	update();
 	if ((rightlever >=90 && rightlever <= 120) || (rightlever>=-90 && rightlever<=-120)) {
         // divide number between 90 and 120, -90 and -120, to make the controls more precise
    if (rightlever%2 == 1 || rightlever%2 == -1) { //make power variable even so it can be divided by 2
      rightlever++;
    }
    rightlever = rightlever/2;
  }

  if ((leftlever >=-90 && leftlever <= -120) || (leftlever >=90 && leftlever <= 120)) {
         // divide number between 90 and 120, -90 and -120, to make the controls more precise
    if (leftlever%2 == 1 || leftlever%2 == -1){ // make power2 variable even so it can be divided by 2
       leftlever++;
    }

    leftlever = leftlever/2;
  }
 	motorSet(1, rightlever);
  motorSet(9, -leftlever);
}
void operatorControl() {
	analogCalibrate(1);
	analogReadCalibratedHR(1);
	while (1) {
		update();
		drive();
		delay(20);
		printf("%f", accelm);
	}
}
