/*
 * Copyright (c) 2021, Tim Flynn <trflynn89@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/DOM/Attr.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/DOM/Element.h>
#include <LibWeb/DOM/MutationType.h>
#include <LibWeb/DOM/StaticNodeList.h>

namespace Web::DOM {

JS::NonnullGCPtr<Attr> Attr::create(Document& document, FlyString local_name, DeprecatedString value, Element const* owner_element)
{
    return document.heap().allocate<Attr>(document.realm(), document, QualifiedName(move(local_name), {}, {}), move(value), owner_element);
}

JS::NonnullGCPtr<Attr> Attr::clone(Document& document)
{
    return *heap().allocate<Attr>(realm(), document, m_qualified_name, m_value, nullptr);
}

Attr::Attr(Document& document, QualifiedName qualified_name, DeprecatedString value, Element const* owner_element)
    : Node(document, NodeType::ATTRIBUTE_NODE)
    , m_qualified_name(move(qualified_name))
    , m_value(move(value))
    , m_owner_element(owner_element)
{
    set_prototype(&Bindings::cached_web_prototype(document.realm(), "Attr"));
}

void Attr::visit_edges(Cell::Visitor& visitor)
{
    Base::visit_edges(visitor);
    visitor.visit(m_owner_element.ptr());
}

Element* Attr::owner_element()
{
    return m_owner_element.ptr();
}

Element const* Attr::owner_element() const
{
    return m_owner_element.ptr();
}

void Attr::set_owner_element(Element const* owner_element)
{
    m_owner_element = owner_element;
}

// https://dom.spec.whatwg.org/#set-an-existing-attribute-value
void Attr::set_value(DeprecatedString value)
{
    // 1. If attribute’s element is null, then set attribute’s value to value.
    if (!owner_element()) {
        m_value = move(value);
        return;
    }

    // 2. Otherwise, change attribute to value.
    // https://dom.spec.whatwg.org/#concept-element-attributes-change
    // 1. Handle attribute changes for attribute with attribute’s element, attribute’s value, and value.
    handle_attribute_changes(*owner_element(), m_value, value);

    // 2. Set attribute’s value to value.
    m_value = move(value);
}

// https://dom.spec.whatwg.org/#handle-attribute-changes
void Attr::handle_attribute_changes(Element& element, DeprecatedString const& old_value, [[maybe_unused]] DeprecatedString const& new_value)
{
    // 1. Queue a mutation record of "attributes" for element with attribute’s local name, attribute’s namespace, oldValue, « », « », null, and null.
    element.queue_mutation_record(MutationType::attributes, local_name(), namespace_uri(), old_value, StaticNodeList::create(realm(), {}), StaticNodeList::create(realm(), {}), nullptr, nullptr);

    // FIXME: 2. If element is custom, then enqueue a custom element callback reaction with element, callback name "attributeChangedCallback", and an argument list containing attribute’s local name, oldValue, newValue, and attribute’s namespace.

    // FIXME: 3. Run the attribute change steps with element, attribute’s local name, oldValue, newValue, and attribute’s namespace.
}

}
