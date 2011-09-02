## Generated by ./build-aux/build_unit_test.sh, do not modify.

check_PROGRAMS =

# Starting a conditional unit test list.
if HAVE_MAGICKXX
if HAVE_QT
if HAVE_TESSERACT
if HAVE_TIFF
check_PROGRAMS +=  \
scribo_toolchain_nepomuk_text_extraction

scribo_toolchain_nepomuk_text_extraction_CPPFLAGS= ${TIFF_CPPFLAGS} -DHAVE_TIFF ${TESSERACT_CPPFLAGS} -DHAVE_TESSERACT ${QT_CPPFLAGS} -DHAVE_QT ${MAGICKXX_CPPFLAGS} -DHAVE_MAGICKXX ${AM_CPPFLAGS}
scribo_toolchain_nepomuk_text_extraction_LDFLAGS= ${TIFF_LDFLAGS}  ${TESSERACT_LDFLAGS}  ${QT_LDFLAGS}  ${MAGICKXX_LDFLAGS}  ${AM_LDFLAGS}
scribo_toolchain_nepomuk_text_extraction_SOURCES = scribo_toolchain_nepomuk_text_extraction.cc
endif HAVE_TIFF
endif HAVE_TESSERACT
endif HAVE_QT
endif HAVE_MAGICKXX

# Starting a conditional unit test list.
if HAVE_MAGICKXX
if HAVE_TESSERACT
if HAVE_TIFF
check_PROGRAMS +=  \
scribo_toolchain_internal_content_in_doc_functor

scribo_toolchain_internal_content_in_doc_functor_CPPFLAGS= ${TIFF_CPPFLAGS} -DHAVE_TIFF ${TESSERACT_CPPFLAGS} -DHAVE_TESSERACT ${MAGICKXX_CPPFLAGS} -DHAVE_MAGICKXX ${AM_CPPFLAGS}
scribo_toolchain_internal_content_in_doc_functor_LDFLAGS= ${TIFF_LDFLAGS}  ${TESSERACT_LDFLAGS}  ${MAGICKXX_LDFLAGS}  ${AM_LDFLAGS}
scribo_toolchain_internal_content_in_doc_functor_SOURCES = scribo_toolchain_internal_content_in_doc_functor.cc
endif HAVE_TIFF
endif HAVE_TESSERACT
endif HAVE_MAGICKXX

# Starting a conditional unit test list.
if HAVE_QT
check_PROGRAMS +=  \
scribo_convert_from_base64 \
scribo_convert_from_qimage \
scribo_io_xml_internal_full_xml_visitor \
scribo_io_xml_internal_save_image_to_xml \
scribo_io_xml_load

scribo_convert_from_base64_CPPFLAGS= ${QT_CPPFLAGS} -DHAVE_QT ${AM_CPPFLAGS}
scribo_convert_from_base64_LDFLAGS= ${QT_LDFLAGS}  ${AM_LDFLAGS}
scribo_convert_from_base64_SOURCES = scribo_convert_from_base64.cc
scribo_convert_from_qimage_CPPFLAGS= ${QT_CPPFLAGS} -DHAVE_QT ${AM_CPPFLAGS}
scribo_convert_from_qimage_LDFLAGS= ${QT_LDFLAGS}  ${AM_LDFLAGS}
scribo_convert_from_qimage_SOURCES = scribo_convert_from_qimage.cc
scribo_io_xml_internal_full_xml_visitor_CPPFLAGS= ${QT_CPPFLAGS} -DHAVE_QT ${AM_CPPFLAGS}
scribo_io_xml_internal_full_xml_visitor_LDFLAGS= ${QT_LDFLAGS}  ${AM_LDFLAGS}
scribo_io_xml_internal_full_xml_visitor_SOURCES = scribo_io_xml_internal_full_xml_visitor.cc
scribo_io_xml_internal_save_image_to_xml_CPPFLAGS= ${QT_CPPFLAGS} -DHAVE_QT ${AM_CPPFLAGS}
scribo_io_xml_internal_save_image_to_xml_LDFLAGS= ${QT_LDFLAGS}  ${AM_LDFLAGS}
scribo_io_xml_internal_save_image_to_xml_SOURCES = scribo_io_xml_internal_save_image_to_xml.cc
scribo_io_xml_load_CPPFLAGS= ${QT_CPPFLAGS} -DHAVE_QT ${AM_CPPFLAGS}
scribo_io_xml_load_LDFLAGS= ${QT_LDFLAGS}  ${AM_LDFLAGS}
scribo_io_xml_load_SOURCES = scribo_io_xml_load.cc
endif HAVE_QT

# Starting a conditional unit test list.
if HAVE_QT
if HAVE_TESSERACT
if HAVE_TIFF
check_PROGRAMS += 

endif HAVE_TIFF
endif HAVE_TESSERACT
endif HAVE_QT

# Starting a conditional unit test list.
if HAVE_TESSERACT
if HAVE_TIFF
check_PROGRAMS +=  \
scribo_text_recognition \
scribo_toolchain_internal_text_in_doc_functor \
scribo_toolchain_content_in_doc \
scribo_toolchain_text_in_doc

scribo_text_recognition_CPPFLAGS= ${TIFF_CPPFLAGS} -DHAVE_TIFF ${TESSERACT_CPPFLAGS} -DHAVE_TESSERACT ${AM_CPPFLAGS}
scribo_text_recognition_LDFLAGS= ${TIFF_LDFLAGS}  ${TESSERACT_LDFLAGS}  ${AM_LDFLAGS}
scribo_text_recognition_SOURCES = scribo_text_recognition.cc
scribo_toolchain_internal_text_in_doc_functor_CPPFLAGS= ${TIFF_CPPFLAGS} -DHAVE_TIFF ${TESSERACT_CPPFLAGS} -DHAVE_TESSERACT ${AM_CPPFLAGS}
scribo_toolchain_internal_text_in_doc_functor_LDFLAGS= ${TIFF_LDFLAGS}  ${TESSERACT_LDFLAGS}  ${AM_LDFLAGS}
scribo_toolchain_internal_text_in_doc_functor_SOURCES = scribo_toolchain_internal_text_in_doc_functor.cc
scribo_toolchain_content_in_doc_CPPFLAGS= ${TIFF_CPPFLAGS} -DHAVE_TIFF ${TESSERACT_CPPFLAGS} -DHAVE_TESSERACT ${AM_CPPFLAGS}
scribo_toolchain_content_in_doc_LDFLAGS= ${TIFF_LDFLAGS}  ${TESSERACT_LDFLAGS}  ${AM_LDFLAGS}
scribo_toolchain_content_in_doc_SOURCES = scribo_toolchain_content_in_doc.cc
scribo_toolchain_text_in_doc_CPPFLAGS= ${TIFF_CPPFLAGS} -DHAVE_TIFF ${TESSERACT_CPPFLAGS} -DHAVE_TESSERACT ${AM_CPPFLAGS}
scribo_toolchain_text_in_doc_LDFLAGS= ${TIFF_LDFLAGS}  ${TESSERACT_LDFLAGS}  ${AM_LDFLAGS}
scribo_toolchain_text_in_doc_SOURCES = scribo_toolchain_text_in_doc.cc
endif HAVE_TIFF
endif HAVE_TESSERACT

# Starting non-conditional unit test list.
check_PROGRAMS +=  \
scribo_all \
scribo_binarization_all \
scribo_binarization_global_threshold \
scribo_binarization_global_threshold_auto \
scribo_binarization_internal_compute_sauvola_threshold \
scribo_binarization_internal_first_pass_functor \
scribo_binarization_internal_sauvola_debug \
scribo_binarization_local_threshold \
scribo_binarization_sauvola \
scribo_binarization_sauvola_ms \
scribo_binarization_sauvola_ms_split \
scribo_binarization_sauvola_threshold_image \
scribo_canvas_integral_browsing \
scribo_convert_to_base64 \
scribo_core_all \
scribo_core_central_sites \
scribo_core_component_features_data \
scribo_core_component_info \
scribo_core_component_set \
scribo_core_concept_dmax_functor \
scribo_core_concept_link_functor \
scribo_core_concept_serializable \
scribo_core_concept_serialize_visitor \
scribo_core_def_color_type \
scribo_core_def_lbl_type \
scribo_core_document \
scribo_core_erase_objects \
scribo_core_group_info \
scribo_core_init_integral_image \
scribo_core_internal_doc_serializer \
scribo_core_internal_sort_comp_ids \
scribo_core_line_info \
scribo_core_line_links \
scribo_core_line_set \
scribo_core_macros \
scribo_core_object_groups \
scribo_core_object_links \
scribo_core_paragraph_info \
scribo_core_paragraph_set \
scribo_core_stats \
scribo_core_tag_anchor \
scribo_core_tag_component \
scribo_core_tag_line \
scribo_core_tag_paragraph \
scribo_debug_alignment_decision_image \
scribo_debug_all \
scribo_debug_bboxes_enlarged_image \
scribo_debug_bboxes_image \
scribo_debug_char_space_image \
scribo_debug_decision_image \
scribo_debug_highlight_text_area \
scribo_debug_line_info_image \
scribo_debug_linked_bboxes_image \
scribo_debug_links_decision_image \
scribo_debug_links_image \
scribo_debug_logger \
scribo_debug_looks_like_a_text_line_image \
scribo_debug_mean_and_base_lines_image \
scribo_debug_option_check \
scribo_debug_option_parser \
scribo_debug_save_comp_diff \
scribo_debug_save_label_image \
scribo_debug_save_table_image \
scribo_debug_text_areas_image \
scribo_debug_text_color_image \
scribo_debug_usage \
scribo_draw_all \
scribo_draw_bounding_box_links \
scribo_draw_bounding_boxes \
scribo_draw_groups_bboxes \
scribo_draw_line_components \
scribo_estim_components_features \
scribo_estim_font_boldness \
scribo_estim_font_color \
scribo_estim_internal_compute_skeleton \
scribo_estim_object_groups_mean_width \
scribo_filter_all \
scribo_filter_common_objects_photo \
scribo_filter_images_in_paragraph \
scribo_filter_internal_alignment_angle \
scribo_filter_internal_component_aligned \
scribo_filter_internal_compute \
scribo_filter_line_links_x_height \
scribo_filter_object_groups_mean_width \
scribo_filter_object_groups_size_ratio \
scribo_filter_object_groups_small \
scribo_filter_object_groups_with_holes \
scribo_filter_object_links_aligned \
scribo_filter_object_links_bbox_h_ratio \
scribo_filter_object_links_bbox_overlap \
scribo_filter_object_links_bbox_ratio \
scribo_filter_object_links_bbox_w_ratio \
scribo_filter_object_links_bottom_aligned \
scribo_filter_object_links_center_aligned \
scribo_filter_object_links_left_aligned \
scribo_filter_object_links_non_aligned_simple \
scribo_filter_object_links_right_aligned \
scribo_filter_object_links_top_aligned \
scribo_filter_objects_h_thick \
scribo_filter_objects_h_thin \
scribo_filter_objects_large \
scribo_filter_objects_on_border \
scribo_filter_objects_size_ratio \
scribo_filter_objects_small \
scribo_filter_objects_thick \
scribo_filter_objects_thin \
scribo_filter_objects_v_thick \
scribo_filter_objects_v_thin \
scribo_filter_objects_with_holes \
scribo_filter_paragraphs_bbox_overlap \
scribo_filter_paragraphs_in_borders \
scribo_filter_paragraphs_in_image \
scribo_filter_separators_in_borders \
scribo_filter_separators_in_element \
scribo_filter_separators_in_paragraph \
scribo_filter_separators_vert_in_borders \
scribo_fun_v2b_label_to_bool \
scribo_fun_v2b_objects_large_filter \
scribo_fun_v2b_objects_on_border_filter \
scribo_fun_v2b_objects_small_filter \
scribo_fun_v2v_highlight \
scribo_io_img_internal_debug_img_visitor \
scribo_io_img_internal_draw_edges \
scribo_io_img_internal_full_img_visitor \
scribo_io_img_internal_non_text_img_visitor \
scribo_io_img_internal_text_img_visitor \
scribo_io_img_save \
scribo_io_text_boxes_save \
scribo_io_xml_internal_compute_text_colour \
scribo_io_xml_internal_extended_page_xml_visitor \
scribo_io_xml_internal_page_xml_visitor \
scribo_io_xml_internal_print_box_coords \
scribo_io_xml_internal_print_image_coords \
scribo_io_xml_internal_print_page_preambule \
scribo_io_xml_internal_time_info \
scribo_io_xml_save \
scribo_make_all \
scribo_make_debug_filename \
scribo_make_influence_zone_graph \
scribo_make_text_blocks_image \
scribo_make_text_components_image \
scribo_postprocessing_all \
scribo_postprocessing_fill_object_holes \
scribo_postprocessing_images_to_drop_capital \
scribo_preprocessing_all \
scribo_preprocessing_crop \
scribo_preprocessing_crop_without_localization \
scribo_preprocessing_denoise \
scribo_preprocessing_denoise_bg \
scribo_preprocessing_denoise_fg \
scribo_preprocessing_deskew \
scribo_preprocessing_deskew_crop \
scribo_preprocessing_homogeneous_contrast \
scribo_preprocessing_rotate_90 \
scribo_preprocessing_split_bg_fg \
scribo_primitive_all \
scribo_primitive_extract_alignments \
scribo_primitive_extract_all \
scribo_primitive_extract_canvas \
scribo_primitive_extract_cells \
scribo_primitive_extract_components \
scribo_primitive_extract_horizontal_separators \
scribo_primitive_extract_internal_union \
scribo_primitive_extract_lines_discontinued \
scribo_primitive_extract_lines_h_discontinued \
scribo_primitive_extract_lines_h_pattern \
scribo_primitive_extract_lines_h_single \
scribo_primitive_extract_lines_h_thick \
scribo_primitive_extract_lines_h_thick_and_single \
scribo_primitive_extract_lines_h_thick_and_thin \
scribo_primitive_extract_lines_pattern \
scribo_primitive_extract_lines_thick \
scribo_primitive_extract_lines_v_discontinued \
scribo_primitive_extract_lines_v_pattern \
scribo_primitive_extract_lines_v_single \
scribo_primitive_extract_lines_v_thick \
scribo_primitive_extract_lines_v_thick_and_single \
scribo_primitive_extract_non_text \
scribo_primitive_extract_non_text_hdoc \
scribo_primitive_extract_non_text_kmean \
scribo_primitive_extract_separators \
scribo_primitive_extract_separators_nonvisible \
scribo_primitive_extract_vertical_separators \
scribo_primitive_group_all \
scribo_primitive_group_apply \
scribo_primitive_group_from_double_link \
scribo_primitive_group_from_double_link_any \
scribo_primitive_group_from_graph \
scribo_primitive_group_from_single_link \
scribo_primitive_identify \
scribo_primitive_internal_all \
scribo_primitive_internal_find_graph_link \
scribo_primitive_internal_find_root \
scribo_primitive_internal_have_link_valid \
scribo_primitive_internal_is_link_valid \
scribo_primitive_internal_rd \
scribo_primitive_internal_update_graph_link \
scribo_primitive_link_all \
scribo_primitive_link_compute \
scribo_primitive_link_internal_compute_anchor \
scribo_primitive_link_internal_dmax_default \
scribo_primitive_link_internal_dmax_functor_base \
scribo_primitive_link_internal_dmax_hrules \
scribo_primitive_link_internal_dmax_width_and_height \
scribo_primitive_link_internal_dmax_width_only \
scribo_primitive_link_internal_find_link \
scribo_primitive_link_internal_link_functor_base \
scribo_primitive_link_internal_link_several_dmax_base \
scribo_primitive_link_internal_link_single_dmax_base \
scribo_primitive_link_internal_link_single_dmax_ratio_aligned_base \
scribo_primitive_link_internal_link_single_dmax_ratio_aligned_delta_base \
scribo_primitive_link_internal_link_single_dmax_ratio_base \
scribo_primitive_link_merge_double_link \
scribo_primitive_link_merge_double_link_closest_aligned \
scribo_primitive_link_with_graph \
scribo_primitive_link_with_rag \
scribo_primitive_link_with_several_graphes \
scribo_primitive_link_with_several_left_links \
scribo_primitive_link_with_several_right_links \
scribo_primitive_link_with_single_down_link \
scribo_primitive_link_with_single_left_link \
scribo_primitive_link_with_single_left_link_dmax_ratio \
scribo_primitive_link_with_single_left_link_dmax_ratio_aligned \
scribo_primitive_link_with_single_right_link \
scribo_primitive_link_with_single_right_link_bottom \
scribo_primitive_link_with_single_right_link_dmax_ratio \
scribo_primitive_link_with_single_right_link_dmax_ratio_aligned \
scribo_primitive_link_with_single_right_link_top \
scribo_primitive_link_with_single_up_link \
scribo_primitive_merge_components \
scribo_primitive_regroup_from_single_left_link \
scribo_primitive_regroup_from_single_left_link_wrt_h_ratio \
scribo_primitive_remove_separators \
scribo_subsampling_bilinear \
scribo_subsampling_integral \
scribo_subsampling_integral_single_image \
scribo_table_align_lines_horizontaly \
scribo_table_align_lines_verticaly \
scribo_table_all \
scribo_table_connect_horizontal_lines \
scribo_table_connect_vertical_lines \
scribo_table_erase \
scribo_table_extract \
scribo_table_internal_align_lines \
scribo_table_internal_all \
scribo_table_internal_connect_lines \
scribo_table_internal_repair_lines \
scribo_table_rebuild \
scribo_table_repair_horizontal_lines \
scribo_table_repair_vertical_lines \
scribo_text_all \
scribo_text_clean \
scribo_text_clean_inplace \
scribo_text_extract_lines \
scribo_text_extract_lines_with_features \
scribo_text_extract_lines_wo_merge \
scribo_text_extract_paragraphs \
scribo_text_extract_paragraphs_hdoc \
scribo_text_link_lines \
scribo_text_look_like_text_lines \
scribo_text_merging \
scribo_text_merging_hdoc \
scribo_text_paragraphs_closing \
scribo_toolchain_content_in_hdoc \
scribo_toolchain_internal_content_in_hdoc_functor \
scribo_toolchain_internal_text_in_doc_preprocess_functor \
scribo_toolchain_internal_text_in_picture_functor \
scribo_toolchain_internal_toolchain_functor \
scribo_toolchain_text_in_doc_preprocess \
scribo_toolchain_text_in_picture \
scribo_upsampling_bs2x \
scribo_upsampling_eagle \
scribo_util_box_intersection \
scribo_util_box_is_included \
scribo_util_color_to_hex \
scribo_util_component_precise_outline \
scribo_util_hex_to_color

scribo_all_SOURCES = scribo_all.cc
scribo_binarization_all_SOURCES = scribo_binarization_all.cc
scribo_binarization_global_threshold_SOURCES = scribo_binarization_global_threshold.cc
scribo_binarization_global_threshold_auto_SOURCES = scribo_binarization_global_threshold_auto.cc
scribo_binarization_internal_compute_sauvola_threshold_SOURCES = scribo_binarization_internal_compute_sauvola_threshold.cc
scribo_binarization_internal_first_pass_functor_SOURCES = scribo_binarization_internal_first_pass_functor.cc
scribo_binarization_internal_sauvola_debug_SOURCES = scribo_binarization_internal_sauvola_debug.cc
scribo_binarization_local_threshold_SOURCES = scribo_binarization_local_threshold.cc
scribo_binarization_sauvola_SOURCES = scribo_binarization_sauvola.cc
scribo_binarization_sauvola_ms_SOURCES = scribo_binarization_sauvola_ms.cc
scribo_binarization_sauvola_ms_split_SOURCES = scribo_binarization_sauvola_ms_split.cc
scribo_binarization_sauvola_threshold_image_SOURCES = scribo_binarization_sauvola_threshold_image.cc
scribo_canvas_integral_browsing_SOURCES = scribo_canvas_integral_browsing.cc
scribo_convert_to_base64_SOURCES = scribo_convert_to_base64.cc
scribo_core_all_SOURCES = scribo_core_all.cc
scribo_core_central_sites_SOURCES = scribo_core_central_sites.cc
scribo_core_component_features_data_SOURCES = scribo_core_component_features_data.cc
scribo_core_component_info_SOURCES = scribo_core_component_info.cc
scribo_core_component_set_SOURCES = scribo_core_component_set.cc
scribo_core_concept_dmax_functor_SOURCES = scribo_core_concept_dmax_functor.cc
scribo_core_concept_link_functor_SOURCES = scribo_core_concept_link_functor.cc
scribo_core_concept_serializable_SOURCES = scribo_core_concept_serializable.cc
scribo_core_concept_serialize_visitor_SOURCES = scribo_core_concept_serialize_visitor.cc
scribo_core_def_color_type_SOURCES = scribo_core_def_color_type.cc
scribo_core_def_lbl_type_SOURCES = scribo_core_def_lbl_type.cc
scribo_core_document_SOURCES = scribo_core_document.cc
scribo_core_erase_objects_SOURCES = scribo_core_erase_objects.cc
scribo_core_group_info_SOURCES = scribo_core_group_info.cc
scribo_core_init_integral_image_SOURCES = scribo_core_init_integral_image.cc
scribo_core_internal_doc_serializer_SOURCES = scribo_core_internal_doc_serializer.cc
scribo_core_internal_sort_comp_ids_SOURCES = scribo_core_internal_sort_comp_ids.cc
scribo_core_line_info_SOURCES = scribo_core_line_info.cc
scribo_core_line_links_SOURCES = scribo_core_line_links.cc
scribo_core_line_set_SOURCES = scribo_core_line_set.cc
scribo_core_macros_SOURCES = scribo_core_macros.cc
scribo_core_object_groups_SOURCES = scribo_core_object_groups.cc
scribo_core_object_links_SOURCES = scribo_core_object_links.cc
scribo_core_paragraph_info_SOURCES = scribo_core_paragraph_info.cc
scribo_core_paragraph_set_SOURCES = scribo_core_paragraph_set.cc
scribo_core_stats_SOURCES = scribo_core_stats.cc
scribo_core_tag_anchor_SOURCES = scribo_core_tag_anchor.cc
scribo_core_tag_component_SOURCES = scribo_core_tag_component.cc
scribo_core_tag_line_SOURCES = scribo_core_tag_line.cc
scribo_core_tag_paragraph_SOURCES = scribo_core_tag_paragraph.cc
scribo_debug_alignment_decision_image_SOURCES = scribo_debug_alignment_decision_image.cc
scribo_debug_all_SOURCES = scribo_debug_all.cc
scribo_debug_bboxes_enlarged_image_SOURCES = scribo_debug_bboxes_enlarged_image.cc
scribo_debug_bboxes_image_SOURCES = scribo_debug_bboxes_image.cc
scribo_debug_char_space_image_SOURCES = scribo_debug_char_space_image.cc
scribo_debug_decision_image_SOURCES = scribo_debug_decision_image.cc
scribo_debug_highlight_text_area_SOURCES = scribo_debug_highlight_text_area.cc
scribo_debug_line_info_image_SOURCES = scribo_debug_line_info_image.cc
scribo_debug_linked_bboxes_image_SOURCES = scribo_debug_linked_bboxes_image.cc
scribo_debug_links_decision_image_SOURCES = scribo_debug_links_decision_image.cc
scribo_debug_links_image_SOURCES = scribo_debug_links_image.cc
scribo_debug_logger_SOURCES = scribo_debug_logger.cc
scribo_debug_looks_like_a_text_line_image_SOURCES = scribo_debug_looks_like_a_text_line_image.cc
scribo_debug_mean_and_base_lines_image_SOURCES = scribo_debug_mean_and_base_lines_image.cc
scribo_debug_option_check_SOURCES = scribo_debug_option_check.cc
scribo_debug_option_parser_SOURCES = scribo_debug_option_parser.cc
scribo_debug_save_comp_diff_SOURCES = scribo_debug_save_comp_diff.cc
scribo_debug_save_label_image_SOURCES = scribo_debug_save_label_image.cc
scribo_debug_save_table_image_SOURCES = scribo_debug_save_table_image.cc
scribo_debug_text_areas_image_SOURCES = scribo_debug_text_areas_image.cc
scribo_debug_text_color_image_SOURCES = scribo_debug_text_color_image.cc
scribo_debug_usage_SOURCES = scribo_debug_usage.cc
scribo_draw_all_SOURCES = scribo_draw_all.cc
scribo_draw_bounding_box_links_SOURCES = scribo_draw_bounding_box_links.cc
scribo_draw_bounding_boxes_SOURCES = scribo_draw_bounding_boxes.cc
scribo_draw_groups_bboxes_SOURCES = scribo_draw_groups_bboxes.cc
scribo_draw_line_components_SOURCES = scribo_draw_line_components.cc
scribo_estim_components_features_SOURCES = scribo_estim_components_features.cc
scribo_estim_font_boldness_SOURCES = scribo_estim_font_boldness.cc
scribo_estim_font_color_SOURCES = scribo_estim_font_color.cc
scribo_estim_internal_compute_skeleton_SOURCES = scribo_estim_internal_compute_skeleton.cc
scribo_estim_object_groups_mean_width_SOURCES = scribo_estim_object_groups_mean_width.cc
scribo_filter_all_SOURCES = scribo_filter_all.cc
scribo_filter_common_objects_photo_SOURCES = scribo_filter_common_objects_photo.cc
scribo_filter_images_in_paragraph_SOURCES = scribo_filter_images_in_paragraph.cc
scribo_filter_internal_alignment_angle_SOURCES = scribo_filter_internal_alignment_angle.cc
scribo_filter_internal_component_aligned_SOURCES = scribo_filter_internal_component_aligned.cc
scribo_filter_internal_compute_SOURCES = scribo_filter_internal_compute.cc
scribo_filter_line_links_x_height_SOURCES = scribo_filter_line_links_x_height.cc
scribo_filter_object_groups_mean_width_SOURCES = scribo_filter_object_groups_mean_width.cc
scribo_filter_object_groups_size_ratio_SOURCES = scribo_filter_object_groups_size_ratio.cc
scribo_filter_object_groups_small_SOURCES = scribo_filter_object_groups_small.cc
scribo_filter_object_groups_with_holes_SOURCES = scribo_filter_object_groups_with_holes.cc
scribo_filter_object_links_aligned_SOURCES = scribo_filter_object_links_aligned.cc
scribo_filter_object_links_bbox_h_ratio_SOURCES = scribo_filter_object_links_bbox_h_ratio.cc
scribo_filter_object_links_bbox_overlap_SOURCES = scribo_filter_object_links_bbox_overlap.cc
scribo_filter_object_links_bbox_ratio_SOURCES = scribo_filter_object_links_bbox_ratio.cc
scribo_filter_object_links_bbox_w_ratio_SOURCES = scribo_filter_object_links_bbox_w_ratio.cc
scribo_filter_object_links_bottom_aligned_SOURCES = scribo_filter_object_links_bottom_aligned.cc
scribo_filter_object_links_center_aligned_SOURCES = scribo_filter_object_links_center_aligned.cc
scribo_filter_object_links_left_aligned_SOURCES = scribo_filter_object_links_left_aligned.cc
scribo_filter_object_links_non_aligned_simple_SOURCES = scribo_filter_object_links_non_aligned_simple.cc
scribo_filter_object_links_right_aligned_SOURCES = scribo_filter_object_links_right_aligned.cc
scribo_filter_object_links_top_aligned_SOURCES = scribo_filter_object_links_top_aligned.cc
scribo_filter_objects_h_thick_SOURCES = scribo_filter_objects_h_thick.cc
scribo_filter_objects_h_thin_SOURCES = scribo_filter_objects_h_thin.cc
scribo_filter_objects_large_SOURCES = scribo_filter_objects_large.cc
scribo_filter_objects_on_border_SOURCES = scribo_filter_objects_on_border.cc
scribo_filter_objects_size_ratio_SOURCES = scribo_filter_objects_size_ratio.cc
scribo_filter_objects_small_SOURCES = scribo_filter_objects_small.cc
scribo_filter_objects_thick_SOURCES = scribo_filter_objects_thick.cc
scribo_filter_objects_thin_SOURCES = scribo_filter_objects_thin.cc
scribo_filter_objects_v_thick_SOURCES = scribo_filter_objects_v_thick.cc
scribo_filter_objects_v_thin_SOURCES = scribo_filter_objects_v_thin.cc
scribo_filter_objects_with_holes_SOURCES = scribo_filter_objects_with_holes.cc
scribo_filter_paragraphs_bbox_overlap_SOURCES = scribo_filter_paragraphs_bbox_overlap.cc
scribo_filter_paragraphs_in_borders_SOURCES = scribo_filter_paragraphs_in_borders.cc
scribo_filter_paragraphs_in_image_SOURCES = scribo_filter_paragraphs_in_image.cc
scribo_filter_separators_in_borders_SOURCES = scribo_filter_separators_in_borders.cc
scribo_filter_separators_in_element_SOURCES = scribo_filter_separators_in_element.cc
scribo_filter_separators_in_paragraph_SOURCES = scribo_filter_separators_in_paragraph.cc
scribo_filter_separators_vert_in_borders_SOURCES = scribo_filter_separators_vert_in_borders.cc
scribo_fun_v2b_label_to_bool_SOURCES = scribo_fun_v2b_label_to_bool.cc
scribo_fun_v2b_objects_large_filter_SOURCES = scribo_fun_v2b_objects_large_filter.cc
scribo_fun_v2b_objects_on_border_filter_SOURCES = scribo_fun_v2b_objects_on_border_filter.cc
scribo_fun_v2b_objects_small_filter_SOURCES = scribo_fun_v2b_objects_small_filter.cc
scribo_fun_v2v_highlight_SOURCES = scribo_fun_v2v_highlight.cc
scribo_io_img_internal_debug_img_visitor_SOURCES = scribo_io_img_internal_debug_img_visitor.cc
scribo_io_img_internal_draw_edges_SOURCES = scribo_io_img_internal_draw_edges.cc
scribo_io_img_internal_full_img_visitor_SOURCES = scribo_io_img_internal_full_img_visitor.cc
scribo_io_img_internal_non_text_img_visitor_SOURCES = scribo_io_img_internal_non_text_img_visitor.cc
scribo_io_img_internal_text_img_visitor_SOURCES = scribo_io_img_internal_text_img_visitor.cc
scribo_io_img_save_SOURCES = scribo_io_img_save.cc
scribo_io_text_boxes_save_SOURCES = scribo_io_text_boxes_save.cc
scribo_io_xml_internal_compute_text_colour_SOURCES = scribo_io_xml_internal_compute_text_colour.cc
scribo_io_xml_internal_extended_page_xml_visitor_SOURCES = scribo_io_xml_internal_extended_page_xml_visitor.cc
scribo_io_xml_internal_page_xml_visitor_SOURCES = scribo_io_xml_internal_page_xml_visitor.cc
scribo_io_xml_internal_print_box_coords_SOURCES = scribo_io_xml_internal_print_box_coords.cc
scribo_io_xml_internal_print_image_coords_SOURCES = scribo_io_xml_internal_print_image_coords.cc
scribo_io_xml_internal_print_page_preambule_SOURCES = scribo_io_xml_internal_print_page_preambule.cc
scribo_io_xml_internal_time_info_SOURCES = scribo_io_xml_internal_time_info.cc
scribo_io_xml_save_SOURCES = scribo_io_xml_save.cc
scribo_make_all_SOURCES = scribo_make_all.cc
scribo_make_debug_filename_SOURCES = scribo_make_debug_filename.cc
scribo_make_influence_zone_graph_SOURCES = scribo_make_influence_zone_graph.cc
scribo_make_text_blocks_image_SOURCES = scribo_make_text_blocks_image.cc
scribo_make_text_components_image_SOURCES = scribo_make_text_components_image.cc
scribo_postprocessing_all_SOURCES = scribo_postprocessing_all.cc
scribo_postprocessing_fill_object_holes_SOURCES = scribo_postprocessing_fill_object_holes.cc
scribo_postprocessing_images_to_drop_capital_SOURCES = scribo_postprocessing_images_to_drop_capital.cc
scribo_preprocessing_all_SOURCES = scribo_preprocessing_all.cc
scribo_preprocessing_crop_SOURCES = scribo_preprocessing_crop.cc
scribo_preprocessing_crop_without_localization_SOURCES = scribo_preprocessing_crop_without_localization.cc
scribo_preprocessing_denoise_SOURCES = scribo_preprocessing_denoise.cc
scribo_preprocessing_denoise_bg_SOURCES = scribo_preprocessing_denoise_bg.cc
scribo_preprocessing_denoise_fg_SOURCES = scribo_preprocessing_denoise_fg.cc
scribo_preprocessing_deskew_SOURCES = scribo_preprocessing_deskew.cc
scribo_preprocessing_deskew_crop_SOURCES = scribo_preprocessing_deskew_crop.cc
scribo_preprocessing_homogeneous_contrast_SOURCES = scribo_preprocessing_homogeneous_contrast.cc
scribo_preprocessing_rotate_90_SOURCES = scribo_preprocessing_rotate_90.cc
scribo_preprocessing_split_bg_fg_SOURCES = scribo_preprocessing_split_bg_fg.cc
scribo_primitive_all_SOURCES = scribo_primitive_all.cc
scribo_primitive_extract_alignments_SOURCES = scribo_primitive_extract_alignments.cc
scribo_primitive_extract_all_SOURCES = scribo_primitive_extract_all.cc
scribo_primitive_extract_canvas_SOURCES = scribo_primitive_extract_canvas.cc
scribo_primitive_extract_cells_SOURCES = scribo_primitive_extract_cells.cc
scribo_primitive_extract_components_SOURCES = scribo_primitive_extract_components.cc
scribo_primitive_extract_horizontal_separators_SOURCES = scribo_primitive_extract_horizontal_separators.cc
scribo_primitive_extract_internal_union_SOURCES = scribo_primitive_extract_internal_union.cc
scribo_primitive_extract_lines_discontinued_SOURCES = scribo_primitive_extract_lines_discontinued.cc
scribo_primitive_extract_lines_h_discontinued_SOURCES = scribo_primitive_extract_lines_h_discontinued.cc
scribo_primitive_extract_lines_h_pattern_SOURCES = scribo_primitive_extract_lines_h_pattern.cc
scribo_primitive_extract_lines_h_single_SOURCES = scribo_primitive_extract_lines_h_single.cc
scribo_primitive_extract_lines_h_thick_SOURCES = scribo_primitive_extract_lines_h_thick.cc
scribo_primitive_extract_lines_h_thick_and_single_SOURCES = scribo_primitive_extract_lines_h_thick_and_single.cc
scribo_primitive_extract_lines_h_thick_and_thin_SOURCES = scribo_primitive_extract_lines_h_thick_and_thin.cc
scribo_primitive_extract_lines_pattern_SOURCES = scribo_primitive_extract_lines_pattern.cc
scribo_primitive_extract_lines_thick_SOURCES = scribo_primitive_extract_lines_thick.cc
scribo_primitive_extract_lines_v_discontinued_SOURCES = scribo_primitive_extract_lines_v_discontinued.cc
scribo_primitive_extract_lines_v_pattern_SOURCES = scribo_primitive_extract_lines_v_pattern.cc
scribo_primitive_extract_lines_v_single_SOURCES = scribo_primitive_extract_lines_v_single.cc
scribo_primitive_extract_lines_v_thick_SOURCES = scribo_primitive_extract_lines_v_thick.cc
scribo_primitive_extract_lines_v_thick_and_single_SOURCES = scribo_primitive_extract_lines_v_thick_and_single.cc
scribo_primitive_extract_non_text_SOURCES = scribo_primitive_extract_non_text.cc
scribo_primitive_extract_non_text_hdoc_SOURCES = scribo_primitive_extract_non_text_hdoc.cc
scribo_primitive_extract_non_text_kmean_SOURCES = scribo_primitive_extract_non_text_kmean.cc
scribo_primitive_extract_separators_SOURCES = scribo_primitive_extract_separators.cc
scribo_primitive_extract_separators_nonvisible_SOURCES = scribo_primitive_extract_separators_nonvisible.cc
scribo_primitive_extract_vertical_separators_SOURCES = scribo_primitive_extract_vertical_separators.cc
scribo_primitive_group_all_SOURCES = scribo_primitive_group_all.cc
scribo_primitive_group_apply_SOURCES = scribo_primitive_group_apply.cc
scribo_primitive_group_from_double_link_SOURCES = scribo_primitive_group_from_double_link.cc
scribo_primitive_group_from_double_link_any_SOURCES = scribo_primitive_group_from_double_link_any.cc
scribo_primitive_group_from_graph_SOURCES = scribo_primitive_group_from_graph.cc
scribo_primitive_group_from_single_link_SOURCES = scribo_primitive_group_from_single_link.cc
scribo_primitive_identify_SOURCES = scribo_primitive_identify.cc
scribo_primitive_internal_all_SOURCES = scribo_primitive_internal_all.cc
scribo_primitive_internal_find_graph_link_SOURCES = scribo_primitive_internal_find_graph_link.cc
scribo_primitive_internal_find_root_SOURCES = scribo_primitive_internal_find_root.cc
scribo_primitive_internal_have_link_valid_SOURCES = scribo_primitive_internal_have_link_valid.cc
scribo_primitive_internal_is_link_valid_SOURCES = scribo_primitive_internal_is_link_valid.cc
scribo_primitive_internal_rd_SOURCES = scribo_primitive_internal_rd.cc
scribo_primitive_internal_update_graph_link_SOURCES = scribo_primitive_internal_update_graph_link.cc
scribo_primitive_link_all_SOURCES = scribo_primitive_link_all.cc
scribo_primitive_link_compute_SOURCES = scribo_primitive_link_compute.cc
scribo_primitive_link_internal_compute_anchor_SOURCES = scribo_primitive_link_internal_compute_anchor.cc
scribo_primitive_link_internal_dmax_default_SOURCES = scribo_primitive_link_internal_dmax_default.cc
scribo_primitive_link_internal_dmax_functor_base_SOURCES = scribo_primitive_link_internal_dmax_functor_base.cc
scribo_primitive_link_internal_dmax_hrules_SOURCES = scribo_primitive_link_internal_dmax_hrules.cc
scribo_primitive_link_internal_dmax_width_and_height_SOURCES = scribo_primitive_link_internal_dmax_width_and_height.cc
scribo_primitive_link_internal_dmax_width_only_SOURCES = scribo_primitive_link_internal_dmax_width_only.cc
scribo_primitive_link_internal_find_link_SOURCES = scribo_primitive_link_internal_find_link.cc
scribo_primitive_link_internal_link_functor_base_SOURCES = scribo_primitive_link_internal_link_functor_base.cc
scribo_primitive_link_internal_link_several_dmax_base_SOURCES = scribo_primitive_link_internal_link_several_dmax_base.cc
scribo_primitive_link_internal_link_single_dmax_base_SOURCES = scribo_primitive_link_internal_link_single_dmax_base.cc
scribo_primitive_link_internal_link_single_dmax_ratio_aligned_base_SOURCES = scribo_primitive_link_internal_link_single_dmax_ratio_aligned_base.cc
scribo_primitive_link_internal_link_single_dmax_ratio_aligned_delta_base_SOURCES = scribo_primitive_link_internal_link_single_dmax_ratio_aligned_delta_base.cc
scribo_primitive_link_internal_link_single_dmax_ratio_base_SOURCES = scribo_primitive_link_internal_link_single_dmax_ratio_base.cc
scribo_primitive_link_merge_double_link_SOURCES = scribo_primitive_link_merge_double_link.cc
scribo_primitive_link_merge_double_link_closest_aligned_SOURCES = scribo_primitive_link_merge_double_link_closest_aligned.cc
scribo_primitive_link_with_graph_SOURCES = scribo_primitive_link_with_graph.cc
scribo_primitive_link_with_rag_SOURCES = scribo_primitive_link_with_rag.cc
scribo_primitive_link_with_several_graphes_SOURCES = scribo_primitive_link_with_several_graphes.cc
scribo_primitive_link_with_several_left_links_SOURCES = scribo_primitive_link_with_several_left_links.cc
scribo_primitive_link_with_several_right_links_SOURCES = scribo_primitive_link_with_several_right_links.cc
scribo_primitive_link_with_single_down_link_SOURCES = scribo_primitive_link_with_single_down_link.cc
scribo_primitive_link_with_single_left_link_SOURCES = scribo_primitive_link_with_single_left_link.cc
scribo_primitive_link_with_single_left_link_dmax_ratio_SOURCES = scribo_primitive_link_with_single_left_link_dmax_ratio.cc
scribo_primitive_link_with_single_left_link_dmax_ratio_aligned_SOURCES = scribo_primitive_link_with_single_left_link_dmax_ratio_aligned.cc
scribo_primitive_link_with_single_right_link_SOURCES = scribo_primitive_link_with_single_right_link.cc
scribo_primitive_link_with_single_right_link_bottom_SOURCES = scribo_primitive_link_with_single_right_link_bottom.cc
scribo_primitive_link_with_single_right_link_dmax_ratio_SOURCES = scribo_primitive_link_with_single_right_link_dmax_ratio.cc
scribo_primitive_link_with_single_right_link_dmax_ratio_aligned_SOURCES = scribo_primitive_link_with_single_right_link_dmax_ratio_aligned.cc
scribo_primitive_link_with_single_right_link_top_SOURCES = scribo_primitive_link_with_single_right_link_top.cc
scribo_primitive_link_with_single_up_link_SOURCES = scribo_primitive_link_with_single_up_link.cc
scribo_primitive_merge_components_SOURCES = scribo_primitive_merge_components.cc
scribo_primitive_regroup_from_single_left_link_SOURCES = scribo_primitive_regroup_from_single_left_link.cc
scribo_primitive_regroup_from_single_left_link_wrt_h_ratio_SOURCES = scribo_primitive_regroup_from_single_left_link_wrt_h_ratio.cc
scribo_primitive_remove_separators_SOURCES = scribo_primitive_remove_separators.cc
scribo_subsampling_bilinear_SOURCES = scribo_subsampling_bilinear.cc
scribo_subsampling_integral_SOURCES = scribo_subsampling_integral.cc
scribo_subsampling_integral_single_image_SOURCES = scribo_subsampling_integral_single_image.cc
scribo_table_align_lines_horizontaly_SOURCES = scribo_table_align_lines_horizontaly.cc
scribo_table_align_lines_verticaly_SOURCES = scribo_table_align_lines_verticaly.cc
scribo_table_all_SOURCES = scribo_table_all.cc
scribo_table_connect_horizontal_lines_SOURCES = scribo_table_connect_horizontal_lines.cc
scribo_table_connect_vertical_lines_SOURCES = scribo_table_connect_vertical_lines.cc
scribo_table_erase_SOURCES = scribo_table_erase.cc
scribo_table_extract_SOURCES = scribo_table_extract.cc
scribo_table_internal_align_lines_SOURCES = scribo_table_internal_align_lines.cc
scribo_table_internal_all_SOURCES = scribo_table_internal_all.cc
scribo_table_internal_connect_lines_SOURCES = scribo_table_internal_connect_lines.cc
scribo_table_internal_repair_lines_SOURCES = scribo_table_internal_repair_lines.cc
scribo_table_rebuild_SOURCES = scribo_table_rebuild.cc
scribo_table_repair_horizontal_lines_SOURCES = scribo_table_repair_horizontal_lines.cc
scribo_table_repair_vertical_lines_SOURCES = scribo_table_repair_vertical_lines.cc
scribo_text_all_SOURCES = scribo_text_all.cc
scribo_text_clean_SOURCES = scribo_text_clean.cc
scribo_text_clean_inplace_SOURCES = scribo_text_clean_inplace.cc
scribo_text_extract_lines_SOURCES = scribo_text_extract_lines.cc
scribo_text_extract_lines_with_features_SOURCES = scribo_text_extract_lines_with_features.cc
scribo_text_extract_lines_wo_merge_SOURCES = scribo_text_extract_lines_wo_merge.cc
scribo_text_extract_paragraphs_SOURCES = scribo_text_extract_paragraphs.cc
scribo_text_extract_paragraphs_hdoc_SOURCES = scribo_text_extract_paragraphs_hdoc.cc
scribo_text_link_lines_SOURCES = scribo_text_link_lines.cc
scribo_text_look_like_text_lines_SOURCES = scribo_text_look_like_text_lines.cc
scribo_text_merging_SOURCES = scribo_text_merging.cc
scribo_text_merging_hdoc_SOURCES = scribo_text_merging_hdoc.cc
scribo_text_paragraphs_closing_SOURCES = scribo_text_paragraphs_closing.cc
scribo_toolchain_content_in_hdoc_SOURCES = scribo_toolchain_content_in_hdoc.cc
scribo_toolchain_internal_content_in_hdoc_functor_SOURCES = scribo_toolchain_internal_content_in_hdoc_functor.cc
scribo_toolchain_internal_text_in_doc_preprocess_functor_SOURCES = scribo_toolchain_internal_text_in_doc_preprocess_functor.cc
scribo_toolchain_internal_text_in_picture_functor_SOURCES = scribo_toolchain_internal_text_in_picture_functor.cc
scribo_toolchain_internal_toolchain_functor_SOURCES = scribo_toolchain_internal_toolchain_functor.cc
scribo_toolchain_text_in_doc_preprocess_SOURCES = scribo_toolchain_text_in_doc_preprocess.cc
scribo_toolchain_text_in_picture_SOURCES = scribo_toolchain_text_in_picture.cc
scribo_upsampling_bs2x_SOURCES = scribo_upsampling_bs2x.cc
scribo_upsampling_eagle_SOURCES = scribo_upsampling_eagle.cc
scribo_util_box_intersection_SOURCES = scribo_util_box_intersection.cc
scribo_util_box_is_included_SOURCES = scribo_util_box_is_included.cc
scribo_util_color_to_hex_SOURCES = scribo_util_color_to_hex.cc
scribo_util_component_precise_outline_SOURCES = scribo_util_component_precise_outline.cc
scribo_util_hex_to_color_SOURCES = scribo_util_hex_to_color.cc

