#pragma once
#include "packet.h"

enum {
	X_AXIS = 0,
	Y_AXIS = 1,
	Z_AXIS = 2,
};

class CSensor
{
public:
	CSensor();
	~CSensor();

	short m_sDiag;
	short m_sAlm[3];
	short m_sData[3][512];
	ALARM_t m_Alarms[6];
	ALARMF_t m_fAlarms[6];

private:

	char m_cRegister[0x7d];
	STATUS_t m_Status;


public:
	int Init();
	STATUS_PTR_t GetStatus();
	int IsRun();
	int SetStart();
	int SetStop();
	int SetScale(int nScale);
	int SetBandwidth(int nBand);



	int GetMode();
	int SetMode(int nMode);
	int GenRandom();
	float GetLimit(int nSel);
	float GetScale(int nSel);
	int RecalcAlarmFloat(int nScale, int nRate);
	int RecalcAlarmInt(int nScale, int nRate);
	int ChkAlarm();
	float m_fScale;
	int SetScale(float fScale);
	int SetAlarmCtrl(char cParam);
	char GetAlarmCtrl(void);
};

