project(receiver)

add_executable(${PROJECT_NAME} main.cpp stb_image_write.h)

target_include_directories(${PROJECT_NAME} PUBLIC "../../thirdparty/ndi/include")

target_link_libraries(${PROJECT_NAME} "../../thirdparty/ndi/lib/x64/Processing.NDI.Lib.x64")
