set(TIMA_SOURCE_FILES
        exception.hpp
        program.cpp
        program.hpp
        program_args.hpp
)

add_library(tima STATIC ${TIMA_SOURCE_FILES})

set(suffix "$<IF:$<CONFIG:Debug>,d,>")
target_link_libraries(tima 
        speed${suffix}
        -lasound
        -lm
)
