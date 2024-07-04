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

/**
 * Prints the contents of the plane
 * 
 * Symbols:
 * + - Target
 * x - Filled
 * 9B - not filled
 *
 * @param targetValue Make negative value if you aren't targeting anything
 */
void printPlane(const Plane* plane, int targetValue);

/**
 * Saves the plane into a dat file
 *
 * @param plane Saves current plane configuration to fileName
 *
 * @return
 *  PLN_FILE_OPEN_FAILED - Failed to open file respect to file name
 *  PLN_SUCCESS
 */
PlaneErrors savePlane(const Plane* plane, const char* fileName);

/**
 * Loads a new plane and returns its data to loadedPlane
 *
 * @param[out] loadedPlane Output of the plane in file
 *
 * @return
 *  PLN_FILE_OPEN_FAILED
 *  PLN_SUCCESS
 */
PlaneErrors loadPlane(Plane* loadedPlane, const char* fileName);

/**
 * Sets the seat flag to true on specified column and row
 * Note: Column and row starts from 0
 *
 * @param[out] plane Output of plane after filling the seat
 * @param[in] accountID Value that will be set to the seat, in this case accountID
 *
 * @return
 *  PLN_OUT_OF_RANGE - Column or row is not within range of aircraft seats set on PLANE_COLUMN or PLANE_ROW
 *  PLN_SEAT_FILLED - Target seat is occupied
 *  PLN_SUCCESS
 */
PlaneErrors getToSeat(Plane* plane, int accountID, int column, int row);

/**
 * Clears the seat of plane
 *
 * @param[out] plane Output of new plane after clearing the seat
 *
 * @return
 *  PLN_OUT_OF_RANGE - Column or row is not within range of aircraft seats set on PLANE_COLUMN or PLANE_ROW
 *  PLN_SEAT_NOT_FILLED - Target seat is not occupied for removal
 *  PLN_SUCCESS
 */
PlaneErrors clearSeat(Plane* plane, int column, int row);

/**
 * Sets value of previous seat to new seat
 * @param[out] plane Output of new plane after clearing the seat
 * @param[in] prevColumn & prevRow Current seat to be copied
 * @param[in] newColumn & newRow Target seat to be changed
 *
 * @return
 *  PLN_OUT_OF_RANGE - Column or row is not within range of aircraft seats set on PLANE_COLUMN or PLANE_ROW
 *  PLN_SEAT_NOT_FILLED - prev seats are not occupied 
 *  PLN_SEAT_FILLED - target seats are occupied
 */
PlaneErrors moveSeat(Plane* plane, int prevColumn, int prevRow, int newColumn, int newRow);

/**
 * Returns the value of seat with error checking
 *
 * @param[out] value Output of the seat
 * @return 
 *  PLN_OUT_OF_RANGE
 *  PLN_SUCCESS
 */
PlaneErrors getValueOfSeat(int* value, const Plane* plane, int column, int row);

/**
 * Checks if plane is full
 *
 * @return If plane is full
 */
bool isPlaneFull(const Plane* plane);

