#include <stdio.h>
#include <time.h>
#include "ui.h"
#include "styles.h"
#include "fonts.h"

void create_display(ui_t *);

void create_obj_left_top_container_day(ui_t *);
void create_obj_left_top_container_month(ui_t *);
void create_obj_left_top_container_year(ui_t *);
void create_obj_top_container_actions(ui_t *);
void create_obj_top_container_time(ui_t *);

void create_page_left_bottom_itinerary(ui_t *);
void create_page_left_bottom_main(ui_t *);
void create_page_left_main(ui_t *);
void create_page_left_top_container(ui_t *);
void create_page_left_top_main(ui_t *);
void create_page_main(ui_t *);
void create_page_right_calendar(ui_t *);
void create_page_right_main(ui_t *);
void create_page_settings(ui_t *);
void create_page_top_container(ui_t *);
void create_page_top_main(ui_t *);

void create_screen(ui_t *);

lv_coord_t get_next_row_pos(lv_obj_t * obj, int padding);

void cb_action_settings_event_handler(lv_obj_t * obj, lv_event_t event);
void cb_calendar_event_handler(lv_obj_t * obj, lv_event_t event);
void cb_list_btn_general(lv_obj_t * obj, lv_event_t event);
void cb_list_btn_calendars(lv_obj_t * obj, lv_event_t event);
void cb_settings_win_close(lv_obj_t * obj, lv_event_t event);
void cb_settings_btnmatrix(lv_obj_t * obj, lv_event_t event);
void cb_settings_win_msgbox(lv_obj_t * obj, lv_event_t event);
void cb_time_task_handler(lv_task_t * task);
void cb_toggle_switch_event_handler(lv_obj_t * obj, lv_event_t event);

void update_toggle_buttons(ui_t *);
void update_calendar(ui_t ui);
void update_time(ui_t ui);

static ui_t * p_Ui;

void ui_init() {
    static bool ui_initialized = false;

    if (ui_initialized) {
        LV_LOG_WARN("ui_init: already inited");
        return;
    }

    p_Ui = malloc(sizeof(ui_t));

    styles_init();
    font_init();

    create_display(p_Ui);
    create_screen(p_Ui);
    create_page_main(p_Ui);
    create_page_top_main(p_Ui);
    create_page_right_main(p_Ui);
    create_page_top_container(p_Ui);
    create_obj_top_container_time(p_Ui);
    create_obj_top_container_actions(p_Ui);
    create_page_left_main(p_Ui);
    create_page_left_top_main(p_Ui);
    create_page_left_top_container(p_Ui);
    create_obj_left_top_container_month(p_Ui);
    create_obj_left_top_container_day(p_Ui);
    create_obj_left_top_container_year(p_Ui);
    create_page_left_bottom_main(p_Ui);
    create_page_left_bottom_itinerary(p_Ui);
    create_page_right_calendar(p_Ui);

    ui_initialized = true;
    LV_LOG_INFO("ui_init ready");
}

void cb_action_settings_event_handler(lv_obj_t * obj, lv_event_t event) {
    if (lv_debug_check_obj_valid(obj) && event == LV_EVENT_PRESSED) {
        // Copy the global configuration into the local version.
        p_Ui->settings.config = *p_config;

        create_page_settings(p_Ui);

        if (p_Ui->settings.main) {
            lv_win_ext_t * win_ext = lv_obj_get_ext_attr(p_Ui->settings.main);
            p_Ui->settings.header_offset = lv_obj_get_height_fit(win_ext->header);

            p_Ui->settings.list = lv_list_create(p_Ui->settings.main, NULL);
            lv_obj_set_size(p_Ui->settings.list, DEFAULT_LIST_WIDTH, lv_obj_get_height_fit(p_Ui->settings.main) - p_Ui->settings.header_offset);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_default_border_none);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_default_background_transparent_full);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_default_border_sides_only_right);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_default_border_color_white);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_default_border_thin);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_default_radius_none);
            lv_obj_add_style(p_Ui->settings.list, LV_LIST_PART_BG, &style_focused_border_color_white);

            lv_obj_t * btn_general = lv_list_add_btn(p_Ui->settings.list, NULL, "General");
            lv_obj_add_style(btn_general, LV_BTN_PART_MAIN, &style_default_border_none);
            lv_obj_add_style(btn_general, LV_BTN_PART_MAIN, &style_default_background_transparent_full);
            lv_obj_set_style_local_outline_width(btn_general, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, 0);
            lv_obj_set_style_local_border_side(btn_general, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, LV_BORDER_SIDE_LEFT);
            lv_obj_set_style_local_border_color(btn_general, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, LV_COLOR_WHITE);
            lv_obj_set_style_local_border_width(btn_general, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, DEFAULT_BORDER * 2);
            lv_obj_set_style_local_pad_ver(btn_general, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, DEFAULT_PADDING * 2);
            lv_obj_set_event_cb(btn_general, cb_list_btn_general);

            lv_obj_t * btn_calendars = lv_list_add_btn(p_Ui->settings.list, NULL, "Calendars");
            lv_obj_add_style(btn_calendars, LV_BTN_PART_MAIN, &style_default_border_none);
            lv_obj_add_style(btn_calendars, LV_BTN_PART_MAIN, &style_default_background_transparent_full);
            lv_obj_set_style_local_outline_width(btn_calendars, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, 0);
            lv_obj_set_style_local_border_side(btn_calendars, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, LV_BORDER_SIDE_LEFT);
            lv_obj_set_style_local_border_color(btn_calendars, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, LV_COLOR_WHITE);
            lv_obj_set_style_local_border_width(btn_calendars, LV_BTN_PART_MAIN, LV_STATE_FOCUSED, DEFAULT_BORDER * 2);
            lv_obj_set_style_local_pad_ver(btn_calendars, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, DEFAULT_PADDING * 2);
            lv_obj_set_event_cb(btn_calendars, cb_list_btn_calendars);

            lv_obj_t * actions_bar = lv_obj_create(p_Ui->settings.main, NULL);
            lv_obj_set_width(actions_bar, lv_obj_get_width(p_Ui->settings.main) - lv_obj_get_width(p_Ui->settings.list));
            lv_obj_align(actions_bar, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
            lv_obj_add_style(actions_bar, LV_OBJ_PART_MAIN, &style_default_border_none);
            lv_obj_add_style(actions_bar, LV_OBJ_PART_MAIN, &style_default_background_transparent_full);

            static const char * btns[] = {"Cancel", "Save", ""};
            p_Ui->settings.action_btns = lv_btnmatrix_create(actions_bar, NULL);
            lv_obj_set_height(p_Ui->settings.action_btns, lv_obj_get_height(actions_bar));
            lv_obj_align(p_Ui->settings.action_btns, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
            lv_obj_add_style(p_Ui->settings.action_btns, LV_BTNMATRIX_PART_BG, &style_default_border_none);
            lv_obj_add_style(p_Ui->settings.action_btns, LV_BTNMATRIX_PART_BG, &style_default_background_transparent_full);
            lv_obj_add_style(p_Ui->settings.action_btns, LV_BTNMATRIX_PART_BTN, &style_default_border_color_white);
            lv_obj_add_style(p_Ui->settings.action_btns, LV_BTNMATRIX_PART_BTN, &style_default_background_color_black);
            lv_obj_add_style(p_Ui->settings.action_btns, LV_BTNMATRIX_PART_BTN, &style_pressed_background_color_white);
            lv_obj_add_style(p_Ui->settings.action_btns, LV_BTNMATRIX_PART_BTN, &style_pressed_text_color_black);

            lv_btnmatrix_set_map(p_Ui->settings.action_btns, btns);
            lv_obj_set_event_cb(p_Ui->settings.action_btns, cb_settings_btnmatrix);

            p_Ui->settings.header_offset = p_Ui->settings.header_offset + lv_obj_get_height(actions_bar);

            lv_list_focus_btn(p_Ui->settings.list, btn_general);
            lv_event_send(btn_general, LV_EVENT_PRESSED, NULL);
        }
    }
}

void cb_calendar_event_handler(lv_obj_t * obj, lv_event_t event) {
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t * date = lv_calendar_get_pressed_date(obj);

        if (date) {
            p_Ui->date.month = date->month;
            p_Ui->date.day = date->day;
            p_Ui->date.year = date->year;

            update_calendar(*p_Ui);
        }
    }
}

void cb_list_btn_general(lv_obj_t * obj, lv_event_t event) {
    if (lv_debug_check_obj_valid(obj) && event == LV_EVENT_PRESSED) {
        if (p_Ui->settings.calendars.main && lv_debug_check_obj_valid(p_Ui->settings.calendars.main)) {
            lv_obj_del(p_Ui->settings.calendars.main);
            p_Ui->settings.calendars.main = NULL;
        }

        p_Ui->settings.general.main = lv_page_create(p_Ui->settings.main, NULL);
        lv_obj_set_size(p_Ui->settings.general.main, lv_obj_get_width_fit(p_Ui->settings.main) - DEFAULT_LIST_WIDTH, lv_obj_get_height_fit(p_Ui->settings.main) - p_Ui->settings.header_offset);
        lv_obj_set_pos(p_Ui->settings.general.main, DEFAULT_LIST_WIDTH, 0);
        lv_obj_add_style(p_Ui->settings.general.main, LV_STATE_DEFAULT, &style_default_border_none);
        lv_obj_add_style(p_Ui->settings.general.main, LV_STATE_DEFAULT, &style_default_background_transparent_full);
        lv_obj_add_style(p_Ui->settings.general.main, LV_STATE_DEFAULT, &style_default_padding_default);

        lv_obj_t * heading_time = lv_obj_create(p_Ui->settings.general.main, NULL);
        lv_obj_set_width(heading_time, lv_obj_get_width_fit(p_Ui->settings.general.main));
        lv_obj_add_style(heading_time, LV_OBJ_PART_MAIN, &style_default_border_sides_only_bottom);
        lv_obj_add_style(heading_time, LV_OBJ_PART_MAIN, &style_default_border_thin);
        lv_obj_add_style(heading_time, LV_OBJ_PART_MAIN, &style_default_radius_none);
        lv_obj_add_style(heading_time, LV_OBJ_PART_MAIN, &style_default_background_transparent_full);

        lv_obj_t * heading_label = lv_label_create(heading_time, NULL);
        lv_label_set_text(heading_label, "Time");

        lv_obj_set_height(heading_time, lv_obj_get_height(heading_label));

        lv_obj_t * row_format = lv_label_create(p_Ui->settings.general.main, NULL);
        lv_label_set_long_mode(row_format, LV_LABEL_LONG_BREAK);
        lv_obj_set_width(row_format, lv_obj_get_width_fit(p_Ui->settings.general.main));
        lv_label_set_text(row_format, "24-hour Formatting (e.g., 16:00)");
        lv_obj_set_pos(row_format, 0, get_next_row_pos(heading_time, DEFAULT_PADDING));

        p_Ui->settings.general.toggle_format = lv_switch_create(p_Ui->settings.general.main, NULL);
        lv_obj_set_event_cb(p_Ui->settings.general.toggle_format, cb_toggle_switch_event_handler);
        lv_obj_align(p_Ui->settings.general.toggle_format, row_format, LV_ALIGN_IN_RIGHT_MID, 0, 0);

        lv_obj_t * row_meridiem = lv_label_create(p_Ui->settings.general.main, NULL);
        lv_label_set_long_mode(row_meridiem, LV_LABEL_LONG_BREAK);
        lv_obj_set_width(row_meridiem, lv_obj_get_width_fit(p_Ui->settings.general.main));
        lv_label_set_text(row_meridiem, "Use Meridiem (e.g., am/pm)");
        lv_obj_set_pos(row_meridiem, 0, get_next_row_pos(row_format, DEFAULT_PADDING + DEFAULT_PADDING));

        p_Ui->settings.general.toggle_meridiem = lv_switch_create(p_Ui->settings.general.main, NULL);
        lv_obj_set_event_cb(p_Ui->settings.general.toggle_meridiem, cb_toggle_switch_event_handler);
        lv_obj_align(p_Ui->settings.general.toggle_meridiem, row_meridiem, LV_ALIGN_IN_RIGHT_MID, 0, 0);

        lv_obj_t * row_flash_colon = lv_label_create(p_Ui->settings.general.main, NULL);
        lv_label_set_long_mode(row_flash_colon, LV_LABEL_LONG_BREAK);
        lv_obj_set_width(row_flash_colon, lv_obj_get_width_fit(p_Ui->settings.general.main));
        lv_label_set_text(row_flash_colon, "Flash the Time Separators");
        lv_obj_set_pos(row_flash_colon, 0, get_next_row_pos(row_meridiem, DEFAULT_PADDING + DEFAULT_PADDING));

        p_Ui->settings.general.toggle_flash = lv_switch_create(p_Ui->settings.general.main, NULL);
        lv_obj_set_event_cb(p_Ui->settings.general.toggle_flash, cb_toggle_switch_event_handler);
        lv_obj_align(p_Ui->settings.general.toggle_flash, row_flash_colon, LV_ALIGN_IN_RIGHT_MID, 0, 0);

        lv_obj_t * row_screensaver = lv_label_create(p_Ui->settings.general.main, NULL);
        lv_label_set_long_mode(row_screensaver, LV_LABEL_LONG_BREAK);
        lv_obj_set_width(row_screensaver, lv_obj_get_width_fit(p_Ui->settings.general.main));
        lv_label_set_text(row_screensaver, "Time as Screensaver");
        lv_obj_set_pos(row_screensaver, 0, get_next_row_pos(row_flash_colon, DEFAULT_PADDING + DEFAULT_PADDING));

        p_Ui->settings.general.toggle_screensaver = lv_switch_create(p_Ui->settings.general.main, NULL);
        lv_obj_set_event_cb(p_Ui->settings.general.toggle_screensaver, cb_toggle_switch_event_handler);
        lv_obj_align(p_Ui->settings.general.toggle_screensaver, row_screensaver, LV_ALIGN_IN_RIGHT_MID, 0, 0);

        update_toggle_buttons(p_Ui);
    }
}

void cb_list_btn_calendars(lv_obj_t * obj, lv_event_t event) {
    if (lv_debug_check_obj_valid(obj) && event == LV_EVENT_PRESSED) {
        if (p_Ui->settings.general.main && lv_debug_check_obj_valid(p_Ui->settings.general.main)) {
            lv_obj_del(p_Ui->settings.general.main);
            p_Ui->settings.general.main = NULL;
        }

        p_Ui->settings.calendars.main = lv_page_create(p_Ui->settings.main, NULL);
        lv_obj_set_size(p_Ui->settings.calendars.main, lv_obj_get_width_fit(p_Ui->settings.main) - DEFAULT_LIST_WIDTH, lv_obj_get_height_fit(p_Ui->settings.main) - p_Ui->settings.header_offset);
        lv_obj_set_pos(p_Ui->settings.calendars.main, DEFAULT_LIST_WIDTH, 0);
        lv_obj_add_style(p_Ui->settings.calendars.main, LV_STATE_DEFAULT, &style_default_border_none);
        lv_obj_add_style(p_Ui->settings.calendars.main, LV_STATE_DEFAULT, &style_default_background_transparent_full);
    }
}

void cb_settings_win_close(lv_obj_t * obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        if (compareTo(p_Ui->settings.config, *p_config) != 0) {
            static const char * btns[] ={"Continue", "Go Back", ""};
            p_Ui->settings.msgbox = lv_msgbox_create(p_Ui->settings.main, NULL);
            lv_obj_add_style(p_Ui->settings.msgbox, LV_OBJ_PART_MAIN, &style_default_background_color_black);
            lv_obj_add_style(p_Ui->settings.msgbox, LV_OBJ_PART_MAIN, &style_default_border_color_white);
            lv_obj_add_style(p_Ui->settings.msgbox, LV_OBJ_PART_MAIN, &style_default_shadow_none);
            lv_obj_align(p_Ui->settings.msgbox, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);

            lv_msgbox_add_btns(p_Ui->settings.msgbox, btns);
            lv_msgbox_set_text(p_Ui->settings.msgbox, "Unsaved changes will be lost.\n\nDo you wish to continue?");
            lv_msgbox_set_anim_time(p_Ui->settings.msgbox, 0);

            lv_obj_set_event_cb(p_Ui->settings.msgbox, cb_settings_win_msgbox);

            return;
        }

        if (p_Ui->settings.general.main && lv_debug_check_obj_valid(p_Ui->settings.general.main)) {
            lv_obj_del(p_Ui->settings.general.main);
        }

        if (p_Ui->settings.calendars.main && lv_debug_check_obj_valid(p_Ui->settings.calendars.main)) {
            lv_obj_del(p_Ui->settings.calendars.main);
        }

        if (p_Ui->settings.overlay && lv_debug_check_obj_valid(p_Ui->settings.overlay)) {
            lv_obj_del(p_Ui->settings.overlay);
        }

        p_Ui->settings.general.main = NULL;
        p_Ui->settings.calendars.main = NULL;
        p_Ui->settings.overlay = NULL;
    }

    lv_win_close_event_cb(obj, event);
}

void cb_settings_btnmatrix(lv_obj_t * obj, lv_event_t event) {
    if (event == LV_EVENT_VALUE_CHANGED) {
        const char * text = lv_btnmatrix_get_active_btn_text(obj);

        if (strcasecmp(text, "Cancel") == 0) {
            cb_settings_win_close(p_Ui->settings.btn_close, LV_EVENT_RELEASED);
        }
    }
}

void cb_settings_win_msgbox(lv_obj_t * obj, lv_event_t event) {
    if (event == LV_EVENT_VALUE_CHANGED) {
        const char * text = lv_msgbox_get_active_btn_text(obj);

        if (strcasecmp(text, "Continue") == 0) {
            // Reset the local config and close the settings
            p_Ui->settings.config = *p_config;
            cb_settings_win_close(p_Ui->settings.btn_close, LV_EVENT_RELEASED);
            return;
        }

        lv_msgbox_start_auto_close(p_Ui->settings.msgbox, 0);
    }
}

void cb_time_task_handler(lv_task_t * task) {
    ui_t ui = *(ui_t *) (task->user_data);
    update_time(ui);
}

void cb_toggle_switch_event_handler(lv_obj_t * obj, lv_event_t event) {
    if (event == LV_EVENT_VALUE_CHANGED) {
        bool state = lv_switch_get_state(obj);

        if (obj == p_Ui->settings.general.toggle_flash) {
            p_Ui->settings.config.time_flash = state ? 1 : 0;
        }

        if (obj == p_Ui->settings.general.toggle_format) {
            p_Ui->settings.config.time_format24 = state ? 1 : 0;
        }

        if (obj == p_Ui->settings.general.toggle_meridiem) {
            p_Ui->settings.config.time_meridiem = state ? 1 : 0;
        }

        if (obj == p_Ui->settings.general.toggle_screensaver) {
            p_Ui->settings.config.time_screensaver = state ? 1 : 0;
        }

        update_toggle_buttons(p_Ui);
    }
}

void create_display(ui_t * ui) {
    ui->display = lv_disp_get_default();
}

void create_obj_left_top_container_day(ui_t * ui) {
    ui->page.left.top.container_day = lv_obj_create(ui->page.left.top.container, ui->page.main);
    lv_obj_set_size(ui->page.left.top.container_day, lv_obj_get_width_fit(ui->page.left.top.container) / 3, lv_obj_get_height_fit(ui->page.left.top.container));
    lv_obj_set_pos(ui->page.left.top.container_day, lv_obj_get_width_fit(ui->page.left.top.container_month), 0);

    ui->page.left.top.label_day = lv_label_create(ui->page.left.top.container_day, NULL);
    lv_label_set_text(ui->page.left.top.label_day, "");
    lv_obj_set_auto_realign(ui->page.left.top.label_day, true);
    lv_obj_align(ui->page.left.top.label_day, ui->page.left.top.container_day, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(ui->page.left.top.label_day, LV_LABEL_PART_MAIN, &style_default_font_black_xxlarge);
}

void create_obj_left_top_container_month(ui_t * ui) {
    ui->page.left.top.container_month = lv_obj_create(ui->page.left.top.container, ui->page.main);
    lv_obj_set_size(ui->page.left.top.container_month, lv_obj_get_width_fit(ui->page.left.top.container) / 3, lv_obj_get_height_fit(ui->page.left.top.container));

    ui->page.left.top.label_month = lv_label_create(ui->page.left.top.container_month, NULL);
    lv_label_set_text(ui->page.left.top.label_month, "");
    lv_obj_set_auto_realign(ui->page.left.top.label_month, true);
    lv_obj_align(ui->page.left.top.label_month, ui->page.left.top.container_month, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(ui->page.left.top.label_month, LV_LABEL_PART_MAIN, &style_default_font_black_medium);
}

void create_obj_left_top_container_year(ui_t * ui) {
    ui->page.left.top.container_year = lv_obj_create(ui->page.left.top.container, ui->page.main);
    lv_obj_set_size(ui->page.left.top.container_year, lv_obj_get_width_fit(ui->page.left.top.container) / 3, lv_obj_get_height_fit(ui->page.left.top.container));
    lv_obj_set_pos(ui->page.left.top.container_year, lv_obj_get_width_fit(ui->page.left.top.container_month) + lv_obj_get_width_fit(ui->page.left.top.container_day), 0);

    ui->page.left.top.label_year = lv_label_create(ui->page.left.top.container_year, NULL);
    lv_label_set_text(ui->page.left.top.label_year, "");
    lv_obj_set_auto_realign(ui->page.left.top.label_year, true);
    lv_label_set_long_mode(ui->page.left.top.label_year, LV_LABEL_LONG_BREAK);
    lv_obj_set_size(ui->page.left.top.label_year, lv_obj_get_width_fit(ui->page.left.top.container_year), lv_obj_get_height(ui->page.left.top.container_year));
    lv_label_set_align(ui->page.left.top.label_year, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(ui->page.left.top.label_year, ui->page.left.top.container_year, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(ui->page.left.top.label_year, LV_LABEL_PART_MAIN, &style_default_font_black_medium);
}

void create_obj_top_container_actions(ui_t * ui) {
    ui->page.top.container_actions = lv_obj_create(ui->page.top.container, ui->page.top.main);

    lv_obj_set_size(ui->page.top.container_actions, lv_obj_get_width_fit(ui->page.top.container) / 2, lv_obj_get_height_fit(ui->page.top.main));
    lv_obj_set_pos(ui->page.top.container_actions, lv_obj_get_width(ui->page.top.container_time), 0);

    ui->page.top.btn_action_settings = lv_btn_create(ui->page.top.container_actions, NULL);
    lv_obj_set_size(ui->page.top.btn_action_settings, lv_obj_get_width_fit(ui->page.top.container_actions) / 12, lv_obj_get_height_fit(ui->page.top.container_actions));
    lv_obj_align(ui->page.top.btn_action_settings, ui->page.top.container_actions, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_obj_reset_style_list(ui->page.top.btn_action_settings, LV_BTN_PART_MAIN);

    lv_obj_t * label = lv_label_create(ui->page.top.btn_action_settings, NULL);
    lv_label_set_text(label, LV_SYMBOL_SETTINGS);
    lv_obj_set_auto_realign(label, true);
    lv_obj_align(label, ui->page.top.container_actions, LV_ALIGN_IN_RIGHT_MID, -DEFAULT_PADDING / 2, 0);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &style_default_font_regular_small);

    lv_obj_set_style_local_text_color(ui->page.top.btn_action_settings, LV_LABEL_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_AQUA);
    lv_obj_set_event_cb(ui->page.top.btn_action_settings, cb_action_settings_event_handler);
}

void create_obj_top_container_time(ui_t * ui) {
    ui->page.top.container_time = lv_obj_create(ui->page.top.container, ui->page.top.main);
    lv_obj_set_size(ui->page.top.container_time, lv_obj_get_width_fit(ui->page.top.main) / 2, lv_obj_get_height_fit(ui->page.top.main));

    ui->page.top.label_time = lv_label_create(ui->page.top.container_time, NULL);
    lv_label_set_text(ui->page.top.label_time, "");
    lv_obj_set_auto_realign(ui->page.top.label_time, true);
    lv_obj_align(ui->page.top.label_time, ui->page.top.container_time, LV_ALIGN_IN_LEFT_MID, DEFAULT_PADDING / 2, 0);
    lv_obj_add_style(ui->page.top.label_time, LV_LABEL_PART_MAIN, &style_default_font_regular_small);

    lv_task_create(cb_time_task_handler, 500, LV_TASK_PRIO_MID, ui);
}

void create_page_left_bottom_itinerary(ui_t * ui) {
    ui->page.left.bottom.itinerary = lv_obj_create(ui->page.left.bottom.main, ui->page.main);

    lv_obj_set_size(ui->page.left.bottom.itinerary, lv_page_get_width_fit(ui->page.left.bottom.main), lv_page_get_height_fit(ui->page.left.bottom.main));
    lv_obj_add_style(ui->page.left.bottom.itinerary, LV_PAGE_PART_BG, &style_default_background_overlay_color_black);
}

void create_page_left_bottom_main(ui_t * ui) {
    ui->page.left.bottom.main = lv_page_create(ui->page.left.main, ui->page.main);

    lv_obj_set_pos(ui->page.left.bottom.main, 0, lv_obj_get_height(ui->page.left.top.main));
    lv_obj_set_size(ui->page.left.bottom.main, lv_obj_get_width(ui->page.left.main), lv_obj_get_height(ui->page.left.main) - lv_obj_get_height(ui->page.left.top.main));
    lv_obj_set_style_local_pad_top(ui->page.left.bottom.main, LV_PAGE_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(ui->page.left.bottom.main, LV_PAGE_PART_BG, &style_default_padding_default);
}

void create_page_left_main(ui_t * ui) {
    ui->page.left.main = lv_obj_create(ui->page.main, ui->page.main);

    lv_obj_set_size(ui->page.left.main, (LV_HOR_RES * 1 / 3), LV_VER_RES - lv_obj_get_height(ui->page.top.main));
    lv_obj_align(ui->page.left.main, ui->page.main, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
}

void create_page_left_top_container(ui_t * ui) {
    ui->page.left.top.container = lv_obj_create(ui->page.left.top.main, ui->page.main);

    lv_obj_set_size(ui->page.left.top.container, lv_page_get_width_fit(ui->page.left.top.main), lv_page_get_height_fit(ui->page.left.top.main));
    lv_obj_add_style(ui->page.left.top.container, LV_PAGE_PART_BG, &style_default_background_overlay_color_black);
}

void create_page_left_top_main(ui_t * ui) {
    ui->page.left.top.main = lv_page_create(ui->page.left.main, ui->page.main);

    lv_obj_set_size(ui->page.left.top.main, lv_obj_get_width(ui->page.left.main), (lv_obj_get_width(ui->page.left.main) * 2 / 3) - DEFAULT_PADDING * 2);
    lv_obj_add_style(ui->page.left.top.main, LV_PAGE_PART_BG, &style_default_padding_default);
}

void create_page_main(ui_t * ui) {
    ui->page.main = lv_page_create(ui->screen, NULL);

    lv_obj_set_size(ui->page.main, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_style(ui->page.main, LV_PAGE_PART_BG, &style_default_border_none);
    lv_obj_add_style(ui->page.main, LV_PAGE_PART_BG, &style_default_padding_none);
    lv_obj_add_style(ui->page.main, LV_PAGE_PART_BG, &style_default_background_transparent_full);
}

void create_page_right_calendar(ui_t * ui) {
    ui->page.right.container = lv_obj_create(ui->page.right.main, ui->page.main);
    lv_obj_set_size(ui->page.right.container, lv_obj_get_width_fit(ui->page.right.main), lv_obj_get_height_fit(ui->page.right.main));
    lv_obj_add_style(ui->page.right.container, LV_PAGE_PART_BG, &style_default_padding_default);
    lv_obj_add_style(ui->page.right.container, LV_PAGE_PART_BG, &style_default_padding_left_none);

    ui->page.right.calendar = lv_calendar_create(ui->page.right.container, NULL);
    lv_obj_set_size(ui->page.right.calendar, lv_obj_get_width_fit(ui->page.right.container), lv_obj_get_height_fit(ui->page.right.container));
    lv_obj_align(ui->page.right.calendar, ui->page.right.main, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_BG, &style_default_background_overlay_color_black);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_BG, &style_default_border_none);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_DATE, &style_focused_background_blend_mode_additive);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_DATE, &style_focused_background_color_purple);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_DATE, &style_focused_text_color_black);

    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_HEADER, &style_default_font_black_medium);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_DAY_NAMES, &style_default_font_black_small);
    lv_obj_add_style(ui->page.right.calendar, LV_CALENDAR_PART_BG, &style_default_font_regular_xsmall);

    lv_obj_set_event_cb(ui->page.right.calendar, cb_calendar_event_handler);

    update_calendar(*ui);
}

void create_page_right_main(ui_t * ui) {
    ui->page.right.main = lv_obj_create(ui->page.main, ui->page.main);

    lv_obj_set_size(ui->page.right.main, (LV_HOR_RES * 2 / 3), LV_VER_RES - lv_obj_get_height(ui->page.top.main));
    lv_obj_align(ui->page.right.main, ui->page.main, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
}

void create_page_settings(ui_t * ui) {
    if (lv_debug_check_obj_valid(ui->settings.main)) {
        return;
    }

    ui->settings.main = lv_win_create(ui->screen, NULL);
    lv_obj_set_size(ui->settings.main, lv_page_get_width_fit(ui->page.main) - DEFAULT_MENU_HEIGHT - (DEFAULT_PADDING * 2), lv_page_get_height_fit(ui->page.main) - DEFAULT_MENU_HEIGHT - (DEFAULT_PADDING * 2));
    lv_obj_align(ui->settings.main, ui->screen, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_background_color_black);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_border_color_white);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_border_sides_only_bottom);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_border_thin);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_font_regular_small);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_padding_top_none);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_padding_right_none);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_HEADER, &style_default_padding_bottom_none);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_BG, &style_default_background_color_black);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_BG, &style_default_radius_normal);
    lv_obj_add_style(ui->settings.main, LV_WIN_PART_CONTENT_SCROLLABLE, &style_default_padding_none);

    lv_win_set_title(ui->settings.main, "Settings");

    ui->settings.btn_close = lv_win_add_btn(ui->settings.main, LV_SYMBOL_CLOSE);
    lv_obj_set_event_cb(ui->settings.btn_close, cb_settings_win_close);

    // Overlay to simulate a modal dialog
    ui->settings.overlay = lv_obj_create(ui->page.main, NULL);
    lv_obj_set_size(ui->settings.overlay, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(ui->settings.overlay, 0, 0);
    lv_obj_add_style(ui->settings.overlay, LV_OBJ_PART_MAIN, &style_default_background_overlay_color_black);
    lv_obj_add_style(ui->settings.overlay, LV_OBJ_PART_MAIN, &style_default_border_none);
}

void create_page_top_container(ui_t * ui) {
    ui->page.top.container = lv_obj_create(ui->page.top.main, ui->page.main);

    lv_obj_set_size(ui->page.top.container, lv_obj_get_width_fit(ui->page.top.main), lv_obj_get_height_fit(ui->page.top.main));
}

void create_page_top_main(ui_t * ui) {
    ui->page.top.main = lv_obj_create(ui->page.main, ui->page.main);

    lv_obj_set_size(ui->page.top.main, LV_HOR_RES, DEFAULT_MENU_HEIGHT);
    lv_obj_add_style(ui->page.top.main, LV_PAGE_PART_BG, &style_default_background_overlay_color_black);
    lv_obj_add_style(ui->page.top.main, LV_PAGE_PART_BG, &style_default_radius_none);
}

void create_screen(ui_t * ui) {
    ui->screen = lv_scr_act();

    lv_obj_add_style(ui->screen, LV_OBJMASK_PART_MAIN, &style_default_background_transparent_full);
}

lv_coord_t get_next_row_pos(lv_obj_t * obj, int padding) {
    return lv_obj_get_y(obj) + lv_obj_get_height(obj) + padding;
}

void update_toggle_buttons(ui_t * ui) {
    if (ui->settings.config.time_flash == 1) {
        lv_switch_on(ui->settings.general.toggle_flash, false);
    } else {
        lv_switch_off(ui->settings.general.toggle_flash, false);
    }

    if (ui->settings.config.time_format24 == 1) {
        lv_switch_on(ui->settings.general.toggle_format, false);
        lv_obj_set_click(ui->settings.general.toggle_meridiem, false);
        lv_obj_add_style(ui->settings.general.toggle_meridiem, LV_SWITCH_PART_BG, &style_default_background_transparent_30);
        lv_obj_add_style(ui->settings.general.toggle_meridiem, LV_SWITCH_PART_INDIC, &style_default_background_transparent_30);
        lv_obj_add_style(ui->settings.general.toggle_meridiem, LV_SWITCH_PART_KNOB, &style_default_background_transparent_30);
    } else {
        lv_switch_off(ui->settings.general.toggle_format, false);
        lv_obj_set_click(ui->settings.general.toggle_meridiem, true);
        lv_obj_remove_style(ui->settings.general.toggle_meridiem, LV_SWITCH_PART_BG, &style_default_background_transparent_30);
        lv_obj_remove_style(ui->settings.general.toggle_meridiem, LV_SWITCH_PART_INDIC, &style_default_background_transparent_30);
        lv_obj_remove_style(ui->settings.general.toggle_meridiem, LV_SWITCH_PART_KNOB, &style_default_background_transparent_30);
    }

    if (ui->settings.config.time_meridiem == 1) {
        lv_switch_on(ui->settings.general.toggle_meridiem, false);
    } else {
        lv_switch_off(ui->settings.general.toggle_meridiem, false);
    }

    if (ui->settings.config.time_screensaver == 1) {
        lv_switch_on(ui->settings.general.toggle_screensaver, false);
    } else {
        lv_switch_off(ui->settings.general.toggle_screensaver, false);
    }
}

void update_calendar(ui_t ui) {
    lv_calendar_date_t date = ui.date;
    time_t t = time(NULL);
    struct tm local = *localtime(&t);

    if (date.year == 0 && date.month == 0 && date.day == 0) {
        date.year = local.tm_year + 1900;
        date.day = local.tm_mday;
        date.month = local.tm_mon + 1;
    }

    lv_calendar_set_today_date(ui.page.right.calendar, &date);
    lv_calendar_set_showed_date(ui.page.right.calendar, &date);

    char bufDay[4], bufMonth[4], bufYear[3], bufCentury[3], selected[11];

    sprintf(selected, "%d-%d-%d", date.year, date.month, date.day);
    strptime(selected, "%Y-%m-%d", &local);

    strftime(bufDay, sizeof(bufDay), "%a", &local);
    strftime(bufMonth, sizeof(bufMonth), "%b", &local);
    strftime(bufYear, sizeof(bufYear), "%y", &local);
    strftime(bufCentury, sizeof(bufCentury), "%C", &local);

    lv_label_set_text_fmt(ui.page.left.top.label_month, "%s\n%s", bufDay, bufMonth);
    lv_label_set_text_fmt(ui.page.left.top.label_day, "%02u", local.tm_mday);
    lv_label_set_text_fmt(ui.page.left.top.label_year, "%s\n%s", bufCentury, bufYear);
}

void update_time(ui_t ui) {
    time_t t = time(NULL);
    struct tm local = *localtime(&t);
    char bufMeridiem[3] = "";
    int tmHour = local.tm_hour;

    if (p_config->time_format24 == 0) {
        if (p_config->time_meridiem == 1) {
            if (local.tm_hour >= 12) {
                sprintf(bufMeridiem, "%s", "pm");
            } else {
                sprintf(bufMeridiem, "%s", "am");
            }
        }

        if (tmHour > 12) {
            tmHour = tmHour - 12;
        }
    }

    static int count_on = 0, count_off = 0;
    char * colon = strstr(lv_label_get_text(ui.page.top.label_time), ":");

    if (((count_on % 2) == 0 && colon == NULL) || (p_config->time_flash == 0)) {
        lv_label_set_text_fmt(ui.page.top.label_time, "%02u:%02u %s", tmHour, local.tm_min, bufMeridiem);

        count_on = 0;
    } else if ((count_off % 2) == 0) {
        lv_label_set_text_fmt(ui.page.top.label_time, "%02u %02u %s", tmHour, local.tm_min, bufMeridiem);

        count_off = 0;
    }

    count_off++;
    count_on++;
}
