/*
 * Copyright (c) 2021, Tim Flynn <trflynn89@serenityos.org>
 * Copyright (c) 2022, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/DeprecatedString.h>
#include <AK/FlyString.h>
#include <AK/Optional.h>
#include <AK/StringView.h>
#include <AK/Vector.h>
#include <LibWeb/Bindings/LegacyPlatformObject.h>
#include <LibWeb/Forward.h>
#include <LibWeb/WebIDL/ExceptionOr.h>

namespace Web::DOM {

// https://dom.spec.whatwg.org/#domtokenlist
class DOMTokenList final : public Bindings::LegacyPlatformObject {
    WEB_PLATFORM_OBJECT(DOMTokenList, Bindings::LegacyPlatformObject);

public:
    static DOMTokenList* create(Element const& associated_element, FlyString associated_attribute);
    ~DOMTokenList() = default;

    void associated_attribute_changed(StringView value);

    virtual bool is_supported_property_index(u32 index) const override;
    virtual JS::Value item_value(size_t index) const override;

    size_t length() const { return m_token_set.size(); }
    DeprecatedString const& item(size_t index) const;
    bool contains(StringView token);
    WebIDL::ExceptionOr<void> add(Vector<DeprecatedString> const& tokens);
    WebIDL::ExceptionOr<void> remove(Vector<DeprecatedString> const& tokens);
    WebIDL::ExceptionOr<bool> toggle(DeprecatedString const& token, Optional<bool> force);
    WebIDL::ExceptionOr<bool> replace(DeprecatedString const& token, DeprecatedString const& new_token);
    WebIDL::ExceptionOr<bool> supports(StringView token);
    DeprecatedString value() const;
    void set_value(DeprecatedString value);

private:
    DOMTokenList(Element const& associated_element, FlyString associated_attribute);

    WebIDL::ExceptionOr<void> validate_token(StringView token) const;
    void run_update_steps();

    WeakPtr<Element> m_associated_element;
    FlyString m_associated_attribute;
    Vector<DeprecatedString> m_token_set;
};

}
