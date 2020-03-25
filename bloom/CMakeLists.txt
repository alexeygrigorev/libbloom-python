include_directories(include)

file(GLOB SOURCE
    "include/*.h"
    "src/*.cpp"
)

add_library(bloom SHARED ${SOURCE})
target_include_directories(bloom PUBLIC include)