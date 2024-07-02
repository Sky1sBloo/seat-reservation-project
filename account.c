#include "account.h"
#include <stdio.h>
#include <string.h>


AccountError createNewAccount(const char* firstName, const char* lastName, const char* password, unsigned char age, Account* newAccount)
{
	FILE* accountFile;
	accountFile = fopen(ACCOUNT_FILE, "a+b");

	if (accountFile == NULL)
	{
		return AE_FILE_OPEN_FAILED;
	}

	newAccount->iD = getNewAccountID();
	strcpy(newAccount->firstName, firstName);
	strcpy(newAccount->lastName, lastName);
	strcpy(newAccount->password, password);
	newAccount->age = age;

	fwrite(newAccount, sizeof(Account), 1, accountFile);

	fclose(accountFile);

	return AE_SUCCESS;
}

AccountError loginAccount(int accountID, const char* password, Account* account)
{
	FILE* accountFile = fopen(ACCOUNT_FILE, "rb");
	if (accountFile == NULL)
	{
		return AE_FILE_OPEN_FAILED;
	}

	Account iAccount;

	while (fread(&iAccount, sizeof(Account), 1, accountFile) > 0)
	{
		if (iAccount.iD!= accountID)  continue;
		
		if (strcmp(iAccount.password, password) == 0)
		{
			*account = iAccount;
			fclose(accountFile);
			return AE_SUCCESS;
		}
		else
		{
			fclose(accountFile);
			return AE_WRONG_USER_OR_PASSWORD;
		}
	}

	fclose(accountFile);

	return AE_WRONG_USER_OR_PASSWORD;
}

int getNewAccountID()
{
	FILE* accountFile;
	accountFile = fopen(ACCOUNT_FILE, "rb");

	if (accountFile == NULL)
	{
		return 0;
	}

	Account iAccount;
	int highestAccountID = 0;

	while (fread(&iAccount, sizeof(Account), 1, accountFile) > 0)
	{
		highestAccountID = iAccount.iD;
	}

	fclose(accountFile);
	return highestAccountID + 1;
}

AccountError changeAccountFirstName(Account* account, const char* newFirstName)
{
	if (strlen(newFirstName) >= NAME_LENGTH)  return AE_INPUT_TOO_LONG;
	strcpy(account->firstName, newFirstName);
	return AE_SUCCESS;
}

AccountError changeAccountLastName(Account* account, const char* newLastName)
{
	if (strlen(newLastName) >= NAME_LENGTH)  return AE_INPUT_TOO_LONG;
	strcpy(account->lastName, newLastName);
	return AE_SUCCESS;
}

AccountError changeAccountPassword(Account* account, const char* newPassword)
{
	if (strlen(newPassword) >= PASSWORD_LENGTH)  return AE_INPUT_TOO_LONG;
	strcpy(account->password, newPassword);
	return AE_SUCCESS;
}

AccountError changeAccountAge(Account* account, unsigned char newAge)
{
	account->age = newAge;

	return AE_SUCCESS;
}

void listAllAccounts()
{
	FILE* accountFile = fopen(ACCOUNT_FILE, "rb");

	if (accountFile == NULL)
	{
		return;
	}
	Account iAccount;
	while (fread(&iAccount, sizeof(Account), 1, accountFile) > 0)
	{
		printf("ID: %d, Usr: %s\n", iAccount.iD, iAccount.firstName);
	}
	
	fclose(accountFile);
}
