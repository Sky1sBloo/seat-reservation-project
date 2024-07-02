#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "account.h"
#include "input-handler.h"

// TODO: Do input filtering especially on number conversion
// TODO: Malloc accountID and pointers
// TODO: Fix possible file errors
void showHelpPrint()
{
	printf("--help                         		   | List help commands\n"
			"--register                                 | Registers new user\n"
			"--login [accountID] [password]             | Logs in using credentials\n"
			"--logout                                   | Logs out user \n"
			"--create [filename]                        | Creates new plane\n"
			"--display [filename]                       | Display current plane contents\n"
			"--go-to-seat [filename] [column] [row]     | Goes to the seat of the plane\n"
			"--clear-current-seat [filename]            | Clears the current seat assigned to the plane\n"
			);

}

void showMissingArgumentsMessage()
{
	fprintf(stderr, "Missing arguments, use --help for correct usage\n");
}

/**
 * Checks if the argument supplied by the user is enough
 * Also does the printing error
 *
 * @param argc Number of arguments
 * @param index Current index of the loop
 * @param numOfReqArgs Number of required arguments
 */
bool argumentIsSupplied(int argc, int index, int numOfReqArgs)
{
	if (argc - (index - numOfReqArgs) <= 0)
	{
		showMissingArgumentsMessage();
		return false;
	}
	return true;
}



/*
 * Features:
 * Login system (with admin)
 */
int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		showHelpPrint();
	}

	// Argument mode
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			showHelpPrint();
		}
		else if (strcmp(argv[i], "--display") == 0)
		{
			if (!argumentIsSupplied(argc, i, 1))  return 1;

			inputDisplayPlane(argv[++i]);
		}
		else if (strcmp(argv[i], "--go-to-seat") == 0)
		{
			if (!argumentIsSupplied(argc, i, 2))  return 1;
			const char* fileName = argv[++i];
			const char* column = argv[++i];
			const char* row = argv[++i];
			

			inputGoToSeat(fileName, column, row);
		}
		else if (strcmp(argv[i], "--clear-current-seat") == 0)
		{
			if (!argumentIsSupplied(argc, i, 1))  return 1;

			inputClearAccountSeat(argv[++i]);
		}
		else if (strcmp(argv[i], "--create") == 0)
		{
			if (!argumentIsSupplied(argc, i, 1))  return 1;

			inputCreatePlane(argv[++i]);
		}
		else if (strcmp(argv[i], "--login") == 0)
		{
			if (!argumentIsSupplied(argc, i, 2))  return 1;

			const char* accountID = argv[++i];
			const char* password = argv[++i];
			
			inputLogin(accountID, password);
		}
		else if (strcmp(argv[i], "--logout") == 0)
		{
			inputLogout();
		}
		else if (strcmp(argv[i], "--register") == 0)
		{
			inputRegister();
		}
		else if (strcmp(argv[i], "--debug-show-accounts") == 0)
		{
			listAllAccounts();
		}
		else
		{
			showHelpPrint();
		}
	}
}
