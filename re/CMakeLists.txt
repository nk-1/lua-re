if (APPLE)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined
		suppress -flat_namespace -g")
endif(APPLE)

# Add C library
add_library(re SHARED re.c libre.c)
target_link_libraries(re pcre2-8)
set_target_properties(re PROPERTIES PREFIX "" OUTPUT_NAME "re")
set_target_properties(re PROPERTIES COMPILE_FLAGS "-g -O0")

# Install module
install(TARGETS re LIBRARY DESTINATION ${TARANTOOL_INSTALL_LIBDIR}/${PROJECT_NAME}/)
