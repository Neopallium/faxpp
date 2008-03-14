/*
 * Copyright 2007 Doxological Ltd.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __FAXPP__TOKENIZER_STATES_H
#define __FAXPP__TOKENIZER_STATES_H

#include <stdlib.h>
#include <string.h>

#include <faxpp/error.h>
#include "xml_tokenizer.h"

/*********************
 *
 *  Tokenizer State Functions
 *
 *********************/

FAXPP_Error default_start_element_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_start_element_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_start_element_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error default_start_element_mandatory_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_start_element_ws_state(FAXPP_TokenizerEnv *env);

FAXPP_Error default_ns_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error default_ns_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error default_ns_name_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error default_ns_name_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error default_ns_name_state5(FAXPP_TokenizerEnv *env);

FAXPP_Error default_attr_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_equals_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_value_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_value_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_attr_value_state_en(FAXPP_TokenizerEnv *env);

FAXPP_Error default_element_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_element_content_rsquare_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error default_element_content_rsquare_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error default_element_content_markup_state(FAXPP_TokenizerEnv *env);

FAXPP_Error default_end_element_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_end_element_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error default_end_element_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error default_end_element_name_seen_colon_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error utf8_start_element_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_start_element_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_start_element_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_start_element_mandatory_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_start_element_ws_state(FAXPP_TokenizerEnv *env);

FAXPP_Error utf8_ns_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_ns_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_ns_name_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_ns_name_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_ns_name_state5(FAXPP_TokenizerEnv *env);

FAXPP_Error utf8_attr_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_equals_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_value_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_value_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_attr_value_state_en(FAXPP_TokenizerEnv *env);

FAXPP_Error utf8_element_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_element_content_rsquare_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_element_content_rsquare_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_element_content_markup_state(FAXPP_TokenizerEnv *env);

FAXPP_Error utf8_end_element_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_end_element_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_end_element_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf8_end_element_name_seen_colon_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error utf16_start_element_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_start_element_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_start_element_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_start_element_mandatory_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_start_element_ws_state(FAXPP_TokenizerEnv *env);

FAXPP_Error utf16_ns_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_ns_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_ns_name_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_ns_name_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_ns_name_state5(FAXPP_TokenizerEnv *env);

FAXPP_Error utf16_attr_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_equals_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_value_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_value_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_attr_value_state_en(FAXPP_TokenizerEnv *env);

FAXPP_Error utf16_element_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_element_content_rsquare_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_element_content_rsquare_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_element_content_markup_state(FAXPP_TokenizerEnv *env);

FAXPP_Error utf16_end_element_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_end_element_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_end_element_name_seen_colon_state(FAXPP_TokenizerEnv *env);
FAXPP_Error utf16_end_element_name_seen_colon_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error ws_plus_state(FAXPP_TokenizerEnv *env);
FAXPP_Error ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error equals_state(FAXPP_TokenizerEnv *env);

FAXPP_Error initial_state(FAXPP_TokenizerEnv *env);
FAXPP_Error initial_misc_state(FAXPP_TokenizerEnv *env);
FAXPP_Error initial_markup_state(FAXPP_TokenizerEnv *env);
FAXPP_Error parsed_entity_state(FAXPP_TokenizerEnv *env);

FAXPP_Error final_state(FAXPP_TokenizerEnv *env);
FAXPP_Error final_markup_state(FAXPP_TokenizerEnv *env);
FAXPP_Error end_of_buffer_state(FAXPP_TokenizerEnv *env);

FAXPP_Error comment_start_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error comment_start_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error comment_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error comment_content_seen_dash_state(FAXPP_TokenizerEnv *env);
FAXPP_Error comment_content_seen_dash_twice_state(FAXPP_TokenizerEnv *env);

FAXPP_Error pi_name_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_name_x_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_name_m_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_name_l_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pi_content_seen_question_state(FAXPP_TokenizerEnv *env);

FAXPP_Error start_element_end_state(FAXPP_TokenizerEnv *env);
FAXPP_Error self_closing_element_state(FAXPP_TokenizerEnv *env);

FAXPP_Error end_element_ws_state(FAXPP_TokenizerEnv *env);

FAXPP_Error reference_state(FAXPP_TokenizerEnv *env);
FAXPP_Error a_entity_reference_state(FAXPP_TokenizerEnv *env);
FAXPP_Error amp_entity_reference_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error amp_entity_reference_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error apos_entity_reference_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error apos_entity_reference_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error apos_entity_reference_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error gt_entity_reference_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error gt_entity_reference_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error lt_entity_reference_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error lt_entity_reference_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error quot_entity_reference_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error quot_entity_reference_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error quot_entity_reference_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error quot_entity_reference_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error entity_reference_state(FAXPP_TokenizerEnv *env);
FAXPP_Error parameter_entity_reference_state(FAXPP_TokenizerEnv *env);
FAXPP_Error char_reference_state(FAXPP_TokenizerEnv *env);
FAXPP_Error dec_char_reference_state(FAXPP_TokenizerEnv *env);
FAXPP_Error hex_char_reference_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error hex_char_reference_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error cdata_or_comment_state(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_start_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_start_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_start_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_start_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_start_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_start_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_end_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error cdata_end_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error xml_decl_or_markup_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_or_pi_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_or_pi_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_or_pi_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_or_pi_state4(FAXPP_TokenizerEnv *env);

FAXPP_Error xml_decl_version_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_value_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_value_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_value_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_value_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_version_value_apos_state(FAXPP_TokenizerEnv *env);

FAXPP_Error xml_decl_encoding_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_state8(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_value_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_value_quot_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_value_quot_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_value_apos_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_encoding_value_apos_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error xml_decl_standalone_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state8(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state9(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_state10(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_value_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_value_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_yes_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_yes_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_no_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_standalone_value_apos_state(FAXPP_TokenizerEnv *env);

FAXPP_Error xml_decl_question_state(FAXPP_TokenizerEnv *env);
FAXPP_Error xml_decl_seen_question_state(FAXPP_TokenizerEnv *env);

FAXPP_Error doctype_or_comment_state(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_initial_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_initial_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_name_seen_colon_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_after_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_internal_subset_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error doctype_end_state(FAXPP_TokenizerEnv *env);

FAXPP_Error internal_subset_state(FAXPP_TokenizerEnv *env);
FAXPP_Error internal_subset_state_en(FAXPP_TokenizerEnv *env);
FAXPP_Error internal_subset_markup_state(FAXPP_TokenizerEnv *env);
FAXPP_Error internal_subset_decl_state(FAXPP_TokenizerEnv *env);

FAXPP_Error external_subset_state(FAXPP_TokenizerEnv *env);
FAXPP_Error external_subset_seen_rsquare_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error external_subset_seen_rsquare_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error external_subset_markup_state(FAXPP_TokenizerEnv *env);
FAXPP_Error external_subset_decl_state(FAXPP_TokenizerEnv *env);

FAXPP_Error system_id_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error system_id_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error system_id_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error system_id_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error system_id_initial_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error system_id_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error system_literal_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error system_literal_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error system_literal_quot_state(FAXPP_TokenizerEnv *env);

FAXPP_Error public_id_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_initial_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pubid_literal_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pubid_literal_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error pubid_literal_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_ws_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error public_id_ws_state3(FAXPP_TokenizerEnv *env);

FAXPP_Error elementdecl_or_entitydecl_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_initial_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_name_seen_colon_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_empty_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_empty_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_empty_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_empty_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_any_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_any_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_mixed_or_children_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_cp_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_cp_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_cp_name_seen_colon_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_cp_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_cp_cardinality_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_cp_separator_or_end_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_end_or_names_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_optional_star_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_end_or_names_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_star_state(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_name_seen_colon_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_pcdata_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error elementdecl_end_state(FAXPP_TokenizerEnv *env);

FAXPP_Error attlistdecl_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_initial_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_initial_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_name_seen_colon_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_name_seen_colon_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attdef_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attdef_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attdef_name_seen_colon_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attdef_name_seen_colon_state2(FAXPP_TokenizerEnv *env);

FAXPP_Error attlistdecl_atttype_state(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_cdata_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_cdata_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_cdata_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_cdata_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_id_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_id_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_idref_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_idref_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_idref_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entity_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entity_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entity_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entity_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entity_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entities_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_entities_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_nmtoken_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_notation_separator_state(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_enumeration_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_enumeration_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_atttype_enumeration_separator_state(FAXPP_TokenizerEnv *env);

FAXPP_Error attlistdecl_default_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_implied_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_implied_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_implied_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_implied_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_implied_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_implied_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_required_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_fixed_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_fixed_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_fixed_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_default_fixed_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attvalue_start_state(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attvalue_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error attlistdecl_attvalue_quot_state(FAXPP_TokenizerEnv *env);

FAXPP_Error notationdecl_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_initial_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_initial_state6(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_initial_state7(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error notationdecl_end_state(FAXPP_TokenizerEnv *env);

FAXPP_Error entitydecl_initial_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_initial_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_initial_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_initial_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_param_or_general_state(FAXPP_TokenizerEnv *env);

FAXPP_Error entitydecl_name_state(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_value_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ws_state(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_or_end_state(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_ndata_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error entitydecl_end_state(FAXPP_TokenizerEnv *env);

FAXPP_Error paramentitydecl_name_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error paramentitydecl_name_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error paramentitydecl_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error paramentitydecl_value_apos_state(FAXPP_TokenizerEnv *env);
FAXPP_Error paramentitydecl_value_quot_state(FAXPP_TokenizerEnv *env);
FAXPP_Error paramentitydecl_end_state(FAXPP_TokenizerEnv *env);

FAXPP_Error conditional_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error conditional_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_content_state(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_content_seen_lt_state(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_content_seen_bang_state(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_content_seen_rsquare_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error ignore_content_seen_rsquare_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error include_state1(FAXPP_TokenizerEnv *env);
FAXPP_Error include_state2(FAXPP_TokenizerEnv *env);
FAXPP_Error include_state3(FAXPP_TokenizerEnv *env);
FAXPP_Error include_state4(FAXPP_TokenizerEnv *env);
FAXPP_Error include_state5(FAXPP_TokenizerEnv *env);
FAXPP_Error include_state6(FAXPP_TokenizerEnv *env);


/*********************
 *
 *  Tokenizer Helper Functions
 *
 *********************/

#ifdef DEBUG
const char *FAXPP_state_to_string(FAXPP_StateFunction state);
#endif

#define read_char_no_check(env) \
{ \
  (env)->char_len = \
    (env)->decode((env)->position, (env)->buffer_end, &(env)->current_char); \
  switch((env)->char_len) { \
  case TRANSCODE_PREMATURE_END_OF_BUFFER: \
    return PREMATURE_END_OF_BUFFER; \
  case TRANSCODE_BAD_ENCODING: \
    return BAD_ENCODING; \
  } \
\
/*   printf("%03d:%03d State: %s, Byte: %c, Char: %08X\n", (env)->line, (env)->column, */ \
/*          FAXPP_state_to_string((env)->state), *(unsigned char*)(env)->position, */ \
/*          (env)->current_char); */ \
}

#define read_char(env) \
{ \
  if((env)->position >= (env)->buffer_end) \
    return PREMATURE_END_OF_BUFFER; \
  read_char_no_check((env)); \
}

#define next_char_append(env) \
{ \
  FAXPP_Error err = FAXPP_buffer_append_ch(&(env)->token_buffer, (env)->transcoder.encode, (env)->current_char); \
  if(err != 0) return err; \
}

#define next_char_position(env) \
{ \
  (env)->column += 1; \
  (env)->position += (env)->char_len; \
}

#define next_char(env) \
{ \
  if((env)->token_buffer.cursor) { \
    next_char_append((env)); \
  } \
\
  next_char_position((env)); \
}

#define token_start_position(env) \
{ \
  if((env)->do_encode) { \
    FAXPP_reset_buffer(&(env)->token_buffer); \
    (env)->token.value.ptr = (env)->token_buffer.cursor; \
  } else { \
    (env)->token.value.ptr = (env)->position; \
    (env)->token_buffer.cursor = 0; \
  } \
  (env)->token.value.len = 0; \
  (env)->token.line = (env)->line; \
  (env)->token.column = (env)->column; \
}

#define token_end_position(env) \
{ \
  if((env)->token_buffer.cursor) { \
    (env)->token.value.len = (env)->token_buffer.cursor - (env)->token.value.ptr; \
    (env)->token_buffer.cursor = 0; \
  } else { \
    (env)->token.value.len = (env)->position - (env)->token.value.ptr; \
  } \
}

#define base_state(env) \
{ \
  if((env)->external_subset || (env)->external_dtd_entity) \
    (env)->state = external_subset_state; \
  else if((env)->nesting_level != 0) \
    (env)->state = (env)->element_content_state; \
  else if((env)->internal_dtd_entity) \
    (env)->state = internal_subset_state_en; \
  else if((env)->element_entity || (env)->external_parsed_entity) \
    (env)->state = parsed_entity_state; \
  else if((env)->seen_doc_element) \
    (env)->state = final_state; \
  else if((env)->internal_subset) \
    (env)->state = internal_subset_state; \
  else (env)->state = initial_misc_state; \
}

#define report_token(token_int, env) \
{ \
  if((env)->token.value.len != 0) { \
    memcpy(&(env)->result_token, &(env)->token, sizeof(FAXPP_Token)); \
    (env)->result_token.type = (token_int); \
    (env)->token.value.ptr = 0; \
    (env)->token.value.len = 0; \
  } \
}

#define report_token_maybe_empty(token_int, env) \
{ \
  if((env)->token.value.len == 0) (env)->token.value.ptr = 0; \
  memcpy(&(env)->result_token, &(env)->token, sizeof(FAXPP_Token)); \
  (env)->result_token.type = (token_int); \
  (env)->token.value.ptr = 0; \
  (env)->token.value.len = 0; \
}

#define report_empty_token(token_int, env) \
{ \
  (env)->result_token.type = (token_int); \
  (env)->result_token.value.ptr = 0; \
  (env)->result_token.value.len = 0; \
  (env)->result_token.line = (env)->line; \
  (env)->result_token.column = (env)->column; \
}

#define store_state(env) (env)->stored_state = (env)->state

#define retrieve_state(env) \
{ \
  (env)->state = (env)->stored_state; \
  (env)->stored_state = 0; \
}

#define LINE_ENDINGS_LABEL(label) \
  case '\r': { \
    Char32 next_char; \
    if((env)->position + (env)->char_len >= (env)->buffer_end || \
       (env)->decode((env)->position + (env)->char_len, (env)->buffer_end, &next_char) \
       == TRANSCODE_PREMATURE_END_OF_BUFFER) { \
      if(!(env)->buffer_done) return PREMATURE_END_OF_BUFFER; \
      goto LINE_ENDINGS_INC ## label; \
    } \
    else if(next_char != '\n') goto LINE_ENDINGS_INC ## label; \
    goto LINE_ENDINGS_END ## label; \
  } \
  case '\n': \
LINE_ENDINGS_INC ## label: \
    (env)->line += 1; \
    (env)->column = (unsigned int)-1; \
LINE_ENDINGS_END ## label:

#define LINE_ENDINGS LINE_ENDINGS_LABEL(0)

/*
 * [3]     S    ::=    (#x20 | #x9 | #xD | #xA)+
 */
#define WHITESPACE \
  LINE_ENDINGS \
  case '\t': \
  case ' '

#endif
