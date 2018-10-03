#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>

#include "engine.hpp"

int main()
{
    std::unique_ptr<my_engine_ns::engine, void (*)(my_engine_ns::engine*)> engine(
        my_engine_ns::create_engine(), my_engine_ns::destroy_engine);

    std::string err = engine->initialize("");
    if (!err.empty())
    {
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    //pic start position
    int x = 300;
    int y = 205;

    bool continue_loop = true;
    while (continue_loop)
    {
        my_engine_ns::event event;

        while (engine->read_input(event, x, y))
        {
            std::cout << event << std::endl;
            switch (event)
            {
                case my_engine_ns::event::turn_off:
                    continue_loop = false;
                    break;
                default:
                    break;
            }
        }
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}
