#ifndef Jerry_h
#define Jerry_h

typedef struct {
    double x, y, z;
    char *name; //Unique string
} Planet;

typedef struct {
    Planet planet;
    char *name;
} Origin;

typedef struct {
    char *name;
    double value;
} PhysicalCharacteristics;

typedef struct {
    Origin origin;
    int happiness_level;
    char *id;
    PhysicalCharacteristics characteristics[]; //dynamic array
    int characteristics_count; //characteristics counter
} Jerry;

void add_jerry(Jerry);
void add_planet(Planet);
void add_origin(Origin);//permission_check
void add_physical_characteristics(PhysicalCharacteristics);//permission_check

#endif