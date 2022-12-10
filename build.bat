gcc .\src\main.c -o .\build\snake.exe -Ofast -Wall -Wextra -std=c17 -Wno-missing-braces -lraylib -Wl,-Bstatic -lopengl32  -lgdi32 -lwinmm -mwindows
