#include "StdAfx.h"
#include "Parser.h"


CParser::CParser(void)
	: m_nState(0)
	, m_nSize(0)
	, m_cCmd(0)
	, m_pBuffer(NULL)
	, m_nInputPos(0)
	, m_nIdxBuf(0)
{
}


CParser::~CParser(void)
{
}


int CParser::Decode(char ch)
{
	int ret = 0;
	
	switch(m_nState) {
	case 0:
		if( ch == 'M' ) m_nState++;
		break;
	case 1:
		if( ch == 'V' ) m_nState++;
		else m_nState = 0;
		m_nSize = 0;
		m_cCmd = 0;
		break;
	//------------------------------
	case 2:
		m_nSize = ch&0xff;
		m_nState++;
		break;
	case 3:
		m_nSize |= (ch<<8)&0xff00;
		m_nSize-=5;
		m_nState++;
		break;
	case 4:
		m_cCmd = ch;
		if( m_nSize > 0 )  {
			m_nInputPos = 0;			
			m_nState++;

			m_nIdxBuf++;
			m_nIdxBuf %=MAX_BUFFER;
			m_pBuffer = m_Buffer[m_nIdxBuf];
		}
		else {
			m_nState = 0;	
			ret = 1;
		}
//		DBG("[%s] length=%d\r\n", __func__, m_size);					
		break;
	//-------------------------------	
	case 5:
		*(m_pBuffer+m_nInputPos) = ch;
		m_nInputPos++;
		if( m_nInputPos >= m_nSize ) {
//			DBG("[%s] data[0]=%x,%x,%x,%x\r\n", __func__, m_buffer[0], m_buffer[1],m_buffer[2],m_buffer[3]);
			m_nState = 0;
			ret = 1;
		}
		break;
	//----------------------------------
	default:
		m_nState = 0;
		break;
		}

	return ret;
}


unsigned char CParser::GetCmd(void)
{
	return m_cCmd;
}


int CParser::GetSize(void)
{
	return m_nSize;
}


char *CParser::GetData(void)
{
	return m_pBuffer;
}
