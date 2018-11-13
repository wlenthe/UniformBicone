if(MINGW)
	add_definitions(-D_hypot=hypot) # if cmath is included on mingw there is a name conflict with python
endif()

# find python executable
find_package(PythonInterp 3 REQUIRED)

# find python include directory
execute_process(COMMAND ${PYTHON_EXECUTABLE} "-c" "from distutils import sysconfig;print(sysconfig.get_python_inc(),end='')" OUTPUT_VARIABLE Python_Output ERROR_VARIABLE Python_Error)
STRING(REGEX REPLACE "\\\\" "/" Python_Output ${Python_Output}) # convert windows backslash to forward slash
if(NOT "${Python_Error}" STREQUAL "")
	message(FATAL_ERROR "failed to find python include directory: ${Python_Error}")
endif()
set(PYTHON_INCLUDE_DIR ${Python_Output} CACHE PATH "python include directories")
include_directories(${PYTHON_INCLUDE_DIR})

# find python library
if(WIN32) # windows
	execute_process(COMMAND ${PYTHON_EXECUTABLE} "-c" "from distutils import sysconfig;print(sysconfig.get_config_var(\"BINDIR\"),end='')" OUTPUT_VARIABLE Python_Output ERROR_VARIABLE Python_Error)
	STRING(REGEX REPLACE "\\\\" "/" Python_Output ${Python_Output})
	if(NOT "${Python_Error}" STREQUAL "")
		message(FATAL_ERROR "failed to find python include directory: ${Python_Error}")
	endif()
	set(PYTHON_LIBRARY_DIR ${Python_Output}/libs CACHE PATH "python library directories")

	if(MSVC)
		find_library(PYTHON_LIBRARY    python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR} HINT ${PYTHON_LIBRARY_DIR} CACHE FILE)
	elseif(MINGW)
		find_library(PYTHON_LIBRARY libpython${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR} HINT ${PYTHON_LIBRARY_DIR} CACHE FILE)
	endif()
else() # *nix
	execute_process(COMMAND ${PYTHON_EXECUTABLE} "-c" "from distutils import sysconfig;print(sysconfig.get_config_var(\"LIBDIR\"),end='')" OUTPUT_VARIABLE Python_Output ERROR_VARIABLE Python_Error)
	if(NOT "${Python_Error}" STREQUAL "")
		message(FATAL_ERROR "failed to find python include directory: ${Python_Error}")
	endif()
	set(PYTHON_LIBRARY_DIR ${Python_Output} CACHE PATH "python library directories")
	find_library(PYTHON_LIBRARY libpython${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}m.dylib HINTS ${PYTHON_LIBRARY_DIR} CACHE FILE)
endif()

# find numpy
execute_process(COMMAND ${PYTHON_EXECUTABLE} "-c" "import numpy;print(numpy.get_include(),end='')" OUTPUT_VARIABLE Python_Output ERROR_VARIABLE Python_Error)
STRING(REGEX REPLACE "\\\\" "/" Python_Output ${Python_Output})
if(NOT "${Python_Error}" STREQUAL "")
	message(FATAL_ERROR "failed to find numpy include directory: ${Python_Error}")
endif()
find_path(NUMPY_INCLUDE_DIR numpy/arrayobject.h HINTS ${Python_Output})
get_filename_component(NUMPY_DIR ${NUMPY_INCLUDE_DIR} DIRECTORY)
include_directories(${NUMPY_INCLUDE_DIR})
find_library(NUMPY_LIBRARY NAMES npymath HINTS ${NUMPY_DIR}/lib)
get_filename_component(NUMPY_LIBRARY_DIR ${NUMPY_LIBRARY} DIRECTORY CACHE)

# add python extension
add_library(colormap MODULE colormap_module.cpp)
target_link_libraries(colormap ${PYTHON_LIBRARY} ${NUMPY_LIBRARY})
set_property(TARGET colormap PROPERTY CXX_STANDARD 11)
set_property(TARGET colormap PROPERTY PREFIX "") # name colormap instead of libcolormap
if(WIN32)
	set_property(TARGET colormap PROPERTY SUFFIX ".pyd") # name colormap.pyd instead of colormap.dll
else()
	set_property(TARGET colormap PROPERTY SUFFIX ".so") # name colormap.so
endif()

# add install rule to put python module in python path
execute_process(COMMAND ${PYTHON_EXECUTABLE} "-c" "from distutils import sysconfig;print(sysconfig.get_python_lib(),end='')" OUTPUT_VARIABLE Python_Output ERROR_VARIABLE Python_Error)
if(NOT "${Python_Error}" STREQUAL "")
	message(FATAL_ERROR "failed to find python include directory: ${Python_Error}")
endif()
set(PYTHON_MODULE_DIR ${Python_Output} CACHE PATH "python module install directory")
install(TARGETS colormap LIBRARY DESTINATION ${PYTHON_MODULE_DIR})
