#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include "nan.h"
#include <elastos.h>
#include "macros.h"
#include "libc-ext.h"
#include "car-interface-adapter.h"
#include "car-object.h"
#include "error.h"

using namespace Nan;
using namespace v8;

_ELASTOS_NAMESPACE_USING
CAR_BRIDGE_NAMESPACE_BEGIN

bool Is(IDataTypeInfo  *dataTypeInfo, Local<Value> value);
bool CanBeUsedAs(IDataTypeInfo  *dataTypeInfo, Local<Value> value, int *priority);
bool IsInt16(Local<Value> value)
{
    int32_t i32;
    if (!value->IsInt32())
        return false;
    i32 = To<int32_t>(value).FromJust();
    return i32 >= INT16_MIN && i32 <= INT16_MAX;
}

bool CanBeUsedAsInt16(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Int32> i32;

    int _priority;
    if (!To<v8::Int32>(value).ToLocal(&i32))
        return false;
    if (!IsInt16(i32))
        return false;
    if (value->IsInt32())
        _priority = 0;
    else if (value->IsNumber())
        _priority = 1;
    else if (value->IsNumberObject())
        _priority = 2;
    else if (value->IsString())
        _priority = 3;
    else if (value->IsStringObject())
        _priority = 4;
    else
        _priority = 5;

    if (priority != nullptr)
        *priority = _priority;

    return true;
}

Int16 ToInt16(Local<Value> value)
{
    return To<int32_t>(value).FromJust();
}

Local<Value> ToValue(Int16 i16)
{
    return New<v8::Int32>(i16);
}

bool IsInt32(Local<Value> value)
{
    return value->IsInt32();
}

bool CanBeUsedAsInt32(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Int32> i32;
    int _priority;
    if (!To<v8::Int32>(value).ToLocal(&i32))
        return false;
    if (!IsInt32(i32))
        return false;
    if (value->IsInt32())
        _priority = 0;
    else if (value->IsNumber())
        _priority = 1;
    else if (value->IsNumberObject())
        _priority = 2;
    else if (value->IsString())
        _priority = 3;
    else if (value->IsStringObject())
        _priority = 4;
    else
        _priority = 5;

    if (priority != nullptr)
        *priority = _priority;

    return true;
}

Elastos::Int32 ToInt32(Local<Value> value)
{
    return To<int32_t>(value).FromJust();
}

Local<Value> ToValue(Elastos::Int32 i32)
{
    return New<v8::Int32>(i32);
}

static bool _IsInteger(Local<Number> number, double *integer)
{
    double d;
    d = To<double>(number).FromJust();
    if (nearbyint(d) != d)
        return false;
    if (integer != nullptr)
        *integer = d;
    return true;
}

bool IsInt64(Local<Value> value)
{
    double integer;
    if (!value->IsNumber())
        return false;
    if (!_IsInteger(value.As<Number>(), &integer))
        return false;
    return integer >= INT64_MIN && integer <= INT64_MAX;
}

bool CanBeUsedAsInt64(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<Number> number;
    int _priority;
    if (!To<Number>(value).ToLocal(&number))
        return false;
    if (!IsInt64(number))
        return false;
    if (value->IsNumber())
        _priority = 0;
    else if (value->IsNumberObject())
        _priority = 1;
    else if (value->IsString())
        _priority = 2;
    else if (value->IsStringObject())
        _priority = 3;
    else
        _priority = 4;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

Int64 ToInt64(Local<Value> value)
{
    return To<int64_t>(value).FromJust();
}

Local<Value> ToValue(Int64 i64)
{
    return New<Number>(i64);
}

bool IsByte(Local<Value> value)
{
    uint32_t ui32;
    if (!value->IsUint32())
        return false;
    ui32 = To<uint32_t>(value).FromJust();
    return ui32 <= UINT8_MAX;
}

bool CanBeUsedAsByte(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<Uint32> ui32;
    int _priority;
    if (!To<Uint32>(value).ToLocal(&ui32))
        return false;
    if (!IsByte(ui32))
        return false;
    if (value->IsUint32())
        _priority = 0;
    else if (value->IsNumber())
        _priority = 1;
    else if (value->IsNumberObject())
        _priority = 2;
    else if (value->IsString())
        _priority = 3;
    else if (value->IsStringObject())
        _priority = 4;
    else
        _priority = 5;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

Byte ToByte(Local<Value> value)
{
    return To<uint32_t>(value).FromJust();
}

Local<Value> ToValue(Byte byte)
{
    return New<Uint32>(byte);
}

bool IsFloat(Local<Value> value)
{
    return value->IsNumber();
}

bool CanBeUsedAsFloat(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<Number> number;
    int _priority;
    if (!To<Number>(value).ToLocal(&number))
        return false;
    if (!IsFloat(number))
        return false;
    if (value->IsNumber())
        _priority = 0;
    else if (value->IsNumberObject())
        _priority = 1;
    else if (value->IsString())
        _priority = 2;
    else if (value->IsStringObject())
        _priority = 3;
    else
        _priority = 4;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

Float ToFloat(Local<Value> value)
{
    return To<double>(value).FromJust();
}

Local<Value> ToValue(Float f)
{
    return New<Number>(f);
}

bool IsDouble(Local<Value> value)
{
    return value->IsNumber();
}

bool CanBeUsedAsDouble(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<Number> number;
    int _priority;
    if (!To<Number>(value).ToLocal(&number))
        return false;
    if (!IsDouble(number))
        return false;
    if (value->IsNumber())
        _priority = 0;
    else if (value->IsNumberObject())
        _priority = 1;
    else if (value->IsString())
        _priority = 2;
    else if (value->IsStringObject())
        _priority = 3;
    else
        _priority = 4;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

Double ToDouble(Local<Value> value)
{
    return To<double>(value).FromJust();
}

Local<Value> ToValue(Double d)
{
    return New<Number>(d);
}

bool IsChar32(Local<Value> value)
{
    if (!value->IsString())
        return false;
    return value.As<v8::String>()->Length() == 1;
}

bool CanBeUsedAsChar32(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::String> string;
    int _priority;
    if (!To<v8::String>(value).ToLocal(&string))
        return false;
    if (!IsChar32(string))
        return false;
    if (value->IsString())
        _priority = 0;
    else if (value->IsStringObject())
        _priority = 1;
    else
        _priority = 2;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

Char32 ToChar32(Local<Value> value)
{
    return To<uint32_t>(value).FromJust();
}

Local<Value> ToValue(Char32 c32)
{
    return New<Uint32>(c32);
}

bool IsString(Local<Value> value)
{
    return value->IsString();
}

bool CanBeUsedAsString(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::String> string;
    int _priority;
    if (!To<v8::String>(value).ToLocal(&string))
        return false;
    if (value->IsString())
        _priority = 0;
    else if (value->IsStringObject())
        _priority = 1;
    else
        _priority = 2;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

void ToString(Elastos::String &s, Local<Value> value)
{
    s = *Utf8String(value);
    Debug_LOG("%s", s.string());
}

Local<Value> ToValue(Elastos::String const &s)
{
    //return New((char const *)s).ToLocalChecked();
    Debug_LOG("%s", s.string());
    return New((char const *)s, s.GetByteLength()).ToLocalChecked();
}

bool IsBoolean(Local<Value> value)
{
    return value->IsBoolean();
}

bool CanBeUsedAsBoolean(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Boolean> boolean;
    int _priority;
    if (!To<v8::Boolean>(value).ToLocal(&boolean))
        return false;
    if (value->IsBoolean())
        _priority = 0;
    else if (value->IsBooleanObject())
        _priority = 1;
    else
        _priority = 2;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

Elastos::Boolean ToBoolean(Local<Value> value)
{
    return To<bool>(value).FromJust() ? TRUE : FALSE;
}

Local<Value> ToValueFromBoolean(Elastos::Boolean b)
{
    bool _b;
    _b = b != FALSE ? true : false;
    return New(_b);
}

inline bool _IsUInt16(Local<Value> value)
{
    if (!value->IsUint32())
        return false;

    return To<uint32_t>(value).FromJust() <= UINT16_MAX;
}

inline bool _IsUInt8(Local<Value> value)
{
    if (!value->IsUint32())
        return false;

    return To<uint32_t>(value).FromJust() <= UINT8_MAX;
}

bool IsEMuid(Local<Value> value)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    Local<Value> data4;
    Local<Array> _data4;
    if (!value->IsObject())
        return false;
    object = value.As<v8::Object>();
    if (GetOwnPropertyNames(object).ToLocalChecked()->Length() != 4)
        return false;
    if (!HasOwnProperty(object, New(".mData1").ToLocalChecked()).FromJust())
        return false;
    if (!HasOwnProperty(object, New(".mData2").ToLocalChecked()).FromJust())
        return false;
    if (!HasOwnProperty(object, New(".mData3").ToLocalChecked()).FromJust())
        return false;
    if (!HasOwnProperty(object, New(".mData4").ToLocalChecked()).FromJust())
        return false;
    if (!Get(object, New(".mData1").ToLocalChecked()).ToLocalChecked()->IsUint32())
        return false;
    if (!_IsUInt16(Get(object, New(".mData2").ToLocalChecked()).ToLocalChecked()))
        return false;
    if (!_IsUInt16(Get(object, New(".mData3").ToLocalChecked()).ToLocalChecked()))
        return false;
    data4 = Get(object, New(".mData4").ToLocalChecked()).ToLocalChecked();
    if (!data4->IsArray())
        return false;
    _data4 = data4.As<Array>();
    if (_data4->Length() != 8)
        return false;
    for (size_t i = 0; i < 8; ++i)
    {
        Nan::HandleScope scope_;
        if (!_IsUInt8(Get(_data4, i).ToLocalChecked()))
            return false;
    }
    return true;
}

bool CanBeUsedAsEMuid(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    int _priority;
    if (!To<v8::Object>(value).ToLocal(&object))
        return false;
    if (!IsEMuid(object))
        return false;
    if (value->IsObject())
        _priority = 0;
    else
        _priority = 1;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

void ToEMuid(EMuid *id, Local<Value> value)
{
    Nan::HandleScope scope;
    Local<v8::Object> _id;
    Local<Array> data4;
    _id = value.As<v8::Object>();

    id->mData1 = To<uint32_t>(Get(_id, New("mData1").ToLocalChecked()).ToLocalChecked()).FromJust();
    id->mData2 = To<uint32_t>(Get(_id, New("mData2").ToLocalChecked()).ToLocalChecked()).FromJust();
    id->mData3 = To<uint32_t>(Get(_id, New("mData3").ToLocalChecked()).ToLocalChecked()).FromJust();
    data4 = Get(_id, New("mData4").ToLocalChecked()).ToLocalChecked().As<Array>();
    for (size_t i = 0; i < 8; ++i)
    {
        Nan::HandleScope scope_;
        id->mData4[i] = To<uint32_t>(Get(data4, i).ToLocalChecked()).FromJust();
    }
}

Local<Value> ToValue(EMuid  *id)
{
    Nan::EscapableHandleScope scope;
    Local<v8::Object> _id;
    Local<Array> data4;

    _id = New<v8::Object>();

    Nan::Set(_id, New(".mData1").ToLocalChecked(), New<Uint32>(id->mData1));
    Nan::Set(_id, New(".mData2").ToLocalChecked(), New<Uint32>(id->mData2));
    Nan::Set(_id, New(".mData3").ToLocalChecked(), New<Uint32>(id->mData3));
    data4 = New<Array>(8);

    for (size_t i = 0; i < 8; ++i)
    {
        Nan::HandleScope scope_;
        Nan::Set(data4, i, New<Uint32>(id->mData4[i]));
    }

    Nan::Set(_id, New(".mData4").ToLocalChecked(), data4);

    return scope.Escape(_id);
}

bool IsEGuid(Local<Value> value)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;

    if (!value->IsObject())
        return false;

    object = value.As<v8::Object>();
    if (GetOwnPropertyNames(object).ToLocalChecked()->Length() != 3)
        return false;
    if (!HasOwnProperty(object, New(".mClsid").ToLocalChecked()).FromJust())
        return false;
    if (!HasOwnProperty(object, New(".mUunm").ToLocalChecked()).FromJust())
        return false;
    if (!HasOwnProperty(object, New(".mCarcode").ToLocalChecked()).FromJust())
        return false;
    if (!IsEMuid(Get(object, New(".mClsid").ToLocalChecked()).ToLocalChecked()))
        return false;
    if (!Get(object, New(".mUunm").ToLocalChecked()).ToLocalChecked()->IsString())
        return false;
    if (!Get(object, New(".mCarcode").ToLocalChecked()).ToLocalChecked()->IsUint32())
        return false;

    return true;
}

bool CanBeUsedAsEGuid(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    int _priority;

    if (!To<v8::Object>(value).ToLocal(&object))
        return false;

    if (!IsEGuid(object))
        return false;

    if (value->IsObject())
        _priority = 0;
    else
        _priority = 1;

    if (priority != nullptr)
        *priority = _priority;

    return true;
}

void ToEGuid(EGuid *id, Local<Value> value)
{
    Nan::HandleScope scope;
    Local<v8::Object> _id;
    _id = value.As<v8::Object>();

    ToEMuid(&id->mClsid, Get(_id, New(".mClsid").ToLocalChecked()).ToLocalChecked());
    id->mUunm = strdup(*Utf8String(Get(_id, New(".mUunm").ToLocalChecked()).ToLocalChecked()));
    if (id->mUunm == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    id->mCarcode = To<uint32_t>(Get(_id, New(".mCarcode").ToLocalChecked()).ToLocalChecked()).FromJust();
}

Local<Value> ToValue(EGuid  *id)
{
    Nan::EscapableHandleScope scope;
    Local<v8::Object> _id;

    _id = New<v8::Object>();

    Nan::Set(_id, New(".mClsid").ToLocalChecked(), ToValue(&id->mClsid));
    Nan::Set(_id, New(".mUunm").ToLocalChecked(), New(id->mUunm).ToLocalChecked());
    Nan::Set(_id, New(".mCarcode").ToLocalChecked(), New<Uint32>(id->mCarcode));

    return scope.Escape(_id);
}

bool IsECode(Local<Value> value)
{
    return value->IsInt32();
}

bool CanBeUsedAsECode(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Int32> i32;
    int _priority;

    if (!To<v8::Int32>(value).ToLocal(&i32))
        return false;

    if (!IsECode(i32))
        return false;
    if (value->IsInt32())
        _priority = 0;
    else if (value->IsNumber())
        _priority = 1;
    else if (value->IsNumberObject())
        _priority = 2;
    else if (value->IsString())
        _priority = 3;
    else if (value->IsStringObject())
        _priority = 4;
    else
        _priority = 5;
    if (priority != nullptr)
        *priority = _priority;

    return true;
}

ECode ToECode(Local<Value> value)
{
    return To<int32_t>(value).FromJust();
}

Local<Value> ToValueFromECode(ECode ecode)
{
    return New<v8::Int32>(ecode);
}

bool IsLocalPtr(Local<Value> value)
{
    return value->IsExternal();
}

bool CanBeUsedAsLocalPtr(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    int _priority;
    Local<Number> number;
    double integer;

    if (IsLocalPtr(value))
    {
        _priority = 0;
        goto exit;
    }

    if (!To<Number>(value).ToLocal(&number))
        return false;
    if (!_IsInteger(number, &integer))
        return false;
    if (integer < 0 || integer > UINTPTR_MAX)
        return false;
    if (value->IsNumber())
        _priority = 1;
    else if (value->IsNumberObject())
        _priority = 2;
    else if (value->IsString())
        _priority = 3;
    else if (value->IsStringObject())
        _priority = 4;
    else
        _priority = 5;

exit:
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

void *ToLocalPtr(Local<Value> value)
{
    if (value->IsExternal())
        return value.As<External>()->Value();
    //?jw
    double temp = To<double>(value).FromJust();
    return (void *) * (uint64_t *) &temp;
}

Local<Value> ToValue(void *localPtr)
{
    return New<External>(localPtr);
}

bool IsEnum(Local<Value> value)
{
    return value->IsInt32();
}

bool CanBeUsedAsEnum(Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Int32> i32;
    int _priority;

    if (!To<v8::Int32>(value).ToLocal(&i32))
        return false;

    if (!IsEnum(i32))
        return false;

    if (value->IsInt32())
        _priority = 0;
    else if (value->IsNumber())
        _priority = 1;
    else if (value->IsNumberObject())
        _priority = 2;
    else if (value->IsString())
        _priority = 3;
    else if (value->IsStringObject())
        _priority = 4;
    else
        _priority = 5;
    if (priority != nullptr)
        *priority = _priority;

    return true;
}

Elastos::Int32 ToEnum(Local<Value> value)
{
    return To<int32_t>(value).FromJust();
}

Local<Value> ToValueFromEnum(Elastos::Int32 enum_)
{
    return New<v8::Int32>(enum_);
}

bool IsCARArray(ICarArrayInfo *carArrayInfo, Local<Value> value)
{
    Local<Array> array;
    ECode ec;
    AutoPtr<IDataTypeInfo> elementTypeInfo;

    IDataTypeInfo *_elementTypeInfo;
    if (!value->IsArray())
        return false;

    array = value.As<Array>();
    ec = carArrayInfo->GetElementTypeInfo(&_elementTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    elementTypeInfo = _elementTypeInfo, _elementTypeInfo->Release();
    for (size_t i = 0, length = array->Length(); i < length; ++i)
    {
        Nan::HandleScope scope_;
        if (!Is(elementTypeInfo, Get(array, i).ToLocalChecked()))
            return false;
    }

    return true;
}

bool CanBeUsedAsCARArray(ICarArrayInfo  *carArrayInfo, Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    int _priority;

    if (!To<v8::Object>(value).ToLocal(&object))
        return false;
    if (!IsCARArray(carArrayInfo, object))
        return false;
    if (value->IsArray())
        _priority = 0;
    else if (value->IsObject())
        _priority = 1;
    else
        _priority = 2;
    if (priority != nullptr)
        *priority = _priority;
    return true;
}

template<class ArraySetter>
inline void _SetInt16Element(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetInt16Element(index, ToInt16(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetInt32Element(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetInt32Element(index, ToInt32(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetInt64Element(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetInt64Element(index, ToInt64(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetByteElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetByteElement(index, ToByte(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetFloatElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetFloatElement(index, ToFloat(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetDoubleElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetDoubleElement(index, ToDouble(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetChar32Element(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetCharElement(index, ToChar32(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetStringElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    Elastos::String s;
    ECode ec;
    ToString(s, value);
    ec = arraySetter->SetStringElement(index, s);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetBooleanElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetBooleanElement(index, ToBoolean(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetEMuidElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    EMuid id;
    ECode ec;
    ToEMuid(&id, value);
    ec = arraySetter->SetEMuidElement(index, &id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetEGuidElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    EGuid id;
    ECode ec;
    ToEGuid(&id, value);
    ec = arraySetter->SetEGuidElement(index, &id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetECodeElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetECodeElement(index, ToECode(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetLocalPtrElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetLocalPtrElement(index, ToLocalPtr(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

template<class ArraySetter>
inline void _SetEnumElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    ECode ec;
    ec = arraySetter->SetEnumElement(index, ToEnum(value));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

void ToCPPVector(ICppVectorInfo  *cppVectorInfo, ICppVectorSetter *cppVectorSetter, Local<Value> value);
template<class ArraySetter>
inline void _SetCPPVectorElement(ArraySetter *arraySetter,
                                 Elastos::Int32 index,
                                 Local<Value> value,
                                 IDataTypeInfo  *dataTypeInfo)
{
    ECode ec;
    AutoPtr<ICppVectorSetter> cppVectorSetter;
    ICppVectorSetter *_cppVectorSetter;

    ec = arraySetter->GetCppVectorElementSetter(index, &_cppVectorSetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    cppVectorSetter = _cppVectorSetter, _cppVectorSetter->Release();
    ToCPPVector(static_cast<ICppVectorInfo *>(dataTypeInfo), cppVectorSetter, value);
}

void ToStruct(IStructInfo  *structInfo, IStructSetter *structSetter, Local<Value> value);
template<class ArraySetter>
inline void _SetStructElement(ArraySetter *arraySetter,
                              Elastos::Int32 index,
                              Local<Value> value,
                              IDataTypeInfo  *dataTypeInfo)
{
    ECode ec;
    AutoPtr<IStructSetter> structSetter;
    IStructSetter *_structSetter;

    ec = arraySetter->GetStructElementSetter(index, &_structSetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    structSetter = _structSetter, _structSetter->Release();
    ToStruct(static_cast<IStructInfo *>(dataTypeInfo), structSetter, value);
}

IInterface *AsCARObject(Local<Value> value);
template<class ArraySetter>
inline void _SetCARObjectElement(ArraySetter *arraySetter, Elastos::Int32 index, Local<Value> value)
{
    IInterface *carObject;
    ECode ec;
    carObject = AsCARObject(value);
    ec = arraySetter->SetObjectPtrElement(index, carObject);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
}

void ToCARArray(ICarArrayInfo *carArrayInfo, ICarArraySetter *carArraySetter, Local<Value> value)
{
    typedef void (*SetElement)(ICarArraySetter * carArraySetter, Elastos::Int32 index, Local<Value> value, ...);
    ECode ec;
    AutoPtr<IDataTypeInfo> elementTypeInfo;
    IDataTypeInfo *_elementTypeInfo;
    CarDataType dataType;
    SetElement setElement;
    Local<Array> array;

    ec = carArrayInfo->GetElementTypeInfo(&_elementTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    elementTypeInfo = _elementTypeInfo, _elementTypeInfo->Release();
    ec = elementTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType)
    {
    case CarDataType_Int16:
        setElement = (SetElement)&_SetInt16Element<ICarArraySetter>;
        break;
    case CarDataType_Int32:
        setElement = (SetElement)&_SetInt32Element<ICarArraySetter>;
        break;
    case CarDataType_Int64:
        setElement = (SetElement)&_SetInt64Element<ICarArraySetter>;
        break;
    case CarDataType_Byte:
        setElement = (SetElement)&_SetByteElement<ICarArraySetter>;
        break;
    case CarDataType_Float:
        setElement = (SetElement)&_SetFloatElement<ICarArraySetter>;
        break;
    case CarDataType_Double:
        setElement = (SetElement)&_SetDoubleElement<ICarArraySetter>;
        break;
    case CarDataType_Char32:
        setElement = (SetElement)&_SetChar32Element<ICarArraySetter>;
        break;
    case CarDataType_String:
        setElement = (SetElement)&_SetStringElement<ICarArraySetter>;
        break;
    case CarDataType_Boolean:
        setElement = (SetElement)&_SetBooleanElement<ICarArraySetter>;
        break;
    case CarDataType_EMuid:
        setElement = (SetElement)&_SetEMuidElement<ICarArraySetter>;
        break;
    case CarDataType_EGuid:
        setElement = (SetElement)&_SetEGuidElement<ICarArraySetter>;
        break;
    case CarDataType_ECode:
        setElement = (SetElement)&_SetECodeElement<ICarArraySetter>;
        break;
    case CarDataType_Enum:
        setElement = (SetElement)&_SetEnumElement<ICarArraySetter>;
        break;
#if 0
    case CarDataType_ArrayOf:
        setElement = (SetElement)&_SetCARArrayElement<ICarArraySetter>;
        break;
#endif
#if 0
    case CarDataType_CppVector:
        setElement = (SetElement)&_SetCPPVectorElement<ICarArraySetter>;
        break;
#endif
    case CarDataType_Struct:
        setElement = (SetElement)&_SetStructElement<ICarArraySetter>;
        break;
    case CarDataType_Interface:
        setElement = (SetElement)&_SetCARObjectElement<ICarArraySetter>;
        break;
    default:
        abort();
    }
    array = value.As<Array>();
    for (size_t i = 0, length = array->Length(); i < length; ++i)
    {
        Nan::HandleScope scope;
        (*setElement)(carArraySetter, i, Get(array, i).ToLocalChecked(), elementTypeInfo.Get());
    }
}

void ToCARArray(ICarArrayInfo *carArrayInfo, CarQuintet *carQuintet, Local<Value> value)
{
    ECode ec;
    AutoPtr<IVariableOfCarArray> variableOfCARArray;
    IVariableOfCarArray *_variableOfCARArray;
    AutoPtr<ICarArraySetter> carArraySetter;
    ICarArraySetter *_carArraySetter;

    ec = carArrayInfo->CreateVariableBox(carQuintet, &_variableOfCARArray);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfCARArray = _variableOfCARArray, _variableOfCARArray->Release();

    ec = variableOfCARArray->GetSetter(&_carArraySetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    carArraySetter = _carArraySetter, _carArraySetter->Release();
    ToCARArray(carArrayInfo, carArraySetter, value);
}

template<class ArrayGetter>
inline Local<Value> _GetInt16Element(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Int16 i16;
    ec = arrayGetter->GetInt16Element(index, &i16);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(i16);
}

template<class ArrayGetter>
inline Local<Value> _GetInt32Element(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Elastos::Int32 i32;
    ec = arrayGetter->GetInt32Element(index, &i32);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(i32);
}

template<class ArrayGetter>
inline Local<Value> _GetInt64Element(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Int64 i64;
    ec = arrayGetter->GetInt64Element(index, &i64);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(i64);
}

template<class ArrayGetter>
inline Local<Value> _GetByteElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Byte byte;
    ec = arrayGetter->GetByteElement(index, &byte);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(byte);
}

template<class ArrayGetter>
inline Local<Value> _GetFloatElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Float f;
    ec = arrayGetter->GetFloatElement(index, &f);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(f);
}

template<class ArrayGetter>
inline Local<Value> _GetDoubleElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Double d;
    ec = arrayGetter->GetDoubleElement(index, &d);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(d);
}

template<class ArrayGetter>
inline Local<Value> _GetChar32Element(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Char32 c32;
    ec = arrayGetter->GetCharElement(index, &c32);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(c32);
}

template<class ArrayGetter>
inline Local<Value> _GetStringElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Elastos::String s;
    ec = arrayGetter->GetStringElement(index, &s);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(s);
}

template<class ArrayGetter>
inline Local<Value> _GetBooleanElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Elastos::Boolean b;
    ec = arrayGetter->GetBooleanElement(index, &b);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValueFromBoolean(b);
}

template<class ArrayGetter>
inline Local<Value> _GetEMuidElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    EMuid id;
    ec = arrayGetter->GetEMuidElement(index, &id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(&id);
}

template<class ArrayGetter>
inline Local<Value> _GetEGuidElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    EGuid id;
    ec = arrayGetter->GetEGuidElement(index, &id);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(&id);
}

template<class ArrayGetter>
inline Local<Value> _GetECodeElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    ECode ecode;
    ec = arrayGetter->GetECodeElement(index, &ecode);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValueFromECode(ecode);
}

template<class ArrayGetter>
inline Local<Value> _GetLocalPtrElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    void *localPtr;
    ec = arrayGetter->GetLocalPtrElement(index, &localPtr);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValue(localPtr);
}

template<class ArrayGetter>
inline Local<Value> _GetEnumElement(ArrayGetter *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    Elastos::Int32 enum_;
    ec = arrayGetter->GetEnumElement(index, &enum_);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    return ToValueFromEnum(enum_);
}
#if 0
template<class ArrayGetter>
inline Local<Value> _GetCARArrayElement(ArrayGetter  *arrayGetter,
                                        Elastos::Int32 index,
                                        IDataTypeInfo  *dataTypeInfo)
{
    return Local<Value>();
}
#endif

Local<Value> ToValue(ICppVectorInfo  *cppVectorInfo, ICppVectorGetter  *cppVectorGetter);
template<class ArrayGetter>
inline Local<Value> _GetCPPVectorElement(ArrayGetter  *arrayGetter,
        Elastos::Int32 index,
        IDataTypeInfo  *dataTypeInfo)
{
    ECode ec;
    AutoPtr<ICppVectorGetter > cppVectorGetter;
    ICppVectorGetter *_cppVectorGetter;

    ec = arrayGetter->GetCppVectorElementGetter(index, &_cppVectorGetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    cppVectorGetter = _cppVectorGetter, _cppVectorGetter->Release();

    return ToValue(static_cast<ICppVectorInfo *>(dataTypeInfo), cppVectorGetter);
}

Local<Value> ToValue(IStructInfo  *structInfo, IStructGetter  *structGetter);
template<class ArrayGetter>
inline Local<Value> _GetStructElement(ArrayGetter  *arrayGetter,
                                      Elastos::Int32 index,
                                      IDataTypeInfo  *dataTypeInfo)
{
    ECode ec;
    AutoPtr<IStructGetter > structGetter;
    IStructGetter *_structGetter;

    ec = arrayGetter->GetStructElementGetter(index, &_structGetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    structGetter = _structGetter, _structGetter->Release();

    return ToValue(static_cast<IStructInfo *>(dataTypeInfo), structGetter);
}

template<class ArrayGetter>
inline Local<Value> _GetCARObjectElement(ArrayGetter  *arrayGetter, Elastos::Int32 index)
{
    ECode ec;
    IInterface *carObject;

    ec = arrayGetter->GetObjectPtrElement(index, &carObject);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    return ToValue(carObject);
}

Local<Value> ToValue(ICarArrayInfo *carArrayInfo, ICarArrayGetter *carArrayGetter)
{
    typedef Local<Value> (*GetElement)(ICarArrayGetter * carArrayGetter, Elastos::Int32 index, ...);
    ECode ec;
    AutoPtr<IDataTypeInfo> elementTypeInfo;
    IDataTypeInfo *_elementTypeInfo;
    CarDataType dataType;
    GetElement getElement;
    Elastos::Int32 used;
    Local<Array> array;

    ec = carArrayInfo->GetElementTypeInfo(&_elementTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    elementTypeInfo = _elementTypeInfo, _elementTypeInfo->Release();
    ec = elementTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType)
    {
    case CarDataType_Int16:
        getElement = (GetElement)&_GetInt16Element<ICarArrayGetter>;
        break;
    case CarDataType_Int32:
        getElement = (GetElement)&_GetInt32Element<ICarArrayGetter>;
        break;
    case CarDataType_Int64:
        getElement = (GetElement)&_GetInt64Element<ICarArrayGetter>;
        break;
    case CarDataType_Byte:
        getElement = (GetElement)&_GetByteElement<ICarArrayGetter>;
        break;
    case CarDataType_Float:
        getElement = (GetElement)&_GetFloatElement<ICarArrayGetter>;
        break;
    case CarDataType_Double:
        getElement = (GetElement)&_GetDoubleElement<ICarArrayGetter>;
        break;
    case CarDataType_Char32:
        getElement = (GetElement)&_GetChar32Element<ICarArrayGetter>;
        break;
    case CarDataType_String:
        getElement = (GetElement)&_GetStringElement<ICarArrayGetter>;
        break;
    case CarDataType_Boolean:
        getElement = (GetElement)&_GetBooleanElement<ICarArrayGetter>;
        break;
    case CarDataType_EMuid:
        getElement = (GetElement)&_GetEMuidElement<ICarArrayGetter>;
        break;
    case CarDataType_EGuid:
        getElement = (GetElement)&_GetEGuidElement<ICarArrayGetter>;
        break;
    case CarDataType_ECode:
        getElement = (GetElement)&_GetECodeElement<ICarArrayGetter>;
        break;

    case CarDataType_Enum:
        getElement = (GetElement)&_GetEnumElement<ICarArrayGetter>;
        break;
#if 0
    case CarDataType_ArrayOf:
        getElement = (GetElement)&_GetCARArrayElement<ICarArrayGetter>;
        break;
#endif
#if 0
    case CarDataType_CppVector:
        getElement = (GetElement)&_GetCPPVectorElement<ICarArrayGetter>;
        break;
#endif
    case CarDataType_Struct:
        getElement = (GetElement)&_GetStructElement<ICarArrayGetter>;
        break;
    case CarDataType_Interface:
        getElement = (GetElement)&_GetCARObjectElement<ICarArrayGetter>;
        break;
    default:
        abort();
    }

    ec = carArrayGetter->GetUsed(&used);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    array = New<Array>(used);
    for (Elastos::Int32 i = 0; i < used; ++i)
    {
        Nan::HandleScope scope;
        Nan::Set(array, i, (*getElement)(carArrayGetter, i, elementTypeInfo.Get()));
    }

    return array;
}

Local<Value> ToValue(ICarArrayInfo *carArrayInfo, CarQuintet  *carQuintet)
{
    ECode ec;
    AutoPtr<IVariableOfCarArray> variableOfCARArray;
    IVariableOfCarArray *_variableOfCARArray;
    AutoPtr<ICarArrayGetter > carArrayGetter;
    ICarArrayGetter *_carArrayGetter;

    ec = carArrayInfo->CreateVariableBox(const_cast<CarQuintet *>(carQuintet), &_variableOfCARArray);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfCARArray = _variableOfCARArray, _variableOfCARArray->Release();
    ec = variableOfCARArray->GetGetter(&_carArrayGetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    carArrayGetter = _carArrayGetter, _carArrayGetter->Release();
    return ToValue(carArrayInfo, carArrayGetter);
}

bool IsCPPVector(ICppVectorInfo *cppVectorInfo, Local<Value> value)
{
    Local<Array> array;
    ECode ec;
    Elastos::Int32 length;
    AutoPtr<IDataTypeInfo > elementTypeInfo;
    IDataTypeInfo *_elementTypeInfo;

    if (!value->IsArray())
        return false;

    array = value.As<Array>();
    ec = cppVectorInfo->GetLength(&length);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (array->Length() != (uint32_t)length)
        return false;

    ec = cppVectorInfo->GetElementTypeInfo(&_elementTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    elementTypeInfo = _elementTypeInfo, _elementTypeInfo->Release();
    for (Elastos::Int32 i = 0; i < length; ++i)
    {
        Nan::HandleScope scope;
        if (!Is(elementTypeInfo, Get(array, i).ToLocalChecked()))
            return false;
    }

    return true;
}

bool CanBeUsedAsCPPVector(ICppVectorInfo  *cppVectorInfo, Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    int _priority;

    if (!To<v8::Object>(value).ToLocal(&object))
        return false;
    if (!IsCPPVector(cppVectorInfo, object))
        return false;
    if (value->IsArray())
        _priority = 0;
    else if (value->IsObject())
        _priority = 1;
    else
        _priority = 2;
    if (priority != nullptr)
        *priority = _priority;

    return true;
}

void ToCPPVector(ICppVectorInfo *cppVectorInfo, ICppVectorSetter *cppVectorSetter, Local<Value> value)
{
    typedef void (*SetElement)(ICppVectorSetter * cppVectorSetter, Elastos::Int32 index, Local<Value> value, ...);
    ECode ec;
    AutoPtr<IDataTypeInfo> elementTypeInfo;
    IDataTypeInfo *_elementTypeInfo;
    CarDataType dataType;
    SetElement setElement;
    Local<Array> array;

    ec = cppVectorInfo->GetElementTypeInfo(&_elementTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    elementTypeInfo = _elementTypeInfo, _elementTypeInfo->Release();
    ec = elementTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType)
    {
    case CarDataType_Int16:
        setElement = (SetElement)&_SetInt16Element<ICppVectorSetter>;
        break;
    case CarDataType_Int32:
        setElement = (SetElement)&_SetInt32Element<ICppVectorSetter>;
        break;
    case CarDataType_Int64:
        setElement = (SetElement)&_SetInt64Element<ICppVectorSetter>;
        break;
    case CarDataType_Byte:
        setElement = (SetElement)&_SetByteElement<ICppVectorSetter>;
        break;
    case CarDataType_Float:
        setElement = (SetElement)&_SetFloatElement<ICppVectorSetter>;
        break;
    case CarDataType_Double:
        setElement = (SetElement)&_SetDoubleElement<ICppVectorSetter>;
        break;
    case CarDataType_Char32:
        setElement = (SetElement)&_SetChar32Element<ICppVectorSetter>;
        break;
#if 0
    case CarDataType_String:
        setElement = (SetElement)&_SetStringElement<ICppVectorSetter>;
        break;
#endif
    case CarDataType_Boolean:
        setElement = (SetElement)&_SetBooleanElement<ICppVectorSetter>;
        break;
    case CarDataType_EMuid:
        setElement = (SetElement)&_SetEMuidElement<ICppVectorSetter>;
        break;
    case CarDataType_EGuid:
        setElement = (SetElement)&_SetEGuidElement<ICppVectorSetter>;
        break;
    case CarDataType_ECode:
        setElement = (SetElement)&_SetECodeElement<ICppVectorSetter>;
        break;
    case CarDataType_LocalPtr:
        setElement = (SetElement)&_SetLocalPtrElement<ICppVectorSetter>;
        break;
    case CarDataType_Enum:
        setElement = (SetElement)&_SetEnumElement<ICppVectorSetter>;
        break;
#if 0
    case CarDataType_ArrayOf:
        setElement = (SetElement)&_SetCARArrayElement<ICppVectorSetter>;
        break;
#endif
    case CarDataType_CppVector:
        setElement = (SetElement)&_SetCPPVectorElement<ICppVectorSetter>;
        break;
    case CarDataType_Struct:
        setElement = (SetElement)&_SetStructElement<ICppVectorSetter>;
        break;
#if 0
    case CarDataType_Interface:
        setElement = (SetElement)&_SetCARObjectElement<ICppVectorSetter>;
        break;
#endif
    default:
        abort();
    }
    array = value.As<Array>();
    for (size_t i = 0, length = array->Length(); i < length; ++i)
    {
        Nan::HandleScope scope;
        (*setElement)(cppVectorSetter, i, Get(array, i).ToLocalChecked(), elementTypeInfo.Get());
    }
}
#if 0
void ToCPPVector(ICppVectorInfo  *cppVectorInfo, void *cppVector, Local<Value> value)
{
}
#endif

Local<Value> ToValue(ICppVectorInfo *cppVectorInfo, ICppVectorGetter *cppVectorGetter)
{
    typedef Local<Value> (*GetElement)(ICppVectorGetter * cppVectorGetter, Elastos::Int32 index, ...);
    ECode ec;
    AutoPtr<IDataTypeInfo> elementTypeInfo;
    IDataTypeInfo *_elementTypeInfo;
    CarDataType dataType;
    GetElement getElement;
    Elastos::Int32 length;
    Local<Array> array;

    ec = cppVectorInfo->GetElementTypeInfo(&_elementTypeInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    elementTypeInfo = _elementTypeInfo, _elementTypeInfo->Release();
    ec = elementTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType)
    {
    case CarDataType_Int16:
        getElement = (GetElement)&_GetInt16Element<ICppVectorGetter>;
        break;
    case CarDataType_Int32:
        getElement = (GetElement)&_GetInt32Element<ICppVectorGetter>;
        break;
    case CarDataType_Int64:
        getElement = (GetElement)&_GetInt64Element<ICppVectorGetter>;
        break;
    case CarDataType_Byte:
        getElement = (GetElement)&_GetByteElement<ICppVectorGetter>;
        break;
    case CarDataType_Float:
        getElement = (GetElement)&_GetFloatElement<ICppVectorGetter>;
        break;
    case CarDataType_Double:
        getElement = (GetElement)&_GetDoubleElement<ICppVectorGetter>;
        break;
    case CarDataType_Char32:
        getElement = (GetElement)&_GetChar32Element<ICppVectorGetter>;
        break;
#if 0
    case CarDataType_String:
        getElement = (GetElement)&_GetStringElement<ICppVectorGetter>;
        break;
#endif
    case CarDataType_Boolean:
        getElement = (GetElement)&_GetBooleanElement<ICppVectorGetter>;
        break;
    case CarDataType_EMuid:
        getElement = (GetElement)&_GetEMuidElement<ICppVectorGetter>;
        break;
    case CarDataType_EGuid:
        getElement = (GetElement)&_GetEGuidElement<ICppVectorGetter>;
        break;
    case CarDataType_ECode:
        getElement = (GetElement)&_GetECodeElement<ICppVectorGetter>;
        break;
    case CarDataType_LocalPtr:
        getElement = (GetElement)&_GetLocalPtrElement<ICppVectorGetter>;
        break;
    case CarDataType_Enum:
        getElement = (GetElement)&_GetEnumElement<ICppVectorGetter>;
        break;
#if 0
    case CarDataType_ArrayOf:
        getElement = (GetElement)&_GetCARArrayElement<ICppVectorGetter>;
        break;
#endif
    case CarDataType_CppVector:
        getElement = (GetElement)&_GetCPPVectorElement<ICppVectorGetter>;
        break;
    case CarDataType_Struct:
        getElement = (GetElement)&_GetStructElement<ICppVectorGetter>;
        break;
#if 0
    case CarDataType_Interface:
        getElement = (GetElement)&_GetCARObjectElement<ICppVectorGetter>;
        break;
#endif
    default:
        abort();
    }
    ec = cppVectorGetter->GetLength(&length);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
    array = New<Array>(length);
    for (Elastos::Int32 i = 0; i < length; ++i)
    {
        Nan::HandleScope scope;
        Nan::Set(array, i, (*getElement)(cppVectorGetter, i, elementTypeInfo.Get()));
    }
    return array;
}
#if 0
Local<Value> ToValue(ICppVectorInfo  *cppVectorInfo, void  *cppVector)
{
    return Local<Value>();
}
#endif

bool IsStruct(IStructInfo *structInfo, Local<Value> value)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    ECode ec;
    Elastos::Int32 nFields;
    AutoPtr<ArrayOf<IFieldInfo *> > fieldInfos;

    if (!value->IsObject())
        return false;

    object = value.As<v8::Object>();
    ec = structInfo->GetFieldCount(&nFields);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (GetOwnPropertyNames(object).ToLocalChecked()->Length() != (uint32_t)nFields)
        return false;

    fieldInfos = ArrayOf<IFieldInfo *>::Alloc(nFields);
    if (fieldInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = structInfo->GetAllFieldInfos(reinterpret_cast<ArrayOf<IFieldInfo *> *>(fieldInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    for (Elastos::Int32 i = 0; i < nFields; ++i)
    {
        Nan::HandleScope scope_;
        IFieldInfo *fieldInfo;
        Elastos::String name;
        AutoPtr<IDataTypeInfo > typeInfo;
        IDataTypeInfo *_typeInfo;
        fieldInfo = (*fieldInfos)[i];

        ec = fieldInfo->GetName(&name);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        if (!HasOwnProperty(object, ToValue(name).As<v8::String>()).FromJust())
            return false;

        ec = fieldInfo->GetTypeInfo(&_typeInfo);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        typeInfo = _typeInfo, _typeInfo->Release();
        if (!Is(typeInfo, Get(object, ToValue(name)).ToLocalChecked()))
            return false;
    }

    return true;
}

bool CanBeUsedAsStruct(IStructInfo *structInfo, Local<Value> value, int *priority)
{
    Nan::HandleScope scope;
    Local<v8::Object> object;
    int _priority;

    if (!To<v8::Object>(value).ToLocal(&object))
        return false;

    if (!IsStruct(structInfo, object))
        return false;

    if (value->IsObject())
        _priority = 0;
    else
        _priority = 1;
    if (priority != nullptr)
        *priority = _priority;

    return true;
}

void ToStruct(IStructInfo *structInfo, IStructSetter *structSetter, Local<Value> value_)
{
    ECode ec;
    Elastos::Int32 nFields;
    AutoPtr<ArrayOf<IFieldInfo *> > fieldInfos;
    Local<v8::Object> object;

    ec = structInfo->GetFieldCount(&nFields);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    fieldInfos = ArrayOf<IFieldInfo *>::Alloc(nFields);
    if (fieldInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = structInfo->GetAllFieldInfos(reinterpret_cast<ArrayOf<IFieldInfo *> *>(fieldInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    object = value_.As<v8::Object>();
    for (Elastos::Int32 i = 0; i < nFields; ++i)
    {
        Nan::HandleScope scope_;
        IFieldInfo *fieldInfo;
        Elastos::String name;
        Local<Value> value;
        AutoPtr<IDataTypeInfo> typeInfo;
        IDataTypeInfo *_typeInfo;
        CarDataType dataType;
        fieldInfo = (*fieldInfos)[i];

        ec = fieldInfo->GetName(&name);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        value = Get(object, ToValue(name)).ToLocalChecked();
        ec = fieldInfo->GetTypeInfo(&_typeInfo);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        typeInfo = _typeInfo, _typeInfo->Release();
        ec = typeInfo->GetDataType(&dataType);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        switch (dataType)
        {
        case CarDataType_Int16:
            ec = structSetter->SetInt16Field(name, ToInt16(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_Int32:
            ec = structSetter->SetInt32Field(name, ToInt32(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_Int64:
            ec = structSetter->SetInt64Field(name, ToInt64(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_Byte:
            ec = structSetter->SetByteField(name, ToByte(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_Float:
            ec = structSetter->SetFloatField(name, ToFloat(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_Double:
            ec = structSetter->SetDoubleField(name, ToDouble(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_Char32:
            ec = structSetter->SetCharField(name, ToChar32(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
#if 0
        case CarDataType_String:
            break;
#endif
        case CarDataType_Boolean:
            ec = structSetter->SetBooleanField(name, ToBoolean(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_EMuid:
        {
            EMuid id;
            ToEMuid(&id, value);
            ec = structSetter->SetEMuidField(name, &id);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
        }
        break;
        case CarDataType_EGuid:
        {
            EGuid id;
            ToEGuid(&id, value);
            ec = structSetter->SetEGuidField(name, &id);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
        }
        break;
        case CarDataType_ECode:
            ec = structSetter->SetECodeField(name, ToECode(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
        case CarDataType_LocalPtr:
            ec = structSetter->SetLocalPtrField(name, ToLocalPtr(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
#if 0
        case CarDataType_LocalType:
            break;
#endif
        case CarDataType_Enum:
            ec = structSetter->SetEnumField(name, ToEnum(value));
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            break;
#if 0
        case CarDataType_ArrayOf:
            break;
#endif
        case CarDataType_CppVector:
        {
            AutoPtr<ICppVectorSetter> cppVectorSetter;
            ICppVectorSetter *_cppVectorSetter;

            ec = structSetter->GetCppVectorFieldSetter(name, &_cppVectorSetter);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            cppVectorSetter = _cppVectorSetter, _cppVectorSetter->Release();
            ToCPPVector(static_cast<ICppVectorInfo *>(typeInfo.Get()), cppVectorSetter, value);
        }
        break;
        case CarDataType_Struct:
        {
            AutoPtr<IStructSetter> _structSetter;
            IStructSetter *__structSetter;

            ec = structSetter->GetStructFieldSetter(name, &__structSetter);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            _structSetter = __structSetter, __structSetter->Release();
            ToStruct(static_cast<IStructInfo *>(typeInfo.Get()), _structSetter, value);
        }
        break;
#if 0
        case CarDataType_Interface:
            break;
#endif
        default:
            break;
        }
    }
}

void ToStruct(IStructInfo *structInfo, void *struct_, Local<Value> value)
{
    ECode ec;
    AutoPtr<IVariableOfStruct> variableOfStruct;
    IVariableOfStruct *_variableOfStruct;
    AutoPtr<IStructSetter> structSetter;
    IStructSetter *_structSetter;

    ec = structInfo->CreateVariableBox(struct_, &_variableOfStruct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfStruct = _variableOfStruct, _variableOfStruct->Release();
    ec = variableOfStruct->GetSetter(&_structSetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    structSetter = _structSetter, _structSetter->Release();
    ToStruct(structInfo, structSetter, value);
}

Local<Value> ToValue(IStructInfo *structInfo, IStructGetter *structGetter)
{
    ECode ec;
    Elastos::Int32 nFields;
    AutoPtr<ArrayOf<IFieldInfo *> > fieldInfos;
    Local<v8::Object> object;
    ec = structInfo->GetFieldCount(&nFields);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
 
    fieldInfos = ArrayOf<IFieldInfo *>::Alloc(nFields);
    if (fieldInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);
 
    ec = structInfo->GetAllFieldInfos(reinterpret_cast<ArrayOf<IFieldInfo *> *>(fieldInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);
 
    object = New<v8::Object>();
    for (Elastos::Int32 i = 0; i < nFields; ++i)
    {
        Nan::HandleScope scope;
        IFieldInfo *fieldInfo;
        Elastos::String name;
        AutoPtr<IDataTypeInfo> typeInfo;
        IDataTypeInfo *_typeInfo;
        CarDataType dataType;
        Local<Value> value;
        fieldInfo = (*fieldInfos)[i];

        ec = fieldInfo->GetName(&name);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        ec = fieldInfo->GetTypeInfo(&_typeInfo);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);
        typeInfo = _typeInfo, _typeInfo->Release();

        ec = typeInfo->GetDataType(&dataType);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        switch (dataType)
        {
        case CarDataType_Int16:
        {
            Int16 i16;
            ec = structGetter->GetInt16Field(name, &i16);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(i16);
        }
        break;
        case CarDataType_Int32:
        {
            Elastos::Int32 i32;
            ec = structGetter->GetInt32Field(name, &i32);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(i32);
        }
        break;
        case CarDataType_Int64:
        {
            Int64 i64;
            ec = structGetter->GetInt64Field(name, &i64);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(i64);
        }
        break;
        case CarDataType_Byte:
        {
            Byte byte;
            ec = structGetter->GetByteField(name, &byte);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(byte);
        }
        break;
        case CarDataType_Float:
        {
            Float f;
            ec = structGetter->GetFloatField(name, &f);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(f);
        }
        break;
        case CarDataType_Double:
        {
            Double d;
            ec = structGetter->GetDoubleField(name, &d);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(d);
        }
        break;
        case CarDataType_Char32:
        {
            Char32 c32;
            ec = structGetter->GetCharField(name, &c32);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(c32);
        }
        break;
#if 0
        case CarDataType_String:
            break;
#endif
        case CarDataType_Boolean:
        {
            Elastos::Boolean b;
            ec = structGetter->GetBooleanField(name, &b);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValueFromBoolean(b);
        }
        break;
        case CarDataType_EMuid:
        {
            EMuid id;
            ec = structGetter->GetEMuidField(name, &id);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(&id);
        }
        break;
        case CarDataType_EGuid:
        {
            EGuid id;
            ec = structGetter->GetEGuidField(name, &id);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(&id);
        }
        break;
        case CarDataType_ECode:
        {
            ECode ecode;
            ec = structGetter->GetECodeField(name, &ecode);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValueFromECode(ecode);
        }
        break;
        case CarDataType_LocalPtr:
        {
            void *localPtr;
            ec = structGetter->GetLocalPtrField(name, &localPtr);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            value = ToValue(localPtr);
        }
        break;
        case CarDataType_Enum:
        {
            Elastos::Int32 enum_;
            ec = structGetter->GetEnumField(name, &enum_);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);
            value = ToValueFromEnum(enum_);
        }
        break;
#if 0
        case CarDataType_ArrayOf:
            break;
#endif
        case CarDataType_CppVector:
        {
            AutoPtr<ICppVectorGetter > cppVectorGetter;
            ICppVectorGetter *_cppVectorGetter;

            ec = structGetter->GetCppVectorFieldGetter(name, &_cppVectorGetter);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            cppVectorGetter = _cppVectorGetter, _cppVectorGetter->Release();
            value = ToValue(static_cast<ICppVectorInfo *>(typeInfo.Get()), cppVectorGetter);
        }
        break;
        case CarDataType_Struct:
        {
            AutoPtr<IStructGetter > _structGetter;
            IStructGetter *__structGetter;

            ec = structGetter->GetStructFieldGetter(name, &__structGetter);
            if (FAILED(ec))
                Throw_LOG(Error::TYPE_ELASTOS, ec);

            _structGetter = __structGetter, __structGetter->Release();
            value = ToValue(static_cast<IStructInfo *>(typeInfo.Get()), _structGetter);
        }
        break;
#if 0
        case CarDataType_Interface:
            break;
#endif
        default:
            abort();
        }
        Nan::Set(object, ToValue(name), value);
    }
    return object;
}

Local<Value> ToValue(IStructInfo *structInfo, void  *struct_)
{
    ECode ec;
    AutoPtr<IVariableOfStruct> variableOfStruct;
    IVariableOfStruct *_variableOfStruct;
    AutoPtr<IStructGetter > structGetter;
    IStructGetter *_structGetter;

    ec = structInfo->CreateVariableBox((void *)struct_, &_variableOfStruct);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    variableOfStruct = _variableOfStruct, _variableOfStruct->Release();

    ec = variableOfStruct->GetGetter(&_structGetter);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    structGetter = _structGetter, _structGetter->Release();
    return ToValue(structInfo, structGetter);
}

bool IsCARObject(Local<Value> value);

bool IsInterface(IInterfaceInfo *interfaceInfo, Local<Value> value)
{
    CARObject *carObject;
    ECode ec;
    AutoPtr<IClassInfo> classInfo;
    IClassInfo *_classInfo;
    Elastos::Boolean has;

    if (!IsCARObject(value)) {
	    Debug_LOG("IsCARObject false.");
        return false;
    }

    carObject = CARObject::Unwrap<CARObject>(value.As<v8::Object>());
    if (carObject == nullptr) {
	    Debug_LOG("carObject is null.");
        return false;
    }

    ec = CObject::ReflectClassInfo(carObject->carObject(), &_classInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    classInfo = _classInfo, _classInfo->Release();
    ec = classInfo->HasInterfaceInfo(interfaceInfo, &has);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

	Debug_LOG("HasInterfaceInfo :%d.", has);
    return has != FALSE ? true : false;
}

bool CanBeUsedAsInterface(IInterfaceInfo *interfaceInfo, Local<Value> value, int *priority)
{
    ECode ec;
    Elastos::Int32 nMethods;
    AutoPtr<ArrayOf<IMethodInfo *> > methodInfos;
    Local<v8::Object> object;

	Debug_LOG("CanBeUsedAsInterface start.");
    if (!value->IsObject()) {
		Debug_LOG("value isn't a object");
        return false;
    }

    if (IsInterface(interfaceInfo, value))
    {
        if (priority != nullptr)
            *priority = 0;
        return true;
    }

    ec = interfaceInfo->GetMethodCount(&nMethods);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    methodInfos = ArrayOf<IMethodInfo *>::Alloc(nMethods);
    if (methodInfos == 0)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = interfaceInfo->GetAllMethodInfos(reinterpret_cast<ArrayOf<IMethodInfo *> *>(methodInfos.Get()));
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    object = value.As<v8::Object>();
    for (Elastos::Int32 i = 0; i < nMethods; ++i)
    {
        IMethodInfo *methodInfo;
        Elastos::String methodName;
        methodInfo = (*methodInfos)[i];
        ec = methodInfo->GetName(&methodName);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

		Debug_LOG("  method: %s", methodName.string());
#if 0//jw temp
        if (!Has(object, ToValue(methodName).As<v8::String>()).FromJust())
            return false;
#endif
    }

    if (priority != nullptr)
        *priority = 1;

	Debug_LOG("CanBeUsedAsInterface finish.");
    return true;
}

//AutoPtr<IInterface> ToInterface(IInterfaceInfo *interfaceInfo, Local<Value> value)
IInterface* ToInterface(IInterfaceInfo *interfaceInfo, Local<Value> value)
{
    int priority;
    Local<v8::Object> object;
    ECode ec;
    InterfaceID interfaceId;
#if 0
    AutoPtr<IInterface> interface_;
#else
    IInterface* interface_;
#endif
    IInterface *_interface;

    Debug_LOG("v8 value to interface.");
    if (!CanBeUsedAsInterface(interfaceInfo, value, &priority))
        Throw_LOG(Error::INVALID_ARGUMENT, 0);

    object = value.As<v8::Object>();
    ec = interfaceInfo->GetId(&interfaceId);

    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (priority == 0)
    {
        CARObject *carObject;
        carObject = CARObject::Unwrap<CARObject>(object);
        return carObject->carObject()->Probe(interfaceId);
    }

    ec = CARInterfaceAdapter::New(&_interface, interfaceInfo, object);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    interface_ = _interface;//, _interface->Release();
    return interface_;
}

Local<Value> ToValue(IInterface *interface_)
{
    Debug_LOG("interface[%p] to value.", interface_);
    return CARObject::New(interface_);
}

bool Is(IDataTypeInfo *dataTypeInfo, Local<Value> value)
{
    ECode ec;
    CarDataType dataType;

    ec = dataTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType)
    {
    case CarDataType_Int16:
        return IsInt16(value);
    case CarDataType_Int32:
        return IsInt32(value);
    case CarDataType_Int64:
        return IsInt64(value);
    case CarDataType_Byte:
        return IsByte(value);
    case CarDataType_Float:
        return IsFloat(value);
    case CarDataType_Double:
        return IsDouble(value);
    case CarDataType_Char32:
        return IsChar32(value);
    case CarDataType_String:
        return IsString(value);
    case CarDataType_Boolean:
        return IsBoolean(value);
    case CarDataType_EMuid:
        return IsEMuid(value);
    case CarDataType_EGuid:
        return IsEGuid(value);
    case CarDataType_ECode:
        return IsECode(value);
    case CarDataType_LocalPtr:
        return IsLocalPtr(value);
    case CarDataType_Enum:
        return IsEnum(value);
    case CarDataType_ArrayOf:
        return IsCARArray(static_cast<ICarArrayInfo *>(dataTypeInfo), value);
    case CarDataType_CppVector:
        return IsCPPVector(static_cast<ICppVectorInfo *>(dataTypeInfo), value);
    case CarDataType_Struct:
        return IsStruct(static_cast<IStructInfo *>(dataTypeInfo), value);
    case CarDataType_Interface:
        return IsInterface(static_cast<IInterfaceInfo *>(dataTypeInfo), value);
    default:
        return false;
    }
}

bool CanBeUsedAs(IDataTypeInfo *dataTypeInfo, Local<Value> value, int *priority)
{
    ECode ec;
    CarDataType dataType;

    ec = dataTypeInfo->GetDataType(&dataType);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    switch (dataType)
    {
    case CarDataType_Int16:
        return CanBeUsedAsInt16(value, priority);
    case CarDataType_Int32:
        return CanBeUsedAsInt32(value, priority);
    case CarDataType_Int64:
        return CanBeUsedAsInt64(value, priority);
    case CarDataType_Byte:
        return CanBeUsedAsByte(value, priority);
    case CarDataType_Float:
        return CanBeUsedAsFloat(value, priority);
    case CarDataType_Double:
        return CanBeUsedAsDouble(value, priority);
    case CarDataType_Char32:
        return CanBeUsedAsChar32(value, priority);
    case CarDataType_String:
        return CanBeUsedAsString(value, priority);
    case CarDataType_Boolean:
        return CanBeUsedAsBoolean(value, priority);
    case CarDataType_EMuid:
        return CanBeUsedAsEMuid(value, priority);
    case CarDataType_EGuid:
        return CanBeUsedAsEGuid(value, priority);
    case CarDataType_ECode:
        return CanBeUsedAsECode(value, priority);
    case CarDataType_LocalPtr:
        return CanBeUsedAsLocalPtr(value, priority);
    case CarDataType_Enum:
        return CanBeUsedAsEnum(value, priority);
    case CarDataType_ArrayOf:
        return CanBeUsedAsCARArray(static_cast<ICarArrayInfo *>(dataTypeInfo), value, priority);
    case CarDataType_CppVector:
        return CanBeUsedAsCPPVector(static_cast<ICppVectorInfo *>(dataTypeInfo), value, priority);
    case CarDataType_Struct:
        return CanBeUsedAsStruct(static_cast<IStructInfo *>(dataTypeInfo), value, priority);
    case CarDataType_Interface:
        return CanBeUsedAsInterface(static_cast<IInterfaceInfo *>(dataTypeInfo), value, priority);
    default:
        return false;
    }
}

bool IsCARObject(Local<Value> value)
{
    if (!value->IsObject()) {
		Debug_LOG("value not a object.");
        return false;
    }
    return CARObject::HasInstance(value.As<v8::Object>());
}

IInterface *AsCARObject(Local<Value> value)
{
    CARObject *carObject;
    carObject = CARObject::Unwrap<CARObject>(value.As<v8::Object>());
    if (carObject == nullptr)
        return 0;
    return carObject->carObject();
}

bool IsGeneric(Local<Value> value)
{
    CARObject *carObject;
    ECode ec;
    AutoPtr<IClassInfo> classInfo;
    IClassInfo *_classInfo;
    Elastos::Boolean isGeneric;
    if (!IsCARObject(value))
        return false;

    carObject = CARObject::Unwrap<CARObject>(value.As<v8::Object>());
    if (carObject == nullptr)
        return false;

    ec = CObject::ReflectClassInfo(carObject->carObject(), &_classInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    classInfo = _classInfo, _classInfo->Release();
    ec = classInfo->IsGeneric(&isGeneric);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    return isGeneric != FALSE ? true : false;
}

bool IsRegime(Local<Value> value)
{
    CARObject *carObject;
    ECode ec;
    AutoPtr<IClassInfo> classInfo;
    IClassInfo *_classInfo;
    Elastos::Boolean isRegime;

    if (!IsCARObject(value))
        return false;

    carObject = CARObject::Unwrap<CARObject>(value.As<v8::Object>());
    if (carObject == nullptr)
        return false;

    ec = CObject::ReflectClassInfo(carObject->carObject(), &_classInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    classInfo = _classInfo, _classInfo->Release();
    ec = classInfo->IsRegime(&isRegime);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    return isRegime != FALSE ? true : false;
}

IRegime *AsRegime(Local<Value> value)
{
    return static_cast<IRegime *>(AsCARObject(value));
}

bool IsAspect(Local<Value> value)
{
    CARObject *carObject;
    ECode ec;
    AutoPtr<IClassInfo> classInfo;
    IClassInfo *_classInfo;
    Elastos::Boolean isAspect;

    if (!IsCARObject(value))
        return false;

    carObject = CARObject::Unwrap<CARObject>(value.As<v8::Object>());
    if (carObject == nullptr)
        return false;

    ec = CObject::ReflectClassInfo(carObject->carObject(), &_classInfo);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    classInfo = _classInfo, _classInfo->Release();
    ec = classInfo->IsAspect(&isAspect);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    return isAspect != FALSE ? true : false;
}

IAspect *AsAspect(Local<Value> value)
{
    return static_cast<IAspect *>(AsCARObject(value));
}

CAR_BRIDGE_NAMESPACE_END
