
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <map>
#include <memory>
#include <new>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <nan.h>
#include <elastos.h>
#include "macros.h"
#include "libc++-ext.h"
#include "nan-ext.h"
#include "elastos-ext.h"
#include "car-object.h"
#include "can-down-delete.h"
#include "car-arguments.h"
#include "car-data-type.h"
#include "car-function.h"
#include "car-function-adapter.h"
#include "error.h"
#include "js-2-car.h"
#include "util.h"
#include "weak-external-base.h"
#include "base/logging.h"

using namespace std;
using namespace Nan;
using namespace v8;

_ELASTOS_NAMESPACE_USING

CAR_BRIDGE_NAMESPACE_BEGIN

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
static Nan::Persistent<v8::FunctionTemplate> _classBaseTemplate;
#pragma clang diagnostic pop

NAN_METHOD(CARObject::On)
{
#if 0 //?try
    try {
#endif
        Nan::HandleScope scope;
        MF2CARFA *mapListenerToCARFunctionAdapter = nullptr;

        Local<Value> arg0;
        Local<Value> arg1;

        CARObject *thatCARObject;
        Local<::v8::String> name;
        ECode ec;

        AutoPtr<ICallbackMethodInfo> callbackMethodInfo;
#if 0//?callback
        ICallbackMethodInfo *_callbackMethodInfo;
#endif
        Local<Function> listener;
        uintptr_t connectionId = 0;
        unique_ptr<CARFunctionAdapter> carFunctionAdapter;
        EventHandler carEventHandler;
        unique_ptr<CARFunctionAdapter> *_carFunctionAdapter = nullptr;

        if (info.Length() < 2)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!arg0->IsString())
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg1 = info[1];
        if (!arg1->IsFunction())
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        thatCARObject = Unwrap<CARObject>(info.This());

        name = arg0.As<::v8::String>();
#if 0//?callback
        ec = thatCARObject->_classInfo->GetCallbackMethodInfo(_ELASTOS String(*Utf8String(name)),
                &_callbackMethodInfo);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        callbackMethodInfo = _callbackMethodInfo, _callbackMethodInfo->Release();
#endif

        listener = arg1.As<Function>();

        if (thatCARObject->_mapNameToMapListenerToCARFunctionAdapter.count(name) > 0) {
#if 0 //?jw _mapNameToMapListenerToCARFunctionAdapter
            mapListenerToCARFunctionAdapter = &thatCARObject->_mapNameToMapListenerToCARFunctionAdapter[name];
#endif
            if (mapListenerToCARFunctionAdapter->count(listener) > 0) {
#if 0 //?jw _mapNameToMapListenerToCARFunctionAdapter
                connectionId = reinterpret_cast<uintptr_t>((*mapListenerToCARFunctionAdapter)[listener].get());
#endif
                goto done;
            } 

        }

        carFunctionAdapter = unique_ptr<CARFunctionAdapter>(
                new(nothrow) CARFunctionAdapter(callbackMethodInfo, listener)
                );
        if (carFunctionAdapter == nullptr)
            Throw_LOG(Error::NO_MEMORY, 0);


#if 0//?jw EventHandler
        carEventHandler = EventHandler::Make(carFunctionAdapter.get(),
#ifdef _ELASTOS_BUG_EVENT_HANDLER_MAKE
                (void *)(ECode (CARFunctionAdapter::*)(IInterface *, ...))
#endif
                &CARFunctionAdapter::Call<IInterface *>);
#endif

#if 0 //?jw _mapNameToMapListenerToCARFunctionAdapter
        if (mapListenerToCARFunctionAdapter == nullptr)
            mapListenerToCARFunctionAdapter = &thatCARObject->_mapNameToMapListenerToCARFunctionAdapter[name];

        _carFunctionAdapter = &(*mapListenerToCARFunctionAdapter)[listener];
#endif
        ec = callbackMethodInfo->AddCallback(thatCARObject->_carObject, carEventHandler);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        *_carFunctionAdapter = move(carFunctionAdapter);

        connectionId = reinterpret_cast<uintptr_t>((*_carFunctionAdapter).get());

        thatCARObject->_connectionIds.insert(connectionId);

done:
        NAN_METHOD_RETURN_VALUE(Nan::New<Number>(connectionId));
#if 0 //?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static bool _MaybeIsConnectionId(Local<Value> value)
{
    double d;

    if (!value->IsNumber())
        return false;

    d = To<double>(value).FromJust();
    if (std::nearbyint(d) != d)
        return false;

    return d >= 0 && d <= UINTPTR_MAX;
}

void CARObject::Off(CARObject *carObject,
        CARFunctionAdapter *carFunctionAdapter,
        Local<::v8::String> name,
        Local<Function> listener)
{
    ICallbackMethodInfo const * _callbackMethodInfo;

    EventHandler carEventHandler;

    ECode ec;

    _callbackMethodInfo = static_cast<ICallbackMethodInfo const *>(carFunctionAdapter->functionInfo());
    ICallbackMethodInfo* callbackMethodInfo = const_cast<ICallbackMethodInfo*>(_callbackMethodInfo);

#if 0 //??jw
    carEventHandler = EventHandler::Make(carFunctionAdapter,
#ifdef _ELASTOS_BUG_EVENT_HANDLER_MAKE
            (void *)(ECode (CARFunctionAdapter::*)(IInterface *, ...))
#endif
            &CARFunctionAdapter::Call<IInterface *>);
#endif
    ec = callbackMethodInfo->RemoveCallback(carObject->_carObject, carEventHandler);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
#if 0//?jw _mapNameToMapListenerToCARFunctionAdapter
    carObject->_mapNameToMapListenerToCARFunctionAdapter[name].erase(listener);
#endif
    carObject->_connectionIds.erase(reinterpret_cast<uintptr_t>(carFunctionAdapter));
}

void CARObject::Off(CARObject *carObject, CARFunctionAdapter *carFunctionAdapter)
{
    Nan::HandleScope scope;
    ECode ec;
    _ELASTOS String name;

    ec = const_cast<IFunctionInfo *>(carFunctionAdapter->functionInfo())->GetName(&name);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    Off(carObject, carFunctionAdapter, ToValue(name).As<::v8::String>(), carFunctionAdapter->function());
}

void CARObject::Off(Local<Object> object, Local<Number> connectionId)
{
    CARObject *carObject;

    uintptr_t _connectionId;

    CARFunctionAdapter *carFunctionAdapter;

    carObject = Unwrap<CARObject>(object);

    _connectionId = To<double>(connectionId).FromJust();

    if (carObject->_connectionIds.count(_connectionId) == 0)
        return;

    carFunctionAdapter = reinterpret_cast<CARFunctionAdapter *>(_connectionId);

    Off(carObject, carFunctionAdapter);
}

void CARObject::Off(Local<Object> object, Local<::v8::String> name, Local<Function> listener)
{
    CARObject *carObject;

    carObject = Unwrap<CARObject>(object);

    if (carObject->_mapNameToMapListenerToCARFunctionAdapter.count(name) == 0)
        return;
#if 0 //?jw _mapNameToMapListenerToCARFunctionAdapter
    auto &mapListenerToCARFunctionAdapter = carObject->_mapNameToMapListenerToCARFunctionAdapter[name];
    if (mapListenerToCARFunctionAdapter.count(listener) == 0)
        return;
    Off(carObject, mapListenerToCARFunctionAdapter[listener].get(), name, listener);
#endif
}

void CARObject::Off(Local<Object> object, Local<::v8::String> name)
{
    CARObject *carObject;

    carObject = Unwrap<CARObject>(object);

    if (carObject->_mapNameToMapListenerToCARFunctionAdapter.count(name) == 0)
        return;
#if 0 //?jw _mapNameToMapListenerToCARFunctionAdapter
    auto &mapListenerToCARFunctionAdapter = carObject->_mapNameToMapListenerToCARFunctionAdapter[name];
    for (auto p = mapListenerToCARFunctionAdapter.begin(), end = mapListenerToCARFunctionAdapter.end();
            p != end;
            ++p) {
        Off(carObject, p->second.get(), name, Nan::New(p->first));

        mapListenerToCARFunctionAdapter.erase(p);
    }

    carObject->_mapNameToMapListenerToCARFunctionAdapter.erase(name);
#endif
}

NAN_METHOD(CARObject::Off)
{
#if 0//?jw try
    try {
#endif
        Local<Object> that = info.This();

        size_t argc;

        Local<Value> arg0;

        argc = info.Length();
        if (argc < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (_MaybeIsConnectionId(arg0))
            Off(that, arg0.As<Number>());
        else if (arg0->IsString()) {
            Local<Value> arg1;

            if (argc > 1 && (arg1 = info[1], arg1->IsFunction()))
                Off(that, arg0.As<::v8::String>(), arg1.As<Function>());
            else
                Off(that, arg0.As<::v8::String>());
        }

        NAN_METHOD_RETURN_UNDEFINED();
#if 0//?jw try
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

NAN_METHOD(CARObject::OffAll)
{
#if 0//?jw try
    try {
#endif
        CARObject *thatCARObject;

        thatCARObject = Unwrap<CARObject>(info.This());
#if 0//?jw try
        ECode ec = thatCARObject->_classInfo->RemoveAllCallbackHandlers(thatCARObject->_carObject);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);
#endif
        thatCARObject->_mapNameToMapListenerToCARFunctionAdapter.clear();
        thatCARObject->_connectionIds.clear();

        NAN_METHOD_RETURN_UNDEFINED();
#if 0//?jw try
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}
#if 0//?car remove
NAN_METHOD(CARObject::EnterRegime)
{
#if 0//?jw try
    try {
#endif
        Local<Value> arg0;
        CARObject *thatCARObject;
        CARObject *regime;


        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!IsRegime(arg0))
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        thatCARObject = Unwrap<CARObject>(info.This());

        regime = Unwrap<CARObject>(arg0.As<Object>());
        ECode ec = CObject::EnterRegime(thatCARObject->_carObject, static_cast<IRegime *>(regime->_carObject.Get()));
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        NAN_METHOD_RETURN_UNDEFINED();
#if 0//?jw try
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif    
}

NAN_METHOD(CARObject::LeaveRegime)
{
#if 0 //?jw
    try {
#endif
        Local<Value> arg0;

        CARObject *thatCARObject;

        CARObject *regime;

        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (IsRegime(arg0))
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        thatCARObject = Unwrap<CARObject>(info.This());

        regime = Unwrap<CARObject>(arg0.As<Object>());

        ECode ec = CObject::LeaveRegime(thatCARObject->_carObject, static_cast<IRegime *>(regime->_carObject.Get()));
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        NAN_METHOD_RETURN_UNDEFINED();
#if 0 //?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}
#endif

NAN_METHOD(CARObject::Equal)
{
#if 0 //?jw
    try {
#endif
        Nan::HandleScope scope;

        Local<Value> arg0;

        CARObject *thatCARObject;

        CARObject *carObject;

        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!IsCARObject(arg0)) {
            NAN_METHOD_RETURN_VALUE(Nan::New(false));

            return;
        }

        thatCARObject = Unwrap<CARObject>(info.This());

        carObject = Unwrap<CARObject>(arg0.As<Object>());

        NAN_METHOD_RETURN_VALUE(
                ToValueFromBoolean(CObject::Compare(thatCARObject->_carObject, carObject->_carObject))
                );
#if 0 //?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

void CARObject::Initialize(void)
{
    Nan::HandleScope scope;

    Local<FunctionTemplate> classBaseTemplate;

    classBaseTemplate = Nan::New<FunctionTemplate>();

    classBaseTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    SetTemplate(classBaseTemplate,
            Nan::New("$what").ToLocalChecked(),
            Nan::New("CARClass").ToLocalChecked(),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    CAR_BRIDGE::SetPrototypeMethod(classBaseTemplate, "$on", On);
    CAR_BRIDGE::SetPrototypeMethod(classBaseTemplate, "$off", Off);
    CAR_BRIDGE::SetPrototypeMethod(classBaseTemplate, "$offAll", OffAll);
#if 0//?jw car remove
    CAR_BRIDGE::SetPrototypeMethod(classBaseTemplate, "$enterRegime", EnterRegime);
    CAR_BRIDGE::SetPrototypeMethod(classBaseTemplate, "$leaveRegime", LeaveRegime);
#endif
    CAR_BRIDGE::SetPrototypeMethod(classBaseTemplate, "$equal", Equal);

    _classBaseTemplate.Reset(classBaseTemplate);
}

struct _Value {
    Nan::Persistent<Value> value;

    Nan::Persistent<Value> data;

    _ELASTOS String type;

    ParamIOAttribute io;

    struct CanWithPriority {
        bool can;

        int priority;
    };

    mutable map<_ELASTOS String, struct CanWithPriority> canBeUsedAsArgumentOf;

    virtual ~_Value()
    {
        data.Reset();

        value.Reset();
    }
};

struct _InputValue: _Value {
    mutable unique_ptr<CanDownDelete> maybeCARData;
};

static struct _Value *_ParseValue(Local<Value> value)
{
    Local<Value> data = value;
    _ELASTOS String type(nullptr);
    ParamIOAttribute io = ParamIOAttribute_In;

    Local<Object> object;

    Local<Value> _io;
    _ELASTOS String __io;
    ParamIOAttribute ___io;

    Local<Value> _data;

    Local<Value> _type;
    _ELASTOS String __type;

    unique_ptr<struct _Value> _value;

    if (!value->IsObject())
        goto exit;

    object = value.As<Object>();

    if (GetOwnPropertyNames(object).ToLocalChecked()->Length() < 2)
        goto exit;

    if (!HasOwnProperty(object, New(".io").ToLocalChecked()).FromJust())
        goto exit;

    _io = Get(object, New(".io").ToLocalChecked()).ToLocalChecked();
    if (!_io->IsString())
        goto exit;

    ToString(__io, _io);

    if (__io == "Input")
        ___io = ParamIOAttribute_In;
    else if (__io == "CalleeAllocOutput")
        ___io = ParamIOAttribute_CalleeAllocOut;
    else if (__io == "CallerAllocOutput")
        ___io = ParamIOAttribute_CallerAllocOut;
    else
        goto exit;

    if (!HasOwnProperty(object, New("data").ToLocalChecked()).FromJust())
        goto exit;

    _data = Get(object, New("data").ToLocalChecked()).ToLocalChecked();

    if (HasOwnProperty(object, New(".type").ToLocalChecked()).FromJust()) {
        _type = Get(object, New(".type").ToLocalChecked()).ToLocalChecked();
        if (!_type->IsString())
            goto exit;

        ToString(__type, _type);
        if (__type.IsEmpty())
            goto exit;
    } else if (___io == ParamIOAttribute_CalleeAllocOut)
        goto exit;
    else if (___io == ParamIOAttribute_CallerAllocOut && _data->IsUndefined())
        goto exit;

    data = _data;
    type = __type;
    io = ___io;

exit:
    if (io == ParamIOAttribute_In)
        _value = unique_ptr<struct _Value>(new(nothrow) struct _InputValue);
    else
        _value = unique_ptr<struct _Value>(new(nothrow) struct _Value);
    if (_value == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    _value->value.Reset(value);
    _value->data.Reset(data);
    _value->type = type;
    _value->io = io;

    return _value.release();
}

template<class Array>
static struct _Value **_ParseValues(size_t argc, Array const &argv)
{
    unique_ptr<unique_ptr<struct _Value> []> _argv(new(nothrow) unique_ptr<struct _Value>[argc]);
    if (_argv == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    for (size_t i = 0; i < argc; ++i)
        _argv[i] = unique_ptr<struct _Value>(_ParseValue(argv[i]));

    return reinterpret_cast<struct _Value **>(_argv.release());
}

static bool _CanBeUsedAsArgumentOf(IParamInfo const *pparamInfo, struct _Value const *value, int *priority)
{
    ECode ec;

    ParamIOAttribute io;

    AutoPtr<IDataTypeInfo > dataTypeInfo;
    IDataTypeInfo *_dataTypeInfo;

    _ELASTOS String fullName;
    IParamInfo *paramInfo = const_cast<IParamInfo *>(pparamInfo);
    ec = paramInfo->GetIOAttribute(&io);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (io != value->io)
        return false;

    ec = paramInfo->GetTypeInfo(&_dataTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    dataTypeInfo = _dataTypeInfo, _dataTypeInfo->Release();

    ec = GetFullName(dataTypeInfo, &fullName);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (value->type != nullptr && fullName != value->type)
        return false;

    if (value->canBeUsedAsArgumentOf.count(fullName) > 0) {
        auto &canWithPriority = value->canBeUsedAsArgumentOf[fullName];

        if (priority != nullptr)
            *priority = canWithPriority.priority;

        return canWithPriority.can;
    }

    auto &canWithPriority = value->canBeUsedAsArgumentOf[fullName];

    canWithPriority.can = CanBeUsedAs(dataTypeInfo, New(value->data), &canWithPriority.priority);

    if (priority != nullptr)
        *priority = canWithPriority.priority;

    return canWithPriority.can;
}

template<class FunctionInfo>
static AutoPtr<IFunctionInfo> _GetMatchingFunctionForCall(
        size_t nFunctionInfos, IFunctionInfo *pfunctionInfos[], size_t argc, struct _Value const *argv[])
{
    _ELASTOS String name;

    vector<AutoPtr<IFunctionInfo >> candidates;

    int priority;

    size_t size;
    IFunctionInfo** functionInfos = const_cast<IFunctionInfo**>(pfunctionInfos);
    priority = INT_MAX;
    for (size_t i = 0; i < nFunctionInfos; ++i) {
        IFunctionInfo *functionInfo;

        ECode ec;

        _ELASTOS String _name;

        _ELASTOS Int32 nParams;

        AutoPtr<ArrayOf<IParamInfo *> > paramInfos;

        int _priority;

        _ELASTOS Int32 j;

        functionInfo = functionInfos[i];

        ec = const_cast<IFunctionInfo *>(functionInfo)->GetName(&_name);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        if (name == nullptr)
            name = _name;
        else if (_name != name)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        ec = const_cast<IFunctionInfo *>(functionInfo)->GetParamCount(&nParams);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        if ((size_t)nParams > argc)
            continue;

        paramInfos = ArrayOf<IParamInfo *>::Alloc(nParams);
        if (paramInfos == 0)
            Throw_LOG(Error::NO_MEMORY, 0);

        ec = const_cast<IFunctionInfo *>(functionInfo)->GetAllParamInfos(
                reinterpret_cast<ArrayOf<IParamInfo *> *>(paramInfos.Get())
                );
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        _priority = 0;
        for (j = 0; j < nParams; ++j) {
            IParamInfo const *paramInfo;

            int __priority;

            paramInfo = (*paramInfos)[j];

            if (!_CanBeUsedAsArgumentOf(paramInfo, argv[j], &__priority))
                break;

            _priority += __priority;
        }

        if (j < nParams)
            continue;

        if (_priority > priority)
            continue;

        if (_priority < priority)
            candidates.clear();

        candidates.push_back(functionInfo);
    }

    size = candidates.size();

    if (size == 0)
        Throw_LOG(Error::NO_MATCHING_FUNCTION_FOR_CALL, 0);

    if (size > 1) {
        ostringstream oss;

        oss << "Call of overloaded '" << name << "(...)' is ambiguous.\n";
        oss << "Candidates are:\n";

        for (size_t i = 0; i < size; ++i) {
            IFunctionInfo *candidate;
            _ELASTOS String annotation;

            candidate = candidates[i];
#if 0//?jw car remove
            ECode ec = candidate->GetAnnotation(&annotation);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
#endif
            oss << annotation << "\n";
        }
#if 0//?jw
        //Throw_LOG(oss.str().data(),Error::AMBIGUOUS_CALL_OF_OVERLOADED_FUNCTION);
#endif
    }
    AutoPtr<IFunctionInfo > rcandidate = candidates[0];
    return rcandidate;
}

static void _SetInputArgumentOfInt16(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfInt16(index, ToInt16(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfInt32(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfInt32(index, ToInt32(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfInt64(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfInt64(index, ToInt64(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfByte(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfByte(index, ToByte(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfFloat(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfFloat(index, ToFloat(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfDouble(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfDouble(index, ToDouble(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfChar32(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfChar(index, ToChar32(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

struct _String: CanDownDelete {
    _ELASTOS String s;
};

static void _SetInputArgumentOfString(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    unique_ptr<struct _String> s(new(nothrow) struct _String);
    if (s == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToString(s->s, New(value->data));

    ec = argumentList->SetInputArgumentOfString(index, s->s);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    value->maybeCARData = move(s);
}

static void _SetInputArgumentOfBoolean(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfBoolean(index, ToBoolean(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

struct _EMuid: CanDownDelete {
    EMuid id;
};

static void _SetInputArgumentOfEMuid(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    unique_ptr<struct _EMuid> id(new(nothrow) struct _EMuid);
    if (id == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToEMuid(&id->id, New(value->data));

    ec = argumentList->SetInputArgumentOfEMuid(index, &id->id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    value->maybeCARData = move(id);
}

struct _EGuid: CanDownDelete {
    EGuid id = {};

    ~_EGuid() final
    {
        delete [] id.mUunm;
    }
};

static void _SetInputArgumentOfEGuid(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    unique_ptr<struct _EGuid> id(new(nothrow) struct _EGuid);
    if (id == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToEGuid(&id->id, New(value->data));

    ec = argumentList->SetInputArgumentOfEGuid(index, &id->id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    value->maybeCARData = move(id);
}

static void _SetInputArgumentOfECode(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfECode(index, ToECode(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOfLocalPtr(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfLocalPtr(index, ToLocalPtr(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

#if 0
struct _LocalType: CanDownDelete {
    unique_ptr<char []> localTypeObject;
};

static void _SetInputArgumentOfLocalType(IDataTypeInfo const *dataTypeInfo,
        IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    MemorySize size;

    struct _LocalType *_localTypeObject;

    ec = dataTypeInfo->GetSize(&size);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    unique_ptr<char []> localTypeObject(new(nothrow) char[size]);
    if (localTypeObject == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToLocalType(dataTypeInfo, localTypeObject, New(value->data));

    ec = argumentList->SetInputArgumentOfLocalType(index, localTypeObject);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    _localTypeObject = new(nothrow) struct _LocalType;
    if (_localTypeObject == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    _localTypeObject->localTypeObject = move(localTypeObject);

    value->maybeCARData = unique_ptr<CanDownDelete>(_localTypeObject);
}

#endif
static void _SetInputArgumentOfEnum(IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    ec = argumentList->SetInputArgumentOfEnum(index, ToEnum(New(value->data)));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

struct _CARArray: CanDownDelete {
    AutoPtr<IVariableOfCarArray> variableOfCARArray;
};

static void _SetInputArgumentOfCARArray(ICarArrayInfo const *pcarArrayInfo,
        IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    Local<Value> data;

    ECode ec;

    AutoPtr<IVariableOfCarArray> variableOfCARArray;
    IVariableOfCarArray *_variableOfCARArray;

    CarQuintet *carQuintet;
    ICarArrayInfo* carArrayInfo = const_cast<ICarArrayInfo*>(pcarArrayInfo);

    data = New(value->data);

    ec = carArrayInfo->CreateVariable(data.As<Array>()->Length(), &_variableOfCARArray);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfCARArray = _variableOfCARArray, _variableOfCARArray->Release();

    ec = variableOfCARArray->GetPayload((void **)&carQuintet);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    ToCARArray(carArrayInfo, carQuintet, data);

    ec = argumentList->SetInputArgumentOfCarArray(index, carQuintet);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    unique_ptr<struct _CARArray> carArray(new(nothrow) struct _CARArray);
    if (carArray == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    carArray->variableOfCARArray = variableOfCARArray;

    value->maybeCARData = move(carArray);
}

struct _Struct: CanDownDelete {
    AutoPtr<IVariableOfStruct> variableOfStruct;
};

static void _SetInputArgumentOfStruct(IStructInfo const *pstructInfo,
        IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;

    AutoPtr<IVariableOfStruct> variableOfStruct;
    IVariableOfStruct *_variableOfStruct;

    void *struct_;
    IStructInfo* structInfo = const_cast<IStructInfo*>(pstructInfo);
    ec = structInfo->CreateVariable(&_variableOfStruct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfStruct = _variableOfStruct, _variableOfStruct->Release();

    ec = variableOfStruct->GetPayload((void **)&struct_);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    ToStruct(structInfo, struct_, New(value->data));

    ec = argumentList->SetInputArgumentOfStructPtr(index, struct_);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    unique_ptr<struct _Struct> _struct(new(nothrow) struct _Struct);
    if (_struct == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    _struct->variableOfStruct = variableOfStruct;

    value->maybeCARData = move(_struct);
}

static void _SetInputArgumentOfInterface(IInterfaceInfo *interfaceInfo,
        IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    AutoPtr<IInterface> interface_;

    ECode ec;

    interface_ = ToInterface(interfaceInfo, New(value->data));

    ec = argumentList->SetInputArgumentOfObjectPtr(index, interface_);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetInputArgumentOf(IDataTypeInfo const *pdataTypeInfo,
        IArgumentList *argumentList, size_t index, struct _InputValue const *value)
{
    ECode ec;
    CarDataType dataType;

    IDataTypeInfo *dataTypeInfo = const_cast<IDataTypeInfo *>(pdataTypeInfo);
    ec = dataTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType) {
    case CarDataType_Int16:
        _SetInputArgumentOfInt16(argumentList, index, value);

        break;

    case CarDataType_Int32:
        _SetInputArgumentOfInt32(argumentList, index, value);

        break;

    case CarDataType_Int64:
        _SetInputArgumentOfInt64(argumentList, index, value);

        break;

    case CarDataType_Byte:
        _SetInputArgumentOfByte(argumentList, index, value);

        break;

    case CarDataType_Float:
        _SetInputArgumentOfFloat(argumentList, index, value);

        break;

    case CarDataType_Double:
        _SetInputArgumentOfDouble(argumentList, index, value);

        break;

    case CarDataType_Char32:
        _SetInputArgumentOfChar32(argumentList, index, value);

        break;

    case CarDataType_String:
        _SetInputArgumentOfString(argumentList, index, value);

        break;

    case CarDataType_Boolean:
        _SetInputArgumentOfBoolean(argumentList, index, value);

        break;

    case CarDataType_EMuid:
        _SetInputArgumentOfEMuid(argumentList, index, value);

        break;

    case CarDataType_EGuid:
        _SetInputArgumentOfEGuid(argumentList, index, value);

        break;

    case CarDataType_ECode:
        _SetInputArgumentOfECode(argumentList, index, value);

        break;

    case CarDataType_LocalPtr:
        _SetInputArgumentOfLocalPtr(argumentList, index, value);

        break;

#if 0
    case CarDataType_LocalType:
        _SetInputArgumentOfLocalType(dataTypeInfo, argumentList, index, value);

        break;

#endif
    case CarDataType_Enum:
        _SetInputArgumentOfEnum(argumentList, index, value);

        break;

    case CarDataType_ArrayOf:
        _SetInputArgumentOfCARArray(static_cast<ICarArrayInfo const *>(dataTypeInfo), argumentList, index, value);

        break;

    case CarDataType_Struct:
        _SetInputArgumentOfStruct(static_cast<IStructInfo const *>(dataTypeInfo), argumentList, index, value);

        break;

    case CarDataType_Interface:
        _SetInputArgumentOfInterface(static_cast<IInterfaceInfo *>(dataTypeInfo), argumentList, index, value);

        break;

    default:
        break;
    }
}

static NAN_SETTER(_SetOutputArgument)
{
#if 0//?jw
    try {
#endif
        struct CARArgumentBase *carArgument;
        carArgument = (struct CARArgumentBase *)info.Data().As<External>()->Value();
        carArgument->value.Reset(value);
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

struct _PersistentData {
    Nan::Persistent<Value> data;

    ~_PersistentData()
    {
        data.Reset();
    }
};

struct _CalleeAllocCARArray: CalleeAllocCARArray, _PersistentData {
private:
    ~_CalleeAllocCARArray() final
    {
        _CarQuintet_Release(*carQuintet);
#if 0
        delete carQuintet;
#endif
    }
};

static NAN_GETTER(_GetCalleeAllocOutputArgumentOfCARArray)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct _CalleeAllocCARArray *carArray;

        carArray = (struct _CalleeAllocCARArray *)info.Data().As<External>()->Value();

        if (carArray->value.IsEmpty()) {
            if (*carArray->carQuintet != 0)
                carArray->value.Reset(ToValue(carArray->carArrayInfo, *carArray->carQuintet));
            else
                carArray->value.Reset(carArray->data);
        }

        NAN_GETTER_RETURN_VALUE(New(carArray->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCalleeAllocOutputArgumentOfCARArray = _SetOutputArgument;

static void __SetCalleeAllocOutputArgumentOfCARArray(ICarArrayInfo const *pcarArrayInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    CarQuintet **_carQuintet;

    ECode ec;
    ICarArrayInfo* carArrayInfo = const_cast<ICarArrayInfo*>(pcarArrayInfo);
    unique_ptr<CarQuintet *> carQuintet(new(nothrow) (CarQuintet *));
    if (carQuintet == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    unique_ptr<struct _CalleeAllocCARArray, _CalleeAllocCARArray::Deleter> carArray(
            CalleeAllocCARArray_<struct _CalleeAllocCARArray>(carArrayInfo, carQuintet.get())
            );
    _carQuintet = carQuintet.release();
    carArray->data.Reset(value->data);

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCalleeAllocOutputArgumentOfCARArray,
            _SetCalleeAllocOutputArgumentOfCARArray,
            carArray->self()), carArray.release();

    ec = argumentList->SetOutputArgumentOfCarArrayPtrPtr(index, _carQuintet);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

struct _CalleeAllocStruct: CalleeAllocStruct, _PersistentData {
private:
    ~_CalleeAllocStruct() final
    {
        operator delete(*struct_);
        delete struct_;
    }
};

static NAN_GETTER(_GetCalleeAllocOutputArgumentOfStruct)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct _CalleeAllocStruct *struct_;

        struct_ = (struct _CalleeAllocStruct *)info.Data().As<External>()->Value();

        if (struct_->value.IsEmpty()) {
            if (*struct_->struct_ != 0)
                struct_->value.Reset(ToValue(struct_->structInfo, *struct_->struct_));
            else
                struct_->value.Reset(struct_->data);
        }

        NAN_GETTER_RETURN_VALUE(New(struct_->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCalleeAllocOutputArgumentOfStruct = _SetOutputArgument;

static void __SetCalleeAllocOutputArgumentOfStruct(IStructInfo  *structInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    void **__struct;

    ECode ec;

    unique_ptr<void *> struct_(new(nothrow) (void *));
    if (struct_ == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    unique_ptr<struct _CalleeAllocStruct, _CalleeAllocStruct::Deleter> _struct(
            CalleeAllocStruct_<struct _CalleeAllocStruct>(structInfo, struct_.get())
            );
    __struct = struct_.release();
    _struct->data.Reset(value->data);

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCalleeAllocOutputArgumentOfStruct,
            _SetCalleeAllocOutputArgumentOfStruct,
            _struct->self()), _struct.release();

    ec = argumentList->SetOutputArgumentOfStructPtrPtr(index, __struct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetCalleeAllocOutputArgumentOf(IDataTypeInfo const *pdataTypeInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    ECode ec;

    CarDataType dataType;
    IDataTypeInfo* dataTypeInfo = const_cast<IDataTypeInfo*>(pdataTypeInfo);
    ec = dataTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType) {
    case CarDataType_ArrayOf:
        __SetCalleeAllocOutputArgumentOfCARArray(static_cast<ICarArrayInfo *>(dataTypeInfo),
                argumentList, index, value);

        break;

    case CarDataType_Struct:
        __SetCalleeAllocOutputArgumentOfStruct(static_cast<IStructInfo *>(dataTypeInfo),
                argumentList, index, value);

        break;

    default:
        break;
    }
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfInt16)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocInt16 *i16;

        i16 = (struct CallerAllocInt16 *)info.Data().As<External>()->Value();

        if (i16->value.IsEmpty())
            i16->value.Reset(ToValue(*i16->i16));

        NAN_GETTER_RETURN_VALUE(New(i16->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfInt16 = _SetOutputArgument;

struct _CallerAllocInt16: CallerAllocInt16 {
private:
    ~_CallerAllocInt16() final
    {
        delete i16;
    }
};

static void __SetCallerAllocOutputArgumentOfInt16(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Int16 *__i16;

    ECode ec;

    unique_ptr<Int16> i16(new(nothrow) Int16);
    if (i16 == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *i16 = ToInt16(New(value->data));

    unique_ptr<struct _CallerAllocInt16, _CallerAllocInt16::Deleter> _i16(
            CallerAllocInt16_<struct _CallerAllocInt16>(i16.get())
            );
    __i16 = i16.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfInt16,
            _SetCallerAllocOutputArgumentOfInt16,
            _i16->self()), _i16.release();

    ec = argumentList->SetOutputArgumentOfInt16Ptr(index, __i16);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfInt32)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocInt32 *i32;

        i32 = (struct CallerAllocInt32 *)info.Data().As<External>()->Value();

        if (i32->value.IsEmpty())
            i32->value.Reset(ToValue(*i32->i32));

        NAN_GETTER_RETURN_VALUE(New(i32->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfInt32 = _SetOutputArgument;

struct _CallerAllocInt32: CallerAllocInt32 {
private:
    ~_CallerAllocInt32() final
    {
        delete i32;
    }
};

static void __SetCallerAllocOutputArgumentOfInt32(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    _ELASTOS Int32 *__i32;

    ECode ec;

    unique_ptr<_ELASTOS Int32> i32(new(nothrow) _ELASTOS Int32);
    if (i32 == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *i32 = ToInt32(New(value->data));

    unique_ptr<struct _CallerAllocInt32, _CallerAllocInt32::Deleter> _i32(
            CallerAllocInt32_<struct _CallerAllocInt32>(i32.get())
            );
    __i32 = i32.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfInt32,
            _SetCallerAllocOutputArgumentOfInt32,
            _i32->self()), _i32.release();

    ec = argumentList->SetOutputArgumentOfInt32Ptr(index, __i32);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfInt64)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocInt64 *i64;

        i64 = (struct CallerAllocInt64 *)info.Data().As<External>()->Value();

        if (i64->value.IsEmpty())
            i64->value.Reset(ToValue(*i64->i64));

        NAN_GETTER_RETURN_VALUE(New(i64->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfInt64 = _SetOutputArgument;

struct _CallerAllocInt64: CallerAllocInt64 {
private:
    ~_CallerAllocInt64() final
    {
        delete i64;
    }
};

static void __SetCallerAllocOutputArgumentOfInt64(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Int64 *__i64;

    ECode ec;

    unique_ptr<Int64> i64(new(nothrow) Int64);
    if (i64 == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *i64 = ToInt64(New(value->data));

    unique_ptr<struct _CallerAllocInt64, _CallerAllocInt64::Deleter> _i64(
            CallerAllocInt64_<struct _CallerAllocInt64>(i64.get())
            );
    __i64 = i64.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfInt64,
            _SetCallerAllocOutputArgumentOfInt64,
            _i64->self()), _i64.release();

    ec = argumentList->SetOutputArgumentOfInt64Ptr(index, __i64);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfByte)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocByte *byte;

        byte = (struct CallerAllocByte *)info.Data().As<External>()->Value();

        if (byte->value.IsEmpty())
            byte->value.Reset(ToValue(*byte->byte));

        NAN_GETTER_RETURN_VALUE(New(byte->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfByte = _SetOutputArgument;

struct _CallerAllocByte: CallerAllocByte {
private:
    ~_CallerAllocByte() final
    {
        delete byte;
    }
};

static void __SetCallerAllocOutputArgumentOfByte(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Byte *__byte;

    ECode ec;

    unique_ptr<Byte> byte(new(nothrow) Byte);
    if (byte == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *byte = ToByte(New(value->data));

    unique_ptr<struct _CallerAllocByte, _CallerAllocByte::Deleter> _byte(
            CallerAllocByte_<struct _CallerAllocByte>(byte.get())
            );
    __byte = byte.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfByte,
            _SetCallerAllocOutputArgumentOfByte,
            _byte->self()), _byte.release();

    ec = argumentList->SetOutputArgumentOfBytePtr(index, __byte);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfFloat)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocFloat *f;

        f = (struct CallerAllocFloat *)info.Data().As<External>()->Value();

        if (f->value.IsEmpty())
            f->value.Reset(ToValue(*f->f));

        NAN_GETTER_RETURN_VALUE(New(f->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfFloat = _SetOutputArgument;

struct _CallerAllocFloat: CallerAllocFloat {
private:
    ~_CallerAllocFloat() final
    {
        delete f;
    }
};

static void __SetCallerAllocOutputArgumentOfFloat(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Float *__f;

    ECode ec;

    unique_ptr<Float> f(new(nothrow) Float);
    if (f == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *f = ToFloat(New(value->data));

    unique_ptr<struct _CallerAllocFloat, _CallerAllocFloat::Deleter> _f(
            CallerAllocFloat_<struct _CallerAllocFloat>(f.get())
            );
    __f = f.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfFloat,
            _SetCallerAllocOutputArgumentOfFloat,
            _f->self()), _f.release();

    ec = argumentList->SetOutputArgumentOfFloatPtr(index, __f);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfDouble)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocDouble *d;

        d = (struct CallerAllocDouble *)info.Data().As<External>()->Value();

        if (d->value.IsEmpty())
            d->value.Reset(ToValue(*d->d));

        NAN_GETTER_RETURN_VALUE(New(d->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfDouble = _SetOutputArgument;

struct _CallerAllocDouble: CallerAllocDouble {
private:
    ~_CallerAllocDouble() final
    {
        delete d;
    }
};

static void __SetCallerAllocOutputArgumentOfDouble(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Double *__d;

    ECode ec;

    unique_ptr<Double> d(new(nothrow) Double);
    if (d == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *d = ToDouble(New(value->data));

    unique_ptr<struct _CallerAllocDouble, _CallerAllocDouble::Deleter> _d(
            CallerAllocDouble_<struct _CallerAllocDouble>(d.get())
            );
    __d = d.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfDouble,
            _SetCallerAllocOutputArgumentOfDouble,
            _d->self()), _d.release();

    ec = argumentList->SetOutputArgumentOfDoublePtr(index, __d);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfChar32)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocChar32 *c32;

        c32 = (struct CallerAllocChar32 *)info.Data().As<External>()->Value();

        if (c32->value.IsEmpty())
            c32->value.Reset(ToValue(*c32->c32));

        NAN_GETTER_RETURN_VALUE(New(c32->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfChar32 = _SetOutputArgument;

struct _CallerAllocChar32: CallerAllocChar32 {
private:
    ~_CallerAllocChar32() final
    {
        delete c32;
    }
};

static void __SetCallerAllocOutputArgumentOfChar32(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Char32 *__c32;

    ECode ec;

    unique_ptr<Char32> c32(new(nothrow) Char32);
    if (c32 == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *c32 = ToChar32(New(value->data));

    unique_ptr<struct _CallerAllocChar32, _CallerAllocChar32::Deleter> _c32(
            CallerAllocChar32_<struct _CallerAllocChar32>(c32.get())
            );
    __c32 = c32.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfChar32,
            _SetCallerAllocOutputArgumentOfChar32,
            _c32->self()), _c32.release();

    ec = argumentList->SetOutputArgumentOfCharPtr(index, __c32);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfString)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocString *s;

        s = (struct CallerAllocString *)info.Data().As<External>()->Value();

        if (s->value.IsEmpty())
            s->value.Reset(ToValue(*s->s));

        NAN_GETTER_RETURN_VALUE(New(s->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfString = _SetOutputArgument;

struct _CallerAllocString: CallerAllocString {
private:
    ~_CallerAllocString() final
    {
        delete s;
    }
};

static void __SetCallerAllocOutputArgumentOfString(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    _ELASTOS String *__s;

    ECode ec;

    unique_ptr<_ELASTOS String> s(new(nothrow) _ELASTOS String);
    if (s == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToString(*s, New(value->data));

    unique_ptr<struct _CallerAllocString, _CallerAllocString::Deleter> _s(
            CallerAllocString_<struct _CallerAllocString>(s.get())
            );
    __s = s.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfString,
            _SetCallerAllocOutputArgumentOfString,
            _s->self()), _s.release();

    ec = argumentList->SetOutputArgumentOfStringPtr(index, __s);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfBoolean)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocBoolean *b;

        b = (struct CallerAllocBoolean *)info.Data().As<External>()->Value();

        if (b->value.IsEmpty())
            b->value.Reset(ToValueFromBoolean(*b->b));

        NAN_GETTER_RETURN_VALUE(New(b->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfBoolean = _SetOutputArgument;

struct _CallerAllocBoolean: CallerAllocBoolean {
private:
    ~_CallerAllocBoolean() final
    {
        delete b;
    }
};

static void __SetCallerAllocOutputArgumentOfBoolean(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    _ELASTOS Boolean *__b;

    ECode ec;

    unique_ptr<_ELASTOS Boolean> b(new(nothrow) _ELASTOS Boolean);
    if (b == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *b = ToBoolean(New(value->data));

    unique_ptr<struct _CallerAllocBoolean, _CallerAllocBoolean::Deleter> _b(
            CallerAllocBoolean_<struct _CallerAllocBoolean>(b.get())
            );
    __b = b.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfBoolean,
            _SetCallerAllocOutputArgumentOfBoolean,
            _b->self()), _b.release();

    ec = argumentList->SetOutputArgumentOfBooleanPtr(index, __b);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfEMuid)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocEMuid *id;

        id = (struct CallerAllocEMuid *)info.Data().As<External>()->Value();

        if (id->value.IsEmpty())
            id->value.Reset(ToValue(id->id));

        NAN_GETTER_RETURN_VALUE(New(id->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif    
}

static auto &_SetCallerAllocOutputArgumentOfEMuid = _SetOutputArgument;

struct _CallerAllocEMuid: CallerAllocEMuid {
private:
    ~_CallerAllocEMuid() final
    {
        delete id;
    }
};

static void __SetCallerAllocOutputArgumentOfEMuid(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    EMuid *__id;

    ECode ec;

    unique_ptr<EMuid> id(new(nothrow) EMuid);
    if (id == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToEMuid(id.get(), New(value->data));

    unique_ptr<struct _CallerAllocEMuid, _CallerAllocEMuid::Deleter> _id(
            CallerAllocEMuid_<struct _CallerAllocEMuid>(id.get())
            );
    __id = id.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfEMuid,
            _SetCallerAllocOutputArgumentOfEMuid,
            _id->self()), _id.release();

    ec = argumentList->SetOutputArgumentOfEMuidPtr(index, __id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfEGuid)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocEGuid *id;

        id = (struct CallerAllocEGuid *)info.Data().As<External>()->Value();

        if (id->value.IsEmpty())
            id->value.Reset(ToValue(id->id));

        NAN_GETTER_RETURN_VALUE(New(id->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfEGuid = _SetOutputArgument;

struct _CallerAllocEGuid: CallerAllocEGuid {
private:
    ~_CallerAllocEGuid() final
    {
        delete [] id->mUunm;

        delete id;
    }
};

static void __SetCallerAllocOutputArgumentOfEGuid(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    EGuid *__id;

    ECode ec;

    unique_ptr<EGuid> id(new(nothrow) EGuid);
    if (id == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToEGuid(id.get(), New(value->data));

    unique_ptr<struct _CallerAllocEGuid, _CallerAllocEGuid::Deleter> _id(
            CallerAllocEGuid_<struct _CallerAllocEGuid>(id.get())
            );
    __id = id.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfEGuid,
            _SetCallerAllocOutputArgumentOfEGuid,
            _id->self()), _id.release();

    ec = argumentList->SetOutputArgumentOfEGuidPtr(index, __id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfECode)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocECode *ecode;

        ecode = (struct CallerAllocECode *)info.Data().As<External>()->Value();

        if (ecode->value.IsEmpty())
            ecode->value.Reset(ToValueFromECode(*ecode->ecode));

        NAN_GETTER_RETURN_VALUE(New(ecode->value));
#if 0//?jw        
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfECode = _SetOutputArgument;

struct _CallerAllocECode: CallerAllocECode {
private:
    ~_CallerAllocECode() final
    {
        delete ecode;
    }
};

static void __SetCallerAllocOutputArgumentOfECode(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    ECode *__ecode;

    ECode ec;

    unique_ptr<ECode> ecode(new(nothrow) ECode);
    if (ecode == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *ecode = ToECode(New(value->data));

    unique_ptr<struct _CallerAllocECode, _CallerAllocECode::Deleter> _ecode(
            CallerAllocECode_<struct _CallerAllocECode>(ecode.get())
            );
    __ecode = ecode.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfECode,
            _SetCallerAllocOutputArgumentOfECode,
            _ecode->self()), _ecode.release();

    ec = argumentList->SetOutputArgumentOfECodePtr(index, __ecode);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfLocalPtr)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocLocalPtr *localPtr;

        localPtr = (struct CallerAllocLocalPtr *)info.Data().As<External>()->Value();

        if (localPtr->value.IsEmpty())
            localPtr->value.Reset(ToValue(*localPtr->localPtr));

        NAN_GETTER_RETURN_VALUE(New(localPtr->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfLocalPtr = _SetOutputArgument;

struct _CallerAllocLocalPtr: CallerAllocLocalPtr {
private:
    ~_CallerAllocLocalPtr() final
    {
        delete localPtr;
    }
};

static void __SetCallerAllocOutputArgumentOfLocalPtr(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    void **__localPtr;

    ECode ec;

    unique_ptr<void *> localPtr(new(nothrow) (void *));
    if (localPtr == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *localPtr = ToLocalPtr(New(value->data));

    unique_ptr<struct _CallerAllocLocalPtr, _CallerAllocLocalPtr::Deleter> _localPtr(
            CallerAllocLocalPtr_<struct _CallerAllocLocalPtr>(localPtr.get())
            );
    __localPtr = localPtr.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfLocalPtr,
            _SetCallerAllocOutputArgumentOfLocalPtr,
            _localPtr->self()), _localPtr.release();

    ec = argumentList->SetOutputArgumentOfLocalPtrPtr(index, __localPtr);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

#if 0
static NAN_GETTER(_GetCallerAllocOutputArgumentOfLocalType)
{
    try {
        Nan::HandleScope scope;

        struct CallerAllocLocalType *localTypeObject;

        localTypeObject = (struct CallerAllocLocalType *)info.Data().As<External>()->Value();

        if (localTypeObject->value.IsEmpty())
            localTypeObject->value.Reset(ToValue(localTypeObject->dataTypeInfo, localTypeObject->localTypeObject));

        NAN_GETTER_RETURN_VALUE(New(localTypeObject->value));
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
}

static auto &_SetCallerAllocOutputArgumentOfLocalType = _SetOutputArgument;

struct _CallerAllocLocalType: CallerAllocLocalType {
private:
    ~_CallerAllocLocalType()
    {
        delete [] (char *)localTypeObject;
    }
};

static void __SetCallerAllocOutputArgumentOfLocalType(IDataTypeInfo const *dataTypeInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    ECode ec;

    MemorySize size;

    char *__localTypeObject;

    ec = dataTypeInfo->GetSize(&size);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    unique_ptr<char []> localTypeObject(new(nothrow) char[size]);
    if (localTypeObject == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ToLocalType(dataTypeInfo, localTypeObject, New(value->data));

    unique_ptr<struct _CallerAllocLocalType, _CallerAllocLocalType::Deleter> _localTypeObject(
            CallerAllocLocalType_<struct _CallerAllocLocalType>(dataTypeInfo, localTypeObject)
            );
    __localTypeObject = localTypeObject.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfLocalType,
            _SetCallerAllocOutputArgumentOfLocalType,
            _localTypeObject->self()), _localTypeObject.release();

    ec = argumentList->SetOutputArgumentOfLocalTypePtr(index, __localTypeObject);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

#endif
static NAN_GETTER(_GetCallerAllocOutputArgumentOfEnum)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocEnum *enum_;

        enum_ = (struct CallerAllocEnum *)info.Data().As<External>()->Value();

        if (enum_->value.IsEmpty())
            enum_->value.Reset(ToValueFromEnum(*enum_->enum_));

        NAN_GETTER_RETURN_VALUE(New(enum_->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfEnum = _SetOutputArgument;

struct _CallerAllocEnum: CallerAllocEnum {
private:
    ~_CallerAllocEnum() final
    {
        delete enum_;
    }
};

static void __SetCallerAllocOutputArgumentOfEnum(IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    _ELASTOS Int32 *__enum;

    ECode ec;

    unique_ptr<_ELASTOS Int32> enum_(new(nothrow) _ELASTOS Int32);
    if (enum_ == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    *enum_ = ToEnum(New(value->data));

    unique_ptr<struct _CallerAllocEnum, _CallerAllocEnum::Deleter> _enum(
            CallerAllocEnum_<struct _CallerAllocEnum>(enum_.get())
            );
    __enum = enum_.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfEnum,
            _SetCallerAllocOutputArgumentOfEnum,
            _enum->self()), _enum.release();

    ec = argumentList->SetOutputArgumentOfEnumPtr(index, __enum);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfCARArray)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocCARArray *carArray;

        carArray = (struct CallerAllocCARArray *)info.Data().As<External>()->Value();

        if (carArray->value.IsEmpty()) {
            ECode ec;

            AutoPtr<ICarArrayGetter> carArrayGetter;
            ICarArrayGetter *_carArrayGetter;

            ec = carArray->variableOfCARArray->GetGetter(&_carArrayGetter);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            carArrayGetter = _carArrayGetter, _carArrayGetter->Release();

            carArray->value.Reset(ToValue(carArray->carArrayInfo, carArrayGetter));
        }

        NAN_GETTER_RETURN_VALUE(New(carArray->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfCARArray = _SetOutputArgument;

static void __SetCallerAllocOutputArgumentOfCARArray(ICarArrayInfo * carArrayInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    Local<Value> data;

    ECode ec;

    AutoPtr<IVariableOfCarArray> variableOfCARArray;
    IVariableOfCarArray *_variableOfCARArray;

    AutoPtr<ICarArraySetter> carArraySetter;
    ICarArraySetter *_carArraySetter;

    CarQuintet *carQuintet;

    data = New(value->data);

    ec = carArrayInfo->CreateVariable(data.As<Array>()->Length(), &_variableOfCARArray);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfCARArray = _variableOfCARArray, _variableOfCARArray->Release();

    ec = variableOfCARArray->GetSetter(&_carArraySetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    carArraySetter = _carArraySetter, _carArraySetter->Release();

    ToCARArray(carArrayInfo, carArraySetter, data);

    unique_ptr<struct CallerAllocCARArray, CallerAllocCARArray::Deleter> carArray(
            CallerAllocCARArray_(carArrayInfo, variableOfCARArray)
            );

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfCARArray,
            _SetCallerAllocOutputArgumentOfCARArray,
            carArray->self()), carArray.release();

    ec = variableOfCARArray->GetPayload((void **)&carQuintet);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    ec = argumentList->SetOutputArgumentOfCarArrayPtr(index, carQuintet);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfStruct)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocStruct *struct_;

        struct_ = (struct CallerAllocStruct *)info.Data().As<External>()->Value();

        if (struct_->value.IsEmpty()) {
            ECode ec;

            AutoPtr<IStructGetter> structGetter;
            IStructGetter *_structGetter;

            ec = struct_->variableOfStruct->GetGetter(&_structGetter);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            structGetter = _structGetter, _structGetter->Release();

            struct_->value.Reset(ToValue(struct_->structInfo, structGetter));
        }

        NAN_GETTER_RETURN_VALUE(New(struct_->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

static auto &_SetCallerAllocOutputArgumentOfStruct = _SetOutputArgument;

static void __SetCallerAllocOutputArgumentOfStruct(IStructInfo * structInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    ECode ec;

    AutoPtr<IVariableOfStruct> variableOfStruct;
    IVariableOfStruct *_variableOfStruct;

    AutoPtr<IStructSetter> structSetter;
    IStructSetter *_structSetter;

    void *_struct;

    ec = structInfo->CreateVariable(&_variableOfStruct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfStruct = _variableOfStruct, _variableOfStruct->Release();

    ec = variableOfStruct->GetSetter(&_structSetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    structSetter = _structSetter, _structSetter->Release();

    ToStruct(structInfo, structSetter, New(value->data));

    unique_ptr<struct CallerAllocStruct, CallerAllocStruct::Deleter> struct_(
            CallerAllocStruct_(structInfo, variableOfStruct)
            );

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfStruct,
            _SetCallerAllocOutputArgumentOfStruct,
            struct_->self()), struct_.release();

    ec = variableOfStruct->GetPayload((void **)&_struct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    ec = argumentList->SetOutputArgumentOfStructPtr(index, _struct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static NAN_GETTER(_GetCallerAllocOutputArgumentOfInterface)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct CallerAllocInterface *interface_;

        interface_ = (struct CallerAllocInterface *)info.Data().As<External>()->Value();

        if (interface_->value.IsEmpty()) {
            ECode ec;

            AutoPtr<IClassInfo> classInfo;
            IClassInfo *_classInfo;

            _ELASTOS Boolean has;

            ec = CObject::ReflectClassInfo(*interface_->interface_, &_classInfo);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            classInfo = const_cast<IClassInfo *>(_classInfo), _classInfo->Release();

            ec = classInfo->HasInterfaceInfo(const_cast<IInterfaceInfo *>(interface_->interfaceInfo.Get()), &has);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            if (has != FALSE)
                interface_->value.Reset(ToValue(*interface_->interface_));
            else
                interface_->value.Reset(Undefined());
        }

        NAN_GETTER_RETURN_VALUE(New(interface_->value));
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif    
}

static auto &_SetCallerAllocOutputArgumentOfInterface = _SetOutputArgument;

struct _CallerAllocInterface: CallerAllocInterface {
private:
    ~_CallerAllocInterface() final
    {
        (*interface_)->Release();

        delete interface_;
    }
};

static void __SetCallerAllocOutputArgumentOfInterface(IInterfaceInfo *interfaceInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    Nan::HandleScope scope;

    AutoPtr<IInterface> interface__;

    IInterface **__interface;

    ECode ec;

    unique_ptr<IInterface *> interface_(new(nothrow) (IInterface *));
    if (interface_ == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    interface__ = ToInterface(interfaceInfo, New(value->data));
    interface__->AddRef(), *interface_ = interface__;

    unique_ptr<struct _CallerAllocInterface, _CallerAllocInterface::Deleter> _interface(
            CallerAllocInterface_<struct _CallerAllocInterface>(interfaceInfo, interface_.get())
            );
    __interface = interface_.release();

    SetAccessor(New(value->value).As<Object>(),
            New("data").ToLocalChecked(),
            _GetCallerAllocOutputArgumentOfInterface,
            _SetCallerAllocOutputArgumentOfInterface,
            _interface->self()), _interface.release();

    ec = argumentList->SetOutputArgumentOfObjectPtrPtr(index, __interface);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

static void _SetCallerAllocOutputArgumentOf(IDataTypeInfo *dataTypeInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    ECode ec;

    CarDataType dataType;

    ec = dataTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType) {
    case CarDataType_Int16:
        __SetCallerAllocOutputArgumentOfInt16(argumentList, index, value);

        break;

    case CarDataType_Int32:
        __SetCallerAllocOutputArgumentOfInt32(argumentList, index, value);

        break;

    case CarDataType_Int64:
        __SetCallerAllocOutputArgumentOfInt64(argumentList, index, value);

        break;

    case CarDataType_Byte:
        __SetCallerAllocOutputArgumentOfByte(argumentList, index, value);

        break;

    case CarDataType_Float:
        __SetCallerAllocOutputArgumentOfFloat(argumentList, index, value);

        break;

    case CarDataType_Double:
        __SetCallerAllocOutputArgumentOfDouble(argumentList, index, value);

        break;

    case CarDataType_Char32:
        __SetCallerAllocOutputArgumentOfChar32(argumentList, index, value);

        break;

    case CarDataType_String:
        __SetCallerAllocOutputArgumentOfString(argumentList, index, value);

        break;

    case CarDataType_Boolean:
        __SetCallerAllocOutputArgumentOfBoolean(argumentList, index, value);

        break;

    case CarDataType_EMuid:
        __SetCallerAllocOutputArgumentOfEMuid(argumentList, index, value);

        break;

    case CarDataType_EGuid:
        __SetCallerAllocOutputArgumentOfEGuid(argumentList, index, value);

        break;

    case CarDataType_ECode:
        __SetCallerAllocOutputArgumentOfECode(argumentList, index, value);

        break;

    case CarDataType_LocalPtr:
        __SetCallerAllocOutputArgumentOfLocalPtr(argumentList, index, value);

        break;

#if 0
    case CarDataType_LocalType:
        __SetCallerAllocOutputArgumentOfLocalType(dataTypeInfo, argumentList, index, value);

        break;

#endif
    case CarDataType_Enum:
        __SetCallerAllocOutputArgumentOfEnum(argumentList, index, value);

        break;

    case CarDataType_ArrayOf:
        __SetCallerAllocOutputArgumentOfCARArray(static_cast<ICarArrayInfo *>(dataTypeInfo),
                argumentList, index, value);

        break;

    case CarDataType_Struct:
        __SetCallerAllocOutputArgumentOfStruct(static_cast<IStructInfo *>(dataTypeInfo),
                argumentList, index, value);

        break;

    case CarDataType_Interface:
        __SetCallerAllocOutputArgumentOfInterface(static_cast<IInterfaceInfo *>(dataTypeInfo),
                argumentList, index, value);

        break;

    default:
        break;
    }
}

static void _SetArgumentOf(IParamInfo *paramInfo,
        IArgumentList *argumentList, size_t index, struct _Value *value)
{
    ECode ec;

    ParamIOAttribute io;

    AutoPtr<IDataTypeInfo> dataTypeInfo;
    IDataTypeInfo *_dataTypeInfo;

    ec = paramInfo->GetIOAttribute(&io);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    ec = paramInfo->GetTypeInfo(&_dataTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    dataTypeInfo = _dataTypeInfo, _dataTypeInfo->Release();

    if (io == ParamIOAttribute_In)
        _SetInputArgumentOf(dataTypeInfo, argumentList, index, static_cast<struct _InputValue const *>(value));
    else if (io == ParamIOAttribute_CalleeAllocOut)
        _SetCalleeAllocOutputArgumentOf(dataTypeInfo, argumentList, index, value);
    else if (io == ParamIOAttribute_CallerAllocOut)
        _SetCallerAllocOutputArgumentOf(dataTypeInfo, argumentList, index, value);
}

template<class FunctionInfo>
static AutoPtr<IArgumentList> _CreateArgumentList(FunctionInfo *functionInfo, size_t argc, struct _Value *argv[])
{
    ECode ec;

    AutoPtr<IArgumentList> argumentList;
    IArgumentList *_argumentList;

    _ELASTOS Int32 nParams;

    AutoPtr<ArrayOf<IParamInfo *> > paramInfos;

    ec = functionInfo->CreateArgumentList(&_argumentList);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    argumentList = _argumentList, _argumentList->Release();

    ec = functionInfo->GetParamCount(&nParams);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    paramInfos = ArrayOf<IParamInfo *>::Alloc(nParams);
    if (paramInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = functionInfo->GetAllParamInfos(reinterpret_cast<ArrayOf<IParamInfo *> *>(paramInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    for (_ELASTOS Int32 i = 0; i < nParams; ++i)
        _SetArgumentOf((*paramInfos)[i], argumentList, i, argv[i]);

    return argumentList;
}

struct _ClassInfo: WeakExternalBase {
    AutoPtr<IClassInfo > classInfo;
    AutoPtr<ArrayOf<IConstructorInfo *> > constructorInfos;

private:
    ~_ClassInfo() final = default;
};

struct _ClassId: WeakExternalBase {
    ClassID classId;

    bool filled = false;

    _ClassId(void)
    {
        classId.mUunm = uunm;
    }

    void Fill(IClassInfo const *pclassInfo)
    {
        IClassInfo* classInfo = const_cast<IClassInfo*>(pclassInfo);
        if (!filled) {
            ECode ec;

            ec = classInfo->GetId(&classId);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            filled = true;
        }
    }

private:
    char uunm[256];

    ~_ClassId() final = default;
};

NAN_METHOD(CARObject::Attach)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct _ClassInfo const *classInfo;

        struct _ClassId *classId;

        Local<Value> arg0;

        classInfo =
            (struct _ClassInfo const *)
            Get(info.This(), Nan::New(".__class__").ToLocalChecked()).ToLocalChecked().As<External>()->Value();

        classId = (struct _ClassId *)info.Data().As<External>()->Value();

        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!arg0->IsObject())
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        classId->Fill(classInfo->classInfo);

        AttachAspect(arg0.As<Object>(), classId->classId);

        NAN_METHOD_RETURN_UNDEFINED();
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

NAN_METHOD(CARObject::Detach)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct _ClassInfo *classInfo;

        struct _ClassId *classId;

        Local<Value> arg0;

        classInfo =
            (struct _ClassInfo *)
            Get(info.This(), Nan::New(".__class__").ToLocalChecked()).ToLocalChecked().As<External>()->Value();

        classId = (struct _ClassId *)info.Data().As<External>()->Value();

        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!arg0->IsObject())
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        classId->Fill(classInfo->classInfo);

        DetachAspect(arg0.As<Object>(), classId->classId);

        NAN_METHOD_RETURN_UNDEFINED();
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

NAN_METHOD(CARObject::Probe)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct _ClassInfo const *classInfo;

        struct _ClassId *classId;

        Local<Value> arg0;

        classInfo =
            (struct _ClassInfo const *)
            Get(info.This(), Nan::New(".__class__").ToLocalChecked()).ToLocalChecked().As<External>()->Value();

        classId = (struct _ClassId *)info.Data().As<External>()->Value();

        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!arg0->IsObject())
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        classId->Fill(classInfo->classInfo);

        NAN_METHOD_RETURN_VALUE(
                New(CAR_BRIDGE::Probe(arg0.As<Object>(), reinterpret_cast<InterfaceID const &>(classId->classId)))
                );
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

CARObject::CARObject(IClassInfo const *classInfo, ArrayOf<IConstructorInfo *>  &constructorInfos,
        IRegime *regime,
        size_t argc, Local<Value> argv[])
{
    IInterface *carObject = nullptr;
    unique_ptr<unique_ptr<struct _Value> []> _argv;
    AutoPtr<IConstructorInfo> constructorInfo;
    AutoPtr<IArgumentList> argumentList;
    AutoPtr<IFunctionInfo> functionInfo;

    _classInfo = const_cast<IClassInfo*>(classInfo);

    if (argc == 0) {
#if 0//?jw car remove
        ECode ec = classInfo->CreateObjectInRegime(regime, &carObject);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);
#endif
        goto done;
    }
#if 0//?jw
    _argv = unique_ptr<unique_ptr<struct _Value> []>(
            reinterpret_cast<unique_ptr<struct _Value> *>(_ParseValues(argc, argv))
            );

    functionInfo = _GetMatchingFunctionForCall<IConstructorInfo>(
                constructorInfos.GetLength(), reinterpret_cast<IFunctionInfo **>(constructorInfos.GetPayload()),
                argc, reinterpret_cast<struct _Value const**>(_argv.get()));
    constructorInfo = static_cast<IConstructorInfo*>(functionInfo.Get());

    argumentList =
        _CreateArgumentList<IConstructorInfo>(constructorInfo, argc, reinterpret_cast<struct _Value **>(_argv.get()));

    ec = constructorInfo->CreateObjectInRegime(regime, argumentList, &carObject);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
#endif
done:
    _carObject = carObject, carObject->Release();
}

struct _ClassInfoInRegime: WeakExternalBase {
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<ArrayOf<IConstructorInfo *> > constructorInfos;

    AutoPtr<IRegime> regime;

private:
    ~_ClassInfoInRegime() final = default;
};

CARObject *CARObject::NewInRegimeConstructor(size_t argc, Local<Value> argv[], Local<Value> data)
{
    struct _ClassInfoInRegime const *classInfoInRegime;

    CARObject *carObject = nullptr;

    classInfoInRegime = (struct _ClassInfoInRegime const *)data.As<External>()->Value();

    carObject = new(nothrow) CARObject(classInfoInRegime->classInfo, *classInfoInRegime->constructorInfos,
            classInfoInRegime->regime,
            argc, argv);
    if (carObject == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    return carObject;
}

NAN_METHOD(CARObject::NewInRegimeConstructor)
{
#if 0//?jw
    try {
#endif
        ClassConstructor(info, NewInRegimeConstructor);
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

NAN_METHOD(CARObject::InRegime)
{
#if 0//?jw
    try {
#endif
        Nan::HandleScope scope;

        struct _ClassInfo  *classInfo;

        Local<Value> arg0;

        CARObject *regime;

        Local<FunctionTemplate> classTemplate;

        classInfo =
            (struct _ClassInfo *)
            Get(info.This(), Nan::New(".__class__").ToLocalChecked()).ToLocalChecked().As<External>()->Value();

        if (info.Length() < 1)
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        arg0 = info[0];
        if (!IsRegime(arg0))
            Throw_LOG(Error::INVALID_ARGUMENT, 0);

        regime = Unwrap<CARObject>(arg0.As<Object>());

        unique_ptr<struct _ClassInfoInRegime, _ClassInfoInRegime::Deleter> classInfoInRegime(
                new(nothrow) struct _ClassInfoInRegime
                );
        if (classInfoInRegime == nullptr)
            Throw_LOG(Error::NO_MEMORY, 0);

        classInfoInRegime->classInfo = classInfo->classInfo;
        classInfoInRegime->constructorInfos = classInfo->constructorInfos;
        classInfoInRegime->regime = static_cast<IRegime *>(regime->_carObject.Get());

        classTemplate = NewClassTemplate(classInfo->classInfo, *classInfo->constructorInfos,
                NewInRegimeConstructor, classInfoInRegime->self()), classInfoInRegime.release();

        NAN_METHOD_RETURN_VALUE(GetFunction(classTemplate).ToLocalChecked());
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

struct _MethodInfos: WeakExternalBase {
    vector<AutoPtr<IMethodInfo >> methodInfos;
};

NAN_METHOD(CARObject::InvokeMethod)
{
#if 0//?jw
    try {
#endif
        CARObject *thatCARObject;

        struct _MethodInfos *methodInfos;

        size_t argc;

        AutoPtr<IMethodInfo> methodInfo;

        AutoPtr<IArgumentList> argumentList;

        ECode ec;

        thatCARObject = Unwrap<CARObject>(info.This());

        methodInfos = (struct _MethodInfos *)info.Data().As<External>()->Value();

        argc = info.Length();
        unique_ptr<unique_ptr<struct _Value> []> argv(
                reinterpret_cast<unique_ptr<struct _Value> *>(_ParseValues(argc, info))
                );

        AutoPtr<IFunctionInfo> functionInfo = _GetMatchingFunctionForCall<IMethodInfo>(
                    methodInfos->methodInfos.size(), (IFunctionInfo **)&methodInfos->methodInfos[0],
                    argc, reinterpret_cast<struct _Value const **>(argv.get()));
        methodInfo = static_cast<IMethodInfo *>(functionInfo.Get());

        argumentList =
            _CreateArgumentList<IMethodInfo>(methodInfo, argc, reinterpret_cast<struct _Value **>(argv.get()));

        ec = methodInfo->Invoke(thatCARObject->_carObject, argumentList);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        NAN_METHOD_RETURN_UNDEFINED();
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

Local<FunctionTemplate> CARObject::NewClassTemplate(IClassInfo const *pclassInfo,
        ArrayOf<IConstructorInfo *> &constructorInfos,
        Nan::FunctionCallback constructor, Local<Value> data)
{
    Nan::EscapableHandleScope scope;

    Local<FunctionTemplate> classTemplate;

    ECode ec;

    ThreadingModel threadingModel;
    char const *_threadingModel;

    enum ClassCategory category;
    char const *_category;

    _ELASTOS Boolean private_;
    _ELASTOS Boolean isSingleton;
    _ELASTOS Boolean isReturnValue;
    _ELASTOS Boolean isBaseClass;
    _ELASTOS String namespace_;
    _ELASTOS String name;
    _ELASTOS Boolean hasBaseClass;
    _ELASTOS Boolean hasGeneric;

    Local<ObjectTemplate> aspectTemplates;
    _ELASTOS Int32 nAspects;
    AutoPtr<ArrayOf<IClassInfo *> > aspectInfos;
    Local<ObjectTemplate> aggregateeTemplates;
    _ELASTOS Int32 nAggregatees;
    AutoPtr<ArrayOf<IClassInfo *> > aggregateeInfos;
    _ELASTOS Int32 nInterfaces;
    AutoPtr<ArrayOf<IInterfaceInfo *> > interfaceInfos;
    _ELASTOS Int32 nCallbackInterfaces = 0;

    AutoPtr<ArrayOf<ICallbackInterfaceInfo *> > callbackInterfaceInfos;

    _ELASTOS Int32 nMethods;

    AutoPtr<ArrayOf<IMethodInfo *> > methodInfos;

    IClassInfo* classInfo = const_cast<IClassInfo*>(pclassInfo);

    map<_ELASTOS String, unique_ptr<struct _MethodInfos, _MethodInfos::Deleter>> mapNameToMethodInfos;

    Local<FunctionTemplate> escapedClassTemplate;
    LOG(INFO) << "CARObject::NewClassTemplate";
    classTemplate = Nan::New<FunctionTemplate>(constructor, data);

    unique_ptr<struct _ClassInfo, _ClassInfo::Deleter> _classInfo(new(nothrow) struct _ClassInfo);
    if (_classInfo == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    _classInfo->classInfo = classInfo;
    _classInfo->constructorInfos = &constructorInfos;
LOG(INFO) << "CARObject::NewClassTemplate";
    SetTemplate(classTemplate,
            Nan::New(".__class__").ToLocalChecked(),
            _classInfo->self(),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum)), _classInfo.release();
LOG(INFO) << "CARObject::NewClassTemplate";
    classTemplate->Inherit(Nan::New(_classBaseTemplate));
LOG(INFO) << "CARObject::NewClassTemplate";
    ec = classInfo->GetThreadingModel(&threadingModel);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
LOG(INFO) << "CARObject::NewClassTemplate";
    switch (threadingModel) {
    case ThreadingModel_Sequenced:
        _threadingModel = "Sequenced";

        break;

    case ThreadingModel_Synchronized:
        _threadingModel = "Synchronized";

        break;

    case ThreadingModel_ThreadSafe:
        _threadingModel = "ThreadSafe";

        break;

    case ThreadingModel_Naked:
        _threadingModel = "Naked";

        break;

    default:
        abort();
    }
LOG(INFO) << "CARObject::NewClassTemplate";
    SetTemplate(classTemplate,
            Nan::New("$threadingModel").ToLocalChecked(),
            ToValue(_ELASTOS String(_threadingModel)),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = GetCategory(classInfo, &category);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (category) {
    case CLASS_CATEGORY_CLASS:
        _category = "class";

        break;

    case CLASS_CATEGORY_GENERIC:
        _category = "generic";

        break;

    case CLASS_CATEGORY_REGIME:
        _category = "regime";

        break;

    case CLASS_CATEGORY_ASPECT:
        _category = "aspect";

        break;

    default:
        abort();
    }
LOG(INFO) << "CARObject::NewClassTemplate";
    SetTemplate(classTemplate,
            Nan::New("$category").ToLocalChecked(),
            ToValue(_ELASTOS String(_category)),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = classInfo->IsPrivate(&private_);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    SetTemplate(classTemplate,
            Nan::New("$private").ToLocalChecked(),
            ToValueFromBoolean(private_),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));
LOG(INFO) << "CARObject::NewClassTemplate";
    ec = classInfo->IsSingleton(&isSingleton);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    SetTemplate(classTemplate,
            Nan::New("$isSingleton").ToLocalChecked(),
            ToValueFromBoolean(isSingleton),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = classInfo->IsReturnValue(&isReturnValue);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    SetTemplate(classTemplate,
            Nan::New("$isReturnValue").ToLocalChecked(),
            ToValueFromBoolean(isReturnValue),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = classInfo->IsBaseClass(&isBaseClass);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    SetTemplate(classTemplate,
            Nan::New("$isBase").ToLocalChecked(),
            ToValueFromBoolean(isBaseClass),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = classInfo->GetNamespace(&namespace_);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    SetTemplate(classTemplate,
            Nan::New("$namespace").ToLocalChecked(),
            ToValue(namespace_),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = classInfo->GetName(&name);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    SetTemplate(classTemplate,
            Nan::New("$name").ToLocalChecked(),
            ToValue(name),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    ec = classInfo->HasBaseClass(&hasBaseClass);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (hasBaseClass != FALSE) {
        AutoPtr<IClassInfo > baseClassInfo;
        IClassInfo *_baseClassInfo;

        ec = classInfo->GetBaseClassInfo(&_baseClassInfo);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        baseClassInfo = _baseClassInfo, _baseClassInfo->Release();

        SetTemplate(classTemplate,
                Nan::New("$base").ToLocalChecked(),
                NewClassTemplate(baseClassInfo),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }

    ec = classInfo->HasGeneric(&hasGeneric);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (hasGeneric != FALSE) {
        AutoPtr<IClassInfo> genericInfo;
        IClassInfo *_genericInfo;

        ec = classInfo->GetGenericInfo(&_genericInfo);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        genericInfo = _genericInfo, _genericInfo->Release();

        SetTemplate(classTemplate,
                Nan::New("$generic").ToLocalChecked(),
                NewClassTemplate(genericInfo),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }

    aspectTemplates = Nan::New<ObjectTemplate>();

    ec = classInfo->GetAspectCount(&nAspects);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    aspectInfos = ArrayOf<IClassInfo *>::Alloc(nAspects);
    if (aspectInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = classInfo->GetAllAspectInfos(reinterpret_cast<ArrayOf<IClassInfo *> *>(aspectInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    for (_ELASTOS Int32 i = 0; i < nAspects; ++i) {
        Nan::HandleScope scope_;

        IClassInfo *aspectInfo;

        _ELASTOS String aspectName;

        aspectInfo = (*aspectInfos)[i];

        ec = aspectInfo->GetName(&aspectName);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        SetTemplate(aspectTemplates,
                ToValue(aspectName).As<::v8::String>(),
                NewClassTemplate(aspectInfo),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }

    SetTemplate(classTemplate,
            Nan::New("$aspects").ToLocalChecked(),
            aspectTemplates,
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));

    aggregateeTemplates = Nan::New<ObjectTemplate>();

    ec = classInfo->GetAggregateeCount(&nAggregatees);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    aggregateeInfos = ArrayOf<IClassInfo *>::Alloc(nAggregatees);
    if (aggregateeInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = classInfo->GetAllAggregateeInfos(reinterpret_cast<ArrayOf<IClassInfo *> *>(aggregateeInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    for (_ELASTOS Int32 i = 0; i < nAggregatees; ++i) {
        Nan::HandleScope scope_;

        IClassInfo *aggregateeInfo;

        _ELASTOS String aggregateeName;

        aggregateeInfo = (*aggregateeInfos)[i];

        ec = aggregateeInfo->GetName(&aggregateeName);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        SetTemplate(aggregateeTemplates,
                ToValue(aggregateeName).As<::v8::String>(),
                NewClassTemplate(aggregateeInfo),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }

    SetTemplate(classTemplate,
            Nan::New("$aggregatees").ToLocalChecked(),
            aggregateeTemplates,
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    SetTemplate(classTemplate,
            Nan::New("constructor").ToLocalChecked(),
            CARConstructor(constructorInfos.GetLength(), constructorInfos.GetPayload()),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));

    ec = classInfo->GetInterfaceCount(&nInterfaces);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    interfaceInfos = ArrayOf<IInterfaceInfo *>::Alloc(nInterfaces);
    if (interfaceInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = classInfo->GetAllInterfaceInfos(reinterpret_cast<ArrayOf<IInterfaceInfo *> *>(interfaceInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    for (_ELASTOS Int32 i = 0; i < nInterfaces; ++i) {
        Nan::HandleScope scope_;

        IInterfaceInfo *interfaceInfo;

        _ELASTOS String interfaceName;

        interfaceInfo = (*interfaceInfos)[i];

        ec = interfaceInfo->GetName(&interfaceName);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        SetTemplate(classTemplate,
                ToValue(interfaceName).As<::v8::String>(),
                CARInterface(interfaceInfo),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }
#if 0//?jw callback
    ec = classInfo->GetCallbackInterfaceCount(&nCallbackInterfaces);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
#endif
    callbackInterfaceInfos = ArrayOf<ICallbackInterfaceInfo *>::Alloc(nCallbackInterfaces);
    if (callbackInterfaceInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);
#if 0//?jw callback
    ec = classInfo->GetAllCallbackInterfaceInfos(
            reinterpret_cast<ArrayOf<ICallbackInterfaceInfo *> *>(callbackInterfaceInfos.Get())
            );
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
#endif
    for (_ELASTOS Int32 i = 0; i < nCallbackInterfaces; ++i) {
        Nan::HandleScope scope_;

        ICallbackInterfaceInfo *callbackInterfaceInfo;

        _ELASTOS String callbackInterfaceName;

        callbackInterfaceInfo = (*callbackInterfaceInfos)[i];

        ec = callbackInterfaceInfo->GetName(&callbackInterfaceName);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        SetTemplate(classTemplate,
                ToValue(callbackInterfaceName).As<::v8::String>(),
                CARCallbackInterface(callbackInterfaceInfo),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }

    if (category == CLASS_CATEGORY_ASPECT) {
        struct _ClassId *_classId;

        unique_ptr<struct _ClassId, _ClassId::Deleter> classId(new(nothrow) struct _ClassId);
        if (classId == nullptr)
            Throw_LOG(Error::NO_MEMORY, 0);

        SetMethod(classTemplate,
                "$attach",
                Attach, classId->self(),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
        _classId = classId.release();

        SetMethod(classTemplate,
                "$detach",
                Detach, _classId->self(),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));

        SetMethod(classTemplate,
                "$probe",
                Probe, _classId->self(),
                static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
    }

    SetMethod(classTemplate, "$inRegime", InRegime, static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));

    ec = classInfo->GetMethodCount(&nMethods);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    methodInfos = ArrayOf<IMethodInfo *>::Alloc(nMethods);
    if (methodInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = classInfo->GetAllMethodInfos(reinterpret_cast<ArrayOf<IMethodInfo *> *>(methodInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    for (_ELASTOS Int32 i = 0; i < nMethods; ++i) {
        IMethodInfo *methodInfo;

        _ELASTOS String methodName;

        methodInfo = (*methodInfos)[i];

        ec = methodInfo->GetName(&methodName);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        auto &_methodInfos = mapNameToMethodInfos[methodName];
        if (_methodInfos == nullptr) {
            _methodInfos = unique_ptr<struct _MethodInfos, _MethodInfos::Deleter>(new(nothrow) struct _MethodInfos);
            if (_methodInfos == nullptr)
                Throw_LOG(Error::NO_MEMORY, 0);
        }
        _methodInfos->methodInfos.push_back(methodInfo);
    }

    for (auto it = mapNameToMethodInfos.begin(), end = mapNameToMethodInfos.end(); it != end; ++it) {
        Nan::HandleScope scope_;

        SetPrototypeMethod(classTemplate, it->first, InvokeMethod, it->second->self()), it->second.release();
    }

    escapedClassTemplate = scope.Escape(classTemplate);

    return escapedClassTemplate;
}

Local<FunctionTemplate> CARObject::NewClassTemplate(IClassInfo const *pclassInfo,
        Nan::FunctionCallback constructor, Local<Value> data)
{
    ECode ec;

    _ELASTOS Int32 nConstructors;

    AutoPtr<ArrayOf<IConstructorInfo*> > constructorInfos;
    IClassInfo* classInfo = const_cast<IClassInfo*>(pclassInfo);
    ec = classInfo->GetConstructorCount(&nConstructors);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    constructorInfos = ArrayOf<IConstructorInfo *>::Alloc(nConstructors);
    if (constructorInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = classInfo->GetAllConstructorInfos(reinterpret_cast<ArrayOf<IConstructorInfo *> *>(constructorInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    return NewClassTemplate(pclassInfo, *constructorInfos, constructor, data);
}

NAN_METHOD_RETURN_TYPE CARObject::ClassConstructor(NAN_METHOD_ARGS_TYPE info, Constructor constructor)
{
    Local<Object> that = info.This();

    size_t argc;

    CARObject *thatCARObject;

    argc = info.Length();
    unique_ptr<Local<Value> []> argv(new(nothrow) Local<Value>[argc]);
    if (argv == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    for (size_t i = 0; i < argc; ++i)
        argv[i] = info[i];

    if (!info.IsConstructCall()) {
        Nan::HandleScope scope;
#if 0//?jw v8 remove
        NAN_METHOD_RETURN_VALUE(NewInstance(info.Callee(), argc, argv.get()).ToLocalChecked());
#endif
        return;
    }

    thatCARObject = constructor(argc, argv.get(), info.Data());

    thatCARObject->Wrap(that);

    NAN_METHOD_RETURN_VALUE(that);
}

CARObject::CARObject(IClassInfo const *pclassInfo, ArrayOf<IConstructorInfo *> &constructorInfos,
        size_t argc, Local<Value> argv[])
{
    ECode ec;
    IInterface *carObject = nullptr;
    unique_ptr<unique_ptr<struct _Value> []> _argv;
    AutoPtr<IConstructorInfo> constructorInfo;
    AutoPtr<IArgumentList> argumentList;
    AutoPtr<IFunctionInfo> functionInfo;

    _classInfo = const_cast<IClassInfo*>(pclassInfo);
    IClassInfo* classInfo = const_cast<IClassInfo*>(pclassInfo);
    if (argc == 0) {
        ec = classInfo->CreateObject(&carObject);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        goto done;
    }

    _argv = unique_ptr<unique_ptr<struct _Value> []>(
            reinterpret_cast<unique_ptr<struct _Value> *>(_ParseValues(argc, argv))
            );

    functionInfo = _GetMatchingFunctionForCall<IConstructorInfo>(
                constructorInfos.GetLength(), reinterpret_cast<IFunctionInfo **>(constructorInfos.GetPayload()),
                argc, reinterpret_cast<struct _Value const **>(_argv.get()));
    constructorInfo = static_cast<IConstructorInfo *>(functionInfo.Get());

    argumentList =
        _CreateArgumentList<IConstructorInfo>(constructorInfo, argc, reinterpret_cast<struct _Value **>(_argv.get()));

    ec = constructorInfo->CreateObject(argumentList, &carObject);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

done:
    _carObject = carObject, carObject->Release();
}

CARObject *CARObject::NewConstructor(size_t argc, Local<Value> argv[], Local<Value> data)
{
    struct _ClassInfo *classInfo;
    CARObject *carObject = nullptr;

    classInfo = (struct _ClassInfo *)data.As<External>()->Value();
    carObject = new(nothrow) CARObject(classInfo->classInfo, *classInfo->constructorInfos, argc, argv);
    if (carObject == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    return carObject;
}

NAN_METHOD(CARObject::NewConstructor)
{
#if 0//?jw
    try {
#endif
        ClassConstructor(info, NewConstructor);
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
static map<AutoPtr<IClassInfo>, CopyablePersistent<FunctionTemplate>> _mapClassInfoToCARClass;
#pragma clang diagnostic pop
Local<FunctionTemplate> CARObject::NewClassTemplate(IClassInfo const *pclassInfo)
{
    Nan::EscapableHandleScope scope;

    ECode ec;

    _ELASTOS Int32 nConstructors;

    AutoPtr<ArrayOf<IConstructorInfo *> > constructorInfos;

    Local<FunctionTemplate> classTemplate;

    Local<FunctionTemplate> escapedClassTemplate;
    IClassInfo* classInfo = const_cast<IClassInfo*>(pclassInfo);
    auto &_classTemplate = _mapClassInfoToCARClass[classInfo];
    if (!_classTemplate.IsEmpty())
        return Nan::New(_classTemplate);

    ec = classInfo->GetConstructorCount(&nConstructors);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    constructorInfos = ArrayOf<IConstructorInfo *>::Alloc(nConstructors);
    if (constructorInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = classInfo->GetAllConstructorInfos(reinterpret_cast<ArrayOf<IConstructorInfo *> *>(constructorInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    unique_ptr<struct _ClassInfo, _ClassInfo::Deleter> _classInfo(new(nothrow) struct _ClassInfo);
    if (_classInfo == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    _classInfo->classInfo = classInfo;
    _classInfo->constructorInfos = constructorInfos;
    LOG(INFO) << "NewClassTemplate classInfo:" << classInfo << " constructorInfos:" << *constructorInfos ; 
    classTemplate = NewClassTemplate(classInfo, *constructorInfos, NewConstructor, _classInfo->self());

    _classInfo.release();

    _classTemplate.Reset(classTemplate);

    escapedClassTemplate = scope.Escape(classTemplate);

    return escapedClassTemplate;
}

CARObject::CARObject(IClassInfo const *classInfo, IInterface *carObject) noexcept
{
    _classInfo = const_cast<IClassInfo*>(classInfo);

    _carObject = carObject;
}

struct _CARObject {
    IClassInfo const *classInfo;

    IInterface *carObject;
};

CARObject *CARObject::WrapConstructor(size_t argc, Local<Value> argv[], Local<Value> data)
{
    struct _CARObject const *carObject;

    CARObject *_carObject;

    carObject = (struct _CARObject const *)data.As<External>()->Value();

    _carObject = new(nothrow) CARObject(carObject->classInfo, carObject->carObject);
    if (_carObject == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    return _carObject;
}

NAN_METHOD(CARObject::WrapConstructor)
{
#if 0//?jw
    try {
#endif
        ClassConstructor(info, WrapConstructor);
#if 0//?jw
    } catch (Error const &error) {
        Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

Local<Object> CARObject::New(IInterface *carObject)
{
    Nan::EscapableHandleScope scope;

    ECode ec;

    AutoPtr<IClassInfo> classInfo;
    IClassInfo *_classInfo;

    struct _CARObject _carObject;

    Local<Function> class_;

    ec = CObject::ReflectClassInfo(carObject, &_classInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    classInfo = _classInfo, _classInfo->Release();

    _carObject.classInfo = classInfo;
    _carObject.carObject = carObject;

    class_ = GetFunction(
            NewClassTemplate(classInfo, WrapConstructor, Nan::New<External>(&_carObject))
            ).ToLocalChecked();

    return scope.Escape(NewInstance(class_).ToLocalChecked());
}

bool CARObject::HasInstance(Local<Object> object)
{
    return Nan::New(_classBaseTemplate)->HasInstance(object);
}

CAR_BRIDGE_NAMESPACE_END