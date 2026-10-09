#include "raster.h"
#include <OSBIND.H>

int main() {
    UINT32 *fb = Physbase();
    clear_screen(fb);
    return 0;
}