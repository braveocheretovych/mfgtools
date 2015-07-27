#include <stdio.h>
#include <string.h>
#include <cstring>
#include "MfgToolLib_Export.h"

int main (int argc,char* argv[]){
	INSTANCE_HANDLE lib;

	char * newpath;
	int hasnewpath = 0;


	std::map<CString, CString> m_uclKeywords;
	std::map<CString, CString>::const_iterator it;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--setting") == 0)
		{
			CString* argString = new CString(argv[i + 1]);
			int loc = argString->find('=', 0);
			if (loc == std::string::npos)
			{
				m_uclKeywords[*argString] = argv[i+2];
				i+=2;
			}
			else
			{
				m_uclKeywords[argString->substr(0, loc)] = argString->substr(loc + 1, argString->size() - loc);
				i++;
			}
			delete argString;
		}
		else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--profile") == 0)
		{
			hasnewpath = 1;
			newpath = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			printf("Usage: [program] [arguments] [settings]=[values]\n");
			printf("  -s  --setting		Specify any UCL keywords.\n");
			printf("	-p	--profile		Specify path to Profiles directory.\n");
			printf("  -h  --help      Display this help information.\n");
			exit(EXIT_SUCCESS);
		}
	}

	OPERATIONS_INFORMATION infoOp;
	int ret=MfgLib_Initialize();
	if(ret!=MFGLIB_ERROR_SUCCESS){
		printf("Failed to initialize MfgLib\n");
		return -1;
	}
	ret=MfgLib_CreateInstanceHandle(&lib);
	if (ret!=MFGLIB_ERROR_SUCCESS){
		printf("CreateInstanceHandle failed\n");
		return -1;
	}



	for ( it=m_uclKeywords.begin(); it!=m_uclKeywords.end(); ++it )
	{
		CString key = it->first;
		CString value = it->second;
		MfgLib_SetUCLKeyWord((char*)key.GetBuffer(),(char*) value.GetBuffer());
	}


	if (hasnewpath == 1)
		ret = MfgLib_SetProfilePath(lib, (BYTE_t *) newpath);

	//set profile and list
	ret = MfgLib_SetProfileName(lib,(BYTE_t *) _T("Linux"));
	if(ret != MFGLIB_ERROR_SUCCESS)
	{
		printf(_T("Set Profile name failed\n"));
		return -1;
	}
	ret = MfgLib_SetListName(lib, (BYTE_t *) _T("SDCard"));
	if(ret != MFGLIB_ERROR_SUCCESS)
	{
		printf(_T("Set List name failed\n"));
		return -1;
	}
	ret = MfgLib_SetMaxBoardNumber(lib,4);// hard coded for test
	if(ret != MFGLIB_ERROR_SUCCESS)
	{
		printf(_T("The specified board number[%d] is invalid, should be 1~4\n"), 4);
		return -1;
	}


	OPERATION_INFOMATION *pOpInformation = NULL;
	pOpInformation = new OPERATION_INFOMATION[1];// has index of how many concurrent boards
	if(NULL == pOpInformation)
	{
		printf(_T("Lack of Memory!!!."));
		return -1;
	}
	infoOp.pOperationInfo = pOpInformation;


	ret=MfgLib_InitializeOperation(lib);
	if(ret!=MFGLIB_ERROR_SUCCESS){
		printf("init op Failed code# %d \n",ret);
		return -1;
	}
	ret= MfgLib_GetOperationInformation(lib,&infoOp);
	if(ret!=MFGLIB_ERROR_SUCCESS){
		printf("Get op info  Failed code# %d \n",ret);
		return -1;
	}     

	ret=MfgLib_StartOperation(lib,infoOp.pOperationInfo[0].OperationID);
	if(ret!=MFGLIB_ERROR_SUCCESS){
		printf("start op Failed code# %d \n",ret);
		return -1;
	}




	while(1)
	{
		sleep(3);
	}
	ret=MfgLib_UninitializeOperation(lib);
	if(ret!=MFGLIB_ERROR_SUCCESS){
		printf("init op Failed %d\n",ret);
		return -1;
	} 



	ret=MfgLib_DestoryInstanceHandle(lib);
	if( ret!=MFGLIB_ERROR_SUCCESS){
		printf("DestroyInstanceHandle failed  %d \n",ret);
		return -1;
	}
	ret=MfgLib_Uninitialize();
	if(ret!=MFGLIB_ERROR_SUCCESS){
		printf("Uninitialize failed  %d  \n",ret);
		return -1;
	}
	return 0;

}
