message(STATUS "Setting up proto " "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.cpp")

# there is a #include 'npctransport.ph.h' in the cpp file
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/include/IMP/npctransport/internal/npctransport.pb.h"
                          "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.h"
                          "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.cpp"
                          COMMAND protoc "--cpp_out=dllexport_decl=IMPNPCTRANSPORTEXPORT:."
                          "-I${CMAKE_CURRENT_SOURCE_DIR}/data/"
                          "${CMAKE_CURRENT_SOURCE_DIR}/data/npctransport.proto"
			  COMMAND ${CMAKE_COMMAND} -E rename npctransport.pb.cc npctransport.pb.cpp
			  COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/patch_protoc.py"
			                 "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.h"
					 "${CMAKE_BINARY_DIR}/include/IMP/npctransport/internal/npctransport.pb.h"
                          DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/npctransport.proto"
                          COMMENT "Creating protoc stuff for npctransport"
                          WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/npctransport")

add_custom_target(IMP.npctransport-proto ALL DEPENDS
                          "${CMAKE_BINARY_DIR}/include/IMP/npctransport/internal/npctransport.pb.h"
                          "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.h"
                          "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.cpp")
set_property(TARGET IMP.npctransport-proto PROPERTY FOLDER "IMP.npctransport")

list(APPEND IMP_npctransport_LIBRARY_EXTRA_SOURCES "${CMAKE_BINARY_DIR}/src/npctransport/npctransport.pb.cpp")

set(IMP_npctransport_LIBRARY_EXTRA_DEPENDENCIES "IMP.npctransport-proto" CACHE INTERNAL "" FORCE)


# there is a #include 'npctransport.ph.h' in the cpp file
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/lib/IMP/npctransport/npctransport_pb2.py"
                          COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/IMP/npctransport/"
                          COMMAND protoc --python_out=${CMAKE_BINARY_DIR}/lib/IMP/npctransport/
                          "-I${CMAKE_CURRENT_SOURCE_DIR}/data/"
                          "${CMAKE_CURRENT_SOURCE_DIR}/data/npctransport.proto"
                          # add config header to resolve export symbols
                          DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/npctransport.proto"
                          COMMENT "Creating python protoc stuff for npctransport"
                          WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/npctransport")

add_custom_target(IMP.npctransport-python_proto ALL DEPENDS "${CMAKE_BINARY_DIR}/lib/IMP/npctransport/npctransport_pb2.py" )
set_property(TARGET IMP.npctransport-python_proto PROPERTY FOLDER "IMP.npctransport")
install(FILES "${CMAKE_BINARY_DIR}/lib/IMP/npctransport/npctransport_pb2.py" DESTINATION "${CMAKE_INSTALL_PYTHONDIR}/IMP/npctransport/")

include_directories(${CMAKE_BINARY_DIR}/include/IMP/npctransport/internal)

set(IMP_npctransport_PYTHON_EXTRA_DEPENDENCIES IMP.npctransport-python_proto CACHE INTERNAL "" FORCE)
