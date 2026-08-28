add_subdirectory(config)

# Create your game executable target as usual
add_executable(main 
    main.cpp
    base/utility.cpp
    base/texture.cpp
    base/text.cpp
    base/sprites.hpp
    objects/ball.cpp
    objects/button.cpp
    base/context.cpp)


target_include_directories(main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_BINARY_DIR}/config
)
