# Find all .c and .cpp files in the component directory
file(GLOB_RECURSE SRC_FILES "*.c")

# Find all header files (.h) in this directory and subdirectories
file(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

# Collect unique directories that contain header files
set(INCLUDE_DIRS_LIST ${CMAKE_CURRENT_SOURCE_DIR} ${HEADER_FILES})
list(REMOVE_DUPLICATES INCLUDE_DIRS_LIST)

# Register the ESP-IDF component with source files and include directories
idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS "."
    REQUIRES spi_flash app event_bus drv_led debug_console system_monitor
)
