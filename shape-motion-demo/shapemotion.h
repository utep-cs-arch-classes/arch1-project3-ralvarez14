#ifndef buzzer_included
#define buzzer_included


#define C3 3821
#define Db3 3607
#define D3 3404
#define Eb3 3213
#define E3 3033
#define F3 2863
#define Gb3 2702
#define G3 2550
#define Ab3 2407
#define A3 2272
#define Bb3 2144
#define B3 2024
#define C4 1910
#define Db4 1803
#define D4 1702
#define Eb4 1606
#define E4 1516
#define F4 1431
#define Gb4 1350
#define G4 1275
#define Ab4 1203
#define A4 1135
#define Bb4 1072
#define B4 1011
#define C5 955
#define Db5 901
#define D5 850
#define Eb5 803
#define E5 757
#define F5 715
#define Gb5 675
#define G5 637
#define Ab5 601
#define A5 567
#define Bb5 535
#define B5 505
#define C6 477
#define Db6 450
#define D6 425
#define Eb6 401
#define E6 378
#define F6 357
#define Gb6 337
#define G6 318
#define Ab6 300
#define A6 283
#define Bb6 267
#define B6 252
#define C7 238
#define Db7 224
#define D7 212
#define Eb7 200
#define E7 189
#define F7 178
#define Gb7 168
#define G7 158
#define Ab7 150
#define A7 141
#define Bb7 133
#define B7 126
#define C8 118
#define Db8 112
#define D8 105
#define Eb8 99 

#define E3 3033
#define Fs3 2703
//#define G3 2551  // delete
#define Gs3 2408
//#define A3 2273  // delete
#define As3 2145
//#define B3 2025  // delete
#define Cb4 
//#define C4 1911 // delete
#define Cs4 1804
//#define Db4 1804 // delete
//#define D4 1703  // delete
#define Ds4 1607
//#define Eb4 1607  // delete
//#define E4 1517  // delete
//#define F4 1432  //delete
#define Fs4 1351
//#define Gb4 1351  //delete
//#define G4 1276  //delete
#define Gs4 1204
//#define Ab4 1204 //delete
//#define A4 1136  //delete
#define As4 1073
//#define Bb4 1073  //delete
//#define B4 1012  //delete
//#define C5 956  //delete
#define Cs5 902
//#define Db5 902 //delete
//#define D5 851  //delete
#define Ds5 804
//#define Eb5 804  //delete
//#define E5 758  //delete
//#define F5 716  //delete
#define Fs5 676
//#define Gb5 676  //delete
//#define G5 638  //delete
#define Gs5 602
//#define Ab5 602  //delete
//#define A5 568  //delete

void buzzer_init();
void buzzer_advance_frequency();
void buzzer_set_period(short cycles);

void sw3_handler();
void switch_init();

char switch_state_changed;

#endif // included
 
