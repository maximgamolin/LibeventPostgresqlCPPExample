cmake_minimum_required(VERSION 3.15)
project(tiwtter)


#add_compile_options(-l sqlite3)
#add_compile_options(-levent)
add_executable(tiwtter main.cpp app/entities.cpp app/entities.h app/repository.cpp app/repository.h app/utils.cpp app/utils.h app/serializers.cpp app/serializers.h app/views.cpp app/views.h app/dto.cpp app/dto.h)

include_directories(/usr/include)
link_directories(/usr/lib)
target_link_libraries(tiwtter sqlite3)
target_link_libraries(tiwtter event)