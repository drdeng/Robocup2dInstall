
add_library(model STATIC
  action_sequence_description.cpp
  action_sequence_log_parser.cpp
  csv_logger.cpp
  debug_client_parser.cpp
  debug_log_data.cpp
  debug_log_holder.cpp
  debug_view_data.cpp
  draw_data_holder.cpp
  field_evaluator_message_parser.cpp
  game_event_holder.cpp
  game_event_log_parser.cpp
  grid_field_evaluation_data.cpp
  main_data.cpp
  monitor_view_data.cpp
  options.cpp
  trainer_data.cpp
  view_holder.cpp
  )


target_include_directories(model PUBLIC
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  ${LIBRCSC_INCLUDE_DIR}
  PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  )
