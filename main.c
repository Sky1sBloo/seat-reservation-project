#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input-handler.h"

void showHelpPrint()
{
	printf("-h, --help                         		   | List help commands\n"
			"-r, --register                                 | Registers new user\n"
			"-l, --login [accountID] [password]             | Logs in using credentials\n"
			"-o, --logout                                   | Logs out user \n"
			"-c, --create [filename]                        | Creates new plane\n"
			"-d, --display [filename]                       | Display current plane contents\n"
			"-g, --go-to-seat [filename] [seat]             | Goes to the seat of the plane\n"
			"-s, --clear-current-seat [filename]            | Clears the current seat assigned to the plane\n"
			"-m, --move-to-seat [filename] [seat]           | Moves to the new seat\n"
			"-v, --view-account-information                 | Views name, ID, and age of the user\n"
			"-n, --change-account-name [first] [lastname]   | Changes account username\n"
			"-p, --change-account-password [old] [new]      | Changes account password\n"
			"-a, --change-account-age [age]                 | Change account age\n\n\n\n"
			"Requires administrator privilages:\n"
			"-D, --debug-show-accounts                      | Lists all accounts\n"
			"-e, --enable-seat [filename] [seat]            | enables seats to be active \n"
			"-x, --disable-seat [filename] [seat]           | disables seats\n"
			"-i, --view-account-in-seat [filename] [seat]   | Views the account info in seat\n"
			"-A, --debug-make-admin                         | Makes userID admin\n"
			);
}

void showMissingArgumentsMessage()
{
	fprintf(stderr, "Missing arguments, use --help for correct usage\n");
	exit(INPUT_ERROR);
}

/// Checks if argument matches with either target arguments
bool isArgumentMatch(const char argument[], const char targetArgument[], const char alternativeArgument[])
{
	return (strcmp(argument, targetArgument) == 0 || strcmp(argument, alternativeArgument) == 0);
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
	if (argc < (index + numOfReqArgs + 1))
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
		if (isArgumentMatch(argv[i], "--help", "-h"))
		{
			showHelpPrint();
		}
		else if (isArgumentMatch(argv[i],"--display", "-d"))
		{
			if (!argumentIsSupplied(argc, i, 1))  return INPUT_ERROR;

			inputDisplayPlane(argv[++i]);
		}
		else if (isArgumentMatch(argv[i], "--go-to-seat", "-g"))
		{
			if (!argumentIsSupplied(argc, i, 2))  return INPUT_ERROR;
			const char* fileName = argv[++i];
			const char* seatPosition = argv[++i];
			

			inputGoToSeat(fileName, seatPosition);
		}
		else if (isArgumentMatch(argv[i], "--move-to-seat", "-m"))
		{
			if (!argumentIsSupplied(argc, i, 2))  return INPUT_ERROR;
			const char* fileName = argv[++i];
			const char* seatPosition = argv[++i];

			inputMoveAccountSeat(fileName, seatPosition);
		}
		else if (isArgumentMatch(argv[i], "--clear-current-seat", "-s"))
		{
			if (!argumentIsSupplied(argc, i, 1))  return INPUT_ERROR;

			inputClearAccountSeat(argv[++i]);
		}
		else if (isArgumentMatch(argv[i], "--create", "-c"))
		{
			if (!argumentIsSupplied(argc, i, 1))  return INPUT_ERROR;

			inputCreatePlane(argv[++i]);
		}
		else if (isArgumentMatch(argv[i], "--login", "-l"))
		{
			if (!argumentIsSupplied(argc, i, 2))  return INPUT_ERROR;

			const char* accountID = argv[++i];
			const char* password = argv[++i];
			
			inputLogin(accountID, password);
		}
		else if (isArgumentMatch(argv[i], "--logout", "-o"))
		{
			inputLogout();
		}
		else if (isArgumentMatch(argv[i], "--register", "-r"))
		{
			inputRegister();
		}
		else if (isArgumentMatch(argv[i], "--view-account-information", "-v"))
		{
			viewAccountInformation();
		}
		else if (isArgumentMatch(argv[i], "--change-account-name", "n"))
		{
			if (!argumentIsSupplied(argc, i, 2)) return INPUT_ERROR;

			const char* firstName = argv[++i];
			const char* lastName = argv[++i];
			
			inputChangeAccountName(firstName, lastName);
		}
		else if (isArgumentMatch(argv[i], "--change-account-password", "-p"))
		{
			if (!argumentIsSupplied(argc, i, 2)) return INPUT_ERROR;

			const char* oldPassword = argv[++i];
			const char* newPassword = argv[++i];

			inputChangeAccountPassword(oldPassword, newPassword);
		}
		else if (isArgumentMatch(argv[i], "--change-account-age", "-a"))
		{
			if (!argumentIsSupplied(argc, i, 1)) return INPUT_ERROR;

			inputChangeAccountAge(argv[++i]);
		}
		else if (isArgumentMatch(argv[i], "--debug-show-accounts", "-D"))
		{
			inputShowAccounts();
		}
		else if (isArgumentMatch(argv[i], "--enable-seat", "-e"))
		{
			if (!argumentIsSupplied(argc, i, 2)) return INPUT_ERROR;
			const char* fileName = argv[++i];
			const char* seat = argv[++i];

			inputEnableSeat(fileName, seat);
		}
		else if (isArgumentMatch(argv[i], "--disable-seat", "-x"))
		{
			if (!argumentIsSupplied(argc, i, 2)) return INPUT_ERROR;
			const char* fileName = argv[++i];
			const char* seat = argv[++i];

			inputDisableSeat(fileName, seat);
		}
		else if (isArgumentMatch(argv[i], "--view-account-in-seat", "-i"))
		{
			if (!argumentIsSupplied(argc, i, 2))  return INPUT_ERROR;

			const char* fileName = argv[++i];
			const char* seat = argv[++i];

			inputViewAccountInSeat(fileName, seat);
		}
		else if (isArgumentMatch(argv[i], "--debug-make-admin", "-A"))
		{
			inputDebugMakeAdmin();
		}
		else
		{
			showHelpPrint();
		}
	}
}
