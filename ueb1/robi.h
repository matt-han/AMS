#ifndef ROBI_H
#define ROBI_H

// Abstand zwischen den Kontaktpunkten der Räder in cm
#define RAD_ABSTAND 34.36

// Durchmesser eines Rades in cm
#define RAD_DURCHMESSER 9.7  // VALUE NEW

// Anzahl der Encoderticks  // NEW
#define ENCODER_COUNT 500   // LINE NEW

// Getriebeuntersetzungsfaktor // NEW
#define GETRIEBE_FAKTOR 14     // LINE NEW

// Leerlaufdrehzahl links in U/min
#define MAX_RPM_LEFT 8257      // VALUE NEW
// Leerlaufdrehzahl rechts in U/min
#define MAX_RPM_RIGHT 8632     // VALUE NEW

// Drehung des Roboters pro Tick in µ° (nicht Rad)
#define MICRO_TICK_DEGREE

// Serielle Schnittstelle (device)
#define CNTRL_DEV "/dev/ttyS0"

// Geschwindigkeitsfaktoren, sind definiert als:
// v (cm/s) * SPEED_FACTOR = ansteuerungswert des controllers
#define SPEED_FACTOR_LEFT
#define SPEED_FACTOR_RIGHT

// Indexwerte der Motoren für die Controllerbefehle
#define MOTOR_ID_LEFT 1
#define MOTOR_ID_RIGHT 2

//USER
#define MAX_MEAS 200  // Anzahl der Messwerte
#define MAX_RPM 3000

#define pL	150
#define pR	150
#define iL	12000
#define iR	17000

#define TEILSCHRITTE 50


typedef struct {
  float l;
  float r;
}velo;

double time_shot();  // NEW
velo velocity(float vBahn, float omega);
void drive(float vBahn, float omega, int zeit, VMC::CVmc *vmc);
int umrechne(float v);





#endif