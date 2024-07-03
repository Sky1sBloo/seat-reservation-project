/*
 * Basic account system without any encryption and very vulnerable (especially getNewAccountID)
 */

#pragma once

#define ACCOUNT_FILE "accounts.dat"
#define ACCOUNT_UPDATE_FILE "account-temp.dat"  // Name of the new file when updated (gets quickly removed)
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
	AE_FILE_CREATE_FAILED,
	AE_FILE_REMOVE_FAILED,
	AE_FILE_RENAME_FAILED,
	AE_INPUT_TOO_LONG
} AccountError;

/**
 * Creates a new account, sets its ID based on getNewAccountID()
 *
 * @param[out] newAccount Output struct of the new account
 * 
 * @return
 *  AE_FILE_OPEN_FAILED - Failed to create/append on ACCOUNT_FILE
 *  AE_SUCCESS
 */
AccountError createNewAccount(const char* firstName, const char* lastName, const char* password, unsigned char age, Account* newAccount);

/**
 * Updates new account based on accountID
 *
 * @param accountID of user to be replaced
 * @param newAccount that will be replaced
 *
 * @return
 *  AE_FILE_OPEN_FAILED Failed to read existing ACCOUNT_FILE
 *  AE_FILE_CREATE_FAILED Failed to create new ACCOUNT_FILE
 *  AE_FILE_REMOVE_FAILED Failed to remove old ACCOUNT_FILE
 *  AE_FILE_RENAME_FAILED Failed to rename ACCOUNT_UPDATE_FILE to ACCOUNT_FILE
 *  AE_SUCCESS
 */
AccountError updateAccount(int accountID, const Account* newAccount);

/**
 * Creates a new account ID based on ACCOUNT_FILE
 *
 * @return Current highest ID in ACCOUNT_FILE + 1
 */
int getNewAccountID();

/**
 * Checks password if matches with userID based on ACCOUNT_FILE
 *
 * @param[out] account Output struct when login is successful
 *
 * @return
 *  AE_FILE_OPEN_FAILED - Failed to read ACCOUNT_FILE
 *  AE_WRONG_USER_OR_PASSWORD - Invalid account details
 *  AE_SUCCESS
 */
AccountError loginAccount(int accountID, const char* password, Account* account);

/// Changes account first name
AccountError changeAccountFirstName(Account* account, const char* newFirstName);

/// Changes account last name
AccountError changeAccountLastName(Account* account, const char* newLastName);

/// Changes account password
AccountError changeAccountPassword(Account* account, const char* newPassword);

/// Changes account age
AccountError changeAccountAge(Account* account, unsigned char newAge);

/// Views all possible accounts and their information
void listAllAccounts();
