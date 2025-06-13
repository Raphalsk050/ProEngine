project(SampleApp LANGUAGES CXX)

add_executable(SampleApp Program.cpp)
target_link_libraries(SampleApp PUBLIC ProEngine ImGui)
target_include_directories(SampleApp PUBLIC ${PROJECT_SOURCE_DIR}/ProEngine ${PROJECT_SOURCE_DIR}/ProEngine/ThirdParty/imgui)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET SampleApp PROPERTY CXX_STANDARD 20)
endif()