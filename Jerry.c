#include "Jerry.h"
#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Jerry* add_jerry(Origin origin, int happiness_level, char *id) {
    Jerry *jp = (Jerry*)malloc(sizeof(Jerry));
    if (jp == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    jp->origin = origin;
    jp->happiness_level = happiness_level;
    jp->id = id;
    return jp;
};

Planet* add_planet(char *name, double x, double y, double z) {
    Planet *pp = (Planet*)malloc(sizeof(Planet));
    if (pp == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    pp->name = name;
    pp->x = x;
    pp->y = y;
    pp->z = z;
    return pp;
};

Origin* add_origin(Planet planet, char *name) {
    Origin *op = (Origin*)malloc(sizeof(Origin));
    if (op == NULL) {
        printf("Memory allocation failed\n");
    }
    op->planet = planet;
    op->name = name;
    return op;
};

PhysicalCharacteristics* create_physical_characteristics(char *name, double value) {
    PhysicalCharacteristics *pcp = (PhysicalCharacteristics*)malloc(sizeof(PhysicalCharacteristics));
    if (pcp == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    pcp->name = name;
    pcp->value = value;
    return pcp;
};

enum e_bool PhysicalCharacteristicExist(Jerry *jerry, char *characteristicsName){
    for (int i = 0; i < jerry->characteristics_count; i++) {
        if (strcmp(jerry->characteristics[i].name, characteristicsName) == 0) {
            return true;
        }
    }
    return false;
}

void add_physical_characteristic(Jerry *jerry, PhysicalCharacteristics *characteristic) {
    
}

