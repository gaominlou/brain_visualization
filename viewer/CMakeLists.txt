## Copyright 2021-2024 The Khronos Group
## SPDX-License-Identifier: Apache-2.0

find_package(anari REQUIRED COMPONENTS viewer)

find_package(VTKm REQUIRED)

add_executable(demoViewer 
    demoViewer.cpp
    PolyLineDataReader.cxx    
)
target_link_libraries(demoViewer PRIVATE anari::anari
    vtkm::cont
    vtkm::io 
    anari::anari_viewer
)
target_include_directories(demoViewer PRIVATE ${CMAKE_CURRENT_LIST_DIR})
