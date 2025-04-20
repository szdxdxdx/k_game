#include <stdio.h>
#include <stdlib.h>

#include "k_read_file.h"

char *k_read_txt_file(const char *file_path, char *buf, size_t buf_size, size_t *get_len) {

    char *txt_buf = buf;
    FILE *file = NULL;

    file = fopen(file_path, "r");
    if (file == NULL)
        goto err;

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

    size_t count;
    if (file_size != 0) {
        count = fread(txt_buf, 1, file_size, file);
        if (0 == count || file_size < count)
            goto err;

        txt_buf[count] = '\0';
    }

    fclose(file);

    if (NULL != get_len) {
        *get_len = file_size;
    }

    return txt_buf;

err:
    if (txt_buf != buf) {
        free(txt_buf);
    }
    if (NULL != file) {
        fclose(file);
    }

    return NULL;
}

