// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <Adafruit_MAX31856.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
//Hardware SPI CS pin definition
#define CS_PIN 10

#define K_TYPE 0
#define VMODE_G32 1
#define VMODE_G8 2

//Thermocouple type definition
//#define TCTYPE K_TYPE
#define TCTYPE VMODE_G32
//#define TCTYPE VMODE_G8

//Defines gain for calculating voltage for VMODEs
#if TCTYPE == VMODE_G32
  #define GAIN 32
#elif TCTYPE == VMODE_G8
  #define GAIN 8
#endif

//Thermocouple unit definition
#define FAHRENHEIT
#define CELCIUS


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_max31856_t{
  
};
struct state_max31856_t{
  
};

float tc_vin;

//Provide CS pin to initialize hardward SPI
Adafruit_MAX31856 max = Adafruit_MAX31856(CS_PIN);


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_max31856();
void measure_max31856();


// ================================================================
// ===                          SETUP                           ===
// ================================================================
void setup_max31856() 
{
  max.begin();
  #if TCTYPE == K_TYPE
    max.setThermocoupleType(MAX31856_TCTYPE_K);
  #elif TCTYPE == VMODE_G32
    max.setThermocoupleType(MAX31856_VMODE_G32);
  #elif TCTYPE == VMODE_G8
    max.setThermocoupleType(MAX31856_VMODE_G8);
  #endif
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void measure_max31856() 
{
  #if TCTYPE == K_TYPE
    //Type K processing
    //Cold Junction Temp
    CJTemp = max.readCJTemperature();
    TCTemp = max.readThermocoupleTemperature();
  
    //Serial.print("Cold Junction Temp: "); Serial.println(CJTemp);
    //Serial.print("Thermocouple Temp: "); Serial.println(TCTemp);
    
    // Check and print any faults
    uint8_t fault = max.readFault();
    if (fault) {
      #if LOOM_DEBUG == 1
      if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
      if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
      if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
      if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
      if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
      if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
      if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
      if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
      #endif
    }
    
  #elif TCTYPE == VMODE_G32 || TCTYPE == VMODE_G8
    //Type VMODE_G32/VMODE_G8 processing
    //Note: readVoltage is a custom function NOT included in default max31856 library
    tc_vin = max.readVoltage(GAIN);
    
  #endif
}


void package_max31856(OSCBundle *bndl, char packet_header_string[]) 
{
  char addressString[255];
  #if TCTYPE == K_TYPE
  
  #ifdef CELCIUS
    sprintf(addressString, "%s%s", packet_header_string, "/CJTemp_C");
    bndl->add(addressString).add((float)CJTemp);
    sprintf(addressString, "%s%s", packet_header_string, "/TCTemp_C");
    bndl->add(addressString ).add((float)TCTemp);
  #endif
  
  #ifdef FAHRENHEIT
    sprintf(addressString, "%s%s", packet_header_string, "/CJTemp_F");
    bndl->add(addresString).add((float)(CJTemp * 1.8 + 32));
    sprintf(addressString, "%s%s", packet_header_string, "/TCTemp_F");
    bndl->add(addressString).add((float)(TCTemp * 1.8 + 32));
  #endif
  
  #elif TCTYPE == VMODE_G32 || TCTYPE == VMODE_G8
    sprintf(addressString, "%s%s", packet_header_string, "/voltage");
    bndl->add(addressString).add((float)(tc_vin));
  #endif
}
