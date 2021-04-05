//61 led rings cilindrical map demo
//Yaroslaw Turbin 05-04-2021
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/
//https://twitter.com/ldir_ko

#include <FastLED.h>

#define DATA_PIN     3

#define NUM_LEDS    61
#define NUM_COLS_CILINDR 24           // resolution for cilindrical lookup table
#define NUM_ROWS_CILINDR 5   

#define LED_TYPE    WS2812B          //leds type
#define COLOR_ORDER GRB              //color order of leds
#define MAX_POWER_MILLIAMPS 800  //write here your power in milliamps. default i set 800 mA for safety
#define BRIGHTNESS 255

CRGB leds[NUM_LEDS+1];  //one safe pixel in bottom. its index 61 

uint8_t gCurrentPatternNumber =0; // Index number of which pattern is current

const uint8_t exp_gamma[256] PROGMEM = {
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,
1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  12,  12,
12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,
28,  29,  30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,
39,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  50,  51,  52,
53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  85,
87,  89,  91,  92,  93,  95,  96,  98,  99,  100, 101, 102, 105, 106, 108,
109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 125, 126, 128, 130, 131,
133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 152, 154, 156, 158,
160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
190, 192, 194, 196, 198, 200, 202, 204, 207, 209, 211, 213, 216, 218, 220,
222, 225, 227, 229, 232, 234, 236, 239, 241, 244, 246, 249, 251, 253, 254, 255};

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
  .setCorrection( TypicalLEDStrip );
//   FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);   
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
} 

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {seven,six,five,first, second, trird,four}; 

void loop() {

EVERY_N_SECONDS( 10 ) // speed of change patterns periodically
{gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);} 

gPatterns[gCurrentPatternNumber]();
FastLED.show();  
} // main cycle

void first (){
uint16_t  a = millis()/8;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
     if (index!=61) leds[index].setHue (i*16+(sin8((j<<3)+a))>>1); 
}} //end cycles

}

void second (){
uint16_t  a = millis()/4;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
if (index!=61) {
leds[index].b=sin8((i/2-16)*cos8((j+20)*4)/4+a);
leds[index].g=(sin8(i*16+a/3)+cos8(j*8+a/2))/2;
leds[index].r=sin8(cos8(i*8+a/3)+sin8(j*8+a/4)+a);}
}} //end cycles
GammaCorrection();
}

void trird (){
uint16_t  a = millis()/4;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
    if (index!=61) leds[index].setHue (i*255/(NUM_COLS_CILINDR-1)*3+j*32+a/2); //full moving rainbow

}} //end cycles
}

void four (){
uint16_t  a = millis()/4;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
    if (index!=61) leds[index]= HeatColor(inoise8(i*70+a,j*60+a*2)); //red tonnel

}} //end cycles
}

void five (){
uint16_t  a = millis()/4;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
if (index!=61) leds[index]=HeatColor(qsub8 (inoise8 (i * 80+a , j * 5+ a , a /3), abs8(j - (NUM_ROWS_CILINDR-1)) * 255 / (NUM_ROWS_CILINDR+3)));

}} //end cycles
}

void six (){
uint16_t  a = millis()/8;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
if (index!=61) leds[index].setHue ((sin8((i<<5)+a*2)>>1)+(sin8((j<<5)+a))>>1);

}} //end cycles
}

void seven (){
uint16_t  a = millis()/8;
for (int j = 0; j < NUM_ROWS_CILINDR; j++) {
for (int i = 0; i < NUM_COLS_CILINDR; i++) {
   byte index = XY_cilindrical(i,j);
if (index!=61) 
leds[index].setRGB( (sin8(i*32+a)+cos8(j*16+a/2))/2, sin8(j*16+a/2+sin8(leds[index].r+a)/16), cos8(i*32+j*16-a/2+leds[index].g));
}} //end cycles
GammaCorrection();
}

byte XY_cilindrical (byte x, byte y) {
static const byte CilindricalLookTable [] =     //1/6 of full table for reduce memory size. little bit tricky to calculate index )) 
{0, 1, 2, 3, 24,25,61,26, 42,61,43,61, 54,61,61,61, 60,61,61,61};
static const byte offs [] = {4, 3, 2, 1, 0};

byte index = CilindricalLookTable [y*4+x%4] ;
if (index!=61) {index+= (x/4)*offs[y];}

return (index);
}

void GammaCorrection(){   //gamma correction function 
byte r,g,b;
for (uint16_t i=0; i<61; i++){
r=leds[i].r;
g=leds[i].g;
b=leds[i].b;
leds[i].r = pgm_read_byte(exp_gamma + r);
leds[i].g = pgm_read_byte(exp_gamma + g);
leds[i].b = pgm_read_byte(exp_gamma + b);
}
}
