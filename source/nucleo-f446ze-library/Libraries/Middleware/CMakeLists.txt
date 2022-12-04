set(MIDDLEWARE_SOURCES_FILES
   "Src/led.c"
)

add_library(${MIDDLEWARE_LIBRARY_TARGET} STATIC
   ${MIDDLEWARE_SOURCES_FILES}
)

target_include_directories( ${MIDDLEWARE_LIBRARY_TARGET} PUBLIC
   "../../Libraries/"
)