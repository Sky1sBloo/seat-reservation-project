/*
 * Basic session saving for login by JD
 *
 *
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
 * Saves session to SESSION_FILE name
 */
SessionError saveSession(const void* sessionInfo, size_t size);

/**
 * Loads active session info
 */
SessionError loadSessionInfo(void* sessionInfo, size_t size);

/**
 * Removes active session
 * (Logout)
 */
SessionError removeSession();
