#ifndef RENDER_H
#define RENDER_H

#include "types.h"
#include "Model.h"

void render(Model *model, UINT8* base);
void render_note(Note *note, UINT8* base);
void render_meter(Meter *meter, UINT8* base);
#endif