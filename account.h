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
	unsigned char age; } Account;

typedef enum
{
	AE_SUCCESS,
	AE_WRONG_USER_OR_PASSWORD,
	AE_FILE_OPEN_FAILED,
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

/**
 * Changes account first name
 *
 * @param[out] account Output of changed first name
 *
 * @return
 *  AE_INPUT_TOO_LONG - parameter newFirstName exceeds NAME_LENGTH
 *  AE_SUCCESS
 */
AccountError changeAccountFirstName(Account* account, const char* newFirstName);

/**
 * Changes account last name
 *
 * @param[out] account Output of changed last name
 *
 * @return
 *  AE_INPUT_TOO_LONG - parameter newLastName exceeds NAME_LENGTH
 *  AE_SUCCESS
 */
AccountError changeAccountLastName(Account* account, const char* newLastName);

/**
 * Changes account password
 *
 * @param[out] account Output of changed password
 *
 * @return
 *  AE_INPUT_TOO_LONG - parameter newPassword exceeds PASSWORD_LENGTH
 *  AE_SUCCESS
 */
AccountError changeAccountPassword(Account* account, const char* newPassword);

/**
 * Changes account age
 *
 *
 * @param[out] account Output of changed age
 *
 * @return
 *  AE_SUCCESS
 */
AccountError changeAccountAge(Account* account, unsigned char newAge);

/// Views all possible accounts and their information
void listAllAccounts();
