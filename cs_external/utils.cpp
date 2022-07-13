#include "./utils.hpp"

namespace utils {
    void move_mouse(long dx, long dy) {
        tagINPUT input = { 0 };
        tagMOUSEINPUT mouse_input = { dx, dy, 0, MOUSEEVENTF_MOVE, 0, 0 };

        input.mi = mouse_input;
        input.type = INPUT_MOUSE;
        SendInput(1, &input, sizeof(INPUT));
    }
}