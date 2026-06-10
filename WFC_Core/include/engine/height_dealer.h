#ifndef HEIGHT_DEALER_H
#define HEIGHT_DEALER_H

#include "models/cell.h"
#include "engine/noises/INoise.h"

void set_height_by_height_factors(Cell& cell, INoise& hf_noise);

#endif
