cmake_minimum_required(VERSION 3.15)
project( GaussianBlur )
set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( GaussianBlur DisplayImage.cpp TileImage.h GaussianBlurSelf.h GaussianBlurGen.h Graph/Pareto.h Graph/LFSR.h Graph/Number.h Graph/Operator.h Graph/Randomizer.h Graph/BtoD.h Graph/DeRandomizer.h Graph/Transformer.h Graph/VoidTransformer.h Graph/Graph.h Graph/Adder.h Graph/Multiplier.h Graph/Table.h Graph/TrueRandomizer.h Graph/RandomizerSobol.h Graph/RNGSobol.h Graph/RNGSobol.cpp)
target_link_libraries( GaussianBlur ${OpenCV_LIBS} )
