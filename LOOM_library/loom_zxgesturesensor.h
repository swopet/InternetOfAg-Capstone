// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <ZX_Sensor.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_zxgesturesensor 0x10 								 //0x10, 0x11

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_zxgesturesensor_t {

};

struct state_zxgesturesensor_t {
	//Cannot declare a ZX_Sensor and not initialize
	ZX_Sensor inst_zxgesturesensor = ZX_Sensor(i2c_addr_zxgesturesensor);
	GestureType gesture;
	String gesture_type;
	uint8_t gesture_speed;
	int32_t pos[2];
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_zxgesturesensor_t config_zxgesturesensor;
struct state_zxgesturesensor_t state_zxgesturesensor;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_zxgesturesensor();
void package_zxgesturesensor(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void measure_zxgesturesensor();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_zxgesturesensor() {
  bool is_setup;
	uint8_t ver;
	
	if(state_zxgesturesensor.inst_zxgesturesensor.init()) {
		is_setup = true;
		#if LOOM_DEBUG == 1
			Serial.println("Initialized zxgesturesensor");
		#endif
	}
	else {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Failed to initialize zxgesturesensor");
		#endif
	}
	
	ver = state_zxgesturesensor.inst_zxgesturesensor.getModelVersion();
	if(ver != ZX_MODEL_VER) {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Incorrect Model Version or unable to read Model Version.");
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.print("Model Version: ");
			Serial.println(ver);
		#endif
	}
  
  // Read the register map version and ensure the library will work
  ver = state_zxgesturesensor.inst_zxgesturesensor.getRegMapVersion();
	if(ver != ZX_REG_MAP_VER) {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Incorrect Register Map Version or unable to read Register Map Version.");
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.print("Register Map Version: ");
			Serial.println(ver);
		#endif
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE ZXGESTURESENSOR ---
// Adds last read Zxgesturesensor readings to provided OSC bundle
// Arguments: bndl (pointer to the bundle to be added to)
//            packet_header_string (header string to send messages with)
//            port (which port of the multiplexer the device is plugged into)
// Return:    none
void package_zxgesturesensor(OSCBundle *bndl, char packet_header_string[], uint8_t port){
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/zxgesturesensor/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("type").add(state_zxgesturesensor.gesture_type);
	msg.add("speed").add((int32_t)state_zxgesturesensor.gesture_speed);
	msg.add("px").add(state_zxgesturesensor.pos[0]);
	msg.add("pz").add(state_zxgesturesensor.pos[1]);
	
	bndl->add(msg);
}



// --- MEASURE ZXGESTURESENSOR ---
// Gets the current sensor readings of the Zxgeesturesensor and stores into its state struct
// Arguments: none
// Return:    none
void measure_zxgesturesensor() {
	uint8_t x;
	uint8_t z;
	
	if(state_zxgesturesensor.inst_zxgesturesensor.positionAvailable()) {
    x = state_zxgesturesensor.inst_zxgesturesensor.readX();
		z = state_zxgesturesensor.inst_zxgesturesensor.readZ();
		
		if((x != ZX_ERROR) && (z != ZX_ERROR)) {
			state_zxgesturesensor.pos[0] = (int32_t)x;
			state_zxgesturesensor.pos[1] = (int32_t)z;
			#if LOOM_DEBUG == 1
				Serial.print("zxgesturesensor X: ");
				Serial.println(state_zxgesturesensor.pos[0]);
				Serial.print("zxgesturesensor Z: ");
				Serial.println(state_zxgesturesensor.pos[1]);
			#endif
		}
		else {
			#if LOOM_DEBUG == 1
				Serial.println("Error occurred while reading position data");
			#endif
		}	
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Position data unavailable for zxgesturesensor");
		#endif
	}
	
	if(state_zxgesturesensor.inst_zxgesturesensor.gestureAvailable()) {
    state_zxgesturesensor.gesture = state_zxgesturesensor.inst_zxgesturesensor.readGesture();
    state_zxgesturesensor.gesture_speed = state_zxgesturesensor.inst_zxgesturesensor.readGestureSpeed();
		
		switch (state_zxgesturesensor.gesture) {
			case NO_GESTURE:
					state_zxgesturesensor.gesture_type = "No Gesture";
				break;
			case RIGHT_SWIPE:
					state_zxgesturesensor.gesture_type = "Right Swipe";
				break;
			case LEFT_SWIPE:
					state_zxgesturesensor.gesture_type = "Left Swipe";
				break;
			case UP_SWIPE:
					state_zxgesturesensor.gesture_type = "Up Swipe";
			default:
				break;
		}
		#if DEBUG == 1
			Serial.print("Gesture type: ");
			Serial.println(state_zxgesturesensor.gesture_type);
			Serial.print("Gesture speed: ");
			Serial.println(state_zxgesturesensor.gesture_speed, DEC);
		#endif
  }
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Gesture data unavailable for zxgesturesensor");
		#endif
	}
}

