#include "bard_win32.h"

#define BARDLIB_IMPLEMENTATION
#include "bardlib.h"

#define BARD_ABS(x) (x) <= 0 ? -(x) : (x)

// TODO: implement https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
bool idx_is_on_line(int64_t idx, int64_t x1, int64_t y1, int64_t x2, int64_t y2)
{
    if (idx >= (int64_t)(global_state.window_height * global_state.window_width)) {printf_win32("WARNING: idx_is_on_line: idx too big \n)"); return false;}

    int64_t idx_x = (int64_t)(idx % global_state.window_width);
    int64_t idx_y = (int64_t)(idx / global_state.window_width);

    bool satisfies_y = y1 <= y2 ? y1 <= idx_y && idx_y <= y2 : y2 <= idx_y && idx_y <= y1;
    bool satisfies_x = x1 <= x2 ? x1 <= idx_x && idx_x <= x2 : x2 <= idx_x && idx_x <= x1;

    if (!satisfies_x || !satisfies_y) return false;

    // naive line formula: https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_two_points
    double tolerance = 0.8; // TODO take thickness/tolerance into account in the satisfies_x/y checks above

    int64_t dx = x1 - x2;
    int64_t dy = y1 - y2;

    if (dy*dy + dx*dx == 0) return idx_x == x1 && idx_y == y1;

    // int64_t distance_from_idx_to_line = ABSOLUTE_VALUE(dy*idx_x - dx*idx_y + x2*y1 - y2*x1) / SQRT(dy*dy + dx*dx);
    // int64_t distance_from_idx_to_line_squared = (dy*idx_x - dx*idx_y + x2*y1 - y2*x1)*(dy*idx_x - dx*idx_y + x2*y1 - y2*x1) / (dy*dy + dx*dx);
    int64_t distance_from_idx_to_line_squared_scaled = (dy*idx_x - dx*idx_y + x2*y1 - y2*x1)*(dy*idx_x - dx*idx_y + x2*y1 - y2*x1);
    int64_t tolerance_squared_scaled = (int64_t)(tolerance * tolerance * (dy*dy + dx*dx));

    // TODO: we could drop the "scaling" since we scale both sides by same amount, but it's not really important
    return distance_from_idx_to_line_squared_scaled < tolerance_squared_scaled;
}

void bard_draw_line(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint32_t color)
{
    uint64_t idx1 = y1 * global_state.window_width + x1;
    uint64_t idx2 = y2 * global_state.window_width + x2;

    if (idx1 == idx2) {global_state.pixels[idx1] = color; printf_win32("WARNING: idx1 == idx2, coloring single pixel and exiting \n)"); return;}

    uint64_t biggeridx = idx1 > idx2 ? idx1 : idx2;
    uint64_t smalleridx = idx1 < idx2 ? idx1 : idx2;

    if (biggeridx >= (global_state.window_height * global_state.window_width)) {printf_win32("WARNING: biggeridx bigger than window :( \n)"); return;}

    for (uint64_t idx = smalleridx; idx <= biggeridx; ++idx)
    {
        if (idx_is_on_line(idx, x1, y1, x2, y2)) {
            global_state.pixels[idx] = color;
        }
    }
}

void bard_clear_background(uint32_t color)
{   // NOTE: global window width/height is more for rendering, they don't change during window resize
    // maybe making this const and local var will allow compiler to be smarter?
    const uint64_t windowsize = global_state.window_width * global_state.window_height;
    for (uint64_t idx = 0; idx < windowsize; ++idx) 
    {
        global_state.pixels[idx] = color;
    }
}

int user_main()
{
    bard_create_window_win32(800, 600, "this is a title");

    while (!global_state.window_should_close)
    {
        bard_begin_drawing_win32();
        bard_clear_background(BARD_RGB(255, 255, 255));

        bard_draw_line(0, 0, 799, 599, BARD_RGB(255, 0, 0));

        bard_end_drawing_win32();
    }
    return 0;
}