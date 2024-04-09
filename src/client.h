#pragma once

#include <iostream>
#include <thread>
#include <condition_variable>
#include <chrono>

#include "SFML/Graphics.hpp"

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include "grpcpp/grpcpp.h"


#include "render.h"
#include "game.h"

#include "chain.grpc.pb.h"