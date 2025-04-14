#ifndef K_JSON_V2_H
#define K_JSON_V2_H

struct k_json;

struct k_json *k_json_parse(const char *text);

void k_json_free(struct k_json *json);

#endif
