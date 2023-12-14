#include <vector>
#include "matplot/matplot.h"

int main()
{
    std::vector<double> x = matplot::linspace(0, 6);
    std::vector<double> y1 = matplot::transform(x, [](auto x) { return sin(x); });
    std::vector<double> y2 = matplot::transform(x, [](auto x) { return cos(x); });
    matplot::plot(x, y1, x, y2, "--");
    matplot::title("sin & cos");
    matplot::xlabel("x");
    matplot::ylabel("y");
    matplot::show();
}
