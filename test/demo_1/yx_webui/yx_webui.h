#ifndef YX_WEBUI_H
#define YX_WEBUI_H

struct yx_webui_float {

    void *data;

    int (*fn_set)(struct yx_webui_float *f, float val);

    float (*fn_get)(struct yx_webui_float *f);
};

void yx_webui_bind_float(float *val);

#endif
