#ifndef __CAR_BRIDGE_CAR_FUNCTION_H
#define __CAR_BRIDGE_CAR_FUNCTION_H
#include <cstddef>
#include <nan.h>
#include <elastos.h>
#include "macros.h"

CAR_BRIDGE_NAMESPACE_BEGIN
extern v8::Local<v8::Object> CARConstructor(IConstructorInfo const *constructorInfo);
extern v8::Local<v8::Object> CARConstructor(size_t nConstructorInfos, IConstructorInfo *constructorInfos[]);
extern v8::Local<v8::Object> CARMethod(IMethodInfo const *methodInfo);
extern v8::Local<v8::Object> CARMethod(size_t nMethodInfos, IMethodInfo *methodInfos[]);
extern v8::Local<v8::Object> CARCallbackMethod(ICallbackMethodInfo const *callbackMethodInfo);
extern v8::Local<v8::Object> CARCallbackMethod(
    size_t nCallbackMethodInfos, ICallbackMethodInfo *callbackMethodInfos[]);
CAR_BRIDGE_NAMESPACE_END

#endif
