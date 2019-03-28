#include "Helpers.h"

using namespace v8;

Local<String> tns::ToV8String(Isolate* isolate, std::string value) {
    return String::NewFromUtf8(isolate, value.c_str(), NewStringType::kNormal).ToLocalChecked();
}

std::string tns::ToString(Isolate* isolate, const Local<Value>& value) {
    if (value.IsEmpty()) {
        return std::string();
    }

    String::Utf8Value result(isolate, value);
    return std::string(*result);
}

void tns::SetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName, const Local<Value>& value) {
    Local<Private> privateKey = Private::ForApi(isolate, propName);
    bool success;
    if (!obj->SetPrivate(isolate->GetCurrentContext(), privateKey, value).To(&success) || !success) {
        assert(false);
    }
}

Local<Value> tns::GetPrivateValue(Isolate* isolate, const Local<Object>& obj, const Local<String>& propName) {
    Local<Private> privateKey = Private::ForApi(isolate, propName);

    Maybe<bool> hasPrivate = obj->HasPrivate(isolate->GetCurrentContext(), privateKey);

    assert(!hasPrivate.IsNothing());

    if (!hasPrivate.FromMaybe(false)) {
        return Local<Value>();
    }

    Local<Value> result;
    if (!obj->GetPrivate(isolate->GetCurrentContext(), privateKey).ToLocal(&result)) {
        assert(false);
    }

    return result;
}