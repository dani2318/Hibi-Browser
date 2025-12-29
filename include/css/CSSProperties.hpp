#ifndef CSS_PROPERTIES_H
#define CSS_PROPERTIES_H

#include <string>
#include <map>
#include <vector>
#include <sstream>

class CSSProperties {
private:
    std::map<std::string, std::string*> propertyMap;
    
    void initializePropertyMap() {
        
        propertyMap["display"] = &display;
        propertyMap["position"] = &position;
        propertyMap["top"] = &top;
        propertyMap["right"] = &right;
        propertyMap["bottom"] = &bottom;
        propertyMap["left"] = &left;
        propertyMap["float"] = &float_prop;
        propertyMap["clear"] = &clear;
        propertyMap["width"] = &width;
        propertyMap["height"] = &height;
        propertyMap["min-width"] = &min_width;
        propertyMap["max-width"] = &max_width;
        propertyMap["min-height"] = &min_height;
        propertyMap["max-height"] = &max_height;
        propertyMap["margin"] = &margin;
        propertyMap["margin-top"] = &margin_top;
        propertyMap["margin-right"] = &margin_right;
        propertyMap["margin-bottom"] = &margin_bottom;
        propertyMap["margin-left"] = &margin_left;
        propertyMap["padding"] = &padding;
        propertyMap["padding-top"] = &padding_top;
        propertyMap["padding-right"] = &padding_right;
        propertyMap["padding-bottom"] = &padding_bottom;
        propertyMap["padding-left"] = &padding_left;
        propertyMap["box-sizing"] = &box_sizing;
        propertyMap["overflow"] = &overflow;
        propertyMap["overflow-x"] = &overflow_x;
        propertyMap["overflow-y"] = &overflow_y;
        propertyMap["visibility"] = &visibility;
        propertyMap["z-index"] = &z_index;
        propertyMap["inset"] = &inset;
        propertyMap["inset-block"] = &inset_block;
        propertyMap["inset-block-start"] = &inset_block_start;
        propertyMap["inset-block-end"] = &inset_block_end;
        propertyMap["inset-inline"] = &inset_inline;
        propertyMap["inset-inline-start"] = &inset_inline_start;
        propertyMap["inset-inline-end"] = &inset_inline_end;
        
        
        propertyMap["flex"] = &flex;
        propertyMap["flex-direction"] = &flex_direction;
        propertyMap["flex-wrap"] = &flex_wrap;
        propertyMap["flex-flow"] = &flex_flow;
        propertyMap["justify-content"] = &justify_content;
        propertyMap["align-items"] = &align_items;
        propertyMap["align-content"] = &align_content;
        propertyMap["align-self"] = &align_self;
        propertyMap["flex-grow"] = &flex_grow;
        propertyMap["flex-shrink"] = &flex_shrink;
        propertyMap["flex-basis"] = &flex_basis;
        propertyMap["order"] = &order;
        propertyMap["gap"] = &gap;
        propertyMap["row-gap"] = &row_gap;
        propertyMap["flexbox_column_gap"] = &flexbox_column_gap;
        
        
        propertyMap["grid"] = &grid;
        propertyMap["grid-template"] = &grid_template;
        propertyMap["grid-template-columns"] = &grid_template_columns;
        propertyMap["grid-template-rows"] = &grid_template_rows;
        propertyMap["grid-template-areas"] = &grid_template_areas;
        propertyMap["grid-auto-columns"] = &grid_auto_columns;
        propertyMap["grid-auto-rows"] = &grid_auto_rows;
        propertyMap["grid-auto-flow"] = &grid_auto_flow;
        propertyMap["grid-column"] = &grid_column;
        propertyMap["grid-column-start"] = &grid_column_start;
        propertyMap["grid-column-end"] = &grid_column_end;
        propertyMap["grid-row"] = &grid_row;
        propertyMap["grid-row-start"] = &grid_row_start;
        propertyMap["grid-row-end"] = &grid_row_end;
        propertyMap["grid-area"] = &grid_area;
        propertyMap["justify-items"] = &justify_items;
        propertyMap["place-items"] = &place_items;
        propertyMap["place-content"] = &place_content;
        propertyMap["place-self"] = &place_self;
        propertyMap["justify-self"] = &justify_self;
        
        
        propertyMap["font"] = &font;
        propertyMap["font-family"] = &font_family;
        propertyMap["font-size"] = &font_size;
        propertyMap["font-weight"] = &font_weight;
        propertyMap["font-style"] = &font_style;
        propertyMap["font-variant"] = &font_variant;
        propertyMap["font-variant-caps"] = &font_variant_caps;
        propertyMap["font-variant-numeric"] = &font_variant_numeric;
        propertyMap["font-variant-ligatures"] = &font_variant_ligatures;
        propertyMap["font-stretch"] = &font_stretch;
        propertyMap["font-size-adjust"] = &font_size_adjust;
        propertyMap["font-kerning"] = &font_kerning;
        propertyMap["line-height"] = &line_height;
        propertyMap["letter-spacing"] = &letter_spacing;
        propertyMap["word-spacing"] = &word_spacing;
        propertyMap["text-align"] = &text_align;
        propertyMap["text-align-last"] = &text_align_last;
        propertyMap["text-decoration"] = &text_decoration;
        propertyMap["text-decoration-line"] = &text_decoration_line;
        propertyMap["text-decoration-color"] = &text_decoration_color;
        propertyMap["text-decoration-style"] = &text_decoration_style;
        propertyMap["text-decoration-thickness"] = &text_decoration_thickness;
        propertyMap["text-decoration-skip-ink"] = &text_decoration_skip_ink;
        propertyMap["text-underline-offset"] = &text_underline_offset;
        propertyMap["text-underline-position"] = &text_underline_position;
        propertyMap["text-transform"] = &text_transform;
        propertyMap["text-indent"] = &text_indent;
        propertyMap["text-shadow"] = &text_shadow;
        propertyMap["text-overflow"] = &text_overflow;
        propertyMap["text-emphasis"] = &text_emphasis;
        propertyMap["text-emphasis-color"] = &text_emphasis_color;
        propertyMap["text-emphasis-style"] = &text_emphasis_style;
        propertyMap["text-emphasis-position"] = &text_emphasis_position;
        propertyMap["white-space"] = &white_space;
        propertyMap["word-wrap"] = &word_wrap;
        propertyMap["word-break"] = &word_break;
        propertyMap["overflow-wrap"] = &overflow_wrap;
        propertyMap["direction"] = &direction;
        propertyMap["writing-mode"] = &writing_mode;
        propertyMap["text-orientation"] = &text_orientation;
        propertyMap["text-combine-upright"] = &text_combine_upright;
        propertyMap["unicode-bidi"] = &unicode_bidi;
        
        
        propertyMap["color"] = &color;
        propertyMap["background"] = &background;
        propertyMap["background-color"] = &background_color;
        propertyMap["background-image"] = &background_image;
        propertyMap["background-position"] = &background_position;
        propertyMap["background-position-x"] = &background_position_x;
        propertyMap["background-position-y"] = &background_position_y;
        propertyMap["background-size"] = &background_size;
        propertyMap["background-repeat"] = &background_repeat;
        propertyMap["background-attachment"] = &background_attachment;
        propertyMap["background-clip"] = &background_clip;
        propertyMap["background-origin"] = &background_origin;
        propertyMap["background-blend-mode"] = &background_blend_mode;
        propertyMap["opacity"] = &opacity;
        
        
        propertyMap["border"] = &border;
        propertyMap["border-width"] = &border_width;
        propertyMap["border-style"] = &border_style;
        propertyMap["border-color"] = &border_color;
        propertyMap["border-radius"] = &border_radius;
        propertyMap["border-top"] = &border_top;
        propertyMap["border-top-width"] = &border_top_width;
        propertyMap["border-top-style"] = &border_top_style;
        propertyMap["border-top-color"] = &border_top_color;
        propertyMap["border-right"] = &border_right;
        propertyMap["border-right-width"] = &border_right_width;
        propertyMap["border-right-style"] = &border_right_style;
        propertyMap["border-right-color"] = &border_right_color;
        propertyMap["border-bottom"] = &border_bottom;
        propertyMap["border-bottom-width"] = &border_bottom_width;
        propertyMap["border-bottom-style"] = &border_bottom_style;
        propertyMap["border-bottom-color"] = &border_bottom_color;
        propertyMap["border-left"] = &border_left;
        propertyMap["border-left-width"] = &border_left_width;
        propertyMap["border-left-style"] = &border_left_style;
        propertyMap["border-left-color"] = &border_left_color;
        propertyMap["border-top-left-radius"] = &border_top_left_radius;
        propertyMap["border-top-right-radius"] = &border_top_right_radius;
        propertyMap["border-bottom-left-radius"] = &border_bottom_left_radius;
        propertyMap["border-bottom-right-radius"] = &border_bottom_right_radius;
        propertyMap["border-image"] = &border_image;
        propertyMap["border-image-source"] = &border_image_source;
        propertyMap["border-image-slice"] = &border_image_slice;
        propertyMap["border-image-width"] = &border_image_width;
        propertyMap["border-image-outset"] = &border_image_outset;
        propertyMap["border-image-repeat"] = &border_image_repeat;
        propertyMap["border-block"] = &border_block;
        propertyMap["border-block-start"] = &border_block_start;
        propertyMap["border-block-end"] = &border_block_end;
        propertyMap["border-inline"] = &border_inline;
        propertyMap["border-inline-start"] = &border_inline_start;
        propertyMap["border-inline-end"] = &border_inline_end;
        
        
        propertyMap["outline"] = &outline;
        propertyMap["outline-width"] = &outline_width;
        propertyMap["outline-style"] = &outline_style;
        propertyMap["outline-color"] = &outline_color;
        propertyMap["outline-offset"] = &outline_offset;
        
        
        propertyMap["transition"] = &transition;
        propertyMap["transition-property"] = &transition_property;
        propertyMap["transition-duration"] = &transition_duration;
        propertyMap["transition-timing-function"] = &transition_timing_function;
        propertyMap["transition-delay"] = &transition_delay;
        
        
        propertyMap["animation"] = &animation;
        propertyMap["animation-name"] = &animation_name;
        propertyMap["animation-duration"] = &animation_duration;
        propertyMap["animation-timing-function"] = &animation_timing_function;
        propertyMap["animation-delay"] = &animation_delay;
        propertyMap["animation-iteration-count"] = &animation_iteration_count;
        propertyMap["animation-direction"] = &animation_direction;
        propertyMap["animation-fill-mode"] = &animation_fill_mode;
        propertyMap["animation-play-state"] = &animation_play_state;
        propertyMap["animation-timeline"] = &animation_timeline;
        
        
        propertyMap["transform"] = &transform;
        propertyMap["transform-origin"] = &transform_origin;
        propertyMap["transform-style"] = &transform_style;
        propertyMap["transform-box"] = &transform_box;
        propertyMap["perspective"] = &perspective;
        propertyMap["perspective-origin"] = &perspective_origin;
        propertyMap["backface-visibility"] = &backface_visibility;
        propertyMap["rotate"] = &rotate;
        propertyMap["scale"] = &scale;
        propertyMap["translate"] = &translate;
        
        
        propertyMap["filter"] = &filter;
        propertyMap["backdrop-filter"] = &backdrop_filter;
        propertyMap["box-shadow"] = &box_shadow;
        propertyMap["clip-path"] = &clip_path;
        propertyMap["mask"] = &mask;
        propertyMap["mask-image"] = &mask_image;
        propertyMap["mask-mode"] = &mask_mode;
        propertyMap["mask-position"] = &mask_position;
        propertyMap["mask-size"] = &mask_size;
        propertyMap["mask-repeat"] = &mask_repeat;
        propertyMap["mask-origin"] = &mask_origin;
        propertyMap["mask-clip"] = &mask_clip;
        propertyMap["mask-composite"] = &mask_composite;
        propertyMap["mask-type"] = &mask_type;
        propertyMap["mix-blend-mode"] = &mix_blend_mode;
        propertyMap["isolation"] = &isolation;
        
        
        propertyMap["list-style"] = &list_style;
        propertyMap["list-style-type"] = &list_style_type;
        propertyMap["list-style-position"] = &list_style_position;
        propertyMap["list-style-image"] = &list_style_image;
        
        
        propertyMap["table-layout"] = &table_layout;
        propertyMap["caption-side"] = &caption_side;
        propertyMap["border-collapse"] = &border_collapse;
        propertyMap["border-spacing"] = &border_spacing;
        propertyMap["empty-cells"] = &empty_cells;
        
        
        propertyMap["vertical-align"] = &vertical_align;
        propertyMap["object-fit"] = &object_fit;
        propertyMap["object-position"] = &object_position;
        propertyMap["aspect-ratio"] = &aspect_ratio;
        
        
        propertyMap["cursor"] = &cursor;
        propertyMap["pointer-events"] = &pointer_events;
        propertyMap["user-select"] = &user_select;
        propertyMap["resize"] = &resize;
        propertyMap["caret-color"] = &caret_color;
        propertyMap["accent-color"] = &accent_color;
        propertyMap["appearance"] = &appearance;
        
        
        propertyMap["content"] = &content;
        propertyMap["quotes"] = &quotes;
        propertyMap["counter-reset"] = &counter_reset;
        propertyMap["counter-increment"] = &counter_increment;
        propertyMap["counter-set"] = &counter_set;
        
        
        propertyMap["contain"] = &contain;
        propertyMap["content-visibility"] = &content_visibility;
        propertyMap["will-change"] = &will_change;
        
        
        propertyMap["scroll-behavior"] = &scroll_behavior;
        propertyMap["scroll-margin"] = &scroll_margin;
        propertyMap["scroll-margin-top"] = &scroll_margin_top;
        propertyMap["scroll-margin-right"] = &scroll_margin_right;
        propertyMap["scroll-margin-bottom"] = &scroll_margin_bottom;
        propertyMap["scroll-margin-left"] = &scroll_margin_left;
        propertyMap["scroll-padding"] = &scroll_padding;
        propertyMap["scroll-padding-top"] = &scroll_padding_top;
        propertyMap["scroll-padding-right"] = &scroll_padding_right;
        propertyMap["scroll-padding-bottom"] = &scroll_padding_bottom;
        propertyMap["scroll-padding-left"] = &scroll_padding_left;
        propertyMap["scroll-snap-type"] = &scroll_snap_type;
        propertyMap["scroll-snap-align"] = &scroll_snap_align;
        propertyMap["scroll-snap-stop"] = &scroll_snap_stop;
        propertyMap["overscroll-behavior"] = &overscroll_behavior;
        propertyMap["overscroll-behavior-x"] = &overscroll_behavior_x;
        propertyMap["overscroll-behavior-y"] = &overscroll_behavior_y;
        propertyMap["scrollbar-color"] = &scrollbar_color;
        propertyMap["scrollbar-width"] = &scrollbar_width;
        propertyMap["scrollbar-gutter"] = &scrollbar_gutter;
        
        
        propertyMap["columns"] = &columns;
        propertyMap["column-count"] = &column_count;
        propertyMap["column-width"] = &column_width;
        propertyMap["column-gap"] = &column_gap;
        propertyMap["column-rule"] = &column_rule;
        propertyMap["column-rule-width"] = &column_rule_width;
        propertyMap["column-rule-style"] = &column_rule_style;
        propertyMap["column-rule-color"] = &column_rule_color;
        propertyMap["column-span"] = &column_span;
        propertyMap["column-fill"] = &column_fill;
        
        
        propertyMap["page-break-before"] = &page_break_before;
        propertyMap["page-break-after"] = &page_break_after;
        propertyMap["page-break-inside"] = &page_break_inside;
        propertyMap["break-before"] = &break_before;
        propertyMap["break-after"] = &break_after;
        propertyMap["break-inside"] = &break_inside;
        propertyMap["orphans"] = &orphans;
        propertyMap["widows"] = &widows;
        
        
        propertyMap["all"] = &all;
        propertyMap["clip"] = &clip;
        propertyMap["touch-action"] = &touch_action;
        propertyMap["tab-size"] = &tab_size;
        propertyMap["hyphens"] = &hyphens;
        propertyMap["image-rendering"] = &image_rendering;
        propertyMap["image-orientation"] = &image_orientation;
        propertyMap["forced-color-adjust"] = &forced_color_adjust;
        propertyMap["color-scheme"] = &color_scheme;
        propertyMap["print-color-adjust"] = &print_color_adjust;
        
        
        propertyMap["container"] = &container;
        propertyMap["container-name"] = &container_name;
        propertyMap["container-type"] = &container_type;
        
        
        propertyMap["margin-block"] = &margin_block;
        propertyMap["margin-block-start"] = &margin_block_start;
        propertyMap["margin-block-end"] = &margin_block_end;
        propertyMap["margin-inline"] = &margin_inline;
        propertyMap["margin-inline-start"] = &margin_inline_start;
        propertyMap["margin-inline-end"] = &margin_inline_end;
        propertyMap["padding-block"] = &padding_block;
        propertyMap["padding-block-start"] = &padding_block_start;
        propertyMap["padding-block-end"] = &padding_block_end;
        propertyMap["padding-inline"] = &padding_inline;
        propertyMap["padding-inline-start"] = &padding_inline_start;
        propertyMap["padding-inline-end"] = &padding_inline_end;
        propertyMap["block-size"] = &block_size;
        propertyMap["inline-size"] = &inline_size;
        propertyMap["min-block-size"] = &min_block_size;
        propertyMap["max-block-size"] = &max_block_size;
        propertyMap["min-inline-size"] = &min_inline_size;
        propertyMap["max-inline-size"] = &max_inline_size;
    }

public:
    
    std::string display, position, top, right, bottom, left, float_prop, clear;
    std::string width, height, min_width, max_width, min_height, max_height;
    std::string margin, margin_top, margin_right, margin_bottom, margin_left;
    std::string padding, padding_top, padding_right, padding_bottom, padding_left;
    std::string box_sizing, overflow, overflow_x, overflow_y, visibility, z_index;
    std::string inset, inset_block, inset_block_start, inset_block_end;
    std::string inset_inline, inset_inline_start, inset_inline_end;
    
    
    std::string flex, flex_direction, flex_wrap, flex_flow;
    std::string justify_content, align_items, align_content, align_self;
    std::string flex_grow, flex_shrink, flex_basis, order;
    std::string gap, row_gap, flexbox_column_gap;
    
    
    std::string grid, grid_template, grid_template_columns, grid_template_rows;
    std::string grid_template_areas, grid_auto_columns, grid_auto_rows, grid_auto_flow;
    std::string grid_column, grid_column_start, grid_column_end;
    std::string grid_row, grid_row_start, grid_row_end, grid_area;
    std::string justify_items, place_items, place_content, place_self, justify_self;
    
    
    std::string font, font_family, font_size, font_weight, font_style;
    std::string font_variant, font_variant_caps, font_variant_numeric, font_variant_ligatures;
    std::string font_stretch, font_size_adjust, font_kerning;
    std::string line_height, letter_spacing, word_spacing;
    std::string text_align, text_align_last, text_decoration;
    std::string text_decoration_line, text_decoration_color, text_decoration_style;
    std::string text_decoration_thickness, text_decoration_skip_ink;
    std::string text_underline_offset, text_underline_position;
    std::string text_transform, text_indent, text_shadow, text_overflow;
    std::string text_emphasis, text_emphasis_color, text_emphasis_style, text_emphasis_position;
    std::string white_space, word_wrap, word_break, overflow_wrap;
    std::string direction, writing_mode, text_orientation, text_combine_upright, unicode_bidi;
    
    
    std::string color, background, background_color, background_image;
    std::string background_position, background_position_x, background_position_y;
    std::string background_size, background_repeat, background_attachment;
    std::string background_clip, background_origin, background_blend_mode, opacity;
    
    
    std::string border, border_width, border_style, border_color, border_radius;
    std::string border_top, border_top_width, border_top_style, border_top_color;
    std::string border_right, border_right_width, border_right_style, border_right_color;
    std::string border_bottom, border_bottom_width, border_bottom_style, border_bottom_color;
    std::string border_left, border_left_width, border_left_style, border_left_color;
    std::string border_top_left_radius, border_top_right_radius;
    std::string border_bottom_left_radius, border_bottom_right_radius;
    std::string border_image, border_image_source, border_image_slice;
    std::string border_image_width, border_image_outset, border_image_repeat;
    std::string border_block, border_block_start, border_block_end;
    std::string border_inline, border_inline_start, border_inline_end;
    
    
    std::string outline, outline_width, outline_style, outline_color, outline_offset;
    
    
    std::string transition, transition_property, transition_duration;
    std::string transition_timing_function, transition_delay;
    
    
    std::string animation, animation_name, animation_duration, animation_timing_function;
    std::string animation_delay, animation_iteration_count, animation_direction;
    std::string animation_fill_mode, animation_play_state, animation_timeline;
    
    
    std::string transform, transform_origin, transform_style, transform_box;
    std::string perspective, perspective_origin, backface_visibility;
    std::string rotate, scale, translate;
    
    
    std::string filter, backdrop_filter, box_shadow, clip_path;
    std::string mask, mask_image, mask_mode, mask_position, mask_size;
    std::string mask_repeat, mask_origin, mask_clip, mask_composite, mask_type;
    std::string mix_blend_mode, isolation;
    
    
    std::string list_style, list_style_type, list_style_position, list_style_image;
    
    
    std::string table_layout, caption_side, border_collapse, border_spacing, empty_cells;
    
    
    std::string vertical_align, object_fit, object_position, aspect_ratio;
    
    
    std::string cursor, pointer_events, user_select, resize;
    std::string caret_color, accent_color, appearance;
    
    
    std::string content, quotes, counter_reset, counter_increment, counter_set;
    
    
    std::string contain, content_visibility, will_change;
    
    
    std::string scroll_behavior, scroll_margin, scroll_margin_top, scroll_margin_right;
    std::string scroll_margin_bottom, scroll_margin_left;
    std::string scroll_padding, scroll_padding_top, scroll_padding_right;
    std::string scroll_padding_bottom, scroll_padding_left;
    std::string scroll_snap_type, scroll_snap_align, scroll_snap_stop;
    std::string overscroll_behavior, overscroll_behavior_x, overscroll_behavior_y;
    std::string scrollbar_color, scrollbar_width, scrollbar_gutter;
    
    
    std::string columns, column_count, column_width, column_gap;
    std::string column_rule, column_rule_width, column_rule_style, column_rule_color;
    std::string column_span, column_fill;
    
    
    std::string page_break_before, page_break_after, page_break_inside;
    std::string break_before, break_after, break_inside;
    std::string orphans, widows;
    
    
    std::string all, clip, touch_action, tab_size, hyphens;
    std::string image_rendering, image_orientation;
    std::string forced_color_adjust, color_scheme, print_color_adjust;
    
    
    std::string container, container_name, container_type;
    
    
    std::string margin_block, margin_block_start, margin_block_end;
    std::string margin_inline, margin_inline_start, margin_inline_end;
    std::string padding_block, padding_block_start, padding_block_end;
    std::string padding_inline, padding_inline_start, padding_inline_end;
    std::string block_size, inline_size, min_block_size, max_block_size;
    std::string min_inline_size, max_inline_size;
    
    
    CSSProperties() {
        initializePropertyMap();
    }
    
    bool setProperty(const std::string& property, const std::string& value) {
        auto it = propertyMap.find(property);
        if (it != propertyMap.end()) {
            *(it->second) = value;
            return true;
        }
        return false;
    }
    
    std::string getProperty(const std::string& property) const {
        auto it = propertyMap.find(property);
        if (it != propertyMap.end()) {
            return *(it->second);
        }
        return "";
    }
    
    bool hasProperty(const std::string& property) const {
        return propertyMap.find(property) != propertyMap.end();
    }
    
    std::vector<std::string> getAllPropertyNames() const {
        std::vector<std::string> names;
        for (const auto& pair : propertyMap) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    std::string toCSS(const std::string& selector = "") const {
        std::ostringstream css;
        
        if (!selector.empty()) {
            css << selector << " {\n";
        }
        
        for (const auto& pair : propertyMap) {
            if (!pair.second->empty()) {
                css << "  " << pair.first << ": " << *(pair.second) << ";\n";
            }
        }
        
        if (!selector.empty()) {
            css << "}\n";
        }
        
        return css.str();
    }
    
    void fromCSSString(const std::string& cssString) {
        std::istringstream stream(cssString);
        std::string line;
        
        while (std::getline(stream, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string prop = line.substr(0, colonPos);
                std::string val = line.substr(colonPos + 1);
                
                
                prop.erase(0, prop.find_first_not_of(" \t"));
                prop.erase(prop.find_last_not_of(" \t") + 1);
                
                val.erase(0, val.find_first_not_of(" \t"));
                val.erase(val.find_last_not_of(" \t;") + 1);
                
                setProperty(prop, val);
            }
        }
    }
    
    void Clear() {
        for (auto& pair : propertyMap) {
            pair.second->clear();
        }
    }
    
    void merge(const CSSProperties& other) {
        for (const auto& pair : propertyMap) {
            std::string otherValue = other.getProperty(pair.first);
            if (!otherValue.empty()) {
                *(pair.second) = otherValue;
            }
        }
    }
    
    int countSetProperties() const {
        int count = 0;
        for (const auto& pair : propertyMap) {
            if (!pair.second->empty()) {
                count++;
            }
        }
        return count;
    }
};

#endif