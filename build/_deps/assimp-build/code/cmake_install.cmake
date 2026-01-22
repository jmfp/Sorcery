# Install script for directory: /Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.3.0" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-build/bin/libassimp.5.3.0.dylib"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-build/bin/libassimp.5.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.5.3.0.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.5.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" -x "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.3.0" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-build/bin/libassimp.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/anim.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/aabb.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ai_assert.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/camera.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/color4.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/color4.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-build/code/../include/assimp/config.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ColladaMetaData.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/commonMetaData.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/defs.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/cfileio.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/light.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/material.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/material.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/matrix3x3.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/matrix3x3.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/matrix4x4.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/matrix4x4.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/mesh.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ObjMaterial.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/pbrmaterial.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/GltfMaterial.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/postprocess.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/quaternion.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/quaternion.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/scene.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/metadata.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/texture.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/types.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/vector2.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/vector2.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/vector3.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/vector3.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/version.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/cimport.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/AssertHandler.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/importerdesc.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Importer.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/DefaultLogger.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ProgressHandler.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/IOStream.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/IOSystem.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Logger.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/LogStream.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/NullLogger.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/cexport.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Exporter.hpp"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/DefaultIOStream.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/DefaultIOSystem.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ZipArchiveIOSystem.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SceneCombiner.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/fast_atof.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/qnan.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/BaseImporter.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Hash.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/MemoryIOWrapper.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ParsingUtils.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/StreamReader.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/StreamWriter.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/StringComparison.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/StringUtils.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SGSpatialSort.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/GenericProperty.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SpatialSort.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SkeletonMeshBuilder.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SmallVector.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SmoothingGroups.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/SmoothingGroups.inl"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/StandardShapes.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/RemoveComments.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Subdivision.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Vertex.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/LineSplitter.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/TinyFormatter.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Profiler.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/LogAux.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Bitmap.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/XMLTools.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/IOStreamBuffer.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/CreateAnimMesh.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/XmlParser.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/BlobIOSystem.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/MathFunctions.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Exceptional.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/ByteSwapper.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Compiler/pushpack1.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Compiler/poppack1.h"
    "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-src/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/jesseprice/Development/Personal/Britemune/OpenGL/build/_deps/assimp-build/code/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
