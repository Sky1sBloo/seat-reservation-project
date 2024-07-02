#include "input-handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "session.h"

void inputDisplayPlane(const char fileName[])
{
	Plane loadedPlane;
	if (loadPlane(&loadedPlane, fileName) == PLN_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Error: Failed to open %s\n", fileName);
		fprintf(stderr, "  Use --create to create new file\n");
		exit(1);
	}

	printPlane(&loadedPlane);
}

void inputCreatePlane(const char fileName[])
{
	FILE* newPlaneFile= fopen(fileName, "wb");

	if (newPlaneFile == NULL)
	{
		fprintf(stderr, "Error: Failed to create file\n");
		exit(-1);
	}
	
	Plane newPlane = initPlane();
	fwrite(&newPlane, sizeof(Plane), 1, newPlaneFile);

	fclose(newPlaneFile);
}

void inputLogin(const char accountID[], const char password[])
{
	// Check if login
 	Account sessionInfo;
 	if (loadSessionInfo(&sessionInfo, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
 	{
 		fprintf(stderr, "There's an existing session, logout first\n");
		exit(1);
 	}

	// Sanitize input

 	Account accountSession;	
 	AccountError loginError;

	if (!stringIsInt(accountID))
	{
		printf("Account ID is not a numerical value\n");
		exit(0);
	}
	int id = atoi(accountID);

 	loginError = loginAccount(id, password, &accountSession);
 
 	switch (loginError)
 	{
 		case AE_SUCCESS:
 			saveSession(&accountSession, sizeof(Account));
 			printf("Login success\n");
 			break;
 		case AE_WRONG_USER_OR_PASSWORD:
 			printf("Wrong user or password\n");
			exit(0);
 		case AE_FILE_OPEN_FAILED:
 			fprintf(stderr, "Account file failed to open\n");
			exit(1);
 		default:
 			fprintf(stderr, "Error: Login error returns wrong\n");
 	}
}



void inputLogout()
{
	SessionError error;

	if (removeSession() == SS_NO_ACTIVE_SESSION_FOUND)
	{
		fprintf(stderr, "No active session found\n");
		exit(1);
	}
	printf("Logout success\n");
}

void inputRegister()
{
		Account newAccount;
		int newAccountID = getNewAccountID();

		printf("First Name: ");
		scanf("%50s", newAccount.firstName);
		printf("Last Name: ");
		scanf("%50s", newAccount.lastName);
		printf("Age: ");
		scanf("%hhu", &newAccount.age);
		printf("Password: ");
		scanf("%255s", newAccount.password);

		if (createNewAccount(newAccount.firstName, newAccount.lastName, newAccount.password, newAccount.age, &newAccount) == AE_FILE_OPEN_FAILED)
		{
			fprintf(stderr, "Error: Failed to open account\n");
		}

		printf("The account ID is %d\n", newAccount.iD);
}

void inputGoToSeat(const char fileName[], const char inputColumn[], const char inputRow[])
{
	if (!stringIsInt(inputColumn))
	{
		printf("Column is not an integer\n");
		return;
	}
	if (!stringIsInt(inputRow))
	{
		printf("Row is not an integer\n");
		return;
	}

	int column = atoi(inputColumn);
	int row = atoi(inputRow);
	Account accountInfo;
	if (loadSessionInfo(&accountInfo, sizeof(Account)) == SS_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Error: Failed to load session info\n");
		exit(1);
	}

	Plane currentPlane;
	if (loadPlane(&currentPlane, fileName) == PLN_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Error: Failed to load plane\n");
		exit(1);
	}
	
	int currentSeatColumn;
	int currentSeatRow;
	getAccountSeat(&accountInfo, &currentPlane, &currentSeatColumn, &currentSeatRow);
	printf("%d, %d\n", currentSeatColumn, currentSeatRow);

	if (currentSeatColumn != -1 || currentSeatRow != -1)
	{
		printf("Account has already registered a seat\n");
		return;
	}

	PlaneErrors plnError = getToSeat(&currentPlane, accountInfo.iD, column, row);

	switch (plnError)
	{
		case PLN_SUCCESS:
			savePlane(&currentPlane, fileName);
			printf("Success!\n");
			break;
		case PLN_OUT_OF_RANGE:
			fprintf(stderr, "Input out of range\n");
			break;
		case PLN_SEAT_FILLED:
			fprintf(stderr, "Seat filled \n");
			break;
		default:
			fprintf(stderr, "Error: Get to seat failed\n");
			break;
	} 
}


void inputClearAccountSeat(const char fileName[])
{
	Plane currentPlane;
	if (loadPlane(&currentPlane, fileName) == PLN_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "error: Failed to load plane\n");
		exit(1);
	}

	Account accountInfo;
	int currentSeatColumn;
	int currentSeatRow;

	if (loadSessionInfo(&accountInfo, sizeof(Account)) != SS_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to load session info\n");
		exit(1);
	}

	getAccountSeat(&accountInfo, &currentPlane, &currentSeatColumn, &currentSeatRow);
	
	if (currentSeatColumn == -1 || currentSeatRow == -1)
	{
		printf("Account has no seat registered in this place\n");
		return;
	}

	PlaneErrors plnError = clearSeat(&currentPlane, currentSeatColumn, currentSeatRow);

	switch (plnError)
	{
		case PLN_SUCCESS:
			printf("Cleared seat at %d, %d.\n", currentSeatColumn, currentSeatRow);
			savePlane(&currentPlane, fileName);
			break;
		case PLN_OUT_OF_RANGE:
			printf("Seat is out of range\n");
			break;
		case PLN_SEAT_NOT_FILLED:
			printf("Seat is not filled\n");
			break;
		default:
			fprintf(stderr, "Error: Clear seat on %d, %d failed\n", currentSeatColumn, currentSeatRow);
			break;
	}
}

void inputMoveAccountSeat(const char fileName[], const char inputColumn[], const char inputRow[])
{
	Plane currentPlane;
	if (loadPlane(&currentPlane, fileName) == PLN_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "error: Failed to load plane\n");
		exit(1);
	}

	if (!stringIsInt(inputColumn))
	{
		printf("Row is not an integer\n");
		return;
	}

	int column = atoi(inputColumn);
	int row = atoi(inputRow);
	
	Account accountInfo;
	int currentSeatColumn;
	int currentSeatRow;

	if (loadSessionInfo(&accountInfo, sizeof(Account)) != SS_SUCCESS)
	{
		fprintf(stderr, "Error: Failed to load session info\n");
		exit(1);
	}

	getAccountSeat(&accountInfo, &currentPlane, &currentSeatColumn, &currentSeatRow);
	
	if (currentSeatColumn == -1 || currentSeatRow == -1)
	{
		printf("Account has no seat registered in this place\n");
		return;
	}

	PlaneErrors plnError = moveSeat(&currentPlane, currentSeatColumn, currentSeatRow, column, row);

	switch (plnError)
	{
		case PLN_SUCCESS:
			printf("Moved seat from %d, %d to %d, %d\n", currentSeatColumn, currentSeatRow, column, row);
			savePlane(&currentPlane, fileName);
			break;
		case PLN_OUT_OF_RANGE:
			printf("Seat is out of range\n");
			break;
		case PLN_SEAT_FILLED:
			printf("New seat is filled\n");
			break;
		case PLN_SEAT_NOT_FILLED:
			fprintf(stderr, "Error: Previous seat not filled check implementation.\n");
			exit(1);
		default:
			fprintf(stderr, "Error: Failed to move seat\n");
			exit(1);
			break;
	}
}

bool stringIsInt(const char string[])
{
	int length = strlen(string);

	for (int i = 0; i < length ; i++)
	{
		if (string[i] < '0' || string[i] > '9')  return false;
	}

	return true;
}

void getAccountSeat(const Account* account, const Plane* plane, int* column, int* row)
{
	*column = -1;
	*row = -1;
	for (int iColumn = 0; iColumn < PLANE_COLUMN; iColumn++)
	{
		for (int iRow = 0; iRow < PLANE_ROW; iRow++)
		{
			if (plane->seats[iColumn][iRow] == account->iD)
			{
				*column = iColumn;
				*row = iRow;
			}
		}
	}

}
