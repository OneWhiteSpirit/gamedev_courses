#include "render.hpp"
#include "window.hpp"
#include <cassert>
#include <cstdlib>
#include <fstream>

int main()
{
    //using namespace std;
    using namespace one_dc;
    using namespace graphics;

    window game_win("some title", 640, 480);
    render rend(game_win.get_sdl_window());

    while (!game_win.closed()) {

        rend.clear();

        std::ifstream file("vertexes.txt");
        assert(!!file);

        triangle tr;
        file >> tr;

        rend.render_triangle(tr);

        game_win.update();
    }

    return EXIT_SUCCESS;
}
