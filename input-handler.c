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
		exit(FILE_WRITE_ERROR);
	}

	Account accountSession;
	SessionError sessionError = loadSessionInfo(&accountSession, sizeof(Account));

	if (sessionError == SS_NO_ACTIVE_SESSION_FOUND)
	{
		printPlane(&loadedPlane, -1);
	}
	else
	{
		printPlane(&loadedPlane, accountSession.iD);
	}

	if (isPlaneFull(&loadedPlane))
	{
		printf("Plane is full!\n");
	}
}

void inputCreatePlane(const char fileName[])
{
	FILE* newPlaneFile= fopen(fileName, "wb");

	if (newPlaneFile == NULL)
	{
		fprintf(stderr, "Error: Failed to create file\n");
		exit(FILE_WRITE_ERROR);
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
		exit(FILE_READ_ERROR);
 	}

	// Sanitize input

 	Account accountSession;	
 	AccountError loginError;

	if (!stringIsInt(accountID))
	{
		printf("Account ID is not a numerical value\n");
		exit(INPUT_ERROR);
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
			exit(INPUT_ERROR);
 		case AE_FILE_OPEN_FAILED:
 			fprintf(stderr, "Account file failed to open\n");
			exit(FILE_READ_ERROR);
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
		exit(FILE_READ_ERROR);
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
		exit(FILE_WRITE_ERROR);
	}

	printf("The account ID is %d\n", newAccount.iD);
}

void viewAccountInformation()
{
	Account account;
	
	if (loadSessionInfo(&account, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
	{
		fprintf(stderr, "Failed to find account information\n");
		exit(FILE_READ_ERROR);
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
			exit(FILE_READ_ERROR);
			break;
		case AE_FILE_CREATE_FAILED:
			fprintf(stderr, "Error: Failed to create Account Update File\n");
			exit(FILE_WRITE_ERROR);
			break;
		case AE_FILE_REMOVE_FAILED:
			fprintf(stderr, "Error: Failed to remove old account file\n");
			exit(FILE_ERROR);
			break;
		case AE_FILE_RENAME_FAILED:
			fprintf(stderr, "Error: Failed to rename Account Update File\n");
			exit(FILE_ERROR);
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
	
	if (loadSessionInfo(&account, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
	{
		fprintf(stderr, "Error to find account information\n");
		exit(FILE_READ_ERROR);
	}

	if (changeAccountFirstName(&account, firstName) == AE_INPUT_TOO_LONG)
	{
		printf("First name too long\n");
		exit(INPUT_ERROR);
	}
	if (changeAccountLastName(&account, lastName) == AE_INPUT_TOO_LONG)
	{
		printf("Last name too long\n");
		exit(INPUT_ERROR);
	}

	if (saveSession(&account, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
	{
		fprintf(stderr, "Error: Failed to update session file\n");
		exit(FILE_READ_ERROR);
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
 	if (loadSessionInfo(&sessionInfo, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
 	{
 		fprintf(stderr, "There is no session found. Login first.\n");
		exit(FILE_READ_ERROR);
 	}

 	AccountError loginError;

 	loginError = loginAccount(sessionInfo.iD, oldPassword, &sessionInfo);
 
 	switch (loginError)
 	{
 		case AE_SUCCESS:
 			break;
 		case AE_WRONG_USER_OR_PASSWORD:
 			printf("Wrong user or password\n");
			exit(INPUT_ERROR);
 		case AE_FILE_OPEN_FAILED:
 			fprintf(stderr, "Account file failed to open\n");
			exit(FILE_READ_ERROR);
 		default:
 			fprintf(stderr, "Error: Login error returns wrong\n");
			exit(-1);
			break;
 	}

	if (changeAccountPassword(&sessionInfo, newPassword) == AE_INPUT_TOO_LONG)
	{
		fprintf(stderr, "Input too long\n");
		exit(INPUT_ERROR);
	}



	if (updateInputAccount(sessionInfo.iD, &sessionInfo))
	{
		printf("Successfully updated password of User %d\n", sessionInfo.iD);
	}
}

void inputChangeAccountAge(const char newAge[])
{
	if (!stringIsInt(newAge) || strlen(newAge) > 3)
	{
		fprintf(stderr, "Error: Input is not a number or too long\n");
		exit(INPUT_ERROR);
	}

	int age = atoi(newAge);

	if (age > 255)
	{
		fprintf(stderr, "Error: Input is not a number or too long\n");
		exit(INPUT_ERROR);
	}
	
	Account sessionInfo;	
 	if (loadSessionInfo(&sessionInfo, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
 	{
 		fprintf(stderr, "There is no session found. Login first.\n");
		exit(FILE_READ_ERROR);
 	}


	changeAccountAge(&sessionInfo, (unsigned char) age);
	if (saveSession(&sessionInfo, sizeof(Account)) == SS_FILE_CREATE_FAILED)
	{
		fprintf(stderr, "Error: Failed to update session\n");
		exit(FILE_WRITE_ERROR);
	}

	if (updateInputAccount(sessionInfo.iD, &sessionInfo))
	{
		printf("Successfuly updated age of User %d\n", sessionInfo.iD);
	}
}

void inputGoToSeat(const char fileName[], const char seatPosition[])
{
	Account accountInfo;
	inputLoadSessionInfo(&accountInfo);

	Plane currentPlane;
	inputLoadPlane(&currentPlane, fileName);

	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		fprintf(stderr,"Invalid seat input: %s\n", seatPosition);
		exit(INPUT_ERROR);
	}
	
	int currentSeatColumn;
	int currentSeatRow;
	getAccountSeat(&accountInfo, &currentPlane, &currentSeatColumn, &currentSeatRow);

	if (currentSeatColumn != -1 || currentSeatRow != -1)
	{
		fprintf(stderr, "Account has already registered a seat\n");
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
			exit(INPUT_ERROR);
		case PLN_SEAT_FILLED:
			fprintf(stderr, "Seat filled \n");
			exit(INPUT_ERROR);
		default:
			fprintf(stderr, "Error: Get to seat failed\n");
			exit(-1);
	} 
}


void inputClearAccountSeat(const char fileName[])
{
	Plane currentPlane;
	inputLoadPlane(&currentPlane, fileName);


	int currentSeatColumn;
	int currentSeatRow;
	char currentSeatFormatted[2];  // For displaying formatted output when successful


	Account accountInfo;
	inputLoadSessionInfo(&accountInfo);
	getAccountSeat(&accountInfo, &currentPlane, &currentSeatColumn, &currentSeatRow);
	inputConvertSeatToFormatted(currentSeatColumn, currentSeatRow, currentSeatFormatted);
	
	if (currentSeatColumn == -1 || currentSeatRow == -1)
	{
		fprintf(stderr, "Account has no seat registered in this place\n");
		exit(INPUT_ERROR);
	}

	PlaneErrors plnError = clearSeat(&currentPlane, currentSeatColumn, currentSeatRow);

	switch (plnError)
	{
		case PLN_SUCCESS:
			printf("Success! Cleared seat at %s\n", currentSeatFormatted);
			savePlane(&currentPlane, fileName);
			break;
		case PLN_OUT_OF_RANGE:
			fprintf(stderr, "Seat is out of range\n");
			exit(INPUT_ERROR);
		case PLN_SEAT_NOT_FILLED:
			fprintf(stderr, "Seat is not filled\n");
			exit(INPUT_ERROR);
		default:
			fprintf(stderr, "Error: Clear seat on %d, %d failed\n", currentSeatColumn, currentSeatRow);
			break;
	}
}

void inputMoveAccountSeat(const char fileName[], const char seatPosition[])
{
	Plane currentPlane;
	inputLoadPlane(&currentPlane, fileName);

	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		printf("Invalid seat input: %s\n", seatPosition);
		exit(INPUT_ERROR);
	}

	int currentSeatColumn;
	int currentSeatRow;
	char currentSeatFormatted[2];  // For displaying formatted output when successful
	inputConvertSeatToFormatted(currentSeatColumn, currentSeatRow, currentSeatFormatted);

	Account accountInfo;
	inputLoadSessionInfo(&accountInfo);

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
			fprintf(stderr, "Seat is out of range\n");
			exit(INPUT_ERROR);
		case PLN_SEAT_FILLED:
			fprintf(stderr, "Error: Target seat is currently filled\n");
			exit(INPUT_ERROR);
		case PLN_SEAT_NOT_FILLED:
			fprintf(stderr, "Error: Previous seat not filled check implementation.\n");
			exit(-1);
		default:
			fprintf(stderr, "Error: Failed to move seat\n");
			exit(-1);
			break;
	}
}

void inputDisableSeat(const char fileName[], const char seatPosition[])
{
	Account sessionInfo;
	inputLoadSessionInfo(&sessionInfo);

	if (!sessionInfo.isAdmin)
	{
		fprintf(stderr, "Account doesn't have administrator privilages\n");
		exit(NO_ADMIN_PRIVILAGES);
	}

	Plane currentPlane;
	inputLoadPlane(&currentPlane, fileName);

	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		printf("Invalid seat input: %s\n", seatPosition);
		exit(INPUT_ERROR);
	}
	
	if (column < 0 || row < 0 || column > PLANE_COLUMN || row > PLANE_ROW)
	{
		fprintf(stderr, "Input out of range\n");
		exit(INPUT_ERROR);
	}

	currentPlane.seats[column][row] = -2;

	savePlane(&currentPlane, fileName);
	printf("Success! Disabled seat on %s\n", seatPosition);
}

void inputEnableSeat(const char fileName[], const char seatPosition[])
{
	Account sessionInfo;
	inputLoadSessionInfo(&sessionInfo);

	if (!sessionInfo.isAdmin)
	{
		fprintf(stderr, "Account doesn't have administrator privilages\n");
		exit(NO_ADMIN_PRIVILAGES);
	}

	Plane currentPlane;
	inputLoadPlane(&currentPlane, fileName);

	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		fprintf(stderr, "Invalid seat input: %s\n", seatPosition);
		exit(INPUT_ERROR);
	}

	PlaneErrors plnError = clearSeat(&currentPlane, column, row);

	switch (plnError)
	{
		case PLN_SUCCESS:
			savePlane(&currentPlane, fileName);
			printf("Success! Enabled seat on %s\n", seatPosition);
			break;
		case PLN_OUT_OF_RANGE:
			fprintf(stderr, "Seat is out of range\n");
			exit(INPUT_ERROR);		
		case PLN_SEAT_NOT_FILLED:
			fprintf(stderr, "Seat is not filled\n");
			exit(INPUT_ERROR);
		default:
			fprintf(stderr, "Error: Failed to disable seat\n");
			exit(-1);
	}
	
}

void inputViewAccountInSeat(const char fileName[], const char seatPosition[])
{
	Account sessionInfo;
	inputLoadSessionInfo(&sessionInfo);

	if (!sessionInfo.isAdmin)
	{
		fprintf(stderr, "Account doesn't have administrator privilages\n");
		exit(NO_ADMIN_PRIVILAGES);
	}

	
	Plane currentPlane;
	inputLoadPlane(&currentPlane, fileName);

	int column;
	int row;
	if (!inputConvertFormattedSeat(seatPosition, &column, &row))
	{
		fprintf(stderr,"Invalid seat input: %s\n", seatPosition);
		exit(INPUT_ERROR);
	}

	int seatAccountID;
	if (getValueOfSeat(&seatAccountID, &currentPlane, column, row) == PLN_OUT_OF_RANGE)
	{
		fprintf(stderr, "Seat input out of range\n");
		exit(INPUT_ERROR);
	}

	if (seatAccountID < -1)
	{
		fprintf(stderr, "Seat is unavailable for this plane\n");
		exit(INPUT_ERROR);
	}
	
	Account account;	
	if (findAccount(&account, seatAccountID) == AE_CANNOT_FIND_ACCOUNT)
	{
		fprintf(stderr, "Cannot find account\n");
		exit(FILE_READ_ERROR);
	}

	printf("User: %d \nName: %s %s \nAge: %d\n", account.iD, account.firstName, account.lastName, account.age);
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
	unsigned long stringLength = strlen(formattedInput);
	if (stringLength > 3 || stringLength < 2)  return false;

	if (stringLength == 2)
	{
		if (!isalpha(formattedInput[1]))  return false;
	
		char formattedRow = formattedInput[0];

		if (formattedRow < '0' || formattedRow > '9')  return false;
		char formattedColumn = toupper(formattedInput[1]);
	
		*row = formattedRow - '0' - 1;
		*column = formattedColumn - 'A';
	}
	else if (stringLength == 3)
	{
		if (!isalpha(formattedInput[2]))  return false;

		char formattedRow[3] = {formattedInput[0], formattedInput[1], '\0'};
		if (!stringIsInt(formattedRow))  return false;

		char formattedColumn = toupper(formattedInput[2]);

		*row = atoi(formattedRow) - 1;
		*column = formattedColumn - 'A';
	}
	
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

void inputLoadSessionInfo(Account* sessionInfo)
{
 	if (loadSessionInfo(sessionInfo, sizeof(Account)) == SS_NO_ACTIVE_SESSION_FOUND)
 	{
 		fprintf(stderr, "Error: Session file not found. Have you logged in? \n");
		exit(FILE_READ_ERROR);
 	}
}

void inputLoadPlane(Plane* plane, const char fileName[])
{
	if (loadPlane(plane, fileName) == PLN_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Error: Failed to load plane\n");
		exit(FILE_READ_ERROR);
	}
}


void inputDebugMakeAdmin()
{
	Account sessionInfo;
	inputLoadSessionInfo(&sessionInfo);

	if (debugMakeAccountAdmin(sessionInfo.iD) == AE_FILE_OPEN_FAILED)
	{
		fprintf(stderr, "Error: Failed to open account file\n");
		exit(FILE_READ_ERROR);
	}
}

void inputShowAccounts()
{
	Account sessionInfo;
	inputLoadSessionInfo(&sessionInfo);

	if (!sessionInfo.isAdmin)
	{
		fprintf(stderr, "Account doesn't have administrator privilages\n");
		exit(NO_ADMIN_PRIVILAGES);
	}

	listAllAccounts();
}

