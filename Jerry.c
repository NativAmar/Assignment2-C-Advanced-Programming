#include "Jerry.h"
#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **planet_names = NULL;
int planet_number = 0;

Jerry* add_jerry(char *id, int happiness_level,Origin *origin) {
    if (origin == NULL) {
        return NULL;
    }
    Jerry *jp = (Jerry*)malloc(sizeof(Jerry));
    if (jp == NULL) {
        printf("Memory Problem\n"); //CHANGE
        return NULL;
    }
    jp->origin = origin;//should check
    jp->happiness_level = happiness_level;
    jp->characteristics_count=0;
    jp->characteristics = NULL;

    size_t id_length = strlen(id) +1;
    jp->id = (char*)malloc(id_length * sizeof(char));
    if (jp->id == NULL) {
        printf("Memory Problem\n"); //CHANGE
        free(jp);
        return NULL;
    }
    strcpy(jp->id, id);
    return jp;
};


status destroy_jerry(Jerry *jp) {
    if (jp == NULL) {
        return failure;
    }
    if (jp->id != NULL) {
        free(jp->id);
        jp->id = NULL;
    }
    if(jp->characteristics != NULL) {
        for (int i = 0; i < jp->characteristics_count; i++) {
            if (jp->characteristics[i] != NULL) {
                destroy_physical_characteristics(jp->characteristics[i]);
            }
        }
        free(jp->characteristics);
        jp->characteristics = NULL;
    }
    destroy_origin(jp->origin);
    free(jp);
    jp = NULL;
    return success;
}

Planet* add_planet(char *name, double x, double y, double z) {
    if (name == NULL) {
        return NULL;
    }
    enum e_status name_status = unique_name_test(name);
    if (name_status != success) {
        return NULL;
    }
    add_planet_name_to_names_array(name);
    Planet *pp = (Planet*)malloc(sizeof(Planet));
    if (pp == NULL) {
        printf("Memory Problem\n"); //CHANGE
        return NULL;
    }
    size_t name_length = strlen(name) +1;
    pp->name = (char*)malloc(name_length * sizeof(char));
    if (pp->name == NULL) {
        printf("Memory Problem\n"); //CHANGE
        free(pp);
        return NULL;
    }
    strcpy(pp->name, name);
    pp->x = x;
    pp->y = y;
    pp->z = z;
    return pp;
};


status add_planet_name_to_names_array(char *name) {
    if (name == NULL) {
        return failure;
    }

    // Allocate or resize the array
    char **temp = NULL;
    if (planet_names == NULL) {
        temp = (char**)malloc(sizeof(char*));
        if (temp == NULL) {
            return failure; // Allocation failed
        }
    } else {
        temp = realloc(planet_names, (planet_number + 1) * sizeof(char*));
        if (temp == NULL) {
            return failure; // Allocation failed
        }
    }
    planet_names = temp;

    // Allocate memory for the new planet name
    size_t name_length = strlen(name) + 1;
    planet_names[planet_number] = (char*)malloc(name_length);
    if (planet_names[planet_number] == NULL) {
        // Rollback to avoid memory leaks
        if (planet_number == 0) {
            free(planet_names);
            planet_names = NULL;
        }
        return failure;
    }

    // Copy the name and update the counter
    strcpy(planet_names[planet_number], name);
    planet_number++;

    return success;
}


status remove_planet_name_from_names_array(char *name) {
    if (planet_names == NULL || name == NULL) {
        return failure; // Guard clause for invalid input
    }

    // Search for the planet name to delete
    size_t index_to_remove = (size_t)-1;  // Initialize to an invalid index
    for (size_t i = 0; i < planet_number; i++) {
        if (strcmp(planet_names[i], name) == 0) {
            index_to_remove = i;
            break;
        }
    }

    // If the name wasn't found, return failure
    if (index_to_remove == (size_t)-1) {
        return failure;
    }

    // Free the memory for the planet name at the found index
    free(planet_names[index_to_remove]);

    // Shift remaining elements to the left
    for (size_t i = index_to_remove; i < planet_number - 1; i++) {
        planet_names[i] = planet_names[i + 1];
    }

    // Decrease the planet_number
    planet_number--;

    // If no planets remain, free the array and reset pointers
    if (planet_number == 0) {
        free(planet_names);
        planet_names = NULL;
        return success;
    }

    // Reallocate the array to free unused memory
    char **temp = realloc(planet_names, planet_number * sizeof(char*));
    if (temp == NULL) {
        // Rollback: Keep `planet_names` as is to avoid corruption
        return failure;
    }
    planet_names = temp;

    return success;
}



status unique_name_test(char *name) {
    if(planet_names == NULL) { //first name
        return success;
    }
    for(int i = 0; i < planet_number; i++) {
        if (strcmp(planet_names[i], name) == 0) {
            return failure; // the name already exist
        }
    }
    return success;
}


status destroy_planet(Planet *pp) {
    if (pp == NULL) {
        return failure;
    }
    if (pp->name != NULL) {
        // Remove the name from the names array before freeing it
        for (int i = 0; i <= planet_number; i++) {
            if (strcmp(planet_names[i], pp->name) == 0) {
                remove_planet_name_from_names_array(pp->name);
                break;
            }
        }

        // Free the name after it's no longer needed
        free(pp->name);
        pp->name = NULL;
    }
    free(pp);
    //pp=NULL;
    return success;
}


Origin* add_origin(Planet *planet, char *name) {
    if (planet == NULL || name == NULL) {
        return NULL;
    }

    Origin *op = (Origin*)malloc(sizeof(Origin));
    if (op == NULL) {
        printf("Memory Problem\n");
        return NULL;
    }

    op->planet = planet;
    size_t name_length = strlen(name) + 1;
    op->name = (char*)malloc(name_length * sizeof(char));
    if (op->name == NULL) {
        printf("Memory Problem\n");
        free(op); //
        return NULL;
    }
    strcpy(op->name, name);
    return op;
}



status destroy_origin(Origin *op) {
    if (op == NULL) {
        return failure;
    }
    if (op->name != NULL) {
        free(op->name);
        op->name = NULL;
    }
    //destroy_planet(op->planet);
    free(op);
    op=NULL;
    return success;
}


PhysicalCharacteristics* create_physical_characteristics(char *name, double value) {
    PhysicalCharacteristics *pcp = (PhysicalCharacteristics*)malloc(sizeof(PhysicalCharacteristics));
    if (pcp == NULL) {
        printf("Memory Problem\n");//CHANGE
        return NULL;
    }
    size_t name_length = strlen(name) +1;
    pcp->name = (char*)malloc(name_length * sizeof(char));
    if (pcp->name == NULL) {
        printf("Memory Problem\n");//CHANGE
        free(pcp);
        return NULL;
    }
    strcpy(pcp->name, name);
    pcp->value = value;
    return pcp;
};


status destroy_physical_characteristics(PhysicalCharacteristics *pcp) {
    if (pcp == NULL) {
        return failure;
    }
    if (pcp->name != NULL) {
        free(pcp->name);
        pcp->name = NULL;
    }
    free(pcp);
    pcp=NULL;
    return success;
}


//check if the characteristic name exist in the Jerry characteristics array
bool PhysicalCharacteristicExist(Jerry *jerry, char *characteristicsName){
    for (int i = 0; i < jerry->characteristics_count; i++) {
        if (strcmp(jerry->characteristics[i]->name, characteristicsName) == 0) {
            return true;
        }
    }
    return false;
}


//receive Jerry and characteristic pointers and add it to Physical Characteristics array
status add_physical_characteristic(Jerry *jerry, PhysicalCharacteristics *characteristic) {
    // check first if it's the first element in the array
    if(jerry->characteristics == NULL) {
        jerry->characteristics = (PhysicalCharacteristics**)malloc(sizeof(PhysicalCharacteristics*));
        if (jerry->characteristics == NULL) {
            return failure;
        }
        jerry->characteristics[0] = characteristic;
        jerry->characteristics_count++;
    }
    else {
        PhysicalCharacteristics **temp = (PhysicalCharacteristics**)realloc(jerry->characteristics,sizeof(PhysicalCharacteristics*) * (jerry->characteristics_count + 1)); //should check about this adding
        if (temp == NULL) {
            return failure;
        }
        jerry->characteristics = temp;
        jerry->characteristics[jerry->characteristics_count] = characteristic;
        jerry->characteristics_count++;
        return success;
    }
    return success;
}


//receive Jerry and characteristic name and delete it from the Physical Characteristics array
status delete_physical_characteristics(Jerry *jerry, char *characteristicsName) {

    if (jerry->characteristics == NULL || jerry->characteristics_count == 0) {
        //printf("There is no Characteristics at all\n");//CHANGE
        return failure;
    }

    for (int i = 0; i < jerry->characteristics_count; i++) {
        if (strcmp(jerry->characteristics[i]->name, characteristicsName) == 0) {
            destroy_physical_characteristics(jerry->characteristics[i]);

            for (int j = i; j < jerry->characteristics_count-1; j++) {
                jerry->characteristics[j] = jerry->characteristics[j+1];
            }

            jerry->characteristics_count -= 1;
            PhysicalCharacteristics **temp = realloc(jerry->characteristics, jerry->characteristics_count * sizeof(PhysicalCharacteristics *));

            if (temp == NULL){
                return failure;
            }

            jerry->characteristics = temp;
            return success;
        }
    }
    return failure;
}


//Jerry print
void print_jerry(Jerry *jerry){
    if (jerry == NULL) {
        printf("There is no Jerry\n");
        return;
    }
    printf("Jerry , ID - %s: \nHappiness level : %d \nOrigin : %s \nPlanet : %s (%.2f,%.2f,%.2f) \n",
        jerry->id, jerry->happiness_level, jerry->origin->name, jerry->origin->planet->name, jerry->origin->planet->x, jerry->origin->planet->y, jerry->origin->planet->z);
    if(jerry->characteristics_count != 0) { //check if characteristics exist
        printf("Jerry's physical Characteristics available : \n");
        printf("    ");
        for (int i = 0; i < jerry->characteristics_count; i++) {
            if (i == jerry->characteristics_count - 1) {
                PhysicalCharacteristics *pcp = jerry->characteristics[i];
                printf("%s : %.2f ", pcp->name, jerry->characteristics[i]->value);

            }
            else {
                PhysicalCharacteristics *pcp = jerry->characteristics[i];
                printf("%s : %.2f , ", pcp->name, jerry->characteristics[i]->value);
            }
        }
        printf("\n");
    }
}


//Planet print
void print_plant(Planet *planet){
    if (planet->name == NULL) {
        printf("There is no planet\n");
    }
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", planet->name, planet->x, planet->y, planet->z);
}

