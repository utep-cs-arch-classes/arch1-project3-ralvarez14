#include "shapemotion.h"

void rest(){
    for(int i = 0; i <  10000; i++){ // dont play not for a set duration
     buzzer_set_period(0);
    }
}
