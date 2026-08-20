#18-8-2026: CMake: Let op gebruik hier '#' ipv '//'

# Minimale versie
cmake_minimum_required(VERSION 3.20)

# Naam van het project:
project(VectorDatabase)


# Gebruik C++17
set(CMAKE_CXX_STANDARD 17)


# Maak een uitvoerbaarprogramma: Geef de cpp files aan
add_executable(VectorDatabase 
src/main.cpp 
src/Vector.cpp
src/VectorDatabase.cpp)


# Als je naar headerbestanden zoek, kijk dan ook in include.
target_include_directories(VectorDatabase PRIVATE include)
