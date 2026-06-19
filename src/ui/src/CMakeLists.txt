SET(SOURCES ui_Screen1.c
    ui_Screen2.c
    ui_Screen3.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_fl_logo_png.c
    ui_img_settings_png.c
    fonts/ui_font_newfont.c
    fonts/ui_font_typeWriter18.c
    fonts/ui_font_typeWriter22.c
    fonts/ui_font_typeWriter22Bold.c
    fonts/ui_font_typeWriter26Bold.c
    fonts/ui_font_ramen.c
    fonts/ui_font_roboto16.c
    fonts/ui_font_roboto18.c
    fonts/ui_font_roboto28Bold.c
    fonts/ui_font_roboto32Regular.c
    fonts/ui_font_roboto32Thin.c
    fonts/ui_font_roboto34Thin.c
    fonts/ui_font_roboto38Regular.c
    fonts/ui_font_roboto40Thin.c)

add_library(ui ${SOURCES})
