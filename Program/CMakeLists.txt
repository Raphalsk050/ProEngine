project(SampleApp LANGUAGES CXX)

if(POLICY CMP0065)
    cmake_policy(SET CMP0065 NEW)
endif()

if(POLICY CMP0156)
    cmake_policy(SET CMP0156 NEW)
endif()

if(POLICY CMP0179)
    cmake_policy(SET CMP0179 NEW)
endif()


add_executable(SampleApp Program.cpp SampleLayer.cpp SampleLayer.h)
target_link_libraries(SampleApp PUBLIC ProEngine ImGui)
target_include_directories(SampleApp PUBLIC ${PROJECT_SOURCE_DIR}/ProEngine ${PROJECT_SOURCE_DIR}/ProEngine/ThirdParty/imgui)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET SampleApp PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(TARGET SampleApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/ProEngine/Assets
        $<TARGET_FILE_DIR:SampleApp>/../ProEngine/Assets
)

if(APPLE)
    add_executable(MetalSample MetalProgram.cpp MetalSampleLayer.cpp MetalSampleLayer.h)
    target_link_libraries(MetalSample PUBLIC ProEngine ImGui)
    target_include_directories(MetalSample PUBLIC ${PROJECT_SOURCE_DIR}/ProEngine ${PROJECT_SOURCE_DIR}/ProEngine/ThirdParty/imgui)
    if (CMAKE_VERSION VERSION_GREATER 3.12)
        set_property(TARGET MetalSample PROPERTY CXX_STANDARD 20)
    endif()
    add_custom_command(TARGET MetalSample POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/ProEngine/Assets
            $<TARGET_FILE_DIR:MetalSample>/../ProEngine/Assets)
endif()
