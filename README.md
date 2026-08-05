# win32
* Simple graphics library to put stuff on screen.
## USAGE
```c
#define BARDLIB_IMPLEMENTATION
#include "bardlib.h"

int main()
{
    bard_create_window_win32(800, 600, "this is a title");

    while (!global_state.window_should_close)
    {
        bard_begin_drawing_win32();

        for (int y = 0; y < global_state.window_height; y++)
        {
            for (int x = 0; x < global_state.window_width; x++) 
            {
                global_state.pixels[y * global_state.window_width + x] = BARD_RGB(255, 0, 0);
            }
        }

        bard_end_drawing_win32();
    }
    return 0;
}
```
