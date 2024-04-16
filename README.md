# Simple game on SFML and gRPC
This is study project with a lot of bugs (check src/client.cpp:58 for example)

# How to run
This is a windows example. The project is x64 and uses vcpkg (be sure you have it added to the PATH).
```
# Run bootstrap script
.\bootstrap.bat %args%

# Build the project
cmake --build .\build\

# Run the project (here are default flag values, delay - time in ms to ~spam~ do requests (share your "status") to server)
.\build\Debug\server.exe --port=50051
.\build\Debug\client.exe --host=localhost:50051 --name=user --delay=1000
```
args - arguments to pass to cmake generator (nothing by default but you can specify them if you want)

# Plans for the future
I recently found out that gRPC works via TCP that is slow for games and I should use something UDP-based instead. So, I am going to learn something new and replace gRPC with that.
