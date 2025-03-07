/*
 * Copyright (c) 2020-2021, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2022, Kenneth Myhra <kennethmyhra@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/ByteBuffer.h>
#include <AK/RefCounted.h>
#include <AK/URL.h>
#include <AK/Weakable.h>
#include <LibWeb/DOM/EventTarget.h>
#include <LibWeb/Fetch/BodyInit.h>
#include <LibWeb/Fetch/Infrastructure/HTTP/Headers.h>
#include <LibWeb/Fetch/Infrastructure/HTTP/Statuses.h>
#include <LibWeb/HTML/Window.h>
#include <LibWeb/MimeSniff/MimeType.h>
#include <LibWeb/URL/URLSearchParams.h>
#include <LibWeb/WebIDL/ExceptionOr.h>
#include <LibWeb/XHR/XMLHttpRequestEventTarget.h>

namespace Web::XHR {

// https://fetch.spec.whatwg.org/#typedefdef-xmlhttprequestbodyinit
using DocumentOrXMLHttpRequestBodyInit = Variant<JS::Handle<Web::DOM::Document>, JS::Handle<Web::FileAPI::Blob>, JS::Handle<JS::Object>, JS::Handle<Web::URL::URLSearchParams>, AK::DeprecatedString>;

class XMLHttpRequest final : public XMLHttpRequestEventTarget {
    WEB_PLATFORM_OBJECT(XMLHttpRequest, XMLHttpRequestEventTarget);

public:
    enum class State : u16 {
        Unsent = 0,
        Opened = 1,
        HeadersReceived = 2,
        Loading = 3,
        Done = 4,
    };

    static JS::NonnullGCPtr<XMLHttpRequest> construct_impl(JS::Realm&);

    virtual ~XMLHttpRequest() override;

    State ready_state() const { return m_state; };
    Fetch::Infrastructure::Status status() const { return m_status; };
    WebIDL::ExceptionOr<DeprecatedString> response_text() const;
    WebIDL::ExceptionOr<JS::Value> response();
    Bindings::XMLHttpRequestResponseType response_type() const { return m_response_type; }

    WebIDL::ExceptionOr<void> open(DeprecatedString const& method, DeprecatedString const& url);
    WebIDL::ExceptionOr<void> open(DeprecatedString const& method, DeprecatedString const& url, bool async, DeprecatedString const& username = {}, DeprecatedString const& password = {});
    WebIDL::ExceptionOr<void> send(Optional<DocumentOrXMLHttpRequestBodyInit> body);

    WebIDL::ExceptionOr<void> set_request_header(DeprecatedString const& header, DeprecatedString const& value);
    WebIDL::ExceptionOr<void> set_response_type(Bindings::XMLHttpRequestResponseType);

    DeprecatedString get_response_header(DeprecatedString const& name) { return m_response_headers.get(name).value_or({}); }
    DeprecatedString get_all_response_headers() const;

    WebIDL::CallbackType* onreadystatechange();
    void set_onreadystatechange(WebIDL::CallbackType*);

    WebIDL::ExceptionOr<void> override_mime_type(DeprecatedString const& mime);

    u32 timeout() const;
    WebIDL::ExceptionOr<void> set_timeout(u32 timeout);

    bool with_credentials() const;
    WebIDL::ExceptionOr<void> set_with_credentials(bool);

    void abort();

private:
    virtual void visit_edges(Cell::Visitor&) override;
    virtual bool must_survive_garbage_collection() const override;

    void set_status(Fetch::Infrastructure::Status status) { m_status = status; }
    void fire_progress_event(DeprecatedString const&, u64, u64);

    MimeSniff::MimeType get_response_mime_type() const;
    Optional<StringView> get_final_encoding() const;
    MimeSniff::MimeType get_final_mime_type() const;

    DeprecatedString get_text_response() const;

    XMLHttpRequest(HTML::Window&, Fetch::Infrastructure::HeaderList&);

    // Non-standard
    JS::NonnullGCPtr<HTML::Window> m_window;
    Fetch::Infrastructure::Status m_status { 0 };
    HashMap<DeprecatedString, DeprecatedString, CaseInsensitiveStringTraits> m_response_headers;

    // https://xhr.spec.whatwg.org/#concept-xmlhttprequest-state
    // state
    //     One of unsent, opened, headers received, loading, and done; initially unsent.
    State m_state { State::Unsent };

    // https://xhr.spec.whatwg.org/#send-flag
    // send() flag
    //     A flag, initially unset.
    bool m_send { false };

    // https://xhr.spec.whatwg.org/#timeout
    // timeout
    //     An unsigned integer, initially 0.
    u32 m_timeout { 0 };

    // https://xhr.spec.whatwg.org/#cross-origin-credentials
    // cross-origin credentials
    //     A boolean, initially false.
    bool m_cross_origin_credentials { false };

    // https://xhr.spec.whatwg.org/#request-method
    // request method
    //     A method.
    DeprecatedString m_request_method;

    // https://xhr.spec.whatwg.org/#request-url
    // request URL
    //     A URL.
    AK::URL m_request_url;

    // https://xhr.spec.whatwg.org/#author-request-headers
    // author request headers
    //     A header list, initially empty.
    JS::NonnullGCPtr<Fetch::Infrastructure::HeaderList> m_author_request_headers;

    // FIXME: https://xhr.spec.whatwg.org/#request-body

    // https://xhr.spec.whatwg.org/#synchronous-flag
    // synchronous flag
    //     A flag, initially unset.
    bool m_synchronous { false };

    // https://xhr.spec.whatwg.org/#upload-complete-flag
    // upload complete flag
    //     A flag, initially unset.
    bool m_upload_complete { false };

    // https://xhr.spec.whatwg.org/#upload-listener-flag
    // upload listener flag
    //     A flag, initially unset.
    bool m_upload_listener { false };

    // https://xhr.spec.whatwg.org/#timed-out-flag
    // timed out flag
    //     A flag, initially unset.
    bool m_timed_out { false };

    // FIXME: https://xhr.spec.whatwg.org/#response

    // https://xhr.spec.whatwg.org/#received-bytes
    // received bytes
    //     A byte sequence, initially the empty byte sequence.
    ByteBuffer m_received_bytes;

    // https://xhr.spec.whatwg.org/#response-type
    // response type
    //     One of the empty string, "arraybuffer", "blob", "document", "json", and "text"; initially the empty string.
    Bindings::XMLHttpRequestResponseType m_response_type;

    enum class Failure {
        /// ????
    };

    // https://xhr.spec.whatwg.org/#response-object
    // response object
    //     An object, failure, or null, initially null.
    //     NOTE: This needs to be a JS::Value as the JSON response might not actually be an object.
    Variant<JS::Value, Failure, Empty> m_response_object;

    // FIXME: https://xhr.spec.whatwg.org/#xmlhttprequest-fetch-controller

    // https://xhr.spec.whatwg.org/#override-mime-type
    // override MIME type
    //     A MIME type or null, initially null.
    //     NOTE: Can get a value when overrideMimeType() is invoked.
    Optional<MimeSniff::MimeType> m_override_mime_type;
};

}
