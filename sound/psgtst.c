#include "psg.h"
#include <osbind.h>

int main()
{
    /*set_tone(0, 3040);*/
    set_noise(14);
    enable_channel(0, false, true);
    set_volume(0, 0x07);
    /*set_envelope(0x0E, 0x0F00);*/
    Cnecin();
    stop_sound();

    return 0;
}