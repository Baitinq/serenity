/*
 * Copyright (c) 2022, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Forward.h>
#include <LibGfx/Forward.h>
#include <LibGfx/StandardCursor.h>
#include <LibWeb/Forward.h>
#include <LibWebView/Forward.h>

namespace WebView {

class ViewImplementation {
public:
    virtual ~ViewImplementation() { }

    virtual void notify_server_did_layout(Badge<WebContentClient>, Gfx::IntSize content_size) = 0;
    virtual void notify_server_did_paint(Badge<WebContentClient>, i32 bitmap_id) = 0;
    virtual void notify_server_did_invalidate_content_rect(Badge<WebContentClient>, Gfx::IntRect const&) = 0;
    virtual void notify_server_did_change_selection(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_request_cursor_change(Badge<WebContentClient>, Gfx::StandardCursor cursor) = 0;
    virtual void notify_server_did_change_title(Badge<WebContentClient>, DeprecatedString const&) = 0;
    virtual void notify_server_did_request_scroll(Badge<WebContentClient>, i32, i32) = 0;
    virtual void notify_server_did_request_scroll_to(Badge<WebContentClient>, Gfx::IntPoint) = 0;
    virtual void notify_server_did_request_scroll_into_view(Badge<WebContentClient>, Gfx::IntRect const&) = 0;
    virtual void notify_server_did_enter_tooltip_area(Badge<WebContentClient>, Gfx::IntPoint, DeprecatedString const&) = 0;
    virtual void notify_server_did_leave_tooltip_area(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_hover_link(Badge<WebContentClient>, const AK::URL&) = 0;
    virtual void notify_server_did_unhover_link(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_click_link(Badge<WebContentClient>, const AK::URL&, DeprecatedString const& target, unsigned modifiers) = 0;
    virtual void notify_server_did_middle_click_link(Badge<WebContentClient>, const AK::URL&, DeprecatedString const& target, unsigned modifiers) = 0;
    virtual void notify_server_did_start_loading(Badge<WebContentClient>, const AK::URL&, bool is_redirect) = 0;
    virtual void notify_server_did_finish_loading(Badge<WebContentClient>, const AK::URL&) = 0;
    virtual void notify_server_did_request_navigate_back(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_request_navigate_forward(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_request_refresh(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_request_context_menu(Badge<WebContentClient>, Gfx::IntPoint) = 0;
    virtual void notify_server_did_request_link_context_menu(Badge<WebContentClient>, Gfx::IntPoint, const AK::URL&, DeprecatedString const& target, unsigned modifiers) = 0;
    virtual void notify_server_did_request_image_context_menu(Badge<WebContentClient>, Gfx::IntPoint, const AK::URL&, DeprecatedString const& target, unsigned modifiers, Gfx::ShareableBitmap const&) = 0;
    virtual void notify_server_did_request_alert(Badge<WebContentClient>, DeprecatedString const& message) = 0;
    virtual void notify_server_did_request_confirm(Badge<WebContentClient>, DeprecatedString const& message) = 0;
    virtual void notify_server_did_request_prompt(Badge<WebContentClient>, DeprecatedString const& message, DeprecatedString const& default_) = 0;
    virtual void notify_server_did_request_set_prompt_text(Badge<WebContentClient>, DeprecatedString const& message) = 0;
    virtual void notify_server_did_request_accept_dialog(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_request_dismiss_dialog(Badge<WebContentClient>) = 0;
    virtual void notify_server_did_get_source(const AK::URL& url, DeprecatedString const& source) = 0;
    virtual void notify_server_did_get_dom_tree(DeprecatedString const& dom_tree) = 0;
    virtual void notify_server_did_get_dom_node_properties(i32 node_id, DeprecatedString const& computed_style, DeprecatedString const& resolved_style, DeprecatedString const& custom_properties, DeprecatedString const& node_box_sizing) = 0;
    virtual void notify_server_did_output_js_console_message(i32 message_index) = 0;
    virtual void notify_server_did_get_js_console_messages(i32 start_index, Vector<DeprecatedString> const& message_types, Vector<DeprecatedString> const& messages) = 0;
    virtual void notify_server_did_change_favicon(Gfx::Bitmap const& favicon) = 0;
    virtual Vector<Web::Cookie::Cookie> notify_server_did_request_all_cookies(Badge<WebContentClient>, AK::URL const& url) = 0;
    virtual Optional<Web::Cookie::Cookie> notify_server_did_request_named_cookie(Badge<WebContentClient>, AK::URL const& url, DeprecatedString const& name) = 0;
    virtual DeprecatedString notify_server_did_request_cookie(Badge<WebContentClient>, const AK::URL& url, Web::Cookie::Source source) = 0;
    virtual void notify_server_did_set_cookie(Badge<WebContentClient>, const AK::URL& url, Web::Cookie::ParsedCookie const& cookie, Web::Cookie::Source source) = 0;
    virtual void notify_server_did_update_cookie(Badge<WebContentClient>, Web::Cookie::Cookie const& cookie) = 0;
    virtual void notify_server_did_update_resource_count(i32 count_waiting) = 0;
    virtual void notify_server_did_request_restore_window() = 0;
    virtual Gfx::IntPoint notify_server_did_request_reposition_window(Gfx::IntPoint) = 0;
    virtual Gfx::IntSize notify_server_did_request_resize_window(Gfx::IntSize) = 0;
    virtual Gfx::IntRect notify_server_did_request_maximize_window() = 0;
    virtual Gfx::IntRect notify_server_did_request_minimize_window() = 0;
    virtual Gfx::IntRect notify_server_did_request_fullscreen_window() = 0;
    virtual void notify_server_did_request_file(Badge<WebContentClient>, DeprecatedString const& path, i32) = 0;
    virtual void notify_server_did_finish_handling_input_event(bool event_was_accepted) = 0;
};

}
