#include <stdio.h>
#include <stdlib.h>

#include "k_utils.h"

char *k_read_txt_file(const char *file_path, char *buf, size_t buf_size, size_t *get_len) {

    if (NULL == file_path || '\0' == file_path[0])
        return NULL;

    char *txt_buf = buf;

    FILE *file = fopen(file_path, "r");
    if (file == NULL)
        return NULL;

    if (0 != fseek(file, 0, SEEK_END))
        goto err;

    long file_size = ftell(file);
    if (file_size == -1L)
        goto err;

    if (0 != fseek(file, 0, SEEK_SET))
        goto err;

    size_t required_size = file_size + 1;

    if (NULL == buf || buf_size < required_size) {
        txt_buf = malloc(required_size);
        if (NULL == txt_buf)
            goto err;
    }

    if (file_size != 0) {
        if (0 == fread(txt_buf, 1, file_size, file))
            goto err;
    }

    fclose(file);

    txt_buf[file_size] = '\0';

    if (NULL != get_len)
        *get_len = file_size;

    return txt_buf;

err:
    if (txt_buf != buf)
        free(txt_buf);

    fclose(file);

    return NULL;
}

