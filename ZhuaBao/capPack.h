#pragma once
#include<string.h>
#include<malloc.h>

BOOL DirExist(LPCTSTR lpszDirName)			//判断文件目录是否存在
{
	DWORD attr;
	attr = GetFileAttributes(lpszDirName);
	if ((attr != (DWORD)(-1)) && (attr & FILE_ATTRIBUTE_DIRECTORY))
		return TRUE;
	else
		return FALSE;
}

char* GetSavePath() {
	char* FilePath;
	char* CapFileName;
	char* SavePath;
	char* p;				//字符串修改指针
	char temp;
	FilePath = (char*)malloc(MAX_PATH*sizeof(char));
	SavePath = (char*)malloc(MAX_PATH*sizeof(char));
	GetModuleFileName(0, FilePath, MAX_PATH-1);
	CapFileName=strrchr(FilePath,'\\');			// 在字符串中反向查找

	//printf("\n%s\t%d", CapFileName, strlen(CapFileName));
	strncpy(SavePath, FilePath, strlen(FilePath)-strlen(CapFileName)+1);

	SavePath[strlen(FilePath) - strlen(CapFileName) + 1] = '\0';
	CapFileName = "CapData.CAP";
	strcat(SavePath, CapFileName);							//获得默认保存路径

	printf("\n请输入保存抓包数据文件路径(%s):", SavePath);
	fgets(FilePath, MAX_PATH - 1, stdin);					//输入用户指定保存路径
	p = strrchr(FilePath, '\n');							//去除回车键输入流
	*p = '\0';
	if (!strcmp(FilePath, "\0"))						//只输入回车，使用默认路径
	{
		printf("\n使用默认路径:%s", SavePath);
		return SavePath;
	}
	if (strrchr(FilePath, '\\') != NULL)
	{
		CapFileName = (strrchr(FilePath, '\\') + 1);			// 在字符串中反向查找，获取文件名
		p = strrchr(FilePath, '\\');							//截取FilePath文件目录
		temp = *p;
		*p = '\0';
		//printf("\n%s\t%d\t%c", CapFileName, strlen(CapFileName), temp);
	}
	while (!DirExist(FilePath))			//输入路径不存在，则循环输入
	{
		printf("文件路径不存在！请重新输入:");
		fgets(FilePath, MAX_PATH - 1, stdin);				//重新输入
		p = strrchr(FilePath, '\n');
		*p = '\0';
		if (!strcmp(FilePath, "\0"))						//只输入回车，使用默认路径
		{
			printf("\n使用默认路径:%s", SavePath);
			return SavePath;
		}
		if (strrchr(FilePath, '\\') != NULL)
		{
			CapFileName=(strrchr(FilePath, '\\')+1);			// 在字符串中反向查找，获取文件名
			p = strrchr(FilePath, '\\');							//截取FilePath文件目录
			temp = *p;
			*p = '\0';
			//printf("\n%s\t%d\t%c", CapFileName, strlen(CapFileName),temp);
		}
	}
	*p = temp;
	SavePath = FilePath;
	//strcat(SavePath, CapFileName);							//获得用户指定路径
	printf("用户指定路径:%s", SavePath);
	return SavePath;
}