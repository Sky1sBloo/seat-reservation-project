/*
 * Basic account system without any encryption and very vulnerable (especially getNewAccountID)
 */

#pragma once

#define ACCOUNT_FILE "accounts.dat"
#define NAME_LENGTH 50
#define PASSWORD_LENGTH 255

typedef struct Account
{
	int iD;
	char firstName[NAME_LENGTH];
	char lastName[NAME_LENGTH];
	char password[PASSWORD_LENGTH];
	unsigned char age;
} Account;

typedef enum
{
	AE_SUCCESS,
	AE_WRONG_USER_OR_PASSWORD,
	AE_FILE_OPEN_FAILED,
	AE_INPUT_TOO_LONG
} AccountError;

/**
 */
AccountError createNewAccount(const char* firstName, const char* lastName, const char* password, unsigned char age, Account* newAccount);
int getNewAccountID();

AccountError loginAccount(int accountID, const char* password, Account* account);

AccountError changeAccountFirstName(Account* account, const char* newFirstName);


AccountError changeAccountLastName(Account* account, const char* newLastName);

AccountError changeAccountPassword(Account* account, const char* newPassword);

AccountError changeAccountAge(Account* account, unsigned char newAge);

void listAllAccounts();
