#define FAMILY "LOOM"     // Should always be "LOOM", you can change this if you are setting up your own network
#define DEVICE "MuxShield" // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "RelayShield" or "IShield"
#define INIT_INST 1


#define REQUEST_SETTINGS 1

//--OPTIONS--//

#define LOOM_DEBUG 1	  // Set to 1 if you want Serial statements from various functions to print

#define is_ishield 0
#define num_servos 0
#define num_steppers 0
#define is_relay   0
#define is_wifi    1
#define is_lora    0
#define is_analog  0
#define is_decagon 0
//Multiplexer
#define is_tca9548a 1

#if is_tca9548a == 1
	#define UPDATE_PERIOD 5000 //milliseconds
	//Lux Sensor
	#define is_tsl2591 1

	//Accelerometer / Magnetometer
	#define is_fxos8700 1

	//Gyroscope
	#define is_fxas21002 1

	//ZX_Distance Sensor
	#define is_zxgesturesensor 1

	//Temperature / Humidity
	#define is_sht31d 1

	//Sonar
	#define is_mb1232 1

	//Accelerometer / Gyroscope
	#define is_mpu6050 1
#endif

#if is_ishield == 1
  #define is_neopixel 1      // Toggle based on whether Neopixels are being used

  #define is_mpu6050  1

  #if is_neopixel == 1
    #define NEO_0 false
    #define NEO_1 false
    #define NEO_2 true
  #endif  
#endif

#if is_wifi == 1
  #define DEFAULT_MODE      WPA_CLIENT_MODE //AP_MODE, WPA_CLIENT_MODE or WEP_CLIENT_MODE
  #define DEFAULT_NETWORK   "OPEnS"
  #define DEFAULT_PASSWORD  "arduino101"
  #define COMMON_PORT       9440
  #define INIT_PORT         9445
#endif

// LoRa Device Type
// 0: Hub, 1: Node, 2 = Repeater
#if is_lora == 1

  #define lora_device_type 0
  
  #define SERVER_ADDRESS 0          //Use 0-9 for SERVER_ADDRESSes
  #define RF95_FREQ      915.0      //Hardware specific, Tx must match Rx

  #if lora_device_type == 0 // Hub
    #define NUM_FIELDS 32           	//Maximum number of fields accepted by the PushingBox Scenario    
    #include <Ethernet2.h>            // -- ideas on how to move this to declarations? (its needed for IPAddress but that is a user option)
  #endif
  
  #if lora_device_type == 1 // Node
    #define CLIENT_ADDRESS 10       //10 CLIENT_ADDRESSes belong to each SERVER_ADDRESS, 
  #endif														//10-19 for 0, 20 - 29 for 1, etc.
  
#endif

#define is_sleep_period 80          // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
//#define is_sleep_interrupt 11       // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt
