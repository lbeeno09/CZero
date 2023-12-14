#include "matplot/matplot.h"

int main() 
{
    auto image = matplot::imread("lena_gray.png");
    matplot::imshow(image);
    matplot::show();
}