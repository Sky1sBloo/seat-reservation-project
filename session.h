/*
 * Basic session saving for login by JD
 *
 * Supports any data type
 */
#pragma once

#include <stdio.h>
#define SESSION_FILE "session.dat"

typedef enum
{
	SS_SUCCESS,
	SS_NO_ACTIVE_SESSION_FOUND,
	SS_FILE_OPEN_FAILED,
	SS_FILE_CREATE_FAILED,
	SS_FILE_REMOVE_FAILED
} SessionError;

/**
 * Saves session to SESSION_FILE
 *
 * @return
 *  SS_FILE_CREATE_FAILED - Failed to create SESSION_FILE
 *  SS_SUCCESS
 */
SessionError saveSession(const void* sessionInfo, size_t size);

/**
 * Loads active session info
 *
 * @param[out] sessionInfo Output session after loading from SESSION_FILE
 *
 * @return
 *   SS_FILE_OPEN_FAILED - Failed to read SESSION_FILE
 *   SS_SUCCESS
 */
SessionError loadSessionInfo(void* sessionInfo, size_t size);

/**
 * Removes SESSION_FILE
 * (Logout)
 *
 * @return
 *  SS_NO_ACTIVE_SESSION_FOUND - Failed to find SESSION_FILE
 *  SS_FILE_REMOVE_FAILED - Failed to remove SESSION_FILE
 *  SS_SUCCESS
 */
SessionError removeSession();
