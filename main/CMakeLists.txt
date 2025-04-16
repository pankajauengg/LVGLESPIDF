
idf_component_register(SRCS "main.cpp"
                            "ui/ui.c"
                            "ui/ui_Screen1.c"
                    INCLUDE_DIRS "." "ui" "../components"
                    REQUIRES tft lvgl arduino esp_https_server
                    EMBED_TXTFILES web/index.html)
                    
