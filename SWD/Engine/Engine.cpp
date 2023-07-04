/*
 *  Engine.cpp
 *  Engraver2Installer
 *
 *  Created by Maxim Chernousov on 10/21/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Engine.h"
#include "UI.h"
//#include "yesnodialog.h"
//#include "CopyProc.h"
//#import <Cocoa/Cocoa.h>
#include "log.h"
//#include "LRFind.h"
//#include <pthread.h>
int YesNoAlert(char *msg);
char FullPath[1024],ScriptsPath[1024];
char SourcePath[1024];


char _Path[PATHSIZE];
//CFStringRef _CFPath;



void ParseConfig()
{
	char ConfigPath[1024];
	strcpy(ConfigPath,ScriptsPath);
	strcat(ConfigPath,"config.txt");
	//DebugAlert(ConfigPath);
	FILE *hFile=NULL;
	hFile = fopen(ConfigPath,"rb");
	if (hFile)
	{
		char str[1024],*sw,*arg,*val;
		while(1)
		{
			if (!ReadStr(hFile,str))
			 break;
			sw = str;
			SeekOnStartStr(sw);
			GetArgVal(sw,arg,val);
			
			if (arg!=NULL)
			{
				///PARSING
				if (!strcmp(arg,CONFIG_VERSION))
				{
					strcpy(_Version,val);
				}//version
				else
				 if (!strcmp(arg,CONFIG_NEEDSPACE))
				 {
					_NeededSpace = atoi(val);
				 }
				 else
				   if (!strcmp(arg,CONFIG_COPYRIGHT))
				   {
					strcpy(_Copyright,val);
				   }
				   else
					if (!strcmp(arg,CONFIG_INTRO))
					{
						strcpy(_Intro,val);
					}
					else
					 if (!strcmp(arg,CONFIG_COPYINTRO))
					 {
						strcpy(_CopyIntro,val);
					 }
					 else
					  if (!strcmp(arg,CONFIG_STOREFOLDER))
					  {
						 strcpy(_StoreFolder,val);
					  }
					  else
						if (!strcmp(arg,CONFIG_CONG))
						{
							strcpy(_Congratulation,val);
						}
			}//arg != NULL
		}//while
		
		fclose(hFile);
	}//hFile
	else
	{
		DebugAlert("Cannot read config file");
	}
	
}//ParseConfig();
void GetArgVal(char *sw, char* &arg, char* &val)
{
  arg = val = NULL;
	if (strlen(sw)<=1)
		return;
	if (*sw==';')
		return;
	//1 find arg
	arg = sw;
	
	val = strchr(arg,'=');
	
	if (val==NULL)
	{
		val = arg = NULL;
		return;
	}
	*val = 0;
	//get val;
	val++;
	SeekOnStartStr(val);
	char *valw = strchr(val,';');
	if (valw!=NULL)
	{
		*valw = 0;
	}
	
	SeekOnEndStr(arg);
	SeekOnEndStr(val);
	
}//GetArgVal(sw,arg,val);
void SeekOnStartStr(char *  &s)
{
 while (*s==' ')
	 s++;
}
void SeekOnEndStr(char *  &s)
{
int i = strlen(s) - 1;
  while ((s[i]==' ')&&(i>=0))
  {
     s[i] = 0;
	 i--;
  }
}
void StrGetBool(char *sw,  BYTE *z)
{
    
    char Dig[64],cur;
    int i = 0;
    
    int len = (int)strlen(sw);
    strcpy(Dig,"");
    
    while ((i<64)&&(i<len))
    {
        cur = *sw;
        if (((cur<'0')||(cur>'9'))&&(cur!='.')&&(cur!=',')&&(cur!='-')&&(cur!='e')&&(cur!='E'))
        {
            Dig[i]=0;
            break;
        }
        Dig[i] = cur;
        i++;
        sw++;
    }//while i<64
    //Digit = (float)atof(Dig);
    *z = (BYTE)atoi(Dig);
}
void StrGetStr(char *sw, char *res)
{
    char cur;
    int i = 0;
    SeekOnStartStr(sw);
    int len = (int)strlen(sw);
    strcpy(res,"");
    
    while (i<len)
    {
        cur = *sw;
        if ((cur==';')||(cur==10)||(cur=='13')||(cur=='\t'))
        {
            res[i]=0;
            RemoveSpacesInEnd(res);
            return;
        }
        res[i]=cur;
        i++;
        sw++;
    };//while
    res[i]=0;
    
    RemoveSpacesInEnd(res);
}
void RemoveSpacesInEnd(char *str)
{
    int len = (int)strlen(str);
    len--;
    while(len>=0)
    {
        if (str[len]==' ')
            str[len]=0;
        else
            return;
        len--;
    }
}

void StrGetInt(char *sw,  int *z)
{
    
    char Dig[64],cur;
    int i = 0;
    SeekOnStartStr(sw);
    int len = (int)strlen(sw);
    strcpy(Dig,"");
    
    while ((i<64)&&(i<len))
    {
        cur = *sw;
        if (((cur<'0')||(cur>'9'))&&(cur!='.')&&(cur!=',')&&(cur!='-')&&(cur!='e')&&(cur!='E'))
        {
            Dig[i]=0;
            break;
        }
        Dig[i] = cur;
        i++;
        sw++;
    }//while i<64
    //Digit = (float)atof(Dig);
    *z = (int)atoi(Dig);
}

void StrGetFloat(char *sw,  double *z)
{
    
    char Dig[64],cur;
    int i = 0;
    SeekOnStartStr(sw);
    strcpy(Dig,"");
    int len = strlen(sw);
    
    while ((i<64)&&(i<len))
    {
        cur = *sw;
        if (((cur<'0')||(cur>'9'))&&(cur!='.')&&(cur!=',')&&(cur!='-')&&(cur!='e')&&(cur!='E'))
        {
            Dig[i]=0;
            break;
        }
        Dig[i] = cur;
        i++;
        sw++;
    }//while i<64
    //Digit = (float)atof(Dig);
    *z = (double)atof(Dig);
}


int ReadStr(FILE *hFile,char str[])
	{
		int i;
		unsigned char c;
		i=0;
		do{
				if (!fread(&c,1,1,hFile)) return 0;
			if (c==9) c=' ';
			if ((c!=13)||(c!=10))
				str[i]=c;
			else
				str[i] = 0;
			i++;
		}while ((c>13));
		str[i]=0;
		while ((i>=0)&&(str[i]==0))i--;
		while ((i>=0)&&(str[i]==' '))
		{
			str[i] = 0;
			i--;
		}
		return 1;
	};//ReadStr
	

//[Max 01.01.10
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}
//Replace only %20
char* URLEncode2(char* sIn)
{
    char* sOut;
	
    int k;
    const int nLen = strlen(sIn) + 1;

    register BYTE* pOutTmp = NULL;
    BYTE* pOutBuf = NULL;
    register BYTE* pInTmp = NULL;
    BYTE* pInBuf =(BYTE*)sIn;
    BYTE b = 0;

    //count not alphanumeric characters

    k = 0;
	
    pInTmp = pInBuf;
    while(*pInTmp)
    {
        if (!isalnum(*pInTmp) && !isalnum(*pInTmp))
		if (*pInTmp==' ')
		//if (*pInTmp!=47)//not '/'
		{
			/*char str[256];
			sprintf(str,"c=%d",*pInTmp);
			DebugAlert(str);*/
            k++;
		}
        pInTmp++;
    }

    //alloc out buffer

    pOutBuf = (BYTE*)new BYTE[nLen  + 2 * k]; //new BYTE [nLen  + 3 * k];


    if(pOutBuf)
    {
        pInTmp	= pInBuf;
	pOutTmp = pOutBuf;
		
	// do encoding

	while (*pInTmp)
        {
		if (*pInTmp!=' ')
			*pOutTmp++ = *pInTmp;
		else
		{
			 *pOutTmp++ = '%';
		    *pOutTmp++ = '2';
		     *pOutTmp++ = '0';
		}
	    /*if((isalnum(*pInTmp))||(*pInTmp==47))//or '/'
                *pOutTmp++ = *pInTmp;
	    else
		if(isspace(*pInTmp))
		    *pOutTmp++ = '+';
		else
		{
		    *pOutTmp++ = '%';
		    *pOutTmp++ = toHex(*pInTmp>>4);
		     *pOutTmp++ = toHex(*pInTmp%16);
		}*/
	    pInTmp++;
	}
	
	//*pOutTmp = '\0';
	*pOutTmp = 0;
	sOut=(char*)pOutBuf;

	//delete [] pOutBuf;

	//sOut.ReleaseBuffer();
    }
    //sIn.ReleaseBuffer();
    return sOut;
}
char* URLEncode1(const char* sIn)
{
//[Max 06.05.2010
if (!sIn)
	return NULL;
if (!*sIn)
	return NULL;
//]	
    char* sOut;
	
    int k;
    const int nLen = strlen(sIn) + 1;

    register BYTE* pOutTmp = NULL;
    BYTE* pOutBuf = NULL;
    register BYTE* pInTmp = NULL;
    BYTE* pInBuf =(BYTE*)sIn;
    BYTE b = 0;

    //count not alphanumeric characters

    k = 0;
	
    pInTmp = pInBuf;
    while(*pInTmp)
    {
        if (!isalnum(*pInTmp) && !isalnum(*pInTmp))
            k++;
        pInTmp++;
    }

    //alloc out buffer

    pOutBuf = (BYTE*)new BYTE[nLen  + 2 * k]; //new BYTE [nLen  + 3 * k];


    if(pOutBuf)
    {
        pInTmp	= pInBuf;
	pOutTmp = pOutBuf;
		
	// do encoding

	while (*pInTmp)
        {
	    if(isalnum(*pInTmp))
                *pOutTmp++ = *pInTmp;
	    else
		if(isspace(*pInTmp))
		    *pOutTmp++ = '+';
		else
		{
		    *pOutTmp++ = '%';
		    *pOutTmp++ = toHex(*pInTmp>>4);
		     *pOutTmp++ = toHex(*pInTmp%16);
		}
	    pInTmp++;
	}
	
	//*pOutTmp = '\0';
	*pOutTmp = 0;
	sOut=(char*)pOutBuf;

	//delete [] pOutBuf;

	//sOut.ReleaseBuffer();
    }
    //sIn.ReleaseBuffer();
    return sOut;
}


//[Max 2016.06.04
void AddSlashIfNeed(char *str)
{
	if (!str)
		return;
	int len = strlen(str);
	if (len<=0) return;
	if ((str[len-1]=='/')||(str[len-1]=='\\'))
		return;
	strcat(str,"/");
	
	
}

