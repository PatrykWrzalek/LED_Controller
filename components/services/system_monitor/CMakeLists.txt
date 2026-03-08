idf_component_register(SRCS "system_monitor.c"
                    INCLUDE_DIRS "include"
                    REQUIRES debug_console                    
)

set(COMPONENT_KCONFIGS "Kconfig")
