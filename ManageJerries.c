#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Jerry.h"
#include "Defs.h"

#define MAX_LINE_LENGTH 300


// Helper function: Parse Planet
Planet* parse_planet(const char *line) {

    if (line == NULL || strlen(line) == 0) {
        fprintf(stderr, "Error: Invalid planet line.\n");
        return NULL;
    }

    char planet_name[MAX_LINE_LENGTH];
    double x, y, z;

    // Parse the line to extract Planet details
    if (sscanf(line, "%[^,],%lf,%lf,%lf", planet_name, &x, &y, &z) != 4) {
        return NULL;
    }

    // Create the planet
    Planet *planet = add_planet(planet_name, x, y, z);
    if (!planet) {
        return NULL;
    }

    return planet;
}

//Helper function: Parse Jerry
Jerry* parse_jerry(const char *line,int number_of_planets, Planet **planets) {// here I should get also the planets array

    if (line == NULL || strlen(line) == 0) {
        fprintf(stderr, "Error: Invalid jerry line.\n");
        return NULL;
    }

    char id[MAX_LINE_LENGTH];
    char dimension[MAX_LINE_LENGTH];
    char planet_name[MAX_LINE_LENGTH];
    int happiness_level;

    // Parse the line to extract Jerry details
    if (sscanf(line, "%[^,],%[^,],%[^,],%d", id, dimension, planet_name, &happiness_level) != 4) {
        return NULL;
    }

    // Find the planet for the origin
    Planet *planet = NULL;
    for (int i = 0; i < number_of_planets; i++) { //I should supply the planets number here
        if (strcmp(planets[i]->name, planet_name) == 0) { //Hard problem!!
            planet = planets[i];
            break;
        }
    }

    if (!planet) {
        //fprintf(stderr, "Error: Planet '%s' not found for Jerry '%s'\n", planet_name, id);
        return NULL;
    }

    // Create the Origin structure
    Origin *origin = add_origin(planet, dimension);
    if (!origin) {
        fprintf(stderr, "Error: Failed to create origin for Jerry '%s'\n", id);
        return NULL;
    }

    // Create the Jerry structure
    Jerry *jerry = add_jerry(id, happiness_level, origin);
    if (!jerry) {
        fprintf(stderr, "Error: Failed to create Jerry '%s'\n", id);
        destroy_planet(planet);
        destroy_origin(origin); // Clean up if Jerry creation fails
        return NULL;
    }

    return jerry;
}


// Parse configuration file
status parse_config_file(const char *input_file, int number_of_planets, int number_of_jerries, Jerry **jerries, Planet **planets) {
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Unable to open file");
        return failure;
    }

    char line[MAX_LINE_LENGTH];
    int planet_count = 0;
    int jerry_count = 0;
    int section = 0; // 0 = none, 1 = Planets, 2 = Jerries

    Jerry *current_jerry = NULL;

    while (fgets(line, sizeof(line), file)) {

        // Skip empty lines or comments
        if (line[0] == '\0' || line[0] == '#') continue;

        // Detect sections
        if (strcmp(line, "Planets\n") == 0) {
            section = 1;
            continue;
        }
        else if (strcmp(line, "Jerries\n") == 0) {
            section = 2;
            continue;
        }

        if (section == 1) {
            if (planet_count >= number_of_planets) {
                fclose(file);
                return failure;
            }
            planets[planet_count] = parse_planet(line);
            if (!planets[planet_count]) {
                fclose(file);
                return failure;
            }
            planet_count++;

        } else if (section == 2) {
            // Check if it's a characteristic or a new Jerry
            if (strchr(line, ':')) {
                if (!current_jerry) {
                    fprintf(stderr, "Error: Characteristic without a corresponding Jerry.\n");
                    fclose(file);
                    return failure;
                }

                char *content = line;
                while (*content == '\t') content++;

                char key[MAX_LINE_LENGTH];
                double value;
                if (sscanf(content, "%[^:]:%lf", key, &value) != 2) {
                    fclose(file);
                    return failure;
                }

                PhysicalCharacteristics *physical_char = create_physical_characteristics(key, value); //free

                status s = add_physical_characteristic(current_jerry, physical_char);

                if (s != success) {
                    fprintf(stderr, "Error: Failed to create physical characteristics for Jerry.\n");
                    destroy_jerry(current_jerry);
                    fclose(file);
                    return failure;
                }

            } else {
                if (jerry_count >= number_of_jerries) {
                    fclose(file);
                    return failure;
                }
                current_jerry = parse_jerry(line, number_of_planets, planets);
                if(current_jerry == NULL) {
                    fclose(file);
                    return failure;
                }
                jerries[jerry_count] = current_jerry;
                jerry_count++;
            }
        }
    }

    fclose(file);
    return success;
}

void printMenu() {
    printf("AW JEEZ RICK, what do you want to do now ? \n");
    printf("1 : Print all Jerries \n");
    printf("2 : Print all Planets \n");
    printf("3 : Add physical characteristic to Jerry \n");
    printf("4 : Remove physical characteristic from Jerry \n");
    printf("5 : Print Jerries by a planet \n");
    printf("6 : Print Jerries by a physical characteristic \n");
    printf("7 : Go home \n");
}

void add_physical_characteristic_to_jerry(Jerry **jerries, int jerry_count) {
    char id[MAX_LINE_LENGTH];
    char characteristic_name[MAX_LINE_LENGTH];
    double characteristic_value;

    // Ask for Jerry's ID
    printf("What is your Jerry's ID ?\n");
    scanf("%s", id);

    // Find the Jerry by ID
    Jerry *target_jerry = NULL;
    for (int i = 0; i < jerry_count; i++) {
        if (strcmp(jerries[i]->id, id) == 0) {
            target_jerry = jerries[i];
            break;
        }
    }

    if (!target_jerry) {
        printf("OH NO! I CAN'T FIND HIM RICK !\n");
        return;
    }

    // Ask for the characteristic name
    printf("What physical characteristic can you add to Jerry - %s ?\n", id);
    scanf("%s", characteristic_name);

    // Check if the characteristic already exists
    for (int i = 0; i < target_jerry->characteristics_count; i++) {
        if (strcmp(target_jerry->characteristics[i]->name, characteristic_name) == 0) {
            printf("RICK I ALREADY KNOW HIS ");
            for (const char *p = characteristic_name; *p; p++) putchar(toupper(*p));
            printf(" !\n");
            return;
        }
    }

    // Ask for the value of the new characteristic
    printf("What is the value of his %s ?\n", characteristic_name);
    if (scanf("%lf", &characteristic_value) != 1) {
        printf("Invalid input for characteristic value.\n");
        return;
    }

    // Create and add the new characteristic
    PhysicalCharacteristics *new_characteristic = create_physical_characteristics(characteristic_name, characteristic_value);
    if (!new_characteristic) {
        printf("Error: Unable to create new characteristic.\n");
        return;
    }

    // Add the characteristic to the Jerry
    if (add_physical_characteristic(target_jerry, new_characteristic) != success) {
        printf("Error: Unable to add characteristic to Jerry.\n");
        destroy_physical_characteristics(new_characteristic);
        return;
    }

    // Print the updated Jerry details
    print_jerry(target_jerry);
}


void remove_physical_characteristic_to_jerry(Jerry **jerries, int jerry_count) {
    char id[MAX_LINE_LENGTH];
    char characteristic_name[MAX_LINE_LENGTH];

    printf("What is your Jerry's ID ?\n");
    scanf("%s", id);

    Jerry *target_jerry = NULL;
    for (int i = 0; i < jerry_count; i++) {
        if (strcmp(jerries[i]->id, id) == 0) {
            target_jerry = jerries[i];
        }
    }

    if (!target_jerry) {
        printf("OH NO! I CAN'T FIND HIM RICK !\n");
        return;
    }

    printf("What physical characteristic do you want to remove from Jerry - %s ?\n",id);
    scanf("%s", characteristic_name);

    if(delete_physical_characteristics(target_jerry, characteristic_name) != success) {
        printf("RICK I DON'T KNOW HIS ");
        for (const char *p = characteristic_name; *p; p++) putchar(toupper(*p));
        printf(" !\n");
        return;
    }
    print_jerry(target_jerry);
}


void print_jerries_by_planet(Planet **planets, int planet_count, Jerry **jerries, int jerry_count) {
    char planet_name[MAX_LINE_LENGTH];
    printf("What planet is your Jerry from?\n");
    scanf("%s", planet_name);

    Planet *target_planet = NULL;
    for (int i = 0; i < planet_count; i++) {
        if (strcmp(planets[i]->name, planet_name) == 0) {
            target_planet = planets[i];
        }
    }
    if (!target_planet) {
        printf("RICK I NEVER HEARD ABOUT ");
        for (const char *p = planet_name; *p; p++) putchar(toupper(*p));
        printf(" !\n");
        return;
    }

    int flag=0;
    for(int i=0;i<jerry_count;i++) {
        if (strcmp(jerries[i]->origin->planet->name, planet_name) == 0) {
            print_jerry(jerries[i]);
            flag=1;
        }
    }

    if (!flag) {
        printf("OH NO! I DON'T KNOW ANY JERRIES FROM ");
        for (const char *p = planet_name; *p; p++) putchar(toupper(*p));
        printf("!\n");
        return;
    }
}


void print_jerries_by_characterstic(Jerry **jerries, int jerry_count) {

    char characteristic_name[MAX_LINE_LENGTH];
    printf("What do you know about your Jerry?\n");
    scanf("%99s", characteristic_name);

    int flag=0;
    for (int i = 0; i < jerry_count; i++) {
        for (int j = 0; j < jerries[i]->characteristics_count; j++) {
            if (strcmp(characteristic_name, jerries[i]->characteristics[j]->name) == 0) {
                print_jerry(jerries[i]);
                flag=1;
            }
        }
    }
    if (!flag) {
        printf("OH NO! I DON'T KNOW ANY JERRY'S ");
        for (const char *p = characteristic_name; *p; p++) putchar(toupper(*p));
        printf("!\n");
    }
}



int main(int argc, char *argv[]) {

    int num_planets = atoi(argv[1]);
    int num_jerries = atoi(argv[2]);
    const char *file = argv[3];


    Planet **planets = malloc(num_planets * sizeof(Planet *));
    if (!planets) {
        //fprintf(stderr, "Error: Failed to allocate memory for planets.\n");
        return 0;
    }

    Jerry **jerries = malloc(num_jerries * sizeof(Jerry *));
    if (!jerries) {
        //fprintf(stderr, "Error: Failed to allocate memory for jerries.\n");
        free(planets);
        return 0;
    }

    if(parse_config_file(file, num_planets, num_jerries, jerries, planets) == failure){
        fprintf(stderr, "Error: Failed to parse config file.\n");
        free(planets);
        free(jerries);
        return 0;
    }


    int choice;
    do {
        printMenu();

        if (scanf("%d", &choice) != 1) {
            printf("RICK WE DON'T HAVE TIME FOR YOUR GAMES !\n");

            // Clear invalid input from buffer
            while (getchar() != '\n');
            choice = 0; // Reset choice to avoid processing invalid input
            continue;
        }

        if (choice < 1 || choice > 7) {
            //printf("AW JEEZ RICK, what do you want to do now?\n");
            continue; // Re-show the menu for invalid choices
        }

        switch (choice) {
            case 1:
                for (int i = 0; i < num_jerries; i++) {
                    print_jerry(jerries[i]);
                }
                    break;
            case 2:
                for (int i = 0; i < num_planets; i++) {
                    print_plant(planets[i]);
                }
                    break;
            case 3:
                add_physical_characteristic_to_jerry(jerries, num_jerries);
                    break;
            case 4:
                remove_physical_characteristic_to_jerry(jerries, num_jerries);
                    break;
            case 5:
                print_jerries_by_planet(planets, num_planets, jerries, num_jerries);
                    break;
            case 6:
                print_jerries_by_characterstic(jerries, num_jerries);
                    break;
            case 7:
                printf("AW JEEZ RICK, ALL THE JERRIES GOT FREE!\n");
            break;
        }
    } while (choice != 7);


    //Memory clean
    for (int i = 0; i < num_jerries; i++) {
        destroy_jerry(jerries[i]);
    }
    for (int i = 0; i < num_planets; i++) {
        destroy_planet(planets[i]);
    }

    free(jerries);
    free(planets);

    return 0;
}
