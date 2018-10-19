#include "render.hpp"
#include "window.hpp"
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace one_dc;
using namespace graphics;

float blend_vertex(const float& r1, const float& r2,
    const float a)
{
    float new_radius;
    new_radius = (1.0f - a) * r1 + a * r2;

    return new_radius;
}

circle blend(const circle& c1, const circle& c2,
    const float a)
{
    float r;
    if (c1.radius < c2.radius)
        r = blend_vertex(c1.radius, c2.radius, a);
    else
        r = blend_vertex(c2.radius, c1.radius, a);

    circle c(r);

    return c;
}

int main()
{
    //using namespace std;

    window game_win("some title", 640, 480);
    render rend(game_win.get_sdl_window());

    circle c1(0.8f);
    circle c2(0.5f);

    while (!game_win.closed()) {

        rend.clear();

        float alpha = (std::sin(game_win.get_time_from_init()) * 0.5f);

        circle c = blend(c1, c2, alpha);

        rend.render_circle(c);

        game_win.update();
    }

    rend.clear_render_data();

    return EXIT_SUCCESS;
}
