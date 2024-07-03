/*
 * Formatted error handling and input sanitizing
 */
#pragma once
#include <stdbool.h>

#include "account.h"
#include "plane.h"

#define FILE_WRITE_ERROR 2
#define FILE_READ_ERROR 3
#define FILE_ERROR 4
#define INPUT_ERROR 5


/// Prints filled seat in plane
void inputDisplayPlane(const char fileName[]);

/// Creates/overrites a new plane
void inputCreatePlane(const char filename[]);

/// Logs in user
void inputLogin(const char accountID[], const char password[]);

/// Logs out the account
void inputLogout();

/// Creates a new account 
void inputRegister();

/// Views the account information
void viewAccountInformation();

/// Calls update account with error messages and exits if failes
bool updateInputAccount(int accountID, const Account* newAccount);

/// Changes accoount name
void inputChangeAccountName(const char firstName[], const char lastName[]);

/// Changes account password with verification
void inputChangeAccountPassword(const char oldPassword[], const char newPassword[]);

/// Changes account age
void inputChangeAccountAge(const char newAge[]);

/// Sets user to seat
void inputGoToSeat(const char fileName[], const char seatPosition[]);

/// Removes current setted seat of account
void inputClearAccountSeat(const char fileName[]);

/// Moves account seat to new area
void inputMoveAccountSeat(const char fileName[], const char seatPosition[]);

/// Checks if the string is a number (positive)
bool stringIsInt(const char string[]);

/** 
 * Converts formatted string to input and row
 * ex. 3A -> 2, 0
 *
 * @param column Returns the converted column
 * @param row Returns the converted row
 * @return If convertion is successful
 */
bool inputConvertFormattedSeat(const char formattedInput[], int* column, int* row);

/**
 * Converts column and row to formatted string
 * ex. 2, 0 -> 3A
 *
 * @param formattedInput returns the formatted version of seat, excluding null terminator
 * @return If convertion is successful
 */
void inputConvertSeatToFormatted(int column, int row, char formattedInput[2]);

/**
 * Retrieves current seat of account
 *
 * @param account Account struct of account
 * @param plane Where account seat will be checked
 * @param column Column of account seat, returns -1 when it cannot be found
 * @param row Row of account seat, returns -1 when it cannot be found
 */
void getAccountSeat(const Account* account, const Plane* plane, int* column, int* row);

