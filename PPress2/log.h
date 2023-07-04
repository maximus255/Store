//Log file
#ifndef _LOGINCLUDE_
#define _LOGINCLUDE_

//#define _LOG_
#ifdef _LOG_
//#define LOGFILENAME "c:\\PPressTestLog.txt"
#define LOGFILENAME "PPressInstallerLog.txt"
#define DOMAIN_USER getenv("HOME")//"~/Public/"

//#include <windows.h>
//#include <string.h>
/*#define _Log(str) {\
    HANDLE hLogFile=NULL;\
    hLogFile = CreateFile("c:\\IcePattern.log",GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL,0);\
    SetFilePointer(hLogFile,0,NULL,FILE_END);\
    DWORD HowMatch;\
    char ss1[128];\
    strcpy(&ss1[0],str);\
    strcat(&ss1[0],"\n");\
    WriteFile(hLogFile,ss1,strlen(ss1),&HowMatch,NULL);\
    CloseHandle(hLogFile);}
    */
//extern HANDLE hLogFile;
void _Logf(const char *str);
void _LogfInt(const char *str,int arg);
/*
#ifndef _LOGFUN
#define _LOGFUN
#pragma message ("!!!!!!!!!!!!!!_LOGFUN!!!!!!!!!!!")
        BYTE FileExist = 0;
        void _Logf(char *str)
        {
            HANDLE hLogFile=NULL;
            if (!FileExist)
               {
                hLogFile = CreateFile("c:\\IcePattern.log",GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL,0);
            }
            else
                hLogFile = CreateFile("c:\\IcePattern.log",GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,OPEN_ALWAYS ,FILE_ATTRIBUTE_NORMAL,0);
            SetFilePointer(hLogFile,0,NULL,FILE_END);
            DWORD HowMatch;
            char ss1[128];
            strcpy(&ss1[0],str);
            strcat(&ss1[0],"\n");
            WriteFile(hLogFile,ss1,strlen(ss1),&HowMatch,NULL);
            CloseHandle(hLogFile);
        }
#else//_LOGFUN

#endif//_LOGFUN*/
#define _Log(str) _Logf(str);
#define _LogInt(str,arg) _LogfInt(str,arg);
#else
    #define _Log(str)
    #define _LogInt(sre,arg)
#endif

#endif //_LOGINCLUDE_
