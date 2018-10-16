#include "window.hpp"
#include "render.hpp"
#include <cstdlib>
#include <fstream>
#include <cassert>

int main()
{
    //using namespace std;
    using namespace one_dc;
    using namespace graphics;

    window game_window("some title", 640, 480);
    render r;

    while(!game_window.closed())
    {
        game_window.clear();

        std::ifstream file("vertexes.txt");
        assert(!!file);

        triangle tr;
        file >> tr;

        r.render_triangle(tr);

        game_window.update();
    }

    return EXIT_SUCCESS;
}