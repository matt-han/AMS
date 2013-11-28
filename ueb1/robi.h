//Gruppe 2-8
//Matthias Hansert 791744
//Max Knodel 798479
//Niklas Knauer 798163

#ifndef ROBI_H
#define ROBI_H

// Abstand zwischen den Kontaktpunkten der Räder in m
#define RAD_ABSTAND 0.365

// Durchmesser eines Rades in m
#define RAD_DURCHMESSER 0.08920

// Anzahl der Encoderticks
#define ENCODER_COUNT 500

// Getriebeuntersetzungsfaktor
#define GETRIEBE_FAKTOR 14

// Drehung des Roboters pro Tick in µ° (nicht Rad)
#define MICRO_TICK_DEGREE

// Serielle Schnittstelle (device)
#define CNTRL_DEV "/dev/ttyS0"

// Geschwindigkeitsfaktoren, sind definiert als:
// v (cm/s) * SPEED_FACTOR = ansteuerungswert des controllers
#define SPEED_FACTOR_LEFT  0.95
#define SPEED_FACTOR_RIGHT 0.95

// Indexwerte der Motoren für die Controllerbefehle
#define MOTOR_ID_LEFT 1
#define MOTOR_ID_RIGHT 2

#define MAX_MEAS 200  // Anzahl der Messwerte
#define MAX_RPM 3000

#define PL	150
#define PR	150
#define IL	12000
#define IR	17000

#endif
