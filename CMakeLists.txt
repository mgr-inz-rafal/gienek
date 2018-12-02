cmake_minimum_required(VERSION 3.10)

project (Gienek)

if (WIN32)
	add_definitions(-D_WIN32_WINNT=0x0601 -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
endif (WIN32)

set(BOOST_INCLUDE				$ENV{BOOST_INCLUDE_PATH})
set(BOOST_LIBS					$ENV{BOOST_LIB_PATH})
set(ALLEGRO_INCLUDE				$ENV{ALLEGRO_INCLUDE_PATH})
set(ALLEGRO_ADDONS_INCLUDE		$ENV{ALLEGRO_INCLUDE_PATH}/../addons)
set(ALLEGRO_PLATFORM_INCLUDE	$ENV{ALLEGRO_PLATFORM_INCLUDE_PATH})
set(ALLEGRO_LIBS				$ENV{ALLEGRO_LIB_PATH})

find_package(Threads)

include_directories(
	${BOOST_INCLUDE}
	${ALLEGRO_INCLUDE}
	${ALLEGRO_ADDONS_INCLUDE}/font
	${ALLEGRO_ADDONS_INCLUDE}/image
	${ALLEGRO_ADDONS_INCLUDE}/primitives
	${ALLEGRO_PLATFORM_INCLUDE}
	include/
)

if (WIN32)
link_libraries(
	${ALLEGRO_LIBS}/allegro.lib
	${ALLEGRO_LIBS}/allegro_primitives.lib
	${ALLEGRO_LIBS}/allegro_font.lib
	${ALLEGRO_LIBS}/allegro_image.lib
)
endif (WIN32)

if (UNIX)
link_libraries(
	${ALLEGRO_LIBS}/liballegro.so
	${ALLEGRO_LIBS}/liballegro_primitives.so
	${ALLEGRO_LIBS}/liballegro_font.so
	${ALLEGRO_LIBS}/liballegro_image.so
	${BOOST_LIBS}/libboost_system.so
)
endif (UNIX)
 
link_directories(${BOOST_LIBS})

message( ${BOOST_LIBS} )

source_group("Header Files" FILES 
	include/decoder.hpp
	include/cmd_types.hpp
	include/handler_result.hpp
	include/socket_reader.hpp
	include/painter.hpp
	include/mouse.hpp
	include/point.hpp
	include/display_config.hpp
	include/scaler.hpp
	include/toolbox.hpp
	include/user_interactions.hpp
	include/event_loop.hpp
	include/keyboard.hpp
	include/player.hpp
	include/actor_actions.hpp
	include/actor.hpp
	include/player_states.hpp
	include/path.hpp
	include/treenode.hpp
	include/doom_controller.hpp
	)
	
source_group("Header Files - Doom Map" FILES 
	include/vertex.hpp
	include/subsector.hpp
	include/seg.hpp
	include/seg_triangle.hpp
	include/thing.hpp
	include/doommap.hpp
	)

source_group("Header Files - Handlers" FILES 
	include/handler.hpp
	include/handler_vertex.hpp
	include/handler_unknown.hpp
	include/handler_quit.hpp
	include/handler_subsector.hpp
	include/handler_clear.hpp
	include/handler_map_received.hpp
	include/handler_thing.hpp 
	include/handler_thing_update.hpp
	include/handler_thing_remove.hpp
	include/handler_player_angle.hpp
	)
	
add_executable(gienek
	src/main.cc
	src/decoder.cc
	src/handler.cc
	src/handler_vertex.cc
	src/handler_unknown.cc
	src/handler_quit.cc
	src/handler_subsector.cc
	src/socket_reader.cc
	src/doommap.cc
	src/handler_clear.cc
	src/handler_map_received.cc
	src/painter.cc 
	src/mouse.cc
	src/point.cc
	src/display_config.cc
	src/scaler.cc
	src/toolbox.cc
	src/user_interactions.cc
	src/handler_thing.cc
	src/handler_thing_update.cc
	src/handler_thing_remove.cc 
	src/handler_player_angle.cc
	src/event_loop.cc
	src/keyboard.cc
	src/player.cc
	src/actor.cc
	src/path.cc
	src/treenode.cc
	src/subsector.cc
	src/seg.cc
	src/seg_triangle.cc
	src/thing.cc
	src/doom_controller.cc
	include/doom_controller.hpp
	include/thing.hpp
	include/vertex.hpp
	include/vertex.hpp
	include/seg_triangle.hpp
	include/seg.hpp
	include/subsector.hpp
	include/treenode.hpp
	include/path.hpp
	include/player_states.hpp
	include/player.hpp
	include/keyboard.hpp
	include/decoder.hpp
	include/handler.hpp
	include/handler_vertex.hpp
	include/handler_subsector.hpp
	include/cmd_types.hpp
	include/handler_unknown.hpp
	include/handler_quit.hpp
	include/handler_result.hpp
	include/socket_reader.hpp
	include/doommap.hpp
	include/handler_clear.hpp
	include/handler_map_received.hpp
	include/painter.hpp
	include/mouse.hpp
	include/point.hpp
	include/display_config.hpp
	include/scaler.hpp
	include/toolbox.hpp
	include/user_interactions.hpp
	include/handler_thing.hpp 
	include/handler_thing_update.hpp
	include/handler_thing_remove.hpp
	include/handler_player_angle.hpp
	include/event_loop.hpp
	include/actor_actions.hpp
	include/actor.hpp
	)

target_link_libraries(gienek ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET gienek PROPERTY CXX_STANDARD 17)
set_property(TARGET gienek PROPERTY CXX_STANDARD_REQUIRED ON)

