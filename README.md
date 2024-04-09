# Simle game on SFML and gRPC
This is study project with a lot of bugs (check src/client.cpp:58 for example)

# How to run
This is a windows example. Project is x64.
```
# Install vcpkg libraries
vcpkg install

# Generate proto files
.\vcpkg_installed\x64-windows\tools\protobuf\protoc.exe --proto_path=proto --cpp_out=.\src\ --grpc_out=.\src\ --plugin=protoc-gen-grpc=.\vcpkg_installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe .\proto\chain.proto

# Create build folder
mkdir build

# Generate build files
cmake -S . -B .\build\

# Build the project
cmake --build .\build\

# Run the project (here are default flag values, delay - time in ms to ~spam~ do requests (share your "status") to server)
.\build\Debug\server.exe --port=50051
.\build\Debug\client.exe --host=localhost:50051 --name=user --delay=1000
```