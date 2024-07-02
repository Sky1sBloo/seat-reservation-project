/*
 * Contains all logic for plane operations excluding formatting
 */

#pragma once

#include <stdbool.h>

#define PLANE_ROW 10
#define PLANE_COLUMN 4


// Contains inhabited seats that contains accountID
typedef struct Plane
{
	int seats[PLANE_COLUMN][PLANE_ROW];
} Plane;

// Error handling
typedef enum
{
	PLN_SUCCESS,
	PLN_OUT_OF_RANGE,
	PLN_SEAT_FILLED,
	PLN_SEAT_NOT_FILLED,
	PLN_FILE_OPEN_FAILED,
	PLN_FILE_MODIFICATION_FAILED
} PlaneErrors;

// Initializes an empty plane (sets the value of the array to -1)
Plane initPlane();

// Prints the contents of the plane
void printPlane(const Plane* plane);

/**
 * Saves the plane into a dat file
 */
PlaneErrors savePlane(const Plane* plane, const char* fileName);

/**
 * Loads a new plane
 *
 * @param loadedPlane empty struct where the loaded plane will return
 */
PlaneErrors loadPlane(Plane* loadedPlane, const char* fileName);

/**
 * Sets the seat flag to true on specified column and row
 * Note: Column and row starts from 0
 *
 */
PlaneErrors getToSeat(Plane* plane, int accountID, int column, int row);

// Clears the set seat
PlaneErrors clearSeat(Plane* plane, int column, int row);

// Moves the seat from previous to new
PlaneErrors moveSeat(Plane* plane, int prevColumn, int prevRow, int newColumn, int newRow);

// Returns true if plane is full
bool isPlaneFull(const Plane* plane);


