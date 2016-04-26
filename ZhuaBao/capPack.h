#pragma once
#include<string.h>
#include<malloc.h>

BOOL DirExist(LPCTSTR lpszDirName)			//�ж��ļ�Ŀ¼�Ƿ����
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
	char* p;				//�ַ����޸�ָ��
	char temp;
	FilePath = (char*)malloc(MAX_PATH*sizeof(char));
	SavePath = (char*)malloc(MAX_PATH*sizeof(char));
	GetModuleFileName(0, FilePath, MAX_PATH-1);
	CapFileName=strrchr(FilePath,'\\');			// ���ַ����з������

	//printf("\n%s\t%d", CapFileName, strlen(CapFileName));
	strncpy(SavePath, FilePath, strlen(FilePath)-strlen(CapFileName)+1);

	SavePath[strlen(FilePath) - strlen(CapFileName) + 1] = '\0';
	CapFileName = "CapData.CAP";
	strcat(SavePath, CapFileName);							//���Ĭ�ϱ���·��

	printf("\n�����뱣��ץ�������ļ�·��(%s):", SavePath);
	fgets(FilePath, MAX_PATH - 1, stdin);					//�����û�ָ������·��
	p = strrchr(FilePath, '\n');							//ȥ���س���������
	*p = '\0';
	if (!strcmp(FilePath, "\0"))						//ֻ����س���ʹ��Ĭ��·��
	{
		printf("\nʹ��Ĭ��·��:%s", SavePath);
		return SavePath;
	}
	if (strrchr(FilePath, '\\') != NULL)
	{
		CapFileName = (strrchr(FilePath, '\\') + 1);			// ���ַ����з�����ң���ȡ�ļ���
		p = strrchr(FilePath, '\\');							//��ȡFilePath�ļ�Ŀ¼
		temp = *p;
		*p = '\0';
		//printf("\n%s\t%d\t%c", CapFileName, strlen(CapFileName), temp);
	}
	while (!DirExist(FilePath))			//����·�������ڣ���ѭ������
	{
		printf("�ļ�·�������ڣ�����������:");
		fgets(FilePath, MAX_PATH - 1, stdin);				//��������
		p = strrchr(FilePath, '\n');
		*p = '\0';
		if (!strcmp(FilePath, "\0"))						//ֻ����س���ʹ��Ĭ��·��
		{
			printf("\nʹ��Ĭ��·��:%s", SavePath);
			return SavePath;
		}
		if (strrchr(FilePath, '\\') != NULL)
		{
			CapFileName=(strrchr(FilePath, '\\')+1);			// ���ַ����з�����ң���ȡ�ļ���
			p = strrchr(FilePath, '\\');							//��ȡFilePath�ļ�Ŀ¼
			temp = *p;
			*p = '\0';
			//printf("\n%s\t%d\t%c", CapFileName, strlen(CapFileName),temp);
		}
	}
	*p = temp;
	SavePath = FilePath;
	//strcat(SavePath, CapFileName);							//����û�ָ��·��
	printf("�û�ָ��·��:%s", SavePath);
	return SavePath;
}