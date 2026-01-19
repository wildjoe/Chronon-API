
add_library(ChrononAPI INTERFACE)

target_include_directories(ChrononAPI
  INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_sources(ChrononAPI
  INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChrononAPI/APICore.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChrononAPI/API_1_0.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ChrononAPI/iJsonAPI.h"
)

