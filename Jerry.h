#include <stddef.h>
#ifndef Jerry_h
#include "Defs.h"
#define Jerry_h

typedef struct {
    double x, y, z;
    char *name; //Unique string
} Planet;

typedef struct {
    Planet *planet;
    char *name;
} Origin;

typedef struct {
    char *name;
    double value;
} PhysicalCharacteristics;

typedef struct {
    Origin *origin;
    int happiness_level;
    char *id;
    PhysicalCharacteristics **characteristics; //dynamic array
    int characteristics_count; //characteristics counter
} Jerry;


extern char **planet_names;
extern int planet_number;


//
extern Jerry* add_jerry(char *id, int happiness_level,Origin *origin);
//
extern status destroy_jerry(Jerry *jerry);
//
extern Planet* add_planet(char *name, double x, double y, double z);
//
extern status destroy_planet(Planet *p);
//
extern Origin* add_origin(Planet *planet, char *name);//permission_check
//
extern status destroy_origin(Origin *origin);
//
extern PhysicalCharacteristics* create_physical_characteristics(char *name, double value);//permission_check
//
extern status destroy_physical_characteristics(PhysicalCharacteristics *p);
//
extern bool PhysicalCharacteristicExist(Jerry *jerry, char *characteristicsName);
//
extern status add_physical_characteristic(Jerry *jerry, PhysicalCharacteristics *characteristic);//should change the name, very similiar to 3 function up
//
extern status delete_physical_characteristics(Jerry *jerry, char *characteristicsName);
//
extern void print_jerry(Jerry *jerry);
//
extern void print_plant(Planet *planet);

#endif