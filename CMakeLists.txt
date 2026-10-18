cmake_minimum_required(VERSION 3.19)
project(P3P_Save_Editor VERSION 1.0.0 LANGUAGES CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wsign-compare -Wconversion -Wshadow -Wuseless-cast)
endif()

if(MSVC)
    add_compile_options(/W4 /permissive-)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(P3P_Save_Editor
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/gui/mainwindow.ui
    src/models/persona.h
    src/models/skill.h
    src/models/item.h
    src/models/item.cpp
    src/models/arcana.h

    src/data/arcana_repository.h
    src/data/arcana_repository.cpp
    src/gui/tabs/tab_character.h src/gui/tabs/tab_character.cpp src/gui/tabs/tab_character.ui
    src/gui/tabs/tab_inventory.h src/gui/tabs/tab_inventory.cpp src/gui/tabs/tab_inventory.ui
    src/gui/tabs/tab_compendium.h src/gui/tabs/tab_compendium.cpp src/gui/tabs/tab_compendium.ui
    src/data/skill_repository.h src/data/skill_repository.cpp
    src/models/save_data.h
    src/data/persona_repository.h src/data/persona_repository.cpp
    src/data/threshold_repository.h src/data/threshold_repository.cpp
    src/data/item_repository.h src/data/item_repository.cpp
    src/io/save_file_handler.h src/io/save_file_handler.cpp
    src/utils/combo_box_style.h
    src/io/resource_loader.h src/io/resource_loader.cpp
    src/models/progression.h
    src/models/progression.cpp
    src/gui/delegates/spin_box_delegate.h src/gui/delegates/spin_box_delegate.cpp

)

if(WIN32)
    enable_language(RC)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/p3p_save_editor.rc")
    target_sources(P3P_Save_Editor PRIVATE ${app_icon_resource_windows})
endif()

target_include_directories(P3P_Save_Editor
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

target_compile_definitions(P3P_Save_Editor PRIVATE
    APP_VERSION="${PROJECT_VERSION}"
    APP_NAME="P3P Save Editor"
)

qt_add_resources(P3P_Save_Editor "app_resources"
    PREFIX "/"
    FILES
    resources/arcanas.json
    resources/items.json
    resources/personas.json
    resources/skills.json
    resources/xp_thresholds.json
    resources/p3p_save.svg
)

target_link_libraries(P3P_Save_Editor
    PRIVATE
    Qt::Core
    Qt::Widgets
)

install(TARGETS P3P_Save_Editor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET P3P_Save_Editor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})
