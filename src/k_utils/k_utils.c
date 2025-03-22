#include <stdio.h>
#include <stdlib.h>

#include "k_utils.h"

char *k_read_txt_file(const char *file_path, char *buf, size_t buf_size, size_t *get_len) {

    if (NULL == file_path)
        return NULL;

    char *txt_buf = buf;

    FILE *file = fopen(file_path, "rb");
    if (file == NULL)
        return NULL;

    if (0 != fseek(file, 0, SEEK_END))
        goto err;

    long txt_len = ftell(file);
    if (txt_len == -1L)
        goto err;

    if (0 != fseek(file, 0, SEEK_SET))
        goto err;

    size_t required_size = txt_len + 1;

    if (NULL == buf || buf_size < required_size) {
        if (NULL == (txt_buf = malloc(required_size)))
            goto err;
    }

    // TODO
    if (0 == fread(txt_buf, 1, txt_len, file))
        goto err;

    fclose(file);

    txt_buf[txt_len] = '\0';

    if (NULL != get_len)
        *get_len = txt_len;

    return txt_buf;

err:
    if (txt_buf != buf)
        free(txt_buf);

    fclose(file);

    return NULL;
}

