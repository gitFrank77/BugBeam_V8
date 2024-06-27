
//#include <avr/io.h>
//#include <util/delay.h>
#include "utils_config.h"
#include <assert.h>
#define READ_DELAY  2000

volatile float initial_calibrations[8] = {0,0,0,0,0,0,0,0};
int temp =0;

//Modbus IP Object
//ModbusIP mb;

//Data handler Objec

data_agg_t *my_data;

void setup() {
 
 Serial.begin(9600);
 

// The media access control (ethernet hardware) address for the shield
byte mac[] = {0xA8, 0x61, 0x0A, 0xAF, 0x04, 0xC4};  
// The IP address for the shield
byte ip[] = { 10,10,5,16 };   

//Config Modbus IP 
 mb.config(mac, ip);

//-[see file utils_config.h]
mb.addHreg(HoldingReg1);
mb.addHreg(HoldingReg2);
mb.addHreg(HoldingReg3);

Serial.println("Setting up data");
 my_data = (data_agg_t *)malloc(sizeof(data_agg_t));
  if( my_data != NULL)
    {
      data_agg_ctor(my_data);
    }
  else
  {
    exit(0);
  }

 //-[Initilization requirements] 
 set_version(my_data);
 init_outputs(); 
 set_outputs_high();
 delay(2000); // allow lux sensors to 'quench' 
/*
 initial_calibrations[0] = get_analog_value(analogRead(0)); //tube 0
 initial_calibrations[1] = get_analog_value(analogRead(1)); //tube 1
 initial_calibrations[2] = get_analog_value(analogRead(2)); //tube 2
 initial_calibrations[3] = get_analog_value(analogRead(3)); //tube 3
 initial_calibrations[4] = get_analog_value(analogRead(8)); //tube 4
 initial_calibrations[5] = get_analog_value(analogRead(11));//tube 5
 initial_calibrations[6] = get_analog_value(analogRead(13));//tube 6
 initial_calibrations[7] = get_analog_value(analogRead(14));//tube 7
 */
 /*Serial.println(initial_calibrations[0]);
 Serial.println(initial_calibrations[1]);
 Serial.println(initial_calibrations[2]);
 Serial.println(initial_calibrations[3]);
 Serial.println(initial_calibrations[4]);
 Serial.println(initial_calibrations[5]);
 Serial.println(initial_calibrations[6]);
 Serial.println(initial_calibrations[7]);
 */
 temp=  analogRead(0);// allow time for analog input setup
 delay(1000);

 set_outputs_high();
 initial_calibrations[0] =get_od(analogRead(0), calibrations_lookup_table, 0, my_data, initial_calibrations[0]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[1] =get_od(analogRead(1), calibrations_lookup_table, 1, my_data, initial_calibrations[1]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[2] =get_od(analogRead(2), calibrations_lookup_table, 2, my_data, initial_calibrations[2]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[3] =get_od(analogRead(3), calibrations_lookup_table, 3, my_data, initial_calibrations[3]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[4] =get_od(analogRead(8), calibrations_lookup_table, 4, my_data, initial_calibrations[4]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[5] =get_od(analogRead(11), calibrations_lookup_table, 5, my_data, initial_calibrations[5]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[6] =get_od(analogRead(13), calibrations_lookup_table, 6, my_data, initial_calibrations[6]);
 reset_outputs_low();
 delay(READ_DELAY);
 set_outputs_high();
 initial_calibrations[7] =get_od(analogRead(14), calibrations_lookup_table, 7, my_data, initial_calibrations[7]);
 reset_outputs_low();
 delay(READ_DELAY);


}

void loop() {

 /* Serial.println(initial_calibrations[0]);
  Serial.println(initial_calibrations[1]);
  Serial.println(initial_calibrations[2]);
  Serial.println(initial_calibrations[3]);
*/

  //-Tube 1
  set_outputs_high();
  get_od(analogRead(0), calibrations_lookup_table, 0 , my_data, initial_calibrations[0]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
 // Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);


    //-Tube 2
  set_outputs_high();
  get_od(analogRead(1), calibrations_lookup_table, 1 , my_data, initial_calibrations[1]); 
  
  get_reading_data_toHR( my_data);
  reset_outputs_low();
//  Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);


    //-Tube 3
  set_outputs_high();
  get_od(analogRead(2), calibrations_lookup_table, 2 , my_data, initial_calibrations[2]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
 // Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);


    //-Tube 4
  set_outputs_high();
  get_od(analogRead(3), calibrations_lookup_table, 3 , my_data, initial_calibrations[3]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
//  Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);

    //-Tube 5
  set_outputs_high();
  get_od(analogRead(8), calibrations_lookup_table, 4 , my_data, initial_calibrations[4]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
 // Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);

    //-Tube 6
  set_outputs_high();
  get_od(analogRead(11), calibrations_lookup_table, 5 , my_data, initial_calibrations[5]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
 // Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);

    //-Tube 7
  set_outputs_high();
  get_od(analogRead(13), calibrations_lookup_table, 6 , my_data, initial_calibrations[6]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
  //Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);

    //-Tube 8
  set_outputs_high();
  get_od(analogRead(14), calibrations_lookup_table, 7 , my_data, initial_calibrations[7]); 
  get_reading_data_toHR( my_data);
  reset_outputs_low();
  //Serial.println(my_data->HR_tube_data);
  delay(READ_DELAY);





}
