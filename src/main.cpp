#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define EYE_LED  6
#define MOUTH_LED  8
#define HEAD_LED  2

#define LEYE_PIN  16
#define REYE_PIN  17
#define MOUTH_PIN  18
#define HEAD_PIN  19

#define AUDIO_PIN 21

CRGB g_L_EYE[EYE_LED] = {0};
CRGB g_R_EYE[EYE_LED] = {0};
CRGB g_MOUTH[MOUTH_LED] = {0};
CRGB g_HEAD[HEAD_LED] = {0};

uint32_t showTimes[] = {
  1000,

  745,
  50,
  50,

  4872,
  574,
  50,
  50,

  805,
  50,
  50,
  50,
  50,

  4510,
  50,
  50,
  50,
  50,
  50,
  535,
  50,
  50,
  50,
  50,
  50,

  1714,
  50,
  50,
  587,
  50,
  50,
  50,
  1247,

  //First Head Stuff
  671,
  7922,
  501,
  2712,
  534,
  1798,

  //Voice Segment 2
  2351,
  1438,
  50,
  50,

  5249,
  50,
  50,
  546,
  50,

  4875,
  195,
  4176,
  50,
  50,
  50,
  187,
  254,
  324,
  357,
  475,
  487,
  525,
  450,

  //Voice Segment 3
  14923,
  436,
  50,
  50,
  815,
  50,
  50,
  861,
  50,
  391,
  50,
  50,
  50,
  10597,

  //Head Segment 2
  1281,
  6665,

  //Mouth Segment 4
  1798,
  4287,
  50,
  50,
  50,

  2287,
  50,
  50,

  3481,
  50,
  50,
  50,
  613,
  50,
  50,
  50,
  201,
  50,
  50,
  50,

  4872,
  50,
  50,
  50,
  50,
  50,
  50,

  3616,
  50,
  250,
  50,
  50,
  50,

  1679,
  50,
  50,
  50,

  4854,
  50,
  50,
  50,
  50,
  604,
  //Last Head Bit
  2071,
  7620,
  667
};

uint32_t mouthDelay[4][50] = {
  {
    219,
    180,
    365,

    440,
    209,
    132,
    420,

    137,
    166,
    149,
    271,
    269,

    151,
    190,
    163,
    188,
    199,
    369,
    151,
    136,
    285,
    236,
    240,
    434,

    334,
    384,
    527,
    274,
    355,
    135,
    605
  },
  {
    435,
    450,
    422,

    200,
    255,
    475,
    410,
    412,

    359,
    420,
    271,
    247,
    253,
    131,
    72,
    72,
    72,
    72,
    72,
    72,
    72
  },
  {
    195,
    209,
    391,
    149,
    173,
    484,
    266,
    823,
    165,
    461,
    442,
    395
  },
  {
    238,
    262,
    232,
    629,

    223,
    243,
    556,

    286,
    362,
    478,
    758,
    238,
    756,
    602,
    355,
    209,
    516,
    477,
    499,

    401,
    199,
    276,
    549,
    262,
    353,
    604,

    620,
    691,
    276,
    276,
    478,
    402,

    401,
    314,
    640,
    883,

    256,
    296,
    233,
    247,
    604
  }
};

void eyesOn(){
  fill_solid(g_L_EYE,EYE_LED,CRGB(255,255,0)); //Restore Left Eye
  fill_solid(g_R_EYE,EYE_LED,CRGB(255,255,0)); //Restore Right Eye
  FastLED.show();
}

void eyesOff(){
  fill_solid(g_L_EYE,EYE_LED,CRGB::Black); //Turn Off Left Eye
  fill_solid(g_R_EYE,EYE_LED,CRGB::Black); //Turn Off Right Eye
  FastLED.show();
}

void mouthOn(){
  fill_solid(g_MOUTH,MOUTH_LED,CRGB(255,255,0)); //Restore Mouth
  FastLED.show();
}
void mouthStop(){
  fill_solid(g_MOUTH,MOUTH_LED,CRGB(50,50,0));
  FastLED.show();
}

void mouthSpeak(uint32_t holdTime){
  fill_solid(g_MOUTH,MOUTH_LED,CRGB(255,255,0));
  FastLED.show();
  delay(holdTime);
  mouthStop();
}

void mouthOff(){
  fill_solid(g_MOUTH,MOUTH_LED,CRGB::Black); //Turn Off Mouth
  FastLED.show();
}

void headOn(){
  fill_solid(g_HEAD,HEAD_LED,CRGB::White); //Turn Head On
  FastLED.show();
}

void headOff(){
  fill_solid(g_HEAD,HEAD_LED,CRGB::Black); //Turn Head Off
  FastLED.show();
}

void setup() {
  Serial.begin(115200);
  pinMode(LEYE_PIN,OUTPUT);
  pinMode(REYE_PIN, OUTPUT);
  pinMode(MOUTH_PIN,OUTPUT);
  pinMode(HEAD_PIN, OUTPUT);
  pinMode(AUDIO_PIN,OUTPUT);

  digitalWrite(AUDIO_PIN,HIGH);

  FastLED.addLeds<WS2812B,LEYE_PIN,GRB>(g_L_EYE,EYE_LED);
  FastLED.addLeds<WS2812B,REYE_PIN,GRB>(g_R_EYE,EYE_LED);
  FastLED.addLeds<WS2812B,MOUTH_PIN,GRB>(g_MOUTH,MOUTH_LED);
  FastLED.addLeds<WS2812B,HEAD_PIN,GRB>(g_HEAD,HEAD_LED);
  FastLED.setBrightness(255);

  //Initial On
  fill_solid(g_L_EYE,EYE_LED,CRGB::Black);
  fill_solid(g_R_EYE,EYE_LED,CRGB::Black);
  fill_solid(g_MOUTH,MOUTH_LED,CRGB::Black);
  fill_solid(g_HEAD,HEAD_LED,CRGB::Black);
  FastLED.show();
  delay(2000);
}

void loop() {
  digitalWrite(AUDIO_PIN,LOW);
  delay(showTimes[0]);
  eyesOn();
  mouthStop();

  delay(showTimes[1]);
  mouthSpeak(mouthDelay[0][0]);
  delay(showTimes[2]);
  mouthSpeak(mouthDelay[0][1]);
  delay(showTimes[3]);
  mouthSpeak(mouthDelay[0][2]);
  delay(showTimes[4]);

  mouthSpeak(mouthDelay[0][3]);
  delay(showTimes[5]);
  mouthSpeak(mouthDelay[0][4]);
  delay(showTimes[6]);
  mouthSpeak(mouthDelay[0][5]);
  delay(showTimes[7]);
  mouthSpeak(mouthDelay[0][6]);
  delay(showTimes[8]);

  mouthSpeak(mouthDelay[0][7]);
  delay(showTimes[9]);
  mouthSpeak(mouthDelay[0][8]);
  delay(showTimes[10]);
  mouthSpeak(mouthDelay[0][9]);
  delay(showTimes[11]);
  mouthSpeak(mouthDelay[0][10]);
  delay(showTimes[12]);
  mouthSpeak(mouthDelay[0][11]);
  delay(showTimes[13]);

  mouthSpeak(mouthDelay[0][12]);
  delay(showTimes[14]);
  mouthSpeak(mouthDelay[0][13]);
  delay(showTimes[15]);
  mouthSpeak(mouthDelay[0][14]);
  delay(showTimes[16]);
  mouthSpeak(mouthDelay[0][15]);
  delay(showTimes[17]);
  mouthSpeak(mouthDelay[0][16]);
  delay(showTimes[18]);
  mouthSpeak(mouthDelay[0][17]);
  delay(showTimes[19]);
  mouthSpeak(mouthDelay[0][18]);
  delay(showTimes[20]);
  mouthSpeak(mouthDelay[0][19]);
  delay(showTimes[21]);
  mouthSpeak(mouthDelay[0][20]);
  delay(showTimes[22]);
  mouthSpeak(mouthDelay[0][21]);
  delay(showTimes[23]);
  mouthSpeak(mouthDelay[0][22]);
  delay(showTimes[24]);
  mouthSpeak(mouthDelay[0][23]);
  delay(showTimes[25]);

  mouthSpeak(mouthDelay[0][24]);
  delay(showTimes[26]);
  mouthSpeak(mouthDelay[0][25]);
  delay(showTimes[27]);
  mouthSpeak(mouthDelay[0][26]);
  delay(showTimes[28]);
  mouthSpeak(mouthDelay[0][27]);
  delay(showTimes[29]);
  mouthSpeak(mouthDelay[0][28]);
  delay(showTimes[30]);
  mouthSpeak(mouthDelay[0][29]);
  delay(showTimes[31]);
  mouthSpeak(mouthDelay[0][30]);
  delay(showTimes[32]);

  eyesOff();
  mouthOff();
  delay(showTimes[33]);
  headOn();
  delay(showTimes[34]);
  headOff();
  delay(showTimes[35]);
  headOn();
  delay(showTimes[36]);
  headOff();
  delay(showTimes[37]);
  headOn();
  delay(showTimes[38]);
  headOff();
  
  delay(showTimes[39]);
  mouthStop();
  delay(showTimes[40]);
  mouthSpeak(mouthDelay[1][0]);
  delay(showTimes[41]);
  mouthSpeak(mouthDelay[1][1]);
  delay(showTimes[42]);
  mouthSpeak(mouthDelay[1][2]);

  delay(showTimes[43]);
  mouthSpeak(mouthDelay[1][3]);
  delay(showTimes[44]);
  mouthSpeak(mouthDelay[1][4]);
  delay(showTimes[45]);
  mouthSpeak(mouthDelay[1][5]);
  delay(showTimes[46]);
  mouthSpeak(mouthDelay[1][6]);
  delay(showTimes[47]);
  mouthSpeak(mouthDelay[1][7]);

  delay(showTimes[48]);
  mouthSpeak(mouthDelay[1][8]);
  delay(showTimes[49]);
  mouthSpeak(mouthDelay[1][9]);
  delay(showTimes[50]);
  mouthSpeak(mouthDelay[1][10]);
  delay(showTimes[51]);
  mouthSpeak(mouthDelay[1][11]);
  delay(showTimes[52]);
  mouthSpeak(mouthDelay[1][12]);
  delay(showTimes[53]);
  mouthSpeak(mouthDelay[1][13]);
  delay(showTimes[54]);
  mouthSpeak(mouthDelay[1][14]);
  delay(showTimes[55]);
  mouthSpeak(mouthDelay[1][15]);
  delay(showTimes[56]);
  mouthSpeak(mouthDelay[1][16]);
  delay(showTimes[57]);
  mouthSpeak(mouthDelay[1][17]);
  delay(showTimes[58]);
  mouthSpeak(mouthDelay[1][18]);
  delay(showTimes[59]);
  mouthSpeak(mouthDelay[1][19]);
  delay(showTimes[60]);
  mouthSpeak(mouthDelay[1][20]);

  delay(showTimes[61]);
  mouthOff();

  delay(showTimes[62]);
  eyesOn();
  mouthStop();

  delay(showTimes[63]);
  mouthSpeak(mouthDelay[2][0]);
  delay(showTimes[64]);
  mouthSpeak(mouthDelay[2][1]);
  delay(showTimes[65]);
  mouthSpeak(mouthDelay[2][2]);
  delay(showTimes[66]);
  mouthSpeak(mouthDelay[2][3]);
  delay(showTimes[67]);
  mouthSpeak(mouthDelay[2][4]);
  delay(showTimes[68]);
  mouthSpeak(mouthDelay[2][5]);
  delay(showTimes[69]);
  mouthSpeak(mouthDelay[2][6]);
  delay(showTimes[70]);
  mouthSpeak(mouthDelay[2][7]);
  delay(showTimes[71]);
  mouthSpeak(mouthDelay[2][8]);
  delay(showTimes[72]);
  mouthSpeak(mouthDelay[2][9]);
  delay(showTimes[73]);
  mouthSpeak(mouthDelay[2][10]);
  delay(showTimes[74]);
  mouthSpeak(mouthDelay[2][11]);

  delay(showTimes[75]);
  eyesOff();
  mouthOff();

  delay(showTimes[76]);
  headOn();
  delay(showTimes[77]);
  headOff();

  delay(showTimes[78]);
  eyesOn();
  mouthStop();

  delay(showTimes[79]);
  mouthSpeak(mouthDelay[3][0]);
  delay(showTimes[80]);
  mouthSpeak(mouthDelay[3][1]);
  delay(showTimes[81]);
  mouthSpeak(mouthDelay[3][2]);
  delay(showTimes[82]);
  mouthSpeak(mouthDelay[3][3]);

  delay(showTimes[83]);
  mouthSpeak(mouthDelay[3][4]);
  delay(showTimes[84]);
  mouthSpeak(mouthDelay[3][5]);
  delay(showTimes[85]);
  mouthSpeak(mouthDelay[3][6]);
  
  delay(showTimes[86]);
  mouthSpeak(mouthDelay[3][7]);
  delay(showTimes[87]);
  mouthSpeak(mouthDelay[3][8]);
  delay(showTimes[88]);
  mouthSpeak(mouthDelay[3][9]);
  delay(showTimes[89]);
  mouthSpeak(mouthDelay[3][10]);
  delay(showTimes[90]);
  mouthSpeak(mouthDelay[3][11]);
  delay(showTimes[91]);
  mouthSpeak(mouthDelay[3][12]);
  delay(showTimes[92]);
  mouthSpeak(mouthDelay[3][13]);
  delay(showTimes[93]);
  mouthSpeak(mouthDelay[3][14]);
  delay(showTimes[94]);
  mouthSpeak(mouthDelay[3][15]);
  delay(showTimes[95]);
  mouthSpeak(mouthDelay[3][16]);
  delay(showTimes[96]);
  mouthSpeak(mouthDelay[3][17]);
  delay(showTimes[97]);
  mouthSpeak(mouthDelay[3][18]);

  delay(showTimes[98]);
  mouthSpeak(mouthDelay[3][19]);
  delay(showTimes[99]);
  mouthSpeak(mouthDelay[3][20]);
  delay(showTimes[100]);
  mouthSpeak(mouthDelay[3][21]);
  delay(showTimes[101]);
  mouthSpeak(mouthDelay[3][22]);
  delay(showTimes[102]);
  mouthSpeak(mouthDelay[3][23]);
  delay(showTimes[103]);
  mouthSpeak(mouthDelay[3][24]);
  delay(showTimes[104]);
  mouthSpeak(mouthDelay[3][25]);

  delay(showTimes[105]);
  mouthSpeak(mouthDelay[3][26]);
  delay(showTimes[106]);
  mouthSpeak(mouthDelay[3][27]);
  delay(showTimes[107]);
  mouthSpeak(mouthDelay[3][28]);
  delay(showTimes[108]);
  mouthSpeak(mouthDelay[3][29]);
  delay(showTimes[109]);
  mouthSpeak(mouthDelay[3][30]);
  delay(showTimes[110]);
  mouthSpeak(mouthDelay[3][31]);

  delay(showTimes[111]);
  mouthSpeak(mouthDelay[3][32]);
  delay(showTimes[112]);
  mouthSpeak(mouthDelay[3][33]);
  delay(showTimes[113]);
  mouthSpeak(mouthDelay[3][34]);
  delay(showTimes[114]);
  mouthSpeak(mouthDelay[3][35]);

  delay(showTimes[115]);
  mouthSpeak(mouthDelay[3][36]);
  delay(showTimes[116]);
  mouthSpeak(mouthDelay[3][37]);
  delay(showTimes[117]);
  mouthSpeak(mouthDelay[3][38]);
  delay(showTimes[118]);
  mouthSpeak(mouthDelay[3][39]);
  delay(showTimes[119]);
  mouthSpeak(mouthDelay[3][40]);

  delay(showTimes[120]);
  eyesOff();
  mouthOff();

  delay(showTimes[121]);
  headOn();

  delay(showTimes[122]);
  headOff();
  digitalWrite(AUDIO_PIN,HIGH);
  delay(showTimes[123]);
  

  /*eyesOn();
  mouthOn();
  delay(showTimes[1]);
  eyesOff();
  mouthOff();
  delay(showTimes[2]);
  headOn();
  delay(showTimes[3]);
  headOff();
  delay(showTimes[4]);
  headOn();
  delay(showTimes[5]);
  headOff();
  delay(showTimes[6]);
  headOn();
  delay(showTimes[7]);
  headOff();
  delay(showTimes[8]);
  mouthOn();
  delay(showTimes[9]);
  mouthOff();
  delay(showTimes[10]);
  eyesOn();
  mouthOn();
  delay(showTimes[11]);
  eyesOff();
  mouthOff();
  delay(showTimes[12]);
  headOn();
  delay(showTimes[13]);
  headOff();
  delay(showTimes[14]);
  eyesOn();
  mouthOn();
  delay(showTimes[15]);
  eyesOff();
  mouthOff();
  delay(showTimes[16]);
  headOn();
  delay(showTimes[17]);
  headOff();
  delay(showTimes[18]);
  digitalWrite(AUDIO_PIN,HIGH);
  delay(1000); //1 Second beat before sequence loop*/
  //END OF SEQUENCE
}