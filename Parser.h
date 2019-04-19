#pragma once

#define BUFFER_SIZE		4096
#define MAX_BUFFER		12


class CParser
{
public:
	CParser(void);
	~CParser(void);
	int Decode(char cData);
private:
	int m_nInputPos;
	int m_nIdxBuf;
	int m_nState;
	int m_nSize;
	char m_cCmd;
public:
	char m_Buffer[MAX_BUFFER][BUFFER_SIZE];
	char *m_pBuffer;


	unsigned char GetCmd(void);
	int GetSize(void);
	char * GetData(void);
};

