/*
* Copyright (c) 2022, the SerenityOS developers.
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <LibGUI/Dialog.h>

class SettingsDialog : public GUI::Dialog {
    C_OBJECT(SettingsDialog)
public:
    size_t rows() const { return m_rows; };
    size_t columns() const { return m_columns; };
    int timer() const { return m_timer; };

private:
    SettingsDialog(GUI::Window* parent_window, size_t rows, size_t columns, int timer);

    size_t m_rows;
    size_t m_columns;
    int m_timer;
};