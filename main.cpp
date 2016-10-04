#include "bitmap.h"
#include "mandlebrot.h"
#include "parseinput.h"

#include <memory>
#include <iostream>
#include <stdlib.h>

/**********************************************************************
 * TODO
 * change one_pixel_blur to an n-pixel variety version
 * change the argument parser to take a array of tuples
 * (char* arg_match_str, char* usage, function ptr to parse function)
 * parse function should be of the form parse(Input_Params*, arg_match_str, argv[i])
 * move bitmap processing to new file, also move arg parsing to new file
 **********************************************************************/

using namespace std;

int main(int argc, char** argv) {
    auto params = ParseInput::Parse(argc, argv);

    Mandlebrot::Mandlebrot_Parameters fractal_params;
    fractal_params.x_min = params->x_min;
    fractal_params.x_max = params->x_max;
    fractal_params.y_min = params->y_min;
    fractal_params.y_max = params->y_max;
    fractal_params.x_divisions = params->x_divisions;
    fractal_params.y_divisions = params->y_divisions;

    printf("Beginning mandlebrot\n");
    Mandlebrot mandlebrot(fractal_params);
    auto array = mandlebrot.naive_mandlebrot();
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