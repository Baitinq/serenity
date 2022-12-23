#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(GUI::Window* parent_window, size_t rows, size_t columns, int timer)
    : GUI::Dialog(parent_window),
    m_rows(rows), m_columns(columns), m_timer(timer)
{
    set_rect({ 0, 0, 250, 75 });
    set_title("Settings");
    set_icon(parent->icon());
    set_resizable(false);

    auto& main_widget = set_main_widget<GUI::Widget>();
    main_widget.set_fill_with_background_color(true);

    auto& layout = main_widget.set_layout<GUI::VerticalBoxLayout>();
    layout.set_margins(4);

    auto& name_box = main_widget.add<GUI::Widget>();
    auto& input_layout = name_box.set_layout<GUI::HorizontalBoxLayout>();
    input_layout.set_spacing(4);


}