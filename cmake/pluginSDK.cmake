include(FetchContent)
FetchContent_Declare(plugin-sdk
  GIT_REPOSITORY https://github.com/AthallahDzaki/plugin-sdk
  GIT_SHALLOW 2147483647)

set(PSDK_BUILD_SA ON)

message("Fetching plugin-sdk from GitHub...")
FetchContent_MakeAvailable(plugin-sdk)