#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <new>
#include <set>
#include <string>
#include <utility>
#include <nan.h>
#include <elastos.h>
#include "macros.h"
#include "libc-ext.h"
#include "nan-ext.h"
#include "car-data-type.h"
#include "car-object.h"
#include "error.h"

using namespace std;
using namespace Nan;
using namespace v8;

_ELASTOS_NAMESPACE_USING
CAR_BRIDGE_NAMESPACE_BEGIN
class _string: public string
{
public:
    _string(void) noexcept
    {}
    _string(char const *s): string(s)
    {}
    _string &operator=(char const *s)
    {
        *static_cast<string *>(this) = s;
        return *this;
    }
    bool operator==(_string const &string_) const noexcept
    {
        return !compare(string_);
    }
    bool operator!=(_string const &string_) const noexcept
    {
        return !!compare(string_);
    }
    bool operator<(_string const &string_) const noexcept
    {
        return compare(string_) < 0;
    }
    bool operator<=(_string const &string_) const noexcept
    {
        return compare(string_) <= 0;
    }
    bool operator>=(_string const &string_) const noexcept
    {
        return compare(string_) >= 0;
    }
    bool operator>(_string const &string_) const noexcept
    {
        return compare(string_) > 0;
    }
private:
    static int strcmp(char const *s1, char const *s2) noexcept
    {
        for (char const *p1 = s1, *p2 = s2; *p1 != '\0' && *p2 != '\0'; ++p1, ++p2)
            if (*p1 != *p2)
                return *p1 - *p2;
        return 0;
    }
    int compare(_string const &string_) const noexcept
    {
        return strcmp(this->data(), string_.data());
    }
};

enum _NamespacedEntryType
{
    _NAMESPACED_ENTRY_TYPE_NONE,
    _NAMESPACED_ENTRY_TYPE_ENUM,
    _NAMESPACED_ENTRY_TYPE_INTERFACE,
    _NAMESPACED_ENTRY_TYPE_CLASS
};

struct _NamespacedEntryInfo
{
    enum _NamespacedEntryType type;
    AutoPtr<IInterface> info;
};

typedef multimap<_string, struct _NamespacedEntryInfo> _MS2NEI;
static Local<ObjectTemplate> _CARNamespaceTemplate(
    _MS2NEI const *mapFullNameToNamespacedEntryInfo, char const *namespace_)
{
    Local<ObjectTemplate> namespaceTemplate;
    size_t offset;
    pair<_MS2NEI::const_iterator, _MS2NEI::const_iterator> range;
    set<string> namespaces;
    namespaceTemplate = New<ObjectTemplate>();
    offset = strlen(namespace_) + 1;
    range = mapFullNameToNamespacedEntryInfo->equal_range(namespace_);
    for (auto it = range.first; it != range.second; ++it)
    {
        Nan::HandleScope scope;
        char const *fullName;
        char const *relativeName;
        char const *delimiter;
        fullName = it->first.data();
        relativeName = fullName + offset;
        delimiter = strchr(relativeName, '.');
        if (delimiter == nullptr)
        {
            Local<Data> namespacedEntry;
            switch (it->second.type)
            {
            case _NAMESPACED_ENTRY_TYPE_ENUM:
                namespacedEntry = CAREnum(static_cast<IEnumInfo *>(it->second.info.Get()));
                break;
            case _NAMESPACED_ENTRY_TYPE_INTERFACE:
                namespacedEntry = CARInterface(static_cast<IInterfaceInfo *>(it->second.info.Get()));
                break;
            case _NAMESPACED_ENTRY_TYPE_CLASS:
                namespacedEntry = CARObject::NewClassTemplate(static_cast<IClassInfo *>(it->second.info.Get()));
                break;
            default:
                abort();
            }
            SetTemplate(namespaceTemplate,
                        New(relativeName).ToLocalChecked(),
                        namespacedEntry,
                        static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
        }
        else
        {
            char const *_namespace;
            _namespace = strndup(fullName, delimiter - fullName);
            if (_namespace == nullptr)
                Throw_LOG(Error::NO_MEMORY, 0);
            if (namespaces.count(_namespace) > 0)
                continue;
            SetTemplate(namespaceTemplate,
                        New(_namespace + offset).ToLocalChecked(),
                        _CARNamespaceTemplate(mapFullNameToNamespacedEntryInfo, _namespace),
                        static_cast<enum PropertyAttribute>(ReadOnly | DontDelete));
            namespaces.insert(_namespace);
            delete [] _namespace;
        }
    }
    return namespaceTemplate;
}

template<class NamespacedEntryInfo>
struct _GetNamespacedEntryType
{
    static enum _NamespacedEntryType const VALUE = _NAMESPACED_ENTRY_TYPE_NONE;
};

template<>
struct _GetNamespacedEntryType<IEnumInfo>
{
    static enum _NamespacedEntryType const VALUE = _NAMESPACED_ENTRY_TYPE_ENUM;
};

template<>
struct _GetNamespacedEntryType<IInterfaceInfo>
{
    static enum _NamespacedEntryType const VALUE = _NAMESPACED_ENTRY_TYPE_INTERFACE;
};

template<>
struct _GetNamespacedEntryType<IClassInfo>
{
    static enum _NamespacedEntryType const VALUE = _NAMESPACED_ENTRY_TYPE_CLASS;
};

template<class NamespacedEntryInfo>
static void _AddNamespacedEntryInfos(_MS2NEI *mapFullNameToNamespacedEntryInfo,
                                     ArrayOf<NamespacedEntryInfo *> &namespacedEntryInfos)
{
    for (Elastos::Int32 i = 0, nNamespacedEntryInfos = namespacedEntryInfos.GetLength();
            i < nNamespacedEntryInfos;
            ++i)
    {
        NamespacedEntryInfo *namespacedEntryInfo;
        ECode ec;
        Elastos::String namespace_;
        Elastos::String name;
        Elastos::String fullName;
        namespacedEntryInfo = namespacedEntryInfos[i];
        ec = namespacedEntryInfo->GetNamespace(&namespace_);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);
        ec = namespacedEntryInfo->GetName(&name);
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);
        fullName = namespace_ + "." + name;
        mapFullNameToNamespacedEntryInfo->insert(
            pair<_string, struct _NamespacedEntryInfo>((char const *)fullName,
        {_GetNamespacedEntryType<NamespacedEntryInfo>::VALUE, namespacedEntryInfo})
        );
    }
}
typedef map<Elastos::String, CopyablePersistent<ObjectTemplate>> _MS2OT;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
static map<AutoPtr<IModuleInfo>, _MS2OT> _mapModuleInfoToMapNamespaceToCARNamespace;
static map<AutoPtr<IModuleInfo>, unique_ptr<_MS2NEI>> _mapModuleInfoToMapFullNameToNamespacedEntryInfo;
#pragma clang diagnostic pop

Local<ObjectTemplate> CARNamespaceTemplate(IModuleInfo const *pmoduleInfo, char const *namespace_)
{
    ECode ec;
    Elastos::Int32 nEnums;
    Elastos::Int32 nInterfaces;
    Elastos::Int32 nClasses;
    Local<ObjectTemplate> _namespace;
    IModuleInfo *moduleInfo = const_cast<IModuleInfo *>(pmoduleInfo);

    auto &mapNamespaceToCARNamespace = _mapModuleInfoToMapNamespaceToCARNamespace[moduleInfo];
    auto &__namespace = mapNamespaceToCARNamespace[Elastos::String(namespace_)];

    if (!__namespace.IsEmpty())
        return New(__namespace);

    if (_mapModuleInfoToMapFullNameToNamespacedEntryInfo.count(moduleInfo) > 0)
        return _CARNamespaceTemplate(_mapModuleInfoToMapFullNameToNamespacedEntryInfo[moduleInfo].get(), namespace_);

    unique_ptr<_MS2NEI> mapFullNameToNamespacedEntryInfo(new(nothrow) _MS2NEI());
    if (mapFullNameToNamespacedEntryInfo == nullptr)
        Throw_LOG(Error::NO_MEMORY, 0);

    ec = moduleInfo->GetEnumCount(&nEnums);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (nEnums > 0)
    {
        AutoPtr<ArrayOf<IEnumInfo *> > enumInfos;
        enumInfos = ArrayOf<IEnumInfo *>::Alloc(nEnums);
        if (enumInfos == 0)
            Throw_LOG(Error::NO_MEMORY, 0);
        ec = moduleInfo->GetAllEnumInfos(reinterpret_cast<ArrayOf<IEnumInfo *> *>(enumInfos.Get()));
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);
        _AddNamespacedEntryInfos(mapFullNameToNamespacedEntryInfo.get(), *enumInfos);
    }

    ec = moduleInfo->GetInterfaceCount(&nInterfaces);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (nInterfaces > 0)
    {
        AutoPtr<ArrayOf<IInterfaceInfo *> > interfaceInfos;
        interfaceInfos = ArrayOf<IInterfaceInfo *>::Alloc(nInterfaces);
        if (interfaceInfos == 0)
            Throw_LOG(Error::NO_MEMORY, 0);

        ec = moduleInfo->GetAllInterfaceInfos(reinterpret_cast<ArrayOf<IInterfaceInfo *> *>(interfaceInfos.Get()));
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        _AddNamespacedEntryInfos(mapFullNameToNamespacedEntryInfo.get(), *interfaceInfos);
    }
    ec = moduleInfo->GetClassCount(&nClasses);
    if (FAILED(ec))
        Throw_LOG(Error::TYPE_ELASTOS, ec);

    if (nClasses > 0)
    {
        AutoPtr<ArrayOf<IClassInfo *> > classInfos;
        classInfos = ArrayOf<IClassInfo *>::Alloc(nClasses);
        if (classInfos == 0)
            Throw_LOG(Error::NO_MEMORY, 0);

        ec = moduleInfo->GetAllClassInfos(reinterpret_cast<ArrayOf<IClassInfo *> *>(classInfos.Get()));
        if (FAILED(ec))
            Throw_LOG(Error::TYPE_ELASTOS, ec);

        _AddNamespacedEntryInfos(mapFullNameToNamespacedEntryInfo.get(), *classInfos);
    }

    auto &_mapFullNameToNamespacedEntryInfo = _mapModuleInfoToMapFullNameToNamespacedEntryInfo[moduleInfo];
    _mapFullNameToNamespacedEntryInfo = move(mapFullNameToNamespacedEntryInfo);
    _namespace = _CARNamespaceTemplate(_mapFullNameToNamespacedEntryInfo.get(), namespace_);
    __namespace.Reset(_namespace);
    return _namespace;
}
CAR_BRIDGE_NAMESPACE_END