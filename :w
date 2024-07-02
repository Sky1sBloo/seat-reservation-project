/*
 * Formatted error handling and input sanitizing
 */
#pragma once
#include <stdbool.h>

#include "account.h"
#include "plane.h"


void inputDisplayPlane(const char fileName[]);

// Creates/overrites a new plane
void inputCreatePlane(const char filename[]);

// TODO: Add input sanitizer
void inputLogin(const char accountID[], const char password[]);

// Logs the account
void inputLogout();

// Creates a new account 
void inputRegister();

// Sets user to seat
void inputGoToSeat(const char fileName[], const char inputColumn[], const char inputRow[]);

// Removes current setted seat of account
void inputClearAccountSeat(const char fileName[]);

// Checks if the string is a number (positive)
bool stringIsInt(const char string[]);

/**
 * Retrieves current seat of account
 *
 * @param account Account struct of account
 * @param plane Where account seat will be checked
 * @param column Column of account seat, returns -1 when it cannot be found
 * @param row Row of account seat, returns -1 when it cannot be found
 */
void getAccountSeat(const Account* account, const Plane* plane, int* column, int* row);
