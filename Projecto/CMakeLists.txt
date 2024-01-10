cmake_minimum_required(VERSION 3.6)

# Project Name
PROJECT(Engine)

set(CMAKE_CXX_STANDARD 11)  # Adicione esta linha para habilitar o C++11

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} 
    main.cpp 
    engineSrc/tinyXML/tinyxmlparser.cpp 
    engineSrc/tinyXML/tinyxml.cpp 
    engineSrc/tinyXML/tinystr.cpp 
    engineSrc/tinyXML/tinyxmlerror.cpp 
    engineSrc/engineSrcClass.cpp 
    engineSrc/engineSrcAUX.cpp
    /Users/joao/Documents/Universidade/MEI/1_Ano/2s/VTR/nau/contrib/glbinding-master/source/examples/comparison/glew.cpp
)
target_include_directories(Engine PUBLIC
    engineSrc/tinyXML/
    engineSrc/
    /Users/joao/Documents/Universidade/MEI/1_Ano/2s/VTR/nau/contrib/glbinding-master/source/
    /Users/joao/Documents/Universidade/MEI/1_Ano/2s/VTR/nau/contrib/glbinding-master/include/
)

# Adicione essas linhas para vincular as bibliotecas OpenGL e GLEW
find_package(OpenGL REQUIRED)
find_library(GLEW_LIBRARY GLEW)
if(NOT GLEW_LIBRARY)
    message(ERROR ": GLEW not found!")
endif(NOT GLEW_LIBRARY)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

add_executable(Generator 
    generatorSrc/main.cpp 
    generatorSrc/generator.cpp
)
target_include_directories(Generator PUBLIC
    generatorSrc/
    /Users/joao/Documents/Universidade/MEI/1_Ano/2s/VTR/nau/contrib/glbinding-master/source/
    /Users/joao/Documents/Universidade/MEI/1_Ano/2s/VTR/nau/contrib/glbinding-master/include/
)

# Adicione essas linhas para vincular as bibliotecas OpenGL e GLEW
if(NOT GLEW_LIBRARY)
    message(ERROR ": GLEW not found!")
endif(NOT GLEW_LIBRARY)
target_link_libraries(Generator ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})
