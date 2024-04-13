// Author: Eugene Nesterenko
// Ukraine, 2024
// All data according PCF85134 Datasheet
// Universal 60 x 4 LCD segment driver for low multiplex rates
// Commands (Page 22):
// mode-set; load-data-pointer; device-select; bank-select; blink-select
// Control byte description (Page 28)

// Mode-set command bit description (Page 23)
// ENABLE == 1 - Enabled; == 0 - Disable (Blank)
// BIAS == 0 - 1/3 bias; == 1 - 1/2 bias
// MODE == 1 - static (one backplane) 
// MODE == 2 - 1:2 multiplex (two backplanes)
// MODE == 3 - 1:3 multiplex (three backplanes)
// MODE == 0 - Default - 1:4 multiplex (four backplanes)
#define DRIVER_MODE_SET_BASE_DEFAULT        B11000000
#define DRIVER_MODE_SET(ENABLE, BIAS, MODE) DRIVER_MODE_SET_BASE_DEFAULT |  \
                                            (ENABLE) |          \
                                            (BIAS) |            \
                                            (MODE)

#define MODE_SET_ENABLE         B00001000
#define MODE_SET_DISABLE        0
#define MODE_SET_BIAS_1_2       B00000100
#define MODE_SET_BIAS_1_3       0
#define MODE_SET_BIAS_DEFAULT   MODE_SET_BIAS_1_3
#define MODE_SET_MODE_STATIC    1  
#define MODE_SET_MODE_1_2       2
#define MODE_SET_MODE_1_3       3
#define MODE_SET_MODE_1_4       0
#define MODE_SET_MODE_DEFAULT   MODE_SET_MODE_1_4

// Load-data-pointer command bit description (Page 23) 
#define DRIVER_LOAD_DATA_POINTER_DEFAULT   B00000000
#define DRIVER_LOAD_DATA_POINTER(POINTER)  B01111111 & POINTER

// Device-select command bit description (Page 23)
#define DRIVER_DEVICE_SELECT_DEFAULT       B11100000
#define DRIVER_DEVICE_SELECT(A2A1A0)       B11100000 | (B00000111 & A2A1A0)

// Bank-select command bit description (Page 24)
// The bank-select command has no effect in 1:3 or 1:4 multiplex drive modes.
// I (O) == 0; Static - RAM row 0; 1:2 multiplex - RAM rows 0 and 1
// I (O) == 1; Static - RAM row 2; 1:2 multiplex - RAM rows 2 and 3
#define DRIVER_BANK_SELECT_DEFAULT         B11111000
#define DRIVER_BANK_SELECT(I, O)           DRIVER_BANK_SELECT_DEFAULT | (B00000010 & I) | (B00000001 & O)

// Blink-select command bit description (Page 24)
// AB == 0 Normal blinking is assumed when the LCD multiplex drive modes 1:3 or 1:4 are selected.
// AB == 1 Alternate RAM bank blinking does not apply in 1:3 and 1:4 multiplex drive modes.
// BF - blink frequency selection;
// BF = [0..3]
// BF == 0 - off
#define DRIVER_BLINK_SELECT_DEFAULT        B11110000
#define DRIVER_BLINK_SELECT(AB, BF)        DRIVER_BLINK_SELECT_DEFAULT | (B00000100 & AB) | (B00000011 & BF) 


// Control byte description (Page 28)
// [7 - CO][6 - RS][Not relevant - 5..0]
// bit 7: CO - continue bit; == 0 - last control byte; == 1 - control bytes continue
// bit 6: RS - register selection; == 0 - command register; == 1 - data register
// bits 5..0 - unused
#define DRIVER_CONTROL_BYTE(CO, RS)         CO | RS
#define CONTROL_BYTE_LAST_CONTROL       0
#define CONTROL_BYTE_CONTROL_CONTINUE   B10000000
#define CONTROL_BYTE_COMMAND_REGISTER   0
#define CONTROL_BYTE_DATA_REGISTER      B01000000
#define DRIVER_CONTROL_BYTE_COMMAND     DRIVER_CONTROL_BYTE(CONTROL_BYTE_CONTROL_CONTINUE, CONTROL_BYTE_COMMAND_REGISTER)
#define DRIVER_CONTROL_BYTE_DATA        DRIVER_CONTROL_BYTE(CONTROL_BYTE_LAST_CONTROL, CONTROL_BYTE_DATA_REGISTER)

// I2C slave address byte (For Arduino Wire)
#define DRIVER_SLAVE_ADDRESS_DEFAULT    B0111000
#define DRIVER_SLAVE_ADDRESS(SA)        DRIVER_SLAVE_ADDRESS_DEFAULT | (B0000001 & SA) 


