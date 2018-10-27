#include "render.hpp"
#include "window.hpp"
#include <cmath>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cassert>

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

    std::ifstream file("vertex_texture_cord.txt", std::ios::binary);
    assert(!!file);

    triangle tr1;
    triangle tr2;

    file >> tr1 >> tr2;

    while (!game_win.closed()) {

        rend.clear();

        rend.render_triangle(tr1);
        rend.render_triangle(tr2);

        game_win.update();
    }

    rend.clear_render_data();

    return EXIT_SUCCESS;
}
