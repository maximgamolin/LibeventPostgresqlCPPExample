cmake_minimum_required(VERSION 3.15)
project(tiwtter)

set(jansson_DIR "/home/maxim/vcpkg/vcpkg/installed/x64-linux/share/jansson")

add_executable(tiwtter main.cpp app/entities.cpp app/entities.h app/repository.cpp app/repository.h app/utils.cpp app/utils.h app/serializers.cpp app/serializers.h app/views.cpp app/views.h app/dto.cpp app/dto.h app/parser.cpp app/parser.h app/const.h app/const.cpp app/middleware.cpp app/middleware.h)

include_directories(/usr/include)
link_directories(/usr/lib)

find_package(jansson CONFIG REQUIRED)
target_link_libraries(tiwtter sqlite3 event jansson::jansson -lpq)