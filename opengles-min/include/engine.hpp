#include <iosfwd>
#include <string>
#include <string_view>

namespace my_engine_ns
{
/// dendy gamepad emulation events
enum class event
{
    /// input events
    left_pressed,

    right_pressed,

    up_pressed,

    down_pressed,

    escape_pressed,

    enter_pressed,

    lctrl_pressed,

    space_pressed,

    /// virtual console events
    turn_off
};

std::ostream& operator<<(std::ostream& stream, const event e);

class engine;

/// return not null on success
engine* create_engine();
void destroy_engine(engine* e);

class engine
{
public:
    virtual ~engine();
    /// create main window
    /// on success return empty string
    virtual std::string initialize(std::string_view config) = 0;
    /// pool event from input queue
    /// return true if event was written
    virtual bool read_input(event& e) = 0;
    virtual swap_buffers() = 0;
    virtual render_triangel(const triangel) = 0;
    virtual void uninitialize()       = 0;
};

} // end namespace my_engine_ns
