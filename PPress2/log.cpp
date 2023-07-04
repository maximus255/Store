//log
#include "log.h"
#ifdef _LOG_
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "errno.h"
void DebugAlert(char *str);
#pragma message ("!!!!!!!!!!!!!!_LOGFUN!!!!!!!!!!!")
void DebugAlert(char *str);
bool alert_was_shown = false;
unsigned char FileExist = 0;
char _fullName[1024];
bool _fullNameInitialized = false;
char *FULLPATH()
{
    if (!_fullNameInitialized)
    {
        _fullNameInitialized = true;
        strcpy(_fullName,DOMAIN_USER);
        strcat(_fullName,"/Public/");
        strcat(_fullName,LOGFILENAME);
    }
    
    //DebugAlert(_fullName);
    
    return _fullName;
}
void _LogfInt(const char *str,int arg)
{
    char str1[1024];
    sprintf(str1,str,arg);
    _Logf(str1);
}
void _Logf(const char *str)
{
    //HANDLE hLogFile=NULL;
    FILE *file;
    try
    {
        if (!FileExist)
        {
            //hLogFile = CreateFile(LOGFILENAME,GENERIC_WRITE,FILE_SHARE_WRITE ,NULL,CREATE_ALWAYS ,FILE_ATTRIBUTE_NORMAL,0);
            file = fopen(FULLPATH(),"wb");
            FileExist = 1;
        }
        else
        {
            file = fopen(FULLPATH(),"ab");
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
        else {
            if (!alert_was_shown)
            {
                alert_was_shown = true;
                char ss[1024];
                sprintf(ss, "Cannot save txt log file!\n%s\nerrno=%d,\n%s",FULLPATH(),errno,strerror(errno));
                DebugAlert(ss);
                //DebugAlert("Cannot save txt log file! " DOMAIN LOGFILENAME);
            }
        }
    }
    catch (...)
    {
        if (!alert_was_shown)
        {
            alert_was_shown = true;
            char ss[1024];
            //sprintf(ss, "Cannot save txt log file!\n" DOMAIN LOGFILENAME "\nerrno=%d,\n%s",errno,strerror(errno));
            sprintf(ss, "Cannot save txt log file!\n%s\nerrno=%d,\n%s",FULLPATH(),errno,strerror(errno));
            DebugAlert(ss);
            //DebugAlert("Cannot save txt log file! " DOMAIN LOGFILENAME);
        }
    }
    
}

#endif
