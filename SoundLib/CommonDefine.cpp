#include "CommonDefine.h"
#include "fmod_errors.h"

void (*Common_Private_Error)(FMOD_RESULT, const char*, int);


void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line)
{
	if (result != FMOD_OK)
	{
		if (Common_Private_Error)
		{
			Common_Private_Error(result, file, line);
		}
		
		///Common_Fatal("%s(%d): FMOD error %d - %s", file, line, result, FMOD_ErrorString(result));
	}
}
