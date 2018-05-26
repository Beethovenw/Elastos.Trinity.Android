
#include <map>
#include <memory>
#include <new>

#include <nan.h>
#include <elastos.h>
#include "macros.h"
#include "nan-ext.h"
#include "car-module.h"
#include "error.h"
#include "weak-external-base.h"

using namespace std;
using namespace Nan;
using namespace v8;

_ELASTOS_NAMESPACE_USING

CAR_BRIDGE_NAMESPACE_BEGIN

struct _ModuleInfo: WeakExternalBase {
    AutoPtr<IModuleInfo> moduleInfo;

    bool unfolded = false;

private:
    ~_ModuleInfo() override = default;
};

static NAN_METHOD(_Unfold)
{
#if 0 //?jw
    try {
#endif
        ::Nan::HandleScope scope;

        Local<Object> that = info.This();

        struct _ModuleInfo *moduleInfo;

        moduleInfo =
            (struct _ModuleInfo *)
            Get(that, New(".__module__").ToLocalChecked()).ToLocalChecked().As<External>()->Value();

        if (moduleInfo->unfolded == true)
            goto done;

        SetPrototype(that, NewInstance(CARModuleTemplate(moduleInfo->moduleInfo)).ToLocalChecked());

        moduleInfo->unfolded = true;

done:
        NAN_METHOD_RETURN_UNDEFINED();
#if 0 //?jw
    } catch (Error const &error) {
        ::Nan::HandleScope scope;

        ThrowError(ToValue(error));
    } catch (...) {
        ::Nan::HandleScope scope;

        ThrowError(ToValue(Error(Error::FAILED, "")));
    }
#endif
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
static map<AutoPtr<IModuleInfo>, CopyablePersistent<ObjectTemplate>> _mapModuleInfoToCARImportedModule;
#pragma clang diagnostic pop

Local<ObjectTemplate> CARImportedModuleTemplate(IModuleInfo const *imoduleInfo)
{
    ::Nan::EscapableHandleScope scope;

    IModuleInfo *moduleInfo = const_cast<IModuleInfo *>(imoduleInfo);
    Local<FunctionTemplate> importedModuleClassTemplate;

    Local<ObjectTemplate> importedModuleTemplate;

    Local<ObjectTemplate> escapedImportedModuleTemplate;

    auto &_importedModuleTemplate = _mapModuleInfoToCARImportedModule[moduleInfo];
    if (!_importedModuleTemplate.IsEmpty())
        return scope.Escape(New(_importedModuleTemplate));

    importedModuleClassTemplate = New<FunctionTemplate>();

    unique_ptr<struct _ModuleInfo, _ModuleInfo::Deleter> _moduleInfo(new(nothrow) struct _ModuleInfo);
    if (_moduleInfo == nullptr)
        LOG(Error::NO_MEMORY, 0);

    _moduleInfo->moduleInfo = moduleInfo;

    SetPrototypeTemplate(importedModuleClassTemplate,
            New(".__module__").ToLocalChecked(),
            _moduleInfo->self(),
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum)), _moduleInfo.release();

    SetPrototypeMethod(importedModuleClassTemplate,
            "$unfold",
            _Unfold,
            static_cast<enum PropertyAttribute>(ReadOnly | DontDelete | DontEnum));

    importedModuleTemplate = importedModuleClassTemplate->InstanceTemplate();

    _importedModuleTemplate.Reset(importedModuleTemplate);

    escapedImportedModuleTemplate = scope.Escape(importedModuleTemplate);

    return escapedImportedModuleTemplate;
}

CAR_BRIDGE_NAMESPACE_END
