/*
 *  CustomInfoAboutSets.cpp
 *  MachineWashDeluxInstaller
 *
 *  Created by Maxim Chernousov on 10/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "CustomInfoAboutSets.h"
#include "log.h"
//#include "yesnodialog.h"
int YesNoAlert(char *msg);
typedef unsigned char				    BYTE;
#define MALLOC(size) NewPtr(size)
#define MFREE(p) if (p){ DisposePtr((char*)p); p=NULL; }
void DebugAlert(char *str);
void StrGetInt(char *sw,  int *z);
/*void StrGetInt(char *sw,  int *z)
{

	char Dig[64],cur;
	int i = 0;
	int len; 

	//char str[256];

	//SeekOnStartStr(sw);
	while (*sw==' ')
	 sw++;
	len = strlen(sw);
	//MessageBox(0,sw,"sw",0);
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
	//MessageBox(0,Dig,"",0);
	*z = (int)atoi(Dig);
}
*/
int IsUpdatingDBHere(char *aPath)
{
	char FullPath[2048];
	char Msg[2048];
	FILE *hFile;	
	//int len;
	_Log("IsUpdatingDBHere begin")
	
	if (aPath==NULL)
	{
		_Log("aPath=NULL!!!")
		_Log("IsUpdatingDBHere finish")
		return 0;
	}
	_Log(aPath)
	//MessageBox(0,"IsUpdatingDBHere Start","",0);
	strcpy(FullPath,aPath);
	/*len = strlen(FullPath);
	if ((FullPath[len-1]!='/')&&(FullPath[len-1]!='\\'))
	{
		strcat(FullPath,"\\");
	}*/
	strcat(FullPath,"Updatings.db");


	//MessageBox(0,FullPath,"FullPath",0);
	hFile = fopen(FullPath,"rb");
	if (hFile)
	{
		fclose(hFile);
		//MessageBox(0,"before return 1","",0);
		_Log("before return 1")
		_Log("IsUpdatingDBHere finish")
		return 1;
	}
	else
	{		
		sprintf(Msg,"The Path %s doesnt contain Updatings.db file.\n Do you want to install textures here anyway?",aPath);
		//if(MessageBox(0,Msg,"Machine Wash Deluxe Installer",MB_YESNO|MB_ICONWARNING)==IDYES)
		if (YesNoAlert(Msg))
		{
			_Log("Choose Yes")
			_Log("IsUpdatingDBHere finish")
			return 1;
		}
	}

	_Log("IsUpdatingDBHere finish 0")
	return 0;
}
#include <sys/stat.h>
int CreateDBIfNeed(char *FullPath,char *LastVers)
{	
	FILE *hFile	= fopen(FullPath,"rb");
	char *StrBuf;
	int FileSize;
	char *str;
	time_t cur_time;
	BYTE NeedToCreate = 0;	

	_Log("CreateDBIfNeed begin")
	_Log(FullPath)
	//MessageBox(0,FullPath,"",0);
	if (hFile)
	{
		_Log("hFile is found")
		fseek(hFile,0,SEEK_END);
		FileSize = ftell(hFile);
		fseek(hFile,0,SEEK_SET);
		StrBuf = (char*)malloc(FileSize+2);
		if (StrBuf)
		{			
			fread(StrBuf,1,FileSize,hFile);
			str = strstr(StrBuf,"lastvers:");
			if (str==NULL)
			{
				_Log("lastvers not found")
				NeedToCreate = 1;
			}			
		}
		else
		{
			_Log("StrBuf=NULL")
			NeedToCreate = 1;
		}
		fclose(hFile);
	}
	else
	{
		_Log("not find hFile")
		NeedToCreate = 1;
	}

	if (NeedToCreate)
	{
		_Log("NeedToCreate=1")
		
		/*mode_t old_mode = umask(0);
#ifdef _LOG_
		{
			char str[256];
			sprintf(str,"old umask mode is %d = 0x%x",old_mode, old_mode);
			_Log(str)
			
		}
#endif
		
		mode_t new_mode = 0;//((511));//^old_mode);//S_IRWXU; //0x0777;//S_IRWXU | S_IRWXG | S_IRWXO;
#ifdef _LOG_
		{
			char str[256];
			sprintf(str,"new umask mode is %d = 0x%x",new_mode, new_mode);
			_Log(str)
			
		}
#endif
		umask(new_mode);*/
		
		
		hFile	= fopen(FullPath,"wb");
		
		//umask(old_mode);
		
		if (hFile)
		{
			_Log("hFile created")
			time( &cur_time);
			fprintf(hFile,"datestr:%u;",cur_time-60*60*24);

			fprintf(hFile,"lastvers:%s;",LastVers);

			///Add info
			fprintf(hFile,";;;;;;;;");

			fclose(hFile);
		}
#ifdef _LOG_
		else {
			_Log("cannot create file")
			char str[256];
			sprintf(str,"errno=%d %s",errno,strerror(errno));
			_Log(str)
		}

#endif
		_Log("will return 1")
		return 1;
	}//NeedToCreate
	_Log("CreateDBIfNeed Finished, wull return 0")
	return 0;
}
void RefreshUpdatingDate(char *UpdatingDBFile)
{
	FILE *hFile;
	char *StrBuf = NULL;
	int FileSize;
	char *str;
	time_t cur_time;

	_Log("RefreshUpdatingDate begin v.1")
	_Log(UpdatingDBFile)
	//MessageBox(0,UpdatingDBFile,"start RefreshUpdatingDate",0);
	hFile = fopen(UpdatingDBFile,"rb");

	
	if (hFile)
	{
		_Log("hFile !=NULL")
	fseek(hFile,0,SEEK_END);
	FileSize = ftell(hFile);
	fseek(hFile,0,SEEK_SET);
		//MessageBox(0,"before alloc","",0);
	    _Log("before malloc")
		//[Max 30.11.09
		StrBuf = (char*)malloc(FileSize+2);
		if (StrBuf)
		{
			_Log("before fread")
			fread(StrBuf,1,FileSize,hFile);
			StrBuf[FileSize] = 0;

			_Log("before fclose")
			fclose(hFile);
			//MessageBox(0,"before fopen2","",0);
			_Log("before fopen wb")
			hFile = fopen(UpdatingDBFile,"wb");

			if (hFile)
			{
				_Log("hFile!=NULL")
				//MessageBox(0,"before time","",0);

				time( &cur_time);
				//MessageBox(0,"before fprintf 1","",0);
				fprintf(hFile,"datestr:%u;",cur_time-60*60*24);

				///save all result text after "lastvers:" tag
				//MessageBox(0,StrBuf,"StrBuf",0);
				_Log("before find lastvers:")

				str = strstr(StrBuf,"lastvers:");
				if (str!=NULL)
				{
					_Log("str for lastvers !=NULL")
					fwrite(str,1,strlen(str),hFile);
				}
				else
				{
					_Log("str for lastvers = NULL")
					fprintf(hFile,"lastvers:%d;",0);
					fwrite(StrBuf,1,strlen(StrBuf),hFile);
				}
			}

			_Log("before free StrBuf")
			free(StrBuf);
		}
		/*else
		{
			MessageBox(0,"Cannot to alloc","",0);
		}*/
		if (hFile)
		{
			_Log("before fclose")
			fclose(hFile);
		}
	}//hFile
#ifdef _LOG_
	else
	{
		_Log("cannot open file")
	}
#endif
	/*else
	{
		MessageBox(0,"Cannot to open file","",0);
	}*/
	_Log("RefreshUpdatingDate End")
}

void AddSetsInstallInfo( char *FullPath, char* aSetUID, char* aSetName, int aDownloaded)
{
	
	BYTE NeedToSave = 1,NeedToAppend=0;
	char *StrBuf;
	int FileSize;
	char *str;
	unsigned int wI,wUID;
	//[Max 26.11.09
	char *strwrk;
	char NewStr[16],*newstrwrk = NewStr;
	
	char StrDbg[256];
	//]
	//MessageBox(0,FullPath,"test",0);
	_Log("AddSetsInstallInfo begin")
	_Log(FullPath)
	FILE *hFile	= fopen(FullPath,"rb");
	if (hFile)
	{
		_Log("hFile!=NULL")
		NeedToAppend = 1;
		fseek(hFile,0,SEEK_END);
		FileSize = ftell(hFile);
		fseek(hFile,0,SEEK_SET);
		StrBuf = (char*)malloc(FileSize+2);
		if (StrBuf)
		{			
				_Log("malloc success")
			fread(StrBuf,1,FileSize,hFile);
				_Log("fread success")
			str = StrBuf;//!!!!
			wUID = atoi(aSetUID);
			/*{
				sprintf(StrDbg,"aSetUID=%s wUID=%d (atoi)",aSetUID,wUID);
				MessageBox(0,StrDbg,"",0);
			}*/
			/*
			//[Max 2.12.09
			//StrGetInt(aSetUID,&wUID);
			{
				sprintf(StrDbg,"aSetUID=%s wUID=%d (StrGetInt)",aSetUID,wUID);
				MessageBox(0,StrDbg,"",0);
			}*/
			_Log("before parsing installedset:")
			while(str=strstr(str,"installedset:"))
			{
				str = strstr(str,":");
				if (!str) break;
				str++;
				StrGetInt(str,(int*)&wI);
				//MessageBox(0,str,"",0);
				/*{
					sprintf(debugstr,"wI=%d wUID=%d ",wI,wUID);
					MessageBox(0,debugstr,"test",0);
				}*/
				if ((wI==wUID)&&(wI!=0))
				{
					if (!str) break;
					str++;
					//[Max 09.11.09
					//Check downloaded flag
					{
						str = strstr(str,"installeddown");
						if (str)
						{
							str = strstr(str,":");
							if (!str) break;
							str++;
							//MessageBox(0,str,"before StrGetInt",0);
							StrGetInt(str,(int*)&wI);
							if (wI==aDownloaded)
							{
								//MessageBox(0,str,"wI==aDownloaded",0);
								NeedToSave = 0;
								break;
							}
							else
							{
								_Log("wI!=aDownloaded, will Insert")
								//MessageBox(0,"wI!=aDownloaded","",0);
								//[26.11.09
								//Will save improved StrBuf
								//str should hit on old digit								
								strwrk = str;
								//MessageBox(0,str,"before fill ' '",0);
								while ((strwrk)&&(*strwrk!=';'))
								{
									*strwrk = ' ';
									strwrk++;
								}
								str++;
								//MessageBox(0,str,"after fill ' '",0);
								//MessageBox(0,"After fill ' '","",0);
								//we suppose that aDownloaded 0,1,2
								newstrwrk = NewStr;
								sprintf(NewStr,"%d",aDownloaded);

								
								while((newstrwrk!=0)&&(str<strwrk))
								{
									//
									
									*str = *newstrwrk;
									//MessageBox(0,str,newstrwrk,0);

									str++;
									newstrwrk++;
								}
								//Now save updated StrBuf
								//MessageBox(0,"before fclose","",0);
								fclose(hFile);
								hFile = fopen(FullPath,"wb");
								if (hFile)
								{
									_Log("hFile is open for writting")
									//MessageBox(0,"before fwrite","",0);
									fwrite(StrBuf,1,FileSize,hFile);
								}
								//MessageBox(0,"before NeedToSave","",0);
								NeedToSave = 0;
							}
						}
						
					}					
					//NeedToSave = 0;
					//break;
					//]
					//because opimize gluk
					//goto endwhile;
				}
				/*else
				{
					{
						//sprintf(debugstr,"wI=%d wUID=%d ",wI,wUID);
						//MessageBox(0,debugstr,"test",0);
						sprintf(StrDbg,"wI=%d wUID=%d",wI,wUID);
						MessageBox(0,StrDbg,"",0);
					}
				}*/
			}//while

//endwhile:		
			free(StrBuf);
		}
		else
		{
			//MessageBox(0,"Cannot to allocate Buffer","test",0);
			NeedToSave = 1;
			
		}
		fclose(hFile);
	}
	else
	{
		_Log("cannot to open file")
		//MessageBox(0,"Cannot to Create Updatings.db File","Mashine Wash Delux Installer",MB_OK|MB_ICONSTOP);
		NeedToSave = 1;
		NeedToAppend = 0;
	}

	//MessageBox(0,"before Need to Save","",0);
	if (NeedToSave==1)
	{
		_Log("Need To Save = 1")
		//MessageBox(0,"ADD","",0);
		if (NeedToAppend)
		{
			_Log("Need to Append")
			hFile = fopen(FullPath,"ab");
		}
		else
		{
			_Log("need to rewrite")
			hFile = fopen(FullPath,"wb");
		}

		if (hFile)
		{
			_Log("hFile !=NULL")
			//[Max 26.11.09
			if (NeedToAppend)
			{
				fprintf(hFile,"\n");
			}
			fprintf(hFile,"installedset: %s;installedname: %s;installedanswer: 0;installeddown: %d;",aSetUID,aSetName,aDownloaded);
			fclose(hFile);
		}
#ifdef _LOG_
		else {
			_Log("!hFile !!!!")
		}

#endif
	}
	//MessageBox(0,"before RefreshUpdatingDate","",0);
	_Log("before RefreshUpdatingDate")
		RefreshUpdatingDate(FullPath);
	_Log("after RefreshUpdatingDate")
	_Log("AddSetsInstallInfo Finish<<<")
}
