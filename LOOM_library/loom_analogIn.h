// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define num_measurements 4      // Must be 1, 2, 4, or 8 number of analog measurements to sample and average per channel


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_analog_t {
  int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
//struct config_<module>_t *config_<module>;
struct state_analog_t *state_analog;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
uint32_t read_analog(uint8_t chnl);
void package_analog(OSCBundle *bndl, char packet_header_string[]);


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


//void measure_<module>() {
//  //Measure data and change the state here. 
//  //Potentially uses the config data
//}
//



// --- READ ANALOG ---
// Generic subroutine for reading raw sensor data with averaging
// Arguments: none
// Return:    uint32_t measured sensor value
uint32_t read_analog(uint8_t chnl)
{
  int i = num_measurements;
  uint32_t reading = 0;
  
  while (i--) 
    reading += analogRead(chnl);
    
  #if (num_measurements == 1) // Take a reading    
    return (reading);
  #endif
  #if (num_measurements == 2) // Take a reading    
    return (reading >> 1);    // Then divide by 2 to get average sample value
  #endif
  #if (num_measurements == 4) // Take a reading    
    return (reading >> 2);    // Then divide by 4 to get average sample value
  #endif
  #if (num_measurements == 8) // Take a reading    
    return (reading >> 3);    // Then divide by 8 to get average sample value
  #endif
} 



// --- PACKAGE ANALOG ---
// Gets analog reading from pors and button, as enabled,
//  and forms an OSC bundle out of them
// Arguments: none
// Return:    none
void package_analog(OSCBundle *bndl, char packet_header_string[])
{ 
  char addressString[255];
  // Get reading from relevant ports and the button if enabled          
  #if (is_analog > 0) 
    sprintf(addressString, "%s%s", packet_header_string, "/port0");
    bndl->add(addressString).add((int32_t)read_analog(0));
  #endif
  #if (is_analog > 1)
    sprintf(addressString, "%s%s", packet_header_string, "/port1");
    bndl->add(addressString).add((int32_t)read_analog(1));
  #endif
  #if (is_analog > 2)
    sprintf(addressString, "%s%s", packet_header_string, "/port2");
    bndl->add(addressString).add((int32_t)read_analog(2));
  #endif
}


