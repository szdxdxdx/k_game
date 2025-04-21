#ifndef LLK_UI_BUILD_H
#define LLK_UI_BUILD_H

#include "./llk_ui_fwd.h"

struct llk_ui_context *llk_ui_build_context_from_xml_file(const char *file_path);

struct llk_ui_elem *llk_ui_build_elem_from_xml_file(struct llk_ui_context *ui, const char *file_path);

#endif
