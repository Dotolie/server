#ifndef __PACKET_H__
#define __PACKET_H__

#include <stdint.h>


#define MAKE_NETHDR( data, size, cmd ) 	\
	do {								\
		data[0] = 'M'; 					\
		data[1] = 'V'; 					\
		data[2] = size & 0xff;			\
		data[3] = (size >> 8) & 0xff;	\
		data[4] = cmd;					\
	} while(0)

enum {
	PKT_ACK				= 0x01,
	PKT_NAK				= 0x02,
	PKT_GREETING		= 0x03,
	PKT_RESET			= 0x04,
	PKT_VERSION			= 0x05,

	PKT_REG_ALL			= 0x10,
	PKT_REGISTER		= 0x11,
	PKT_GLOB_CMD		= 0x12,
	PKT_START_STOP		= 0x13,
	PKT_SET_MODE		= 0x14,

	PKT_DATA_MANFFT		= 0x20,
	PKT_DATA_AUTOFFT	= 0x21,
	PKT_DATA_MANCAP		= 0x22,
	PKT_DATA_REALTIME	= 0x23,

	PKT_SET_CONFIG_ALARM= 0x30,
	PKT_GET_CONFIG_ALARM= 0x31,
	PKT_CONTROL_ALARM	= 0x32,
	PKT_STATUS_ALARM	= 0x33,
	PKT_CLEAR_ALARM		= 0x34,


	PKT_END				= 0xff,
};


enum {
	MODE_MANUAL_FFT 		= 0,
	MODE_AUTOMATIC_FFT,
	MODE_MANUAL_CAPTURE,
	MODE_REAL_TIME,
};

enum {
	REAL_AXIS_X			= 0,
	REAL_AXIS_Y,
	REAL_AXIS_Z,
};

#pragma pack(push, 1)


typedef struct __REGISTER_t 
{
	uint8_t		type;
	uint16_t	address;
	uint16_t	value;
} REGISTER_t, *REGISTER_PTR_t;

typedef struct __ALARM_t
{
	uint8_t		alm_f_l;
	uint8_t		alm_f_h;
	uint16_t	alm_x_mag1;
	uint16_t	alm_x_mag2;	
	uint16_t	alm_y_mag1;
	uint16_t	alm_y_mag2;	
	uint16_t	alm_z_mag1;
	uint16_t	alm_z_mag2;
} ALARM_t, *ALARM_PTR_t;

typedef struct __ALARMF_t
{
	float			f_fl;
	float			f_fh;
	float			f_x_m1;
	float			f_x_m2;
	float			f_y_m1;
	float			f_y_m2;
	float			f_z_m1;
	float			f_z_m2;
} ALARMF_t, *ALARMF_PTR_t;

typedef struct __STATUS_t
{
	uint16_t	user_id;

	uint8_t		mode;
	uint8_t		record_period;
	uint8_t		real_axis;
	uint8_t		running;
	uint8_t		scale;
	uint8_t		bandwidth;
	
	uint8_t		alm_ctrl;
	ALARM_t		alarms[6];
} STATUS_t, *STATUS_PTR_t;

typedef struct __ALMSTAT_t
{
	uint16_t		diag_stat;
	uint16_t		alm_x_stat;
	uint16_t		alm_y_stat;
	uint16_t		alm_z_stat;
} ALMSTAT_t, *ALMSTAT_PTR_t;

#pragma pack(pop)


#endif	// __PACKET_H__