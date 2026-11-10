#include "psg.h"
#include <osbind.h>

int main()
{
    set_tone(0, 1000);
    enable_channel(0, true, false);
    set_volume(0, 0x07);
    set_envelope(0x0E, 0xFFFF);
    Cnecin();
    stop_sound();

    return 0;
}