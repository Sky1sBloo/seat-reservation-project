#include "input-handler.h"
#include <ctype.h>
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
 	if (loadSessionInfo(&sessionInfo, sizeof(Account)) != SS_NO_ACTIVE_SESSION_FOUND)
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
		exit(-1);
	}

	printf("The account ID is %d\n", newAccount.iD);
}

void viewAccountInformation()
{
	Account account;
	
	if (loadSessionInfo(&account, sizeof(Account)) == SS_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Failed to find account information\n");
		exit(1);
	}

	printf("ID: %d\n"
			"Name: %s %s\n"
			"Age: %d\n",
			account.iD, account.firstName, account.lastName, account.age);
}

bool updateInputAccount(int accountID, const Account* newAccount)
{
	AccountError updateError = updateAccount(accountID, newAccount);

	switch (updateError)
	{
		case AE_SUCCESS:
			break;
		case AE_FILE_OPEN_FAILED:
			fprintf(stderr, "Error: Failed to open Account file\n");
			exit(1);
			break;
		case AE_FILE_CREATE_FAILED:
			fprintf(stderr, "Error: Failed to create Account Update File\n");
			exit(-1);
			break;
		case AE_FILE_REMOVE_FAILED:
			fprintf(stderr, "Error: Failed to remove old account file\n");
			exit(-1);
			break;
		case AE_FILE_RENAME_FAILED:
			fprintf(stderr, "Error: Failed to rename Account Update File\n");
			exit(-1);
			break;
		default:
			fprintf(stderr, "Error: Failed to update account\n");
			exit(-1);
			break;
	}
	return true;
}

void inputChangeAccountName(const char firstName[], const char lastName[])
{	
	Account account;
	
	if (loadSessionInfo(&account, sizeof(Account)) == SS_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Error to find account information\n");
		exit(1);
	}

	if (changeAccountFirstName(&account, firstName) == AE_INPUT_TOO_LONG)
	{
		printf("First name too long\n");
		exit(1);
	}
	if (changeAccountLastName(&account, lastName) == AE_INPUT_TOO_LONG)
	{
		printf("Last name too long\n");
		exit(1);
	}

	if (saveSession(&account, sizeof(Account)) == SS_FILE_CREATE_FAILED)
	{
		fprintf(stderr, "Error: Failed to create file\n");
		exit(-1);
	}

	if (updateInputAccount(account.iD, &account))
	{
		printf("Successfuly updated ID: %d, to %s %s\n", account.iD, firstName, lastName);
	}
}

void inputChangeAccountPassword(const char oldPassword[], const char newPassword[])
{
	// Check if login
 	Account sessionInfo;
 	if (loadSessionInfo(&sessionInfo, sizeof(Account)) == SS_FILE_OPEN_FAILED)
 	{
 		fprintf(stderr, "There is no session found. Login first.\n");
		exit(1);
 	}

 	AccountError loginError;

 	loginError = loginAccount(sessionInfo.iD, oldPassword, &sessionInfo);
 
 	switch (loginError)
 	{
 		case AE_SUCCESS:
 			break;
 		case AE_WRONG_USER_OR_PASSWORD:
 			printf("Wrong user or password\n");
 		case AE_FILE_OPEN_FAILED:
 			fprintf(stderr, "Account file failed to open\n");
			exit(1);
 		default:
 			fprintf(stderr, "Error: Login error returns wrong\n");
			exit(-1);
			break;
 	}

	if (changeAccountPassword(&sessionInfo, newPassword) == AE_INPUT_TOO_LONG)
	{
		fprintf(stderr, "Input too long\n");
		exit(1);
	}

	if (updateInputAccount(sessionInfo.iD, &sessionInfo))
	{
		printf("Successfully updated password of %d\n", sessionInfo.iD);
	}
}

void inputGoToSeat(const char fileName[], const char seatPosition[])
{
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
	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		printf("Invalid seat input: %s\n", seatPosition);
		exit(0);
	}
	
	int currentSeatColumn;
	int currentSeatRow;
	getAccountSeat(&accountInfo, &currentPlane, &currentSeatColumn, &currentSeatRow);

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
			printf("Success! Moved seat to %s\n", seatPosition);
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
	char currentSeatFormatted[2];  // For displaying formatted output when successful
	inputConvertSeatToFormatted(currentSeatColumn, currentSeatRow, currentSeatFormatted);

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
			printf("Success! Cleared seat at %s\n", currentSeatFormatted);
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

void inputMoveAccountSeat(const char fileName[], const char seatPosition[])
{
	Plane currentPlane;
	if (loadPlane(&currentPlane, fileName) == PLN_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "error: Failed to load plane\n");
		exit(1);
	}

	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		printf("Invalid seat input: %s\n", seatPosition);
		exit(0);
	}

	Account accountInfo;
	int currentSeatColumn;
	int currentSeatRow;
	char currentSeatFormatted[2];  // For displaying formatted output when successful
	inputConvertSeatToFormatted(currentSeatColumn, currentSeatRow, currentSeatFormatted);

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
			printf("Success! Moved seat from %s to %s\n", currentSeatFormatted, seatPosition);
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

bool inputConvertFormattedSeat(const char formattedInput[], int* column, int* row)
{
	if (strlen(formattedInput) != 2)  return false;


	if (!isalpha(formattedInput[1]))  return false;

	char formattedRow = toupper(formattedInput[0]);
	char formattedColumn = toupper(formattedInput[1]);

	*row = formattedRow - '0' - 1;
	*column = formattedColumn - 'A';

	return true;
}

void inputConvertSeatToFormatted(int column, int row, char formattedInput[2])
{
	formattedInput[0] = row + 1 + '0';
	formattedInput[1] = column + 'A';
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
