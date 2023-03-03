//
//  main.cpp
//
//  Created by nieyinyin on 2023/3/3.
//

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include "quickjs.h"
#ifdef __cplusplus
}
#endif

using namespace::std;

bool checkJSException(JSContext *ctx, JSValueConst arg, std::string *exception) {
    JSContext* ctx_ = ctx;
    bool ret = false;
    if (JS_IsException(arg)) {
        ret = true;
        if (exception) {
            JSValueConst exception_val = JS_GetException(ctx_);
            const char * str = JS_ToCString(ctx_, exception_val);
            JS_FreeValue(ctx_, exception_val);
            if (str) {
                *exception = std::string(str);
                JS_FreeCString(ctx_, str);
            }
        }
    } else if (JS_IsError(ctx_, arg)) {
        ret = true;
        if (exception) {
            const char *str;
            JSValueConst exception_val = JS_GetException(ctx_);
            JSValueConst error_val = JS_GetPropertyStr(ctx_, exception_val, "stack");
            if (JS_IsUndefined(error_val)) {
                str = JS_ToCString(ctx_, exception_val);
            } else {
                str = JS_ToCString(ctx_, error_val);
            }
            JS_FreeValue(ctx_, exception_val);
            JS_FreeValue(ctx_, error_val);
            if (str) {
                *exception = std::string(str);
                JS_FreeCString(ctx_, str);
            }
        }
    } else {
        cout << " arg is not Exception or Error" << endl;
    }
    return ret;
}


int main(int argc, const char * argv[]) {
    JSRuntime *rt = JS_NewRuntime();
    JSContext *ctx = JS_NewContext(rt);
    string script = "function helloQuickJS() {return 'Hello, QuickJS!'}; helloQuickJS()";
    JSValue result = JS_Eval(ctx, script.c_str(), script.length(), "", 0);
    string exception;
    bool hasException = checkJSException(ctx, result, &exception);
    if (!hasException) {
        if (JS_IsString(result)){
            cout << "result is string" << endl;
            const char *str = JS_ToCString(ctx, result);
            if (str) {
                cout << string(str) << endl;
                JS_FreeCString(ctx, str);
            }
            JS_FreeValue(ctx, result);
        } else {
            cout << "result's data type is not string" << endl;
        }
    } else {
        cout << "exception occured!" << endl;
    }
    //JS_FreeValue(ctx, result); // if we do this, error( interrupted by signal 11: SIGSEGV) will occur when call JS_FreeRuntime, and why? // TODO
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return 0;
}
