#ifndef BASIC_ICS_FONTS_H
#define BASIC_ICS_FONTS_H

#include <lvgl.h>

namespace DBuddy {
    class Fonts {
    public:
        Fonts(Fonts &other) = delete;
        void operator=(const Fonts &) = delete;

        static Fonts * get_instance();

        lv_style_t * style_default_font_black_medium{};
        lv_style_t * style_default_font_black_small{};
        lv_style_t * style_default_font_black_xxlarge{};
        lv_style_t * style_default_font_regular_small{};
        lv_style_t * style_default_font_regular_xsmall{};
    private:
        Fonts();
    };
}
#endif // BASIC_ICS_FONTS_H