#include "stdafx.h"
#include "CSensor.h"
#include <math.h>
#include <time.h>

CSensor::CSensor()
	: m_fScale(0)
{
	Init();
	srand((unsigned int)(time(NULL)));
}


CSensor::~CSensor()
{

}


int CSensor::Init()
{
	// TODO: 여기에 구현 코드 추가.
	int nDatas[6][8] = {
		{   1,  30,  1100,  1500,  2300,  3100,  2200, 3100 },
		{  40,  80,  1400,  1900,  3600,  4200,  2900, 3500 },
		{ 100, 140,  3300,  4200,  3400,  3800,  3500, 4500 },
		{ 150, 170,  3000,  3900,  2700,  3600,  3100, 4000 },
		{ 190, 210,  2300,  2700,  2500,  2900,  2600, 3000 },
		{ 220, 250,  1100,  1500,  1300,  1700,  1500, 1900 },
	};

	m_Status.user_id = 1234;
	m_Status.running = 0;			// 0:stop, 1:start
	m_Status.mode = 1;				// 0:manual, 1:auto, 2:man time cap, 3:real time
	m_Status.record_period = 0;		// 0:1sec
	m_Status.real_axis = 0;
	m_Status.scale = 3;				// 0:1g, 1:5g, 2:10g, 3:20g
	m_Status.bandwidth = 3;			// 0:20k, 1:10k, 2:5k, 3:2.5k, 4:1.2k, 5:0.6kHz
	m_Status.alm_ctrl = 0;

	for (int i = 0; i<6; i++) {
		m_Status.alarms[i].alm_f_l = nDatas[i][0];
		m_Status.alarms[i].alm_f_h = nDatas[i][1];
		m_Status.alarms[i].alm_x_mag1 = nDatas[i][2];
		m_Status.alarms[i].alm_x_mag2 = nDatas[i][3];
		m_Status.alarms[i].alm_y_mag1 = nDatas[i][4];
		m_Status.alarms[i].alm_y_mag2 = nDatas[i][5];
		m_Status.alarms[i].alm_z_mag1 = nDatas[i][6]; 
		m_Status.alarms[i].alm_z_mag2 = nDatas[i][7];
	}

	m_fScale = 1.0;

	return 0;
}


STATUS_PTR_t CSensor::GetStatus()
{
	// TODO: 여기에 구현 코드 추가.
	return &m_Status;
}


int CSensor::IsRun()
{
	// TODO: 여기에 구현 코드 추가.
	return m_Status.running;
}


int CSensor::SetStart()
{
	// TODO: 여기에 구현 코드 추가.
	m_Status.running = 1;

	return 0;
}


int CSensor::SetStop()
{
	// TODO: 여기에 구현 코드 추가.
	m_Status.running = 0;
	return 0;
}


int CSensor::SetScale(int nScale)
{
	// TODO: 여기에 구현 코드 추가.
	m_Status.scale = nScale;
	return 0;
}


int CSensor::SetBandwidth(int nBand)
{
	// TODO: 여기에 구현 코드 추가.
	m_Status.bandwidth = nBand;
	return 0;
}



int CSensor::GetMode()
{
	// TODO: 여기에 구현 코드 추가.
	return m_Status.mode;
}


int CSensor::SetMode(int nMode)
{
	// TODO: 여기에 구현 코드 추가.
	m_Status.mode = nMode;

	return 0;
}


int CSensor::GenRandom()
{
#define NUM_DATA		10

	int spectra[NUM_DATA][2] = {
		{ 596, 20 },
		{ 597, 50 },
		{ 598, 70 },
		{ 599, 120 },
		{ 600, 300 },
		{ 601, 250 },
		{ 602, 180 },
		{ 603, 80 },
		{ 604, 50 },
		{ 605, 20 },
		};

	int limit = 20;
	int base = 10;
	float fBand = 20.0f;
	float fScale = 0.0f;

	fScale = GetScale(m_Status.scale);
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			m_sData[i][j] = (short) (((rand() % limit) + base)/fScale);
		}
	}


	fBand = GetLimit(m_Status.bandwidth);

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < NUM_DATA; k++)
		{
			int j = spectra[k][0]/fBand;
			if (j < 256)
			{
				m_sData[i][j] += (m_fScale * spectra[k][1]+rand()%20) / fScale;
			}
		}
	}


	return 0;
}


float CSensor::GetLimit(int nSel)
{
	float fRet = 0.0f;

	switch (nSel) {
	case 0:	// 10240 Hz
		fRet = 40.0f;
		break;
	case 1:	//  5120 Hz
		fRet = 20.0f;
		break;
	case 2:	// 2560 Hz
		fRet = 10.0f;
		break;
	case 3:	// 1280 Hz
		fRet = 5.0f;
		break;
	case 4:	// 640 Hz
		fRet = 2.5f;
		break;
	case 5:	// 320 Hz
		fRet = 1.25f;
		break;
	case 6:	// 160 Hz
		fRet = 0.625f;
		break;
	case 7:	// 80 Hz
		fRet = 0.313f;
		break;
	case 8:	// 40 Hz
		fRet = 0.156f;
		break;
	case 9: //	20 Hz
		fRet = 0.078f;
		break;
	case 10: // 10 Hz
		fRet = 0.039f;
		break;
	}

	TRACE("[%s:%d] fRet=%0.4f, nSel=%d \n", __FUNCTION__,__LINE__, fRet, nSel);

	return fRet;
}


float CSensor::GetScale(int nSel)
{
	float fTemp = 0.0f;
	int nMode = 0;

	if (nMode == MODE_MANUAL_FFT || nMode == MODE_AUTOMATIC_FFT)
	{
		// ---- FFT Mode ----
		switch (nSel) {
		case 0:
			fTemp = 0.0153f;
			break;
		case 1:
			fTemp = 0.0763f;
			break;
		case 2:
			fTemp = 0.1526f;
			break;
		case 3:
			fTemp = 0.3052f;
			break;
		}
	}
	else
	{
		// ---- time Mode ----
		switch (nSel) {
		case 0:
			fTemp = 0.0305f;
			break;
		case 1:
			fTemp = 0.1526f;
			break;
		case 2:
			fTemp = 0.3052f;
			break;
		case 3:
			fTemp = 0.6104f;
			break;
		}
	}

	TRACE("[%s:%d] fScale=%0.4f, nSel=%d\r\n", __FUNCTION__, __LINE__, fTemp, nSel);

	return fTemp;
}


int CSensor::RecalcAlarmFloat(int nScale, int nRate)
{
	float fLimit = GetLimit(nRate);
	float fScale = GetScale(nScale);

	for (int i = 0; i<6; i++) {
		m_fAlarms[i].f_fl = m_Alarms[i].alm_f_l * fLimit;
		m_fAlarms[i].f_fh = m_Alarms[i].alm_f_h * fLimit;
		m_fAlarms[i].f_x_m1 = m_Alarms[i].alm_x_mag1 * fScale;
		m_fAlarms[i].f_x_m2 = m_Alarms[i].alm_x_mag2 * fScale;
		m_fAlarms[i].f_y_m1 = m_Alarms[i].alm_y_mag1 * fScale;
		m_fAlarms[i].f_y_m2 = m_Alarms[i].alm_y_mag2 * fScale;
		m_fAlarms[i].f_z_m1 = m_Alarms[i].alm_z_mag1 * fScale;
		m_fAlarms[i].f_z_m2 = m_Alarms[i].alm_z_mag2 * fScale;
	}
	return 0;
}


int CSensor::RecalcAlarmInt(int nScale, int nRate)
{
	float fLimit = GetLimit(nRate);
	float fScale = GetScale(nScale);

	for (int i = 0; i<6; i++) {
		m_Alarms[i].alm_f_l = m_fAlarms[i].f_fl / fLimit;
		m_Alarms[i].alm_f_h = m_fAlarms[i].f_fh / fLimit;
		m_Alarms[i].alm_x_mag1 = m_fAlarms[i].f_x_m1 / fScale;
		m_Alarms[i].alm_x_mag2 = m_fAlarms[i].f_x_m2 / fScale;
		m_Alarms[i].alm_y_mag1 = m_fAlarms[i].f_y_m1 / fScale;
		m_Alarms[i].alm_y_mag2 = m_fAlarms[i].f_y_m2 / fScale;
		m_Alarms[i].alm_z_mag1 = m_fAlarms[i].f_z_m1 / fScale;
		m_Alarms[i].alm_z_mag2 = m_fAlarms[i].f_z_m2 / fScale;
	}
	return 0;
}


int CSensor::ChkAlarm()
{
	short sMagValX = 0;
	short sMagValY = 0;
	short sMagValZ = 0;
	short sSetX = 0;
	short sSetY = 0;
	short sSetZ = 0;

	for (int idx = 0; idx < 256; idx++)
	{
		sMagValX = m_sData[X_AXIS][idx];
		sMagValY = m_sData[Y_AXIS][idx];
		sMagValZ = m_sData[Z_AXIS][idx];

		for (int win = 0; win < 6; win++)
		{
			if (idx >= m_Alarms[win].alm_f_l && idx < m_Alarms[win].alm_f_h)
			{
				if( m_Status.alm_ctrl & 0x01 )
				{
					if (sMagValX >= m_Alarms[win].alm_x_mag2)
					{
						// alarmX critical at win
						sSetX |= (2<<(win*2+4));
					}
					else if (sMagValX >= m_Alarms[win].alm_x_mag1)
					{
						// alarmX warnning at win
						sSetX |= (1<<(win*2+4));
					}
				}


				if( m_Status.alm_ctrl & 0x02 )
				{
					if (sMagValY >= m_Alarms[win].alm_y_mag2)
					{
						// alarmX critical at win
						sSetY |= (2 << (win * 2 + 4));
					}
					else if (sMagValY >= m_Alarms[win].alm_y_mag1)
					{
						// alarmX warnning at win
						sSetY |= (1 << (win * 2 + 4));
					}
				}

				if( m_Status.alm_ctrl & 0x04 )
				{
					if (sMagValZ >= m_Alarms[win].alm_z_mag2)
					{
						// alarmX critical at win
						sSetZ |= (2 << (win * 2 + 4));
					}
					else if (sMagValZ >= m_Alarms[win].alm_z_mag1)
					{
						// alarmX warnning at win
						sSetZ |= (1 << (win * 2 + 4));
					}
				}
			}
		}
	}

	m_sAlm[X_AXIS] = sSetX;
	m_sAlm[Y_AXIS] = sSetY;
	m_sAlm[Z_AXIS] = sSetZ;

	return 0;
}


int CSensor::SetScale(float fScale)
{
	m_fScale = fScale;

	return 0;
}


int CSensor::SetAlarmCtrl(char cParam)
{
	m_Status.alm_ctrl = cParam;

	return 0;
}


char CSensor::GetAlarmCtrl(void)
{
	return m_Status.alm_ctrl;
}
