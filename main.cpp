#include "bitmap.h"
#include "mandlebrot.h"

#include <math.h>
#include <memory>
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>

/**********************************************************************
 * TODO
 * change one_pixel_blur to an n-pixel variety version
 * change the argument parser to take a array of tuples
 * (char* arg_match_str, char* usage, function ptr to parse function)
 * parse function should be of the form parse(Input_Params*, arg_match_str, argv[i])
 * move bitmap processing to new file, also move arg parsing to new file
 **********************************************************************/

using namespace std;

const vector<string> Color_Strategies{"bw", "int", "sin", "log", "tanh"};

typedef struct {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    int x_divisions;
    int y_divisions;

    std::string strategy;
} Input_Params;

class ParseInput {
    private:
        double x_min;
        double x_max;
        double y_min;
        double y_max;
        int x_divisions;
        int y_divisions;

        std::string strategy;
    protected:

    public:

        ParseInput();
        ~ParseInput();

        static void set_default_input_params(unique_ptr<Input_Params> const & params) {
            params->x_min = -0.5;
            params->x_max = 0.3;
            params->y_min = 0.5;
            params->y_max = 1.0;
            params->x_divisions = 1000;
            params->y_divisions = 1000;

            params->strategy = "log";

            return;
        }

        static void Parse(int argc, char** argv, unique_ptr<Input_Params> const & params) {
            parse_args(argc, argv, params);

            return; 
        }

        static int parse_args(int argc, char** argv, unique_ptr<Input_Params> const & params) {
    const char x_min_str[] = "xmin=";
    const char x_max_str[] = "xmax=";
    const char y_min_str[] = "ymin=";
    const char y_max_str[] = "ymax=";
    const char x_divisions_str[] = "xdivs=";
    const char y_divisions_str[] = "ydivs=";

    if(params == NULL) {
        return NULL;
    }

    set_default_input_params(params);

    for(int i = 0; i < argc; ++i) {
        if(argv[i][0] != '-') {
            continue;
        }

        if(argv[i][1] == 'h' && argv[i][2] == '\0') {
            //print_usage();
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
            char* ptr = argument + strlen(strat_str);
            string s(ptr);
            for(auto const & v : Color_Strategies) {
                if(s == v) {
                    params->strategy = s;
                printf("%s %s\n", strat_str, v.c_str());
                    break;
                }
            }
        }
    }

    return 0;
}
};


int main(int argc, char** argv) {
    auto params = make_unique<Input_Params>();
    ParseInput::Parse(argc, argv, params);

    Mandlebrot::Mandlebrot_Parameters fractal_params;
    fractal_params.x_min = params->x_min;
    fractal_params.x_max = params->x_max;
    fractal_params.y_min = params->y_min;
    fractal_params.y_max = params->y_max;
    fractal_params.x_divisions = params->x_divisions;
    fractal_params.y_divisions = params->y_divisions;

    printf("Beginning mandlebrot\n");
    Mandlebrot mandlebrot(fractal_params);
    uint8_t* array = mandlebrot.naive_mandlebrot();
    printf("End of mandlebrot\n");

    if(array == NULL) {
        return -1;
    }

    auto bitmapArraySize = params->y_divisions* params->x_divisions;

    auto bitmapArray = make_unique<uint8_t[]>(3* params->x_divisions * params->y_divisions);

    //histogram of brightness, idea is to use this to scale the bitmap array brightness.
    int level[256] = {0};
    int sum = 0;
    for(int i = 0; i < bitmapArraySize; ++i) {
        ++level[bitmapArray[3*i]];
        ++level[bitmapArray[3*i+1]];
        ++level[bitmapArray[3*i+2]];
    }

    for(int i = 0; i < bitmapArraySize; ++i) {
        if(params->strategy == "bw") {
            bitmapArray[3*i] = (array[i] == 0) ? (0) : 1;
        }
        else if(params->strategy == "sin") {
            bitmapArray[3*i] = (255*sin(M_PI/2 * array[i]/255.0)); //sin approach
        }
        else if(params->strategy == "log") {
            bitmapArray[3*i] = (!array[i]) ? (0) : (255*log(array[i]) / log(255)); //log approach, this has the highest 'gain' at the lower end
        }
        else if(params->strategy == "tanh" ) {
            bitmapArray[3*i] = 256 * tanh(array[i]/128); //tanh, check gradiant and bounds
        }
        else { //if (params->strategy == "int") {
            bitmapArray[3*i] = array[i];
        }

        bitmapArray[3*i+1] = 0;
        bitmapArray[3*i+2] = 0;
    }

    //data = four_times_AA(data->bitmapArray, data->horizontal_pixels, data->vertical_pixels);

    printf("Creating bitmap\n");

    BitmapClass bitmap(params->y_divisions, params->x_divisions, bitmapArray);
    bitmap.WriteToDisk("temp.bmp");

    return 0;
}