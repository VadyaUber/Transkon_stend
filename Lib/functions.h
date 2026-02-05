#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "main.h"
#include "stdbool.h"

#pragma anon_unions

#include "led_.h"
#include "relay_.h"
#include "rs485_.h"
#include "can_.h"
#include "temp_.h"
#include "ai_.h"
#include "io_.h"
#include "pwm_l.h"
#include "button_.h"
#include "individual_diagnostic.h"

#define NOMBER_Sten_1 
//#define NOMBER_Sten_2 
//#define NOMBER_Sten_3  //Нижний новгород 

/*
typedef union __attribute__((__packed__, aligned(1)))
{
    struct __attribute__((__packed__, aligned(1))) {
				uint16_t flag_test_start:1;
				uint16_t flag_test_end:1;
				uint16_t kz:1;
				uint16_t can:1;
				uint16_t Transkon_485:1; //связь с трансконом по 485
			  uint16_t reserv1:1;
			  uint16_t reserv2:1;
				uint16_t reserv3:1;
				uint16_t reset_test0:1; //команда на сброс проверки и транскона получина
				uint16_t reset_test1:1; //команда на сброс проверки и транскона обработана 
				//bool reserv4:1;
				//uint16_t reserv5:1;
				uint16_t reserv6:1;
				uint16_t reserv7:1;
				uint16_t reserv8:1;
				uint16_t reset_status:1; //сбросить статус проверки good error
			  uint16_t error:1;
			  uint16_t good:1;
    };
    uint16_t val;
 } str_0;*/
 
 typedef union 
{
    struct {
				uint8_t flag_test:2;
				bool kz:1;
				bool can:1;
				bool Transkon_485:1; //связь с трансконом по 485
			  bool reserv1:1;
			  bool reserv2:1;
				bool reserv3:1;
				uint8_t reset_test:2; //команда на сброс проверки и транскона получина
				//bool reserv4:1;
				//bool reserv5:1;
				bool reserv6:1;
				bool reserv7:1;
				bool reserv8:1;
				bool reset_status:1; //сбросить статус проверки good error
			  bool error:1;
			  bool good:1;
    };
    uint16_t val;
 } str_0;
	
extern str_0 str0;

	

typedef union{
    struct {
        bool CH1_768:1;
				bool CH2_768:1;
				bool CH3_768:1;	
				bool CH4_768:1; 
        bool CH1_1000:1;
				bool CH2_1000:1;
				bool CH3_1000:1;
				bool CH4_1000:1;
        bool CH1_1100:1;
				bool CH2_1100:1;
				bool CH3_1100:1;
				bool CH4_1100:1;
				bool CH1_1330:1;
				bool CH2_1330:1;
				bool CH3_1330:1;
				bool CH4_1330:1;
    };
    uint16_t val;
}str_1;
extern str_1 str1;

typedef union{
    struct {
        bool CH1_2100:1; 
				bool CH2_2100:1;
				bool CH3_2100:1;
				bool CH4_2100:1;
    };
    uint8_t val;
}str_2;
extern str_2 str2;

typedef union{
    struct {
        bool AI14_4v:1; 
				bool AI14_12v:1;
				bool AI14_20v:1;
				bool AI15_4v:1;
				bool AI15_12v:1;
				bool AI15_20v:1;
				bool KL15_4v:1;
				bool KL15_12v:1; 
				bool KL15_20v:1;
    };
    uint16_t val;
}str_3;
extern str_3 str3;

typedef union{
    struct {
				
        bool IO4_24:1; 
        bool IO5_24:1;
        bool IO6_24:1;
				bool IO7_24:1;
			  bool IO4_6:1; 
				bool IO5_6:1;
				bool IO6_6:1;
				bool IO7_6:1; 
				bool IO4_4_5:1;
				bool IO5_4_5:1;
				bool IO6_4_5:1;
				bool IO7_4_5:1;
				bool HL_IO3:1;
				bool HL_IO2:1;
				bool HL_IO1:1;
				bool HL_IO0:1;
    };
    uint16_t val;
}str_4;
extern str_4 str4;

typedef union{
    struct {
        bool IO0_24:1; 
        bool IO1_24:1;
        bool IO2_24:1;
				bool IO3_24:1;
			  bool IO0_6:1; 
				bool IO1_6:1;
				bool IO2_6:1;
				bool IO3_6:1; 
				bool IO0_4_5:1;
				bool IO1_4_5:1;
				bool IO2_4_5:1;
				bool IO3_4_5:1;
				bool HL_IO4:1;
				bool HL_IO5:1;
				bool HL_IO6:1;
				bool HL_IO7:1;
    };
    uint16_t val;
}str_5;
extern str_5 str5;

typedef union{
    struct {
        bool pwm_0:1; 
        bool pwm_1:1;
        bool pwm_2:1;
				bool pwm_3:1;
			  bool pwm_4:1; 
				bool pwm_5:1;
				bool pwm_6:1;
				bool pwm_7:1; 
    };
    uint8_t val;
}str_6;
extern str_6 str6;

typedef union{
    struct {
			uint8_t counter_total:8;
			uint8_t counter_now:8;
		};
	uint16_t val;
}str_7;		
extern str_7 str7;

typedef union{
    struct { 
			uint8_t counter_test:8;
			bool flag_diagnostic_ready:1; 
      bool reserv_1:1;
      bool reserv_2:1;
			bool reserv_3:1;
			bool reserv_4:1; 
			bool flag_isError:1;
			bool flag_isTesting:1;
			bool flag_diagnostic_start_stop:1;
		};
	uint16_t val;
}str_8;		
extern str_8 str8;

typedef union{
    struct { 
			uint16_t mask;
		};
	uint16_t val;
}str_9;		
extern str_9 str9;

extern uint8_t counter;

void status_KZ(void);
void test(void);

void reset(void);

void connect_to_pc_485(void);
void PC_Connect(void);

void my_connect_to_pc_485(void);
void check_start_req(void);
void check_start(void);
void obnull(void);
void update_status_test(void);
//void test_can_permach(void);
#endif /*FUNCTIONS_H*/
