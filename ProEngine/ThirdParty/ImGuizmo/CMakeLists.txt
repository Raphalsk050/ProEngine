cmake_minimum_required(VERSION 3.10)

project(ImGuizmo)

add_library(ImGuizmo STATIC
        example/ImApp.h
        ImGuizmo.h
        ImGuizmo.cpp
)
