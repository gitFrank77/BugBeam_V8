#ifndef __UTILS_V1_H
#define __UTILS_V1_H

#include "utils_config.h"

 ModbusIP mb;

#define VERSION   0x01
#define INITIATED 0x10
#define DATA_READ 0x20
#define DATA_SEND 0x30
#define FAULT     0x40

#define SENSOR_AVG_READS  5

const int HoldingReg1 = 100; // tube number
const int HoldingReg2 = 101; // reading analog-data;
const int HoldingReg3 = 102; // version number and status bits

//-[Prototypes]
//static inline void get_od( int analogValue, float (*look_up_table)[2], int look_up_table_ndx, data_agg_t * const me );


//-[Look up table for sensor calibration values ( 8 sensors total)
static float calibrations_lookup_table[][2] = 
{

  //y-intercept , slope
  { 76.4, -10.1}, //tube 0
  { 78.6, -9.92}, //tube 1
  { 69.8, -9.04}, //tube 2
  { 69.2, -9.64}, //tube 3
  { 65.1, -8.79}, //tube 4 
  { 69.1, -8.71}, //tube 5
  { 74.7, -9.97}, //tube 6
  { 64.9, -8.24}  //tube 7
  
};

//-[Structs]
typedef struct {
  int HR_tube_id;
  int HR_tube_data;
  byte HR_system_status;
 void (*reading_calc )( int, float, int );
}data_agg_t; //aggregate data 

//-[Prototypes]
static inline float get_od( int analogValue, float (*look_up_table)[2], int look_up_table_ndx, data_agg_t * const me, float init_calibration );

//-[constructor]
static inline void data_agg_ctor(data_agg_t  *const me)
{
  me-> HR_tube_id       = 0;
  me-> HR_tube_data     = 0;
  me-> HR_system_status |= INITIATED;
  me-> reading_calc = &get_od;
}


//-[Getters]

static inline float get_analog_value( int analogValue)
{
  float temp;
  for( int i = 0 ; i <= SENSOR_AVG_READS ; i++)
  {
    temp += analogValue;
    delay(10);
  }
 //Serial.println(temp / SENSOR_AVG_READS); //works return float ie 35.80
  return (temp / SENSOR_AVG_READS);
}

/*
static inline float get_analog_initial_calibration(int sensor_select, float (*look_up_table)[2], int look_up_table_ndx)
{
  
  float temp = get_analog_value(sensor_select); //retrieve mean value of reads
  // Serial.println(temp);
  float y_intercept =  *(look_up_table[look_up_table_ndx]);
  
  float slope       =  *(look_up_table[look_up_table_ndx] + 1);
/*  Serial.print(look_up_table_ndx); 
 // Serial.print("calibration offset ");
  //Serial.print(init_calibration);
  Serial.print("\t");
  Serial.print(" y-intercept ");
  Serial.print(y_intercept);
  Serial.print("\t");
  Serial.print("Slope ");
  Serial.print(slope);
  Serial.print("\t");
 
  float od = ( temp - y_intercept) / slope;
     return  od = od *100; // move float decimal value
 Serial.print("OD \t");
 Serial.println(od); // return value like -0.34 to this works 
}
*/
static inline float get_od(int sensor_select, float (*look_up_table)[2], int look_up_table_ndx, data_agg_t * const me, float init_calibration) //-[Function test: PASS ( consider removing serial outputs)
{

  me->HR_tube_id         =  look_up_table_ndx + 1; // tube # ( 1-8)
  me->HR_system_status  &= ~me->HR_system_status;
  me->HR_system_status  |= (VERSION | DATA_READ);

  mb.Hreg(HoldingReg3, me-> HR_system_status); //send status to holding register
  
  float temp = get_analog_value(sensor_select); //retrieve mean value of reads
  //float temp = temp2 - init_calibration; //retrieve mean value of reads
  float y_intercept =  *(look_up_table[look_up_table_ndx]);
  
  float slope       =  *(look_up_table[look_up_table_ndx] + 1);
 /* Serial.print(look_up_table_ndx); 
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("\tcalibration offset ");
  Serial.print(init_calibration);
  Serial.print("\t");
  Serial.print(" y-intercept ");
  Serial.print(y_intercept);
  Serial.print("\t");
  Serial.print("Slope ");
  Serial.print(slope);
  Serial.print("\t");
 */
  float od = ((( temp - y_intercept) / slope) - init_calibration) ;
     //10; // move float decimal value

// Serial.print(me->HR_tube_id);
// Serial.print("\t");
// Serial.print(od * 100); // return value like -0.34 to this works 

  me->HR_tube_data = (od * 100); 
 // Serial.print("\t");
 // Serial.println(me->HR_tube_data); // returns 0
 return od;
}

static inline void get_reading_data_toHR( data_agg_t * const me)//-[Function test: PASS ( consider removing serial outputs)
{

  
  me->HR_system_status  &= ~me->HR_system_status;
  me->HR_system_status  |= (VERSION | DATA_SEND);
  mb.Hreg(HoldingReg1, me-> HR_tube_id);
  mb.Hreg(HoldingReg2, me-> HR_tube_data);
  mb.Hreg(HoldingReg3, me-> HR_system_status);

  mb.task(); //update holding registers (! this doesn't affect network connectivity)
/*
  Serial.print(me->HR_tube_id);
  Serial.print("\t");
  Serial.print(me->HR_tube_data);
  Serial.print("\t");
  Serial.println(me->HR_system_status);
  
 */ 
}

//-[Setters]
static inline void set_version(data_agg_t * const me)
{
 me-> HR_system_status |= VERSION; 
}


//-[Start of Hardware IO init]
static inline void init_outputs()
{
  //-[digital pins D2,D3,D5 as output for leds]
  for( int i = 3; i < 6; i++)
  {
   DDRE |= (1 << i);
   
  }
  //-[digital pin D4 as output for leds]
    DDRG |= (1 << 5);

 //-[digital pins D6, D7,D8, D9 as output for leds]
  for( int i = 3 ; i < 7 ; i++)
  {
    DDRH |= (1 << i);
  }
    
}

static inline void set_outputs_high()
{
  PORTE |= 0b00111000;
  PORTG |= 0b00100000;
  PORTH |= 0b01111000;

}

static inline void reset_outputs_low()
{
 // delay(100);
  PORTE &= ~0b00111000;
  PORTG &= ~0b00100000;
  PORTH &= ~0b01111000;
}
//-[End of Hardware IO init]-\\

#endif /* utils_config.h */
