#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "plane.h"

Plane initPlane()
{
	Plane newPlane;
	for (int column = 0; column < PLANE_COLUMN; column++)
	{
		for (int row = 0; row < PLANE_ROW; row++)
		{
			newPlane.seats[column][row] = -1;
		}
	}
	return newPlane;
}

void printPlane(const Plane* plane)
{
	char planeSeatedSymbol = 'x';

	for (int row = 0; row < PLANE_ROW; row++)
	{
		if (row < 9)
		{
			printf("%d  | ", row + 1);
		}
		else
		{
			printf("%d | ", row + 1);
		}
		for (int column = 0; column < PLANE_COLUMN; column++)
		{
			if (plane->seats[column][row] != -1)
			{
				printf("x    ");
			}
			else
			{
				if (row < 9)
				{
					printf("%d%c   ", row + 1, 'A' + column);
				}
				else
				{
					printf("%d%c  ", row + 1, 'A' + column);
				}
			}
		}
		printf("\n");
	}
}


PlaneErrors savePlane(const Plane* plane, const char* fileName)
{
	FILE* planeFile = fopen(fileName, "wb");

	if (planeFile == NULL)
	{
		return PLN_FILE_OPEN_FAILED;
	}

	fwrite(plane, sizeof(Plane), 1, planeFile);

	fclose(planeFile);

	return PLN_SUCCESS;
}

PlaneErrors loadPlane(Plane* loadedPlane, const char* fileName)
{
	FILE* planeFile = fopen(fileName, "rb");

	if (planeFile == NULL)
	{
		return PLN_FILE_OPEN_FAILED;
	}

	fread(loadedPlane, sizeof(Plane), 1, planeFile);

	fclose(planeFile);
	return PLN_SUCCESS;
}

PlaneErrors getToSeat(Plane* plane, int accountID, int column, int row)
{
	if (column < 0 || column >= PLANE_COLUMN) return PLN_OUT_OF_RANGE;
	if (row < 0 || row >= PLANE_ROW) return PLN_OUT_OF_RANGE;

	if (plane->seats[column][row] != -1) return PLN_SEAT_FILLED;
	plane->seats[column][row] = accountID;
	return PLN_SUCCESS;
}

PlaneErrors clearSeat(Plane* plane, int column, int row)
{
	if (column < 0 || column >= PLANE_COLUMN) return PLN_OUT_OF_RANGE;
	if (row < 0 || row >= PLANE_ROW) return PLN_OUT_OF_RANGE;

	if (!plane->seats[column][row]) return PLN_SEAT_NOT_FILLED;
	plane->seats[column][row] = -1;
	return PLN_SUCCESS;
}

PlaneErrors moveSeat(Plane* plane, int prevColumn, int prevRow, int newColumn, int newRow)
{
	if (prevColumn < 0 || prevColumn >= PLANE_COLUMN) return PLN_OUT_OF_RANGE;
	if (prevRow < 0 || prevRow >= PLANE_ROW) return PLN_OUT_OF_RANGE;
	if (newColumn < 0 || newColumn >= PLANE_COLUMN) return PLN_OUT_OF_RANGE;
	if (newRow < 0 || newRow >= PLANE_ROW) return PLN_OUT_OF_RANGE;

	if (!plane->seats[prevColumn][prevRow]) return PLN_SEAT_NOT_FILLED;
	if (plane->seats[newColumn][newRow]) return PLN_SEAT_FILLED;

	plane->seats[newColumn][newRow] = plane->seats[prevColumn][prevRow];
	plane->seats[prevColumn][prevRow] = -1;

	return PLN_SUCCESS;
}

/**
 * Converts input such as 1B or 3c to column and row integers and returns it on column and row pointers
 */
void convertCharInputToColumnRow(char seatInput[3], int* column, int* row)
{
	*row = seatInput[0] - '0' - 1;  // Since row starts from 1
	if (isupper(seatInput[1]) != 0)
	{
		*column = seatInput[1] - 'A';
	}
	else
	{
		*column = seatInput[1] - 'a';
	}
}

// Returns true if plane is full
bool isPlaneFull(const Plane* plane)
{
	for (int row = 0; row < PLANE_ROW; row++)
	{
		for (int column = 0; column < PLANE_COLUMN; column++)
		{
			if (plane->seats[column][row] == -1)  return false;
		}
	}
	return true;
}
