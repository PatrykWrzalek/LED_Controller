idf_component_register(SRCS "system_monitor.c" "system_monitor_cli.c"
                    INCLUDE_DIRS "include"
                    REQUIRES debug_console app event_bus
)
