#include "widgets/timecontainer.h"
#include "styles.h"

using namespace dbuddy::Widgets;

TimeContainer::TimeContainer(lv_obj_t * parent, lv_obj_t * copy) : Widget(parent, copy) {
    self = lv_obj_create(parent, copy);
}

void TimeContainer::init(Fonts * fonts, Styles * styles) {
    set_size(lv_obj_get_width(get_parent()) / 2, lv_obj_get_height(get_parent()));

    add_style(LV_OBJ_PART_MAIN, &styles->style_default_border_none);
    add_style(LV_OBJ_PART_MAIN, &styles->style_default_radius_none);
    add_style(LV_OBJ_PART_MAIN, &styles->style_default_background_transparent_full);
}
