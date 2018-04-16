include(ExternalProject)

set(INCDIR ${CMAKE_SOURCE_DIR}/include/PG)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/PG)

set(SRC_PHYS

  ${INCDIR}/Physics/Body.hpp
  ${SRCDIR}/Physics/Body.cpp
  
  ${INCDIR}/Physics/CollisionInfo.hpp

  ${SRCDIR}/Physics/Detail/CollisionAlgorithms.hpp
  ${SRCDIR}/Physics/Detail/CollisionAlgorithms.cpp
  ${SRCDIR}/Physics/Detail/Integrator.hpp

  ${INCDIR}/Physics/Shape.hpp
  ${SRCDIR}/Physics/Shape.cpp

  ${INCDIR}/Physics/World.hpp
  ${SRCDIR}/Physics/World.cpp
)
source_group(Physics FILES ${SRC_PHYS})

set(SRC_PHYS_SHAPES

  ${INCDIR}/Physics/Shapes/Circle.hpp
  ${SRCDIR}/Physics/Shapes/Circle.cpp

  ${INCDIR}/Physics/Shapes/LineSegment.hpp
  ${SRCDIR}/Physics/Shapes/LineSegment.cpp

  ${INCDIR}/Physics/Shapes/Rectangle.hpp
  ${SRCDIR}/Physics/Shapes/Rectangle.cpp
)
source_group("Physics\\Shapes" FILES ${SRC_PHYS_SHAPES})
list(APPEND SRC_PHYS ${SRC_PHYS_SHAPES})

set(SRC_GAME

  ${INCDIR}/Game/Engine.hpp
  ${SRCDIR}/Game/Engine.cpp

  ${INCDIR}/Game/Entity.hpp
  ${SRCDIR}/Game/Entity.cpp

  ${INCDIR}/Game/Randomizer.hpp
  ${INCDIR}/Game/Randomizer.inl
  ${SRCDIR}/Game/Randomizer.cpp

  ${INCDIR}/Game/Scene.hpp
  ${SRCDIR}/Game/Scene.cpp
)
source_group(Game FILES ${SRC_GAME})

set(SRC_GAME_SCENES

  ${INCDIR}/Game/Scenes/Game.hpp
  ${SRCDIR}/Game/Scenes/Game.cpp
)
source_group("Game\\Scenes" FILES ${SRC_GAME_SCENES})
list(APPEND SRC_GAME ${SRC_GAME_SCENES})

set(SRC_GAME_ENTITIES

  ${INCDIR}/Game/Entities/Debris.hpp
  ${SRCDIR}/Game/Entities/Debris.cpp

  ${INCDIR}/Game/Entities/Player.hpp
  ${SRCDIR}/Game/Entities/Player.cpp

  ${INCDIR}/Game/Entities/Shot.hpp
  ${SRCDIR}/Game/Entities/Shot.cpp
)
source_group("Game\\Entities" FILES ${SRC_GAME_ENTITIES})
list(APPEND SRC_GAME ${SRC_GAME_ENTITIES})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(PG ${SRC_PHYS} ${SRC_GAME} ${SRCDIR}/main.cpp)

# Math library
externalproject_add(GPM
  GIT_REPOSITORY https://github.com/gene-pool/math.git
  GIT_TAG master

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""

  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)
externalproject_get_property(GPM SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

# SFML
externalproject_add(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 9da895d

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""

  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DBUILD_SHARED_LIBS=OFF
             -DCMAKE_INSTALL_PREFIX=
)
externalproject_get_property(SFML BINARY_DIR)
externalproject_get_property(SFML SOURCE_DIR)

include_directories(${SOURCE_DIR}/include)
add_definitions(-DSFML_STATIC)
add_dependencies(PG GPM SFML)

set(CMK_SLP ${CMAKE_STATIC_LIBRARY_PREFIX})
set(CMK_SLS ${CMAKE_STATIC_LIBRARY_SUFFIX})

set(SFML_BIN_DIR ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR})
target_link_libraries(PG

  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-audio-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-graphics-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-network-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-window-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-system-s-d${CMK_SLS}
  debug ${SFML_BIN_DIR}/${CMK_SLP}sfml-main-d${CMK_SLS}

  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-audio-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-graphics-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-network-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-window-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-system-s${CMK_SLS}
  optimized ${SFML_BIN_DIR}/${CMK_SLP}sfml-main${CMK_SLS}
)

if (MSVC)

  set(SFML_EXT_DIR ${SOURCE_DIR}/extlibs/libs-msvc-universal/x86)

  target_link_libraries(PG
    # SFML dependencies
    ${SFML_EXT_DIR}/flac.lib
    ${SFML_EXT_DIR}/freetype.lib
    ${SFML_EXT_DIR}/ogg.lib
    ${SFML_EXT_DIR}/openal32.lib
    ${SFML_EXT_DIR}/vorbis.lib
    ${SFML_EXT_DIR}/vorbisenc.lib
    ${SFML_EXT_DIR}/vorbisfile.lib

    opengl32.lib
    winmm.lib
    gdi32.lib
    ws2_32.lib
    Wldap32.lib
  )

endif()
