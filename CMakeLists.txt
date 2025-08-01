cmake_minimum_required(VERSION 3.16)
project(YOLO_OpenCV_CUDA)

set(CMAKE_CXX_STANDARD 17)

#Habilitar colores y salida útil
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Ruta al OpenCV instalado (ajusta a tu patch)
set(OpenCV_DIR "C:\\dev\\opencv_cuda\\build\\install")

find_package(OpenCV REQUIRED)

#Incluir CUDA
enable_language(CUDA)

add_executable(main main.cpp)

target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${OpenCV_LIBS})