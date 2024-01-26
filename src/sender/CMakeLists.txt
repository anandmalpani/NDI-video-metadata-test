project(sender)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC "../../thirdparty/ndi/include")

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

target_link_libraries(${PROJECT_NAME} "../../thirdparty/ndi/lib/x64/Processing.NDI.Lib.x64")
