
add_executable(soccerwindow2
  action_sequence_option_window.cpp
  action_sequence_selector.cpp
  action_sequence_tree_view.cpp
  ball_painter.cpp
  ball_painter_rcss.cpp
  ball_trace_painter.cpp
  color_setting_dialog.cpp
  debug_log_dir_dialog.cpp
  debug_log_painter.cpp
  debug_message_window.cpp
  debug_painter.cpp
  debug_server.cpp
  detail_dialog.cpp
  dir_selector.cpp
  draw_config.cpp
  evaluation_window.cpp
  evaluator_control_panel.cpp
  field_canvas.cpp
  field_evaluation_painter.cpp
  field_painter.cpp
  font_setting_dialog.cpp
  image_save_dialog.cpp
  launcher_dialog.cpp
  log_player.cpp
  log_player_tool_bar.cpp
  main.cpp
  main_window.cpp
  monitor_client.cpp
  monitor_move_dialog.cpp
  offside_line_painter.cpp
  player_control_painter.cpp
  player_painter.cpp
  player_painter_rcss.cpp
  player_trace_painter.cpp
  player_type_dialog.cpp
  score_board_painter.cpp
  score_board_painter_rcss.cpp
  team_graphic_painter.cpp
  view_config_dialog.cpp
  voronoi_diagram_painter.cpp
)

target_include_directories(soccerwindow2
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  ${LIBRCSC_INCLUDE_DIR}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src/model
  ${PROJECT_BINARY_DIR}
  )

target_link_libraries(soccerwindow2
  PUBLIC
  ${LIBRCSC_LIB}
  ${QT_LIBS}
  ZLIB::ZLIB
  Boost::system
  PRIVATE
  model
  )

set_target_properties(soccerwindow2
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
  )

install(TARGETS soccerwindow2
  RUNTIME
  DESTINATION bin
  )
