
project(IMGUI)

add_library(imgui STATIC
  ${CMAKE_CURRENT_LIST_DIR}/imconfig.h
  ${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_internal.h
  ${CMAKE_CURRENT_LIST_DIR}/imgui_tables.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
  ${CMAKE_CURRENT_LIST_DIR}/imgui.h
  ${CMAKE_CURRENT_LIST_DIR}/imstb_rectpack.h
  ${CMAKE_CURRENT_LIST_DIR}/imstb_textedit.h
  ${CMAKE_CURRENT_LIST_DIR}/imstb_truetype.h
)

target_include_directories(imgui
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)