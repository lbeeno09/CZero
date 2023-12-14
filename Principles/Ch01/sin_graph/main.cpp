#include <cmath>
#include <vector>
#include "implot.h"

int main()
{
    static double x[1001], y[1001];
    for(int i = 0; i < 1001; i++)
    {
        x[i] = i * 0.001f;
        y[i] = sin(x[i]);
    }

    if(ImPlot::BeginPlot("sin_graph"))
    {
        ImPlot::SetupAxes("x", "y");
        ImPlot::PlotLine("f(x)", x, y, 1001);

        ImPlot::EndPlot();
    }
}
