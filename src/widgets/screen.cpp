#include "widgets/screen.h"
#include "styles.h"

using namespace dbuddy::Widgets;

Screen::Screen(lv_obj_t * parent, lv_obj_t * copy) : Widget(parent, copy) {
    self = lv_scr_act();
}

void Screen::init(Fonts * fonts, Styles * styles) {
    add_style(LV_OBJ_PART_MAIN, &styles->style_default_background_color_blue);
    add_style(LV_OBJ_PART_MAIN, &styles->style_default_background_transparent_cover);
}
