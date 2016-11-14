#include "bitmap.h"
#include "mandlebrot.h"
#include "parseinput.h"
#include "color_strategy.h"

#include <memory>
#include <iostream>
#include <stdlib.h>

/**********************************************************************
 * TODO
 * change one_pixel_blur to an n-pixel variety version
 **********************************************************************/

using namespace std;


//example command
// ./main -vel=true -xmin=-2.0 -xmax=-0.7 -ymax=0.5 -ymin=-0.5 -xdivs=4000 -ydivs=4000 -strat=sin

int main(int argc, char** argv) {
    auto params = ParseInput::Parse(argc, argv);

    auto fractal_params = std::make_unique<Mandlebrot::Mandlebrot_Parameters>();
    fractal_params->x_min = params->x_min;
    fractal_params->x_max = params->x_max;
    fractal_params->y_min = params->y_min;
    fractal_params->y_max = params->y_max;
    fractal_params->x_divisions = params->x_divisions;
    fractal_params->y_divisions = params->y_divisions;

    printf("Beginning mandlebrot\n");
    Mandlebrot mandlebrot(std::move(fractal_params));
    auto array = mandlebrot.naive_mandlebrot();
    printf("End of mandlebrot\n");

    if(array == nullptr) {
        return -1;
    }

    auto bitmapArraySize = params->y_divisions* params->x_divisions;

    auto bitmapArray = make_unique<uint8_t[]>(3* params->x_divisions * params->y_divisions);

    //histogram of brightness
    int level[256] = {0};
    int sum = 0;
    for(int i = 0; i < bitmapArraySize; ++i) {
        ++level[array[i]];
    }

    for(int i = 0; i < 256; ++i) {
        std::cout << "Iteration:\t" << i << "\tCount:\t" << level[i] << std::endl;
    }

    for(int i = 0; i < bitmapArraySize; ++i) {
        bitmapArray[3*i] = (params->strategy).m_function(array[i]);

        bitmapArray[3*i+1] = 0;

        //this is an _extremely naive 'velocity'
        if(params->compute_velocity && i > 1 && i < bitmapArraySize-1) {
            bitmapArray[3*i+2] = (params->strategy).m_function( 0xff & (int)abs( (array[i+1] - array[i-1])/(2) ) );
        } else {
            bitmapArray[3*i+2] = 0;
        }
    }

    //data = four_times_AA(data->bitmapArray, data->horizontal_pixels, data->vertical_pixels);

    printf("Creating bitmap\n");

    BitmapClass bitmap(params->y_divisions, params->x_divisions, bitmapArray);
    bitmap.WriteToDisk("temp.bmp");

    return 0;
}