#include "session.h"


SessionError saveSession(const void* sessionInfo, size_t size)
{
	FILE* sessionFile = fopen(SESSION_FILE, "wb");

	if (sessionFile == NULL)
	{
		return SS_FILE_CREATE_FAILED;
	}

	fwrite(sessionInfo, size, 1, sessionFile);
	fclose(sessionFile);
	return SS_SUCCESS;
}

SessionError loadSessionInfo(void* sessionInfo, size_t size)
{
	FILE* sessionFile = fopen(SESSION_FILE, "rb");

	if (sessionFile == NULL)
	{
		return SS_FILE_OPEN_FAILED;
	}

	fread(sessionInfo, size, 1, sessionFile);

	fclose(sessionFile);


	return SS_SUCCESS;
}

SessionError removeSession()
{
	FILE* sessionFile = fopen(SESSION_FILE, "rb");
	if (sessionFile == NULL)
	{
		return SS_NO_ACTIVE_SESSION_FOUND;
	}
	fclose(sessionFile);
	if (remove(SESSION_FILE) == -1)
	{
		return SS_FILE_REMOVE_FAILED;
	}

	return SS_SUCCESS;
}


