//
//  Set.cpp
//  LiquidCoreiOS
//
//  Created by Eric Lange on 2/8/18.
//  Copyright © 2018 LiquidPlayer. All rights reserved.
//

#include "V82JSC.h"

using namespace v8;

size_t Set::Size() const
{
    Local<Context> context = V82JSC::ToCurrentContext(this);
    JSContextRef ctx = V82JSC::ToContextRef(context);
    JSValueRef obj = V82JSC::ToJSValueRef(this, context);

    JSStringRef ssize = JSStringCreateWithUTF8CString("size");
    JSValueRef excp = 0;
    JSValueRef size = JSObjectGetProperty(ctx, (JSObjectRef)obj, ssize, &excp);
    assert(excp==0);
    return JSValueToNumber(ctx, size, 0);
}
void Set::Clear()
{
    _local<Map>(this).toLocal()->Clear();
}
MaybeLocal<Set> Set::Add(Local<Context> context, Local<Value> key)
{
    JSContextRef ctx = V82JSC::ToContextRef(context);
    JSValueRef obj = V82JSC::ToJSValueRef(this, context);
    LocalException exception(V82JSC::ToContextImpl(context)->m_isolate);
    JSValueRef args[] = {
        obj,
        V82JSC::ToJSValueRef(key, context),
    };
    V82JSC::exec(ctx, "_1.add(_2)", 2, args, &exception);
    if (exception.ShouldThow()) return MaybeLocal<Set>();
    return _local<Set>(this).toLocal();
}
Maybe<bool> Set::Has(Local<Context> context, Local<Value> key)
{
    return _local<Map>(this).toLocal()->Has(context, key);
}
Maybe<bool> Set::Delete(Local<Context> context, Local<Value> key)
{
    return _local<Map>(this).toLocal()->Delete(context, key);
}

/**
 * Returns an array of the keys in this Set.
 */
Local<Array> Set::AsArray() const
{
    Local<Context> context = V82JSC::ToCurrentContext(this);
    JSContextRef ctx = V82JSC::ToContextRef(context);
    JSValueRef obj = V82JSC::ToJSValueRef(this, context);
    return ValueImpl::New(V82JSC::ToContextImpl(context),
                          V82JSC::exec(ctx, "var r = []; _1.forEach((v)=>r.push(v)); return r", 1, &obj)).As<Array>();
}

/**
 * Creates a new empty Set.
 */
Local<Set> Set::New(Isolate* isolate)
{
    Local<Context> context = V82JSC::OperatingContext(isolate);
    return ValueImpl::New(V82JSC::ToContextImpl(context),
                          V82JSC::exec(V82JSC::ToContextRef(context),
                                       "return new Set()", 0, 0)).As<Set>();
}
