# Create your game executable target as usual
add_executable(sdl_tetris 
    main.cpp
    utility.cpp
    texture.cpp
    text.cpp
    sprites.hpp
    objects/ball.cpp
    objects/button.cpp
    context.cpp)
