cmake_minimum_required(VERSION 3.21)

project(sfml_lr)

# set vcpkg target triplet, cmake generator platform, vcpkg_installed folder for windows
if (WIN32)
    set(HOST_OS "x64-windows")
endif (WIN32)

# set vcpkg_installed foldex for linux
if (UNIX)
    set(HOST_OS "x64-linux")
endif (UNIX)

# set source files
set(CLIENT_CPP_FILES src/client.cpp src/render.cpp src/character.cpp src/game.cpp)
set(CLIENT_H_FILES src/client.h src/render.h src/character.h src/game.h)
set(SERVER_CPP_FILES src/server.cpp)
set(SERVER_H_FILES src/server.h)
set(PROTO_FILES src/chain.grpc.pb.cc src/chain.grpc.pb.h src/chain.pb.cc src/chain.pb.h)

# targets
add_executable(client ${CLIENT_CPP_FILES} ${CLIENT_H_FILES} ${PROTO_FILES})
add_executable(server ${SERVER_CPP_FILES} ${SERVER_H_FILES} ${PROTO_FILES})

set(INSTALLED_PATH ${CMAKE_SOURCE_DIR}/vcpkg_installed/${HOST_OS})

# libs necessary for gRPC
set(c-ares_DIR ${INSTALLED_PATH}/share/c-ares)
set(absl_DIR ${INSTALLED_PATH}/share/absl)
set(upb_DIR ${INSTALLED_PATH}/share/upb)
set(re2_DIR ${INSTALLED_PATH}/share/re2)

# set zlib and pkgconf executable for windows
if (WIN32)
    set(ZLIB_INCLUDE_DIR ${INSTALLED_PATH}/lib/zlib.lib)
    set(ZLIB_LIBRARY ${INSTALLED_PATH}/lib/zlib.lib)
    set(PKG_CONFIG_EXECUTABLE ${INSTALLED_PATH}/tools/pkgconf/pkgconf.exe)
endif (WIN32)

# set zlib and pkgconf executable for unix
if (UNIX)
    set(ZLIB_INCLUDE_DIR ${INSTALLED_PATH}/lib/zlib.a)
    set(ZLIB_LIBRARY ${INSTALLED_PATH}/lib/libz.a)
    set(PKG_CONFIG_EXECUTABLE ${INSTALLED_PATH}/tools/pkgconf/pkgconf)
endif (UNIX)

# protobuf
find_package(Protobuf CONFIG REQUIRED HINTS ${INSTALLED_PATH}/share/protobuf)

# gRPC
find_package(gRPC CONFIG REQUIRED HINTS ${INSTALLED_PATH}/share/grpc)
target_link_libraries(server PRIVATE gRPC::grpc++)
target_link_libraries(client PRIVATE gRPC::grpc++)

# absl
find_package(absl CONFIG REQUIRED HINTS ${INSTALLED_PATH}/share/absl)
target_link_libraries(server PRIVATE absl::strings absl::flags absl::flags_parse absl::strings)
target_link_libraries(client PRIVATE absl::strings absl::flags absl::flags_parse absl::strings)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED HINTS ${INSTALLED_PATH}/share/sfml)
target_link_libraries(client PRIVATE sfml-window sfml-graphics)

# copy DLLs and PDBs if windows
if (WIN32)
    # set DLLs and PDBs for copy
    set(DLL_ROOT_PATH ${INSTALLED_PATH})
    set(DLL_PATH ${DLL_ROOT_PATH}/debug/bin)
    set(DLL_TO_COPY 
        ${DLL_PATH}/sfml-system-d-2.dll
        ${DLL_PATH}/sfml-window-d-2.dll
        ${DLL_PATH}/sfml-graphics-d-2.dll
        ${DLL_PATH}/freetyped.dll
        ${DLL_PATH}/zlibd1.dll
        ${DLL_PATH}/libpng16d.dll
        ${DLL_PATH}/brotlidec.dll
        ${DLL_PATH}/bz2d.dll
        ${DLL_PATH}/brotlicommon.dll
        ${DLL_PATH}/libprotobufd.dll
        ${DLL_PATH}/cares.dll
        ${DLL_PATH}/re2.dll
        ${DLL_PATH}/abseil_dll.dll
        ${DLL_ROOT_PATH}/bin/zlib1.dll)
    set(PDB_TO_COPY 
        ${DLL_PATH}/sfml-system-d.pdb
        ${DLL_PATH}/sfml-window-d.pdb
        ${DLL_PATH}/sfml-graphics-d.pdb
        ${DLL_PATH}/freetyped.pdb
        ${DLL_PATH}/zlibd.pdb
        ${DLL_PATH}/libpng16d.pdb
        ${DLL_PATH}/brotlidec.pdb
        ${DLL_PATH}/bz2d.pdb
        ${DLL_PATH}/brotlicommon.pdb
        ${DLL_PATH}/libprotobufd.pdb
        ${DLL_PATH}/cares.pdb
        ${DLL_PATH}/re2.pdb
        ${DLL_PATH}/abseil_dll.pdb)

    # copy DLLs and PDBs
    add_custom_command(
        TARGET client
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${DLL_TO_COPY} $<TARGET_FILE_DIR:client>
    )
    add_custom_command(
        TARGET client
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${PDB_TO_COPY} $<TARGET_FILE_DIR:client>
    )
endif (WIN32)

add_custom_command(
    TARGET client
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_directory ${CMAKE_SOURCE_DIR}/imgs $<TARGET_FILE_DIR:client>/imgs
)