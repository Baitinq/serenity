/*
 * Copyright (c) 2022, Idan Horowitz <idan.horowitz@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/DeprecatedString.h>
#include <LibJS/Runtime/Object.h>

namespace JS::Intl {

class Segmenter final : public Object {
    JS_OBJECT(Segmenter, Object);

public:
    enum class SegmenterGranularity {
        Grapheme,
        Word,
        Sentence,
    };

    virtual ~Segmenter() override = default;

    DeprecatedString const& locale() const { return m_locale; }
    void set_locale(DeprecatedString locale) { m_locale = move(locale); }

    SegmenterGranularity segmenter_granularity() const { return m_segmenter_granularity; }
    void set_segmenter_granularity(StringView);
    StringView segmenter_granularity_string() const;

private:
    explicit Segmenter(Object& prototype);

    DeprecatedString m_locale;                                                       // [[Locale]]
    SegmenterGranularity m_segmenter_granularity { SegmenterGranularity::Grapheme }; // [[SegmenterGranularity]]
};

Object* create_segment_data_object(VM&, Segmenter const&, Utf16View const&, double start_index, double end_index);
enum class Direction {
    Before,
    After,
};
double find_boundary(Segmenter const&, Utf16View const&, double start_index, Direction, Optional<Vector<size_t>>& boundaries_cache);

}
