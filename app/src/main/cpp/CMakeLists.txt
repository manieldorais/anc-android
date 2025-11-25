cmake_minimum_required(VERSION 3.22.1)

# Nome da sua biblioteca principal (deve coincidir com o loadLibrary no Java)
project("ancapp")

# 1. Encontra a biblioteca Oboe instalada via Prefab
find_package(oboe REQUIRED CONFIG)

# Cria sua biblioteca compartilhada
add_library(${CMAKE_PROJECT_NAME} SHARED
        # Lista seus arquivos fonte C++ aqui
        native-lib.cpp
        AudioEngine.cpp
        # ^^^ IMPORTANTE: Adicione o AudioEngine.cpp aqui para ele ser compilado
)

# Procura bibliotecas do sistema (log, android, etc.)
find_library(log-lib log)

# 2. Liga (Link) o Oboe e logs à sua biblioteca
target_link_libraries(${CMAKE_PROJECT_NAME}
        oboe::oboe # <--- Liga o Oboe aqui
        ${log-lib}
)