//log
#include "log.h"
#ifdef _LOG_
#include <errno.h>
#include <stdlib.h>//10.7
#include <string.h>
#include <stdio.h>
#pragma message ("!!!!!!!!!!!!!!_LOGFUN!!!!!!!!!!!")
unsigned char FileExist = 0;
//extern "C"
//{
void _LogfInt(char *str,int arg)
{
	char str1[128];
	sprintf(str1,str,arg);
	_Logf(str1);
}

void _Logf(char *str) 
{
	//HANDLE hLogFile=NULL;
	FILE *file;
	
	if (!FileExist)
	{		
		//hLogFile = CreateFile(LOGFILENAME,GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL,0);
		file = fopen(LOGFILENAME,"wb");
		FileExist = 1;
	}
	else
	{
		file = fopen(LOGFILENAME,"ab");
		//hLogFile = CreateFile(LOGFILENAME,GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL,0);
	}
		
	if (file)
	{
	//SetFilePointer(hLogFile,0,NULL,FILE_END);
	fseek(file,0,SEEK_END);
	//DWORD HowMatch;
	char ss1[1024];
	strcpy(&ss1[0],str);
	strcat(&ss1[0],"\n");
	fwrite(ss1,1,strlen(ss1),file);
	//WriteFile(hLogFile,ss1,strlen(ss1),&HowMatch,NULL);
	//CloseHandle(hLogFile);
	fclose(file);
	}
    else
    {
        //printf(LOGFILENAME);
        printf("\nfile open error=%d\n",errno);
    }
}	
//}//extern "C"

#endif
