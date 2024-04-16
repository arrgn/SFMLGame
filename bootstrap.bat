vcpkg install
.\vcpkg_installed\x64-windows\tools\protobuf\protoc.exe --proto_path=proto --cpp_out=.\src\ --grpc_out=.\src\ --plugin=protoc-gen-grpc=.\vcpkg_installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe .\proto\chain.proto
mkdir build
cmake -S . -B .\build\ %*
