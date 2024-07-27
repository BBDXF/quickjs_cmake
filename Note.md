# The quickjs code note
[MOC]

## define js module 

```javascript
// fib.js

// fib function
export function fib(n)
{
    if (n <= 0)
        return 0;
    else if (n == 1)
        return 1;
    else
        return fib(n - 1) + fib(n - 2);
}

// person class
export class Person {
    constructor(name, age) {
        this.name = name;
        this.age = age;
    }

    greet() {
        console.log(`Hello, my name is ${this.name} and I am ${this.age} years old.`);
    }
}

```
use it

```javascript
/* test_fib.js */
import { fib,Person } from "./fib.js";
console.log("fib(10)=", fib(10));

const person = new Person('Alice', 30);
person.greet();  // Output: Hello, my name is Alice and I am 30 years old.


```
then `qjs test_fib.js`, passed.

## import issue
使用者 `import { fib,Person } from "./fib.js";`, fib.js 使用 `export { fib, Person };` 行不行？
> 可以

fib.js 使用 `export default { fib, Person };` 行不行？
> 不行，`SyntaxError: Could not find export 'fib' in module 'fib.js'`，因为export和import方法不匹配。  
> `export default` 对应的引用方法是`import fib, Person from './fib.js'`，理论上修改test_fib.js就可以了。  
> 实际上quickjs不支持，可以使用`import * as m from './fib.js'`替代，然后使用`m.default.fib(10)`, 和nodejs差别很大。  

```javascript
// export default { fib };
import * as a from "./fib.js";
console.log(a.default)
console.log(a.default.fib)
console.log("fib(10)=", a.default.fib(10));
```

> 所以建议不要使用 `export default {} ` 语法。


## define c module and functions from code
```c
// real call functions or wrapper for C functions
JSValue js_gtest(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { /* TODO */ }

// register work
void define_c_func_in_js(JSContext *ctx)
{
    JSValue global_obj, console;
    // global object [this]
    global_obj = JS_GetGlobalObject(ctx);

    // this.gtest('xxxxx');
    JS_SetPropertyStr(ctx, global_obj, "gtest", JS_NewCFunction(ctx, js_gtest, "gtest", 1));

    // console.log
    // 1. register object console
    console = JS_NewObject(ctx);
    // 2. register function log for object console
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_gtest, "log", 1));
    // 3. regiser console to global object [this]
    JS_SetPropertyStr(ctx, global_obj, "console", console);

    // free global object
    JS_FreeValue(ctx, global_obj);
}
```
so the user can access `gtest` and `console.log` in js code.

## define C extension (so module)

```c
#include "../quickjs.h"

#define countof(x) (sizeof(x) / sizeof((x)[0]))

// my C function
static int myfn1(int n)  { return n; }
static void myfn2(char* buff, int n)  { }

// js function wrapper using C funtions
static JSValue js_myfn1(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    int n, res;
    if (JS_ToInt32(ctx, &n, argv[0]))
        return JS_EXCEPTION;
    res = myfn1(n);
    return JS_NewInt32(ctx, res);
}
static JSValue js_myfn2(JSContext *ctx, JSValueConst this_val,
                      int argc, JSValueConst *argv)
{
    int n;
    char* buff = NULL;
    buff = JS_ToCStringLen(ctx, &n, argv[0]);
    if (buff == NULL)
        return JS_EXCEPTION;
    myfn2(buff, n);
    return JS_NULL;
}

// js function define list
static const JSCFunctionListEntry js_my_funcs[] = {
    JS_CFUNC_DEF("myfn1", 1, js_myfn1 ),
    JS_CFUNC_DEF("myfn2", 2, js_myfn2 ),
};

// module init function, register js functions defined as upper
static int register_my_module(JSContext *ctx, JSModuleDef *m)
{
    return JS_SetModuleExportList(ctx, m, js_my_funcs, countof(js_my_funcs));
}

#ifdef JS_SHARED_LIBRARY
#define JS_INIT_MODULE js_init_module
#else
#define JS_INIT_MODULE js_init_module_my
#endif

// C module loader init
// module_name is the name of the xxx.so file
JSModuleDef *JS_INIT_MODULE(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, register_my_module);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_my_funcs, countof(js_my_funcs));
    return m;
}
```
then build it to `libmy.so`

test js code:
```javascript
import { myfn1, myfn2 } from "./libmy.so";

console.log("myfn1: ", myfn1(123));
myfn2("hello world");
```

# easy way to define js func and property

```c
#define JS_CFUNC_DEF
#define JS_CFUNC_MAGIC_DEF
#define JS_CFUNC_SPECIAL_DEF
#define JS_ITERATOR_NEXT_DEF
#define JS_CGETSET_DEF
#define JS_CGETSET_MAGIC_DEF
#define JS_PROP_STRING_DEF
#define JS_PROP_INT32_DEF
#define JS_PROP_INT64_DEF
#define JS_PROP_DOUBLE_DEF
#define JS_PROP_UNDEFINED_DEF
#define JS_OBJECT_DEF
#define JS_ALIAS_DEF
#define JS_ALIAS_BASE_DEF
```
