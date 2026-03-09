idf_component_register(SRCS "system_monitor.c" "system_monitor_cli.c"
                    INCLUDE_DIRS "include"
                    REQUIRES app event_bus debug_console
)
