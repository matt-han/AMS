//Gruppe 2-8
//Matthias Hansert 791744
//Max Knodel 798479
//Niklas Knauer 798163

#include <stdio.h>
#include <vmc232.h>
#include "robi.h"

void velocity(float vBahn, float omega, int zeit, VMC::CVmc *vmc)
{
  float vl, vr;

  vl = (vBahn + omega*(RAD_ABSTAND/2));
  vr = (vBahn - omega*(RAD_ABSTAND/2));

	vl = GETRIEBE_FAKTOR * vl * 60.0 / (3.14159 * RAD_DURCHMESSER);
	vr = GETRIEBE_FAKTOR * vr * 60.0 / (3.14159 * RAD_DURCHMESSER);
	
	printf ("vl :%d   vr: %d\n\n", (int)vl, (int)vr);
  printf("------------fahren---------------\n");
  vmc->setMotorRPM(MOTOR_ID_LEFT, (int)vl);
	vmc->setMotorRPM(MOTOR_ID_RIGHT, (int)vr*(-1));
  vmc->wait(zeit);

  vmc->setMotors(0, 0);
}
