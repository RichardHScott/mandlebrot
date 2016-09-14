#include "bitmap.h"
#include "mandlebrot.h"

#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/**********************************************************************
 * TODO
 * change one_pixel_blur to an n-pixel variety version
 * change the argument parser to take a array of tuples
 * (char* arg_match_str, char* usage, function ptr to parse function)
 * parse function should be of the form parse(Input_Params*, arg_match_str, argv[i])
 **********************************************************************/

typedef enum { 
    BW, //black (in set) and white (out of set)
    INT, //shade based on iterations required to escape
    SIN, //sin based interpretation to 'lighten' quick escapes
    LOG, //log based interpretation to 'lighten' quick escapes
    TANH, //sigmod based, probably look at using tanh method as well
 } Color_Strategy;

const char* Color_Strategy_Strs[8] = { "bw", "int", "sin", "log", "tanh" };

typedef struct {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    int x_divisions;
    int y_divisions;

    Color_Strategy strategy;
} Input_Params;

void set_default_input_params(Input_Params* params) {
    params->x_min = -0.5;
    params->x_max = 0.3;
    params->y_min = 0.5;
    params->y_max = 1.0;
    params->x_divisions = 1000;
    params->y_divisions = 1000;

    params->strategy = LOG;

    return;
}

void print_usage() {
    printf("");
}

Input_Params* parse_args(int argc, char** argv) {
    const char x_min_str[] = "xmin=";
    const char x_max_str[] = "xmax=";
    const char y_min_str[] = "ymin=";
    const char y_max_str[] = "ymax=";
    const char x_divisions_str[] = "xdivs=";
    const char y_divisions_str[] = "ydivs=";

    Input_Params* params = (Input_Params*) malloc(sizeof(Input_Params));

    if(params == NULL) {
        return NULL;
    }

    set_default_input_params(params);

    for(int i = 0; i < argc; ++i) {
        if(argv[i][0] != '-') {
            continue;
        }

        if(argv[i][1] == 'h' && argv[i][2] == '\0') {
            print_usage();
            return NULL;
        }

        char* argument  = argv[i] + sizeof(char);

        if(0 == strncmp(argument, x_min_str, strlen(x_min_str))) {
            double val = strtod(argument + strlen(x_min_str), NULL);
            if(val != 0) {
                printf("%s %f\n", x_min_str, val);
                params->x_min = val;
            }
            continue;
        }

        if(0 == strncmp(argument, y_min_str, strlen(y_min_str))) {
            double val = strtod(argument + strlen(y_min_str), NULL);
            if(val != 0) {
                printf("%s %f\n", y_min_str, val);
                params->y_min = val;
            }
            continue;
        }

        if(0 == strncmp(argument, x_max_str, strlen(x_max_str))) {
            double val = strtod(argument + strlen(x_max_str), NULL);
            if(val != 0) {
                printf("%s %f\n", x_max_str, val);
                params->x_max = val;
            }
            continue;
        }

        if(0 == strncmp(argument, y_max_str, strlen(y_max_str))) {
            double val = strtod(argument + strlen(y_max_str), NULL);
            if(val != 0) {
                printf("%s %f\n", y_max_str, val);
                params->y_max = val;
            }
            continue;
        }

        if(0 == strncmp(argument, x_divisions_str, strlen(x_divisions_str))) {
            int val = atoi(argument + strlen(x_divisions_str));
            if(val != 0) {
                printf("%s %d\n", x_divisions_str, val);
                params->x_divisions = val;
            }
            continue;
        }

        if(0 == strncmp(argument, y_divisions_str, strlen(y_divisions_str))) {
            int val = atoi(argument + strlen(y_divisions_str));
            if(val != 0) {
                printf("%s %d\n", y_divisions_str, val);
                params->y_divisions = val;
            }
            continue;
        }

        const char* strat_str = "strat=";
        if(0 == strncmp(argument, strat_str, strlen(strat_str))) {
            for(int i=0; i < sizeof(Color_Strategy_Strs)/sizeof(Color_Strategy_Strs[0]); ++i) {
                if(0 == strncmp(argument + strlen(strat_str), Color_Strategy_Strs[i], strlen(Color_Strategy_Strs[i]))) {
                    params->strategy = i;
                    printf("%s %s\n", strat_str, Color_Strategy_Strs[i]);
                    break;
                }
            }
        }
    }

    return params;
}

static inline uint8_t average(uint8_t* data, int width, int top, int left) {
    uint16_t total = 0;

    total = data[3*width*top + left] + data[3*width*top + left + 3] + data[3*width*(top+1) + left] + data[3*width*(top+1) + left + 3];
    
    return total / 4;
}

uint8_t* one_pixel_blur(uint8_t* data, int width, int height) {
    uint8_t* new_data = (uint8_t*) malloc(width * height * 3);

    if(new_data == NULL) {
        return NULL;
    }

    for(int i = 1; i < height-1; ++i) {
        for(int j = 3; j < 3*width-6; j+=3) {
            new_data[(i-1)*3*width + (j-3)] = average(data, width, i-1, j-3);
            new_data[(i-1)*3*width + (j-3) + 1] = average(data, width, i-1, j-3+1);
            new_data[(i-1)*3*width + (j-3) + 2] = average(data, width, i-1, j-3+2);
        }
    }

    return new_data;
}

Bitmap_Data* four_times_AA(uint8_t* data, int width, int height) {
    int new_width = width / 2;
    int new_height = height / 2;
    uint8_t* new_data = (uint8_t*) malloc(new_width * new_height * 3);

    for(int i = 0; i < new_height-1; ++i) {
        for(int j = 0; j < new_width; ++j) {
            new_data[3*i*new_width + 3*j] = (data[6*i*width + 6*j] + data[6*i*width + 6*(j+1)] + data[6*(i+1)*width + 6*j] + data[6*(i+1)*width + 6*(j+1)])/4;
            new_data[3*i*new_width + 3*j + 1] = (data[6*i*width + 6*j + 1] + data[6*i*width + 6*(j+1) + 1] + data[6*(i+1)*width + 6*j + 1] + data[6*(i+1)*width + 6*(j+1) + 1])/4;
            new_data[3*i*new_width + 3*j + 2] = (data[6*i*width + 6*j + 2] + data[6*i*width + 6*(j+1) + 2] + data[6*(i+1)*width + 6*j + 2] + data[6*(i+1)*width + 6*(j+1) + 2])/4;
        }
    }

    Bitmap_Data* bitmap_data = (Bitmap_Data*) malloc(sizeof(Bitmap_Data));
    bitmap_data->horizontal_pixels = new_width;
    bitmap_data->vertical_pixels =  new_height;
    bitmap_data->bitmapArraySize = new_width*new_height;
    bitmap_data->bitmapArray = new_data;

    return bitmap_data;
}

int main(int argc, char** argv) {
    Input_Params* params = parse_args(argc, argv);

    uint8_t* array = naive_mandlebrot(params->x_min, params->x_max, params->y_min, params->y_max, params->x_divisions, params->y_divisions);

    if(array == NULL) {
        return -1;
    }

    Bitmap_Data* data = (Bitmap_Data*) malloc(sizeof(Bitmap_Data));
    data->horizontal_pixels = params->y_divisions;
    data->vertical_pixels = params->x_divisions;
    data->bitmapArraySize = params->y_divisions* params->x_divisions;
    data->bitmapArray = (uint8_t*) malloc(3 * params->x_divisions * params->y_divisions);

    for(int i = 0; i < data->bitmapArraySize; ++i) {
        switch(params->strategy) {
            case BW:
                data->bitmapArray[3*i] = (array[i] == 0) ? (0) : 1;
                break;
            case SIN:
                data->bitmapArray[3*i] = (255*sin(M_PI/2 * array[i]/255.0)); //sin approach
                break;
            case LOG:
                data->bitmapArray[3*i] = (!array[i]) ? (0) : (255*log(array[i]) / log(255)); //log approach, this has the highest 'gain' at the lower end
                break;
            case TANH:
                data->bitmapArray[3*i] = 256 * tanh(array[i]/128); //tanh, check gradiant and bounds
                break;
            case INT:
            default:
                data->bitmapArray[3*i] = array[i];
                break;
        }

        data->bitmapArray[3*i+1] = 0;
        data->bitmapArray[3*i+2] = 0;
    }

    //uint8_t* anti_aliased_data = one_pixel_blur(data->bitmapArray, data->horizontal_pixels, data->vertical_pixels);
    //data->bitmapArray = anti_aliased_data;

    data = four_times_AA(data->bitmapArray, data->horizontal_pixels, data->vertical_pixels);

    FILE* file = fopen("temp.bmp", "wb");

    if(file != NULL) {
        save_bitmap_to_disk(data, file);
        fclose(file);
    }

    free(array);
    return 0;
}