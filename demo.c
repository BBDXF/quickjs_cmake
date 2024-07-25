// quickjs.cpp: 定义应用程序的入口点。
//
#include "demo.h"
#include "cutils.h"
#include "quickjs.h"
#include "quickjs-libc.h"


JSRuntime *rt;
JSContext *ctx;

int eval_buf(JSContext *ctx, const void *buf, int buf_len,
                    const char *filename, int eval_flags)
{
    JSValue val;
    int ret;

    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE) {
        /* for the modules, we compile then run to be able to set
           import.meta */
        val = JS_Eval(ctx, buf, buf_len, filename,
                      eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
        if (!JS_IsException(val)) {
            js_module_set_import_meta(ctx, val, TRUE, TRUE);
            val = JS_EvalFunction(ctx, val);
        }
        val = js_std_await(ctx, val);
    } else {
        val = JS_Eval(ctx, buf, buf_len, filename, eval_flags);
    }
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        ret = -1;
    } else {
        ret = 0;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

int eval_file(JSContext *ctx, const char *filename, int module)
{
    uint8_t *buf;
    int ret, eval_flags;
    size_t buf_len;

    buf = js_load_file(ctx, &buf_len, filename);
    if (!buf) {
        perror(filename);
        exit(1);
    }

    if (module < 0) {
        module = (has_suffix(filename, ".mjs") ||
                  JS_DetectModule((const char *)buf, buf_len));
    }
    if (module)
        eval_flags = JS_EVAL_TYPE_MODULE;
    else
        eval_flags = JS_EVAL_TYPE_GLOBAL;
    ret = eval_buf(ctx, buf, buf_len, filename, eval_flags);
    js_free(ctx, buf);
    return ret;
}

BOOL qjs_init(){
	rt = JS_NewRuntime();
    if(!rt) return FALSE;

	ctx = JS_NewContext(rt);
    if(!ctx) return FALSE;
    
    js_std_init_handlers(rt);

    /* bignum support */
    JS_AddIntrinsicBigFloat(ctx);
    JS_AddIntrinsicBigDecimal(ctx);
    JS_AddIntrinsicOperators(ctx);
    JS_EnableBignumExt(ctx, 1);

    /* std, os modules */
    js_init_module_std(ctx, "std");
    js_init_module_os(ctx, "os");
	js_std_add_helpers(ctx, 0, NULL);

	/* loader for ES6 js modules or xxx.so file */
    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);

    /* make 'std' and 'os' visible to non module code */
    if (FALSE) {
        const char *str = "import * as std from 'std';\n"
            "import * as os from 'os';\n"
            "globalThis.std = std;\n"
            "globalThis.os = os;\n";
        eval_buf(ctx, str, strlen(str), "<input>", JS_EVAL_TYPE_MODULE);
    }

    return TRUE;
}
int qjs_run_buf(const void *buf, int buf_len, int module) {
    int flags = 0;
    if( module ){
        flags = JS_EVAL_TYPE_MODULE;
    }
    if( rt >0 && ctx > 0 ){
        return eval_buf(ctx, buf, buf_len, "user_buf", flags);
    }
    return 0;
}
int qjs_run_file(const char *filename, int module){
    if( rt >0 && ctx > 0 ){
        return eval_file(ctx, filename, module);
    }
    return 0;
}
void qjs_exit(){
    if( rt >0 && ctx > 0 ){
        js_std_free_handlers(rt);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
    }
}

void qjs_loop(){
    if( rt >0 && ctx > 0 ){
        js_std_loop(ctx);
    }
}
int main()
{
	printf("hello: demo to use quickjs like qjs!\r\n");
    if( qjs_init() ){
        qjs_run_file("repl.js", 1);
        qjs_loop();
        qjs_exit();
    }
	return 0;
}
