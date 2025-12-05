/* ui_menu.c
 *
 * Copyright (C)2007 Therodox R&D
 * All rights reserved
 */

#include <ui_menu.h>

int ui_menu_current_4;
int ui_menu_proposed_5;
static menu_t ui_menu_menu_3 = {
  .m_up = &ui_menu_menu_2,
  .m_label = "Threshold",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 2,
                 .si_high = 9,
                 .si_current = &ui_menu_current_4,
                 .si_proposed = &ui_menu_proposed_5 } },
  }
};

int ui_menu_current_7;
int ui_menu_proposed_8;
static char *ui_menu_labels_9[] = {
  "Yes",
  "No",
};

static menu_t ui_menu_menu_6 = {
  .m_up = &ui_menu_menu_2,
  .m_label = "Enabled",
  .m_selection = {
    .s_type = SEL_CHOICE,
    .s_content = {
      .s_choice = { .sc_count = 2,
                    .sc_current = &ui_menu_current_7,
                    .sc_proposed = &ui_menu_proposed_8,
                    .sc_labels = ui_menu_labels_9 } },
  }
};

int ui_menu_current_11;
int ui_menu_proposed_12;
static menu_t ui_menu_menu_10 = {
  .m_up = &ui_menu_menu_2,
  .m_label = "Alarm",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 1,
                 .si_default = 3,
                 .si_high = 10,
                 .si_current = &ui_menu_current_11,
                 .si_proposed = &ui_menu_proposed_12 } },
  }
};

static menu_t *ui_menu_menu_array_13[] = {
  &ui_menu_menu_3,
  &ui_menu_menu_6,
  &ui_menu_menu_10,
};

static menu_t ui_menu_menu_2 = {
  .m_up = &ui_menu_menu_1,
  .m_label = "Channel A",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 3,
                  .sm_entries = ui_menu_menu_array_13 } },
  }
};

int ui_menu_current_16;
int ui_menu_proposed_17;
static menu_t ui_menu_menu_15 = {
  .m_up = &ui_menu_menu_14,
  .m_label = "Threshold",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 2,
                 .si_high = 9,
                 .si_current = &ui_menu_current_16,
                 .si_proposed = &ui_menu_proposed_17 } },
  }
};

int ui_menu_current_19;
int ui_menu_proposed_20;
static char *ui_menu_labels_21[] = {
  "Yes",
  "No",
};

static menu_t ui_menu_menu_18 = {
  .m_up = &ui_menu_menu_14,
  .m_label = "Enabled",
  .m_selection = {
    .s_type = SEL_CHOICE,
    .s_content = {
      .s_choice = { .sc_count = 2,
                    .sc_current = &ui_menu_current_19,
                    .sc_proposed = &ui_menu_proposed_20,
                    .sc_labels = ui_menu_labels_21 } },
  }
};

int ui_menu_current_23;
int ui_menu_proposed_24;
static menu_t ui_menu_menu_22 = {
  .m_up = &ui_menu_menu_14,
  .m_label = "Alarm",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 1,
                 .si_default = 3,
                 .si_high = 10,
                 .si_current = &ui_menu_current_23,
                 .si_proposed = &ui_menu_proposed_24 } },
  }
};

static menu_t *ui_menu_menu_array_25[] = {
  &ui_menu_menu_15,
  &ui_menu_menu_18,
  &ui_menu_menu_22,
};

static menu_t ui_menu_menu_14 = {
  .m_up = &ui_menu_menu_1,
  .m_label = "Channel B",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 3,
                  .sm_entries = ui_menu_menu_array_25 } },
  }
};

int ui_menu_current_28;
int ui_menu_proposed_29;
static menu_t ui_menu_menu_27 = {
  .m_up = &ui_menu_menu_26,
  .m_label = "Year",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 2007,
                 .si_default = 2007,
                 .si_high = 2030,
                 .si_current = &ui_menu_current_28,
                 .si_proposed = &ui_menu_proposed_29 } },
  }
};

int ui_menu_current_31;
int ui_menu_proposed_32;
static menu_t ui_menu_menu_30 = {
  .m_up = &ui_menu_menu_26,
  .m_label = "Month",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 1,
                 .si_default = 6,
                 .si_high = 12,
                 .si_current = &ui_menu_current_31,
                 .si_proposed = &ui_menu_proposed_32 } },
  }
};

int ui_menu_current_34;
int ui_menu_proposed_35;
static menu_t ui_menu_menu_33 = {
  .m_up = &ui_menu_menu_26,
  .m_label = "Day",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 1,
                 .si_default = 19,
                 .si_high = 31,
                 .si_current = &ui_menu_current_34,
                 .si_proposed = &ui_menu_proposed_35 } },
  }
};

int ui_menu_current_37;
int ui_menu_proposed_38;
static menu_t ui_menu_menu_36 = {
  .m_up = &ui_menu_menu_26,
  .m_label = "Hour",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 20,
                 .si_high = 23,
                 .si_current = &ui_menu_current_37,
                 .si_proposed = &ui_menu_proposed_38 } },
  }
};

int ui_menu_current_40;
int ui_menu_proposed_41;
static menu_t ui_menu_menu_39 = {
  .m_up = &ui_menu_menu_26,
  .m_label = "Min",
  .m_selection = {
    .s_type = SEL_INT,
    .s_content = {
      .s_int = { .si_low = 0,
                 .si_default = 33,
                 .si_high = 59,
                 .si_current = &ui_menu_current_40,
                 .si_proposed = &ui_menu_proposed_41 } },
  }
};

static menu_t *ui_menu_menu_array_42[] = {
  &ui_menu_menu_27,
  &ui_menu_menu_30,
  &ui_menu_menu_33,
  &ui_menu_menu_36,
  &ui_menu_menu_39,
};

static menu_t ui_menu_menu_26 = {
  .m_up = &ui_menu_menu_1,
  .m_label = "Date & time",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 5,
                  .sm_entries = ui_menu_menu_array_42 } },
  }
};

static menu_t *ui_menu_menu_array_43[] = {
  &ui_menu_menu_2,
  &ui_menu_menu_14,
  &ui_menu_menu_26,
};

static menu_t ui_menu_menu_1 = {
  .m_up = &root,
  .m_label = "Change",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 3,
                  .sm_entries = ui_menu_menu_array_43 } },
  }
};

static menu_t *ui_menu_menu_array_46[] = {
};

static menu_t ui_menu_menu_45 = {
  .m_up = &ui_menu_menu_44,
  .m_label = "Log",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 0,
                  .sm_entries = ui_menu_menu_array_46 } },
  }
};

static menu_t ui_menu_menu_47 = {
  .m_up = &ui_menu_menu_44,
  .m_label = "Serial",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_content = {
      .s_display = { .sd_label = "30492039-40-XA" } },
  }
};

static menu_t ui_menu_menu_48 = {
  .m_up = &ui_menu_menu_44,
  .m_label = "Version",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_content = {
      .s_display = { .sd_label = "3.1415926" } },
  }
};

static menu_t ui_menu_menu_50 = {
  .m_up = &ui_menu_menu_49,
  .m_label = "Channel A",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_content = {
      .s_display = { .sd_label = "38937" } },
  }
};

static menu_t ui_menu_menu_51 = {
  .m_up = &ui_menu_menu_49,
  .m_label = "Channel B",
  .m_selection = {
    .s_type = SEL_DISPLAY,
    .s_content = {
      .s_display = { .sd_label = "1928" } },
  }
};

static menu_t *ui_menu_menu_array_52[] = {
  &ui_menu_menu_50,
  &ui_menu_menu_51,
};

static menu_t ui_menu_menu_49 = {
  .m_up = &ui_menu_menu_44,
  .m_label = "Counts",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 2,
                  .sm_entries = ui_menu_menu_array_52 } },
  }
};

static menu_t *ui_menu_menu_array_53[] = {
  &ui_menu_menu_45,
  &ui_menu_menu_47,
  &ui_menu_menu_48,
  &ui_menu_menu_49,
};

static menu_t ui_menu_menu_44 = {
  .m_up = &root,
  .m_label = "View",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 4,
                  .sm_entries = ui_menu_menu_array_53 } },
  }
};

static menu_t ui_menu_menu_55 = {
  .m_up = &ui_menu_menu_54,
  .m_label = "Reset",
  .m_selection = {
    .s_type = SEL_NONE,
  }
};

static menu_t ui_menu_menu_56 = {
  .m_up = &ui_menu_menu_54,
  .m_label = "Trigger",
  .m_selection = {
    .s_type = SEL_NONE,
  }
};

static menu_t ui_menu_menu_57 = {
  .m_up = &ui_menu_menu_54,
  .m_label = "Quit",
  .m_selection = {
    .s_type = SEL_NONE,
  }
};

static menu_t *ui_menu_menu_array_58[] = {
  &ui_menu_menu_55,
  &ui_menu_menu_56,
  &ui_menu_menu_57,
};

static menu_t ui_menu_menu_54 = {
  .m_up = &root,
  .m_label = "Action",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 3,
                  .sm_entries = ui_menu_menu_array_58 } },
  }
};

static menu_t *ui_menu_menu_array_59[] = {
  &ui_menu_menu_1,
  &ui_menu_menu_44,
  &ui_menu_menu_54,
};

menu_t root = {
  .m_up = 0,
  .m_label = "Menu",
  .m_selection = {
    .s_type = SEL_MENU,
    .s_content = {
      .s_menu = { .sm_count = 3,
                  .sm_entries = ui_menu_menu_array_59 } },
  }
};

