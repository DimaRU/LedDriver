// Hardware definitions


#ifdef TEST_CONFIG
#define NightLedPin 2
#else
#define NightLedPin 21
#endif
#define YellowLedPin 18
#define WhiteLedPin 19

#define LEDC_FREQUENCY 15000
#define LEDC_RESOLUTION 12
#define LEDC_MAXVALUE 1023
#define LEDC_MINVALUE 0

#define YellowChannel 0
#define WhiteChannel 1