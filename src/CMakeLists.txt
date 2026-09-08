add_subdirectory(config)

# Create your game executable target as usual
add_executable(main 
    main.cpp
    base/utility.cpp
    base/texture.cpp
    base/text.cpp
    base/game_context.cpp
    base/settings.cpp
    base/sprites.hpp
    base/timer.cpp
    base/object_base.cpp
    base/resource_manager.cpp
    base/input_manager.cpp
    objects/ball.cpp
    objects/button.cpp
    objects/frame_label.cpp
    objects/arrow.cpp
    objects/label.cpp
    menu/settings_menu.cpp)


target_include_directories(main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_BINARY_DIR}/config
)
