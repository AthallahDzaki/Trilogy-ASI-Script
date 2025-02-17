# #####################################################################
# ########################## GTA San Andreas ##########################
set(CHAOS_SA ${PROJECT_NAME}.SA)

file(GLOB_RECURSE SA_SOURCES CONFIGURE_DEPENDS src/TrilogyChaosMod.cpp src/shared/*.cpp src/gtasa/*.cpp)

add_library(${CHAOS_SA} SHARED ${SA_SOURCES})

set_target_properties(${CHAOS_SA} PROPERTIES SUFFIX ".asi")

target_include_directories(${CHAOS_SA} PUBLIC "src/shared/" "src/gtasa" "${dxsdk_SOURCE_DIR}/Include" "lib/curl")

target_link_directories(${CHAOS_SA} PUBLIC
    lib/
    "${dxsdk_SOURCE_DIR}/Lib/x86"
)
target_link_libraries(${CHAOS_SA} PUBLIC plugin_sa nlohmann_json uWebSockets minhook efsw bass libcurl Dbghelp)

target_compile_definitions(${CHAOS_SA} PUBLIC NOMINMAX)

# Include global definitions like colors
target_precompile_headers(
    ${CHAOS_SA}
    PUBLIC
    src/shared/_include/Definitions.h
    src/shared/_include/cpptoml.h
    src/shared/_include/ini.h
    lib/bass.h
)

# Include specific libraries for easier use
target_precompile_headers(
    ${CHAOS_SA}
    PRIVATE
    <plugin.h>
    <nlohmann/json.hpp>
    <App.h>
    <rwplcore.h>
)

# Add Zi debug information when using MSVC
if(MSVC)
    target_compile_options(${CHAOS_SA} PRIVATE /Zi)
    set_target_properties(${CHAOS_SA} PROPERTIES COMPILE_PDB_NAME ${CHAOS_SA})
	target_link_options(${CHAOS_SA} PRIVATE /DEBUG /PDB:${CMAKE_BINARY_DIR}/${CHAOS_SA}.pdb)
endif()
