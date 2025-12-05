/* ui_menu.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <ui_menu.h>

static const menu_t ui_menu_menu_1 = {
  .m_up = &ui_menu_root,
  .m_label = "Log",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "View log?",
                    .sa_notify = ui_view_log } },
  }
};

static const menu_t ui_menu_menu_2 = {
  .m_up = &ui_menu_root,
  .m_label = "Auto-adjust",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "Readjust?",
                    .sa_notify = ui_readjust } },
  }
};

static const menu_t ui_menu_menu_4 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "Load",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "Load from disk?",
                    .sa_notify = ui_load_settings } },
  }
};

static const menu_t ui_menu_menu_5 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "Threshold",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = SEL_INIT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 16,
                 .si_high = 30,
                 .si_notify = ui_alarm_threshold_notify,
                 .si_formatter = 0,
                 .si_current = &settings.st_alarm_threshold } },
  }
};

static const menu_t ui_menu_menu_6 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "Speed",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = SEL_INIT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 20,
                 .si_high = 30,
                 .si_notify = ui_alarm_decay_notify,
                 .si_formatter = 0,
                 .si_current = &settings.st_alarm_decay } },
  }
};

static const char * const ui_menu_labels_8[] = {
  "No",
  "Yes",
};

static const menu_t ui_menu_menu_7 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "Enabled",
  .m_selection = {
    .s_type = SEL_CHOICE,
    .s_flags = SEL_INIT | SEL_DEFAULT1,
    .s_content = {
      .s_choice = { .sc_count = 2,
                    .sc_current = &settings.st_alarm_enabled,
                    .sc_labels = ui_menu_labels_8,
                    .sc_notify = ui_alarm_enabled_notify } },
  }
};

static const menu_t ui_menu_menu_9 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "EAS detect",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = SEL_INIT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 2,
                 .si_high = 16,
                 .si_notify = 0,
                 .si_formatter = ui_eas_threshold_formatter,
                 .si_current = &settings.st_eas_threshold } },
  }
};

static const menu_t ui_menu_menu_10 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "Indicator",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = SEL_INIT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 5,
                 .si_high = 10,
                 .si_notify = 0,
                 .si_formatter = 0,
                 .si_current = &settings.st_indicator_threshold } },
  }
};

static const menu_t ui_menu_menu_11 = {
  .m_up = &ui_menu_menu_3,
  .m_label = "Save",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "Save to disk?",
                    .sa_notify = ui_save_settings } },
  }
};

static const menu_t * const ui_menu_array_12[] = {
  &ui_menu_menu_4,
  &ui_menu_menu_5,
  &ui_menu_menu_6,
  &ui_menu_menu_7,
  &ui_menu_menu_9,
  &ui_menu_menu_10,
  &ui_menu_menu_11,
};

static const menu_t ui_menu_menu_3 = {
  .m_up = &ui_menu_root,
  .m_label = "Settings",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 7,
                  .sm_entries = ui_menu_array_12 } },
  }
};

static const menu_t ui_menu_menu_14 = {
  .m_up = &ui_menu_menu_13,
  .m_label = "Log",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "Erase log?",
                    .sa_notify = ui_erase_log } },
  }
};

static const menu_t ui_menu_menu_15 = {
  .m_up = &ui_menu_menu_13,
  .m_label = "Counter",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "Reset?",
                    .sa_notify = ui_reset_counter } },
  }
};

static const menu_t * const ui_menu_array_16[] = {
  &ui_menu_menu_14,
  &ui_menu_menu_15,
};

static const menu_t ui_menu_menu_13 = {
  .m_up = &ui_menu_root,
  .m_label = "Reset",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 2,
                  .sm_entries = ui_menu_array_16 } },
  }
};

int ui_date_year;
static const menu_t ui_menu_menu_18 = {
  .m_up = &ui_menu_menu_17,
  .m_label = "Year",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = 0,
    .s_content = {
      .s_int = { .si_low = 2007,
                 .si_default = 2007,
                 .si_high = 2030,
                 .si_notify = 0,
                 .si_formatter = 0,
                 .si_current = &ui_date_year } },
  }
};

int ui_date_month;
static const menu_t ui_menu_menu_19 = {
  .m_up = &ui_menu_menu_17,
  .m_label = "Month",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = 0,
    .s_content = {
      .s_int = { .si_low = 1,
                 .si_default = 6,
                 .si_high = 12,
                 .si_notify = 0,
                 .si_formatter = ui_month_formatter,
                 .si_current = &ui_date_month } },
  }
};

int ui_date_day;
static const menu_t ui_menu_menu_20 = {
  .m_up = &ui_menu_menu_17,
  .m_label = "Day",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = 0,
    .s_content = {
      .s_int = { .si_low = 1,
                 .si_default = 19,
                 .si_high = 31,
                 .si_notify = 0,
                 .si_formatter = 0,
                 .si_current = &ui_date_day } },
  }
};

int ui_date_hours;
static const menu_t ui_menu_menu_21 = {
  .m_up = &ui_menu_menu_17,
  .m_label = "Hour",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = 0,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 20,
                 .si_high = 23,
                 .si_notify = 0,
                 .si_formatter = 0,
                 .si_current = &ui_date_hours } },
  }
};

int ui_date_minutes;
static const menu_t ui_menu_menu_22 = {
  .m_up = &ui_menu_menu_17,
  .m_label = "Min",
  .m_selection = {
    .s_type = SEL_INT,
    .s_flags = 0,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 33,
                 .si_high = 59,
                 .si_notify = 0,
                 .si_formatter = 0,
                 .si_current = &ui_date_minutes } },
  }
};

static const menu_t ui_menu_menu_23 = {
  .m_up = &ui_menu_menu_17,
  .m_label = "Apply",
  .m_selection = {
    .s_type = SEL_ACTION,
    .s_flags = 0,
    .s_content = {
      .s_action = { .sa_prompt = "Change clock?",
                    .sa_notify = ui_date_and_time_set } },
  }
};

static const menu_t * const ui_menu_array_24[] = {
  &ui_menu_menu_18,
  &ui_menu_menu_19,
  &ui_menu_menu_20,
  &ui_menu_menu_21,
  &ui_menu_menu_22,
  &ui_menu_menu_23,
};

static const menu_t ui_menu_menu_17 = {
  .m_up = &ui_menu_root,
  .m_label = "Date & time",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 6,
                  .sm_notify_enter = ui_date_and_time_enter,
                  .sm_notify_exit = 0,
                  .sm_entries = ui_menu_array_24 } },
  }
};

static const menu_t ui_menu_menu_26 = {
  .m_up = &ui_menu_menu_25,
  .m_label = "Version",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_flags = 0,
    .s_content = {
      .s_display = { .sd_label = "Firmware 1.00" } },
  }
};

static const menu_t ui_menu_menu_27 = {
  .m_up = &ui_menu_menu_25,
  .m_label = "Serial",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_flags = 0,
    .s_content = {
      .s_display = { .sd_label = "J-0002" } },
  }
};

static const menu_t ui_menu_menu_28 = {
  .m_up = &ui_menu_menu_25,
  .m_label = "About",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_flags = 0,
    .s_content = {
      .s_display = { .sd_label = "(C) Therodox R&D" } },
  }
};

static const menu_t ui_menu_menu_29 = {
  .m_up = &ui_menu_menu_25,
  .m_label = "URL",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_flags = 0,
    .s_content = {
      .s_display = { .sd_label = "therodox.com" } },
  }
};

static const menu_t * const ui_menu_array_30[] = {
  &ui_menu_menu_26,
  &ui_menu_menu_27,
  &ui_menu_menu_28,
  &ui_menu_menu_29,
};

static const menu_t ui_menu_menu_25 = {
  .m_up = &ui_menu_root,
  .m_label = "Info",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 4,
                  .sm_entries = ui_menu_array_30 } },
  }
};

static const menu_t * const ui_menu_array_31[] = {
  &ui_menu_menu_1,
  &ui_menu_menu_2,
  &ui_menu_menu_3,
  &ui_menu_menu_13,
  &ui_menu_menu_17,
  &ui_menu_menu_25,
};

const menu_t ui_menu_root = {
  .m_up = 0,
  .m_label = "Menu",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 6,
                  .sm_entries = ui_menu_array_31 } },
  }
};

