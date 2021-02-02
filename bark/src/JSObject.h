#pragma once

#include <cinttypes>
#include <string>

class JSObject
{
public:
    static uint64_t next_script_unique_id;
    static void* js_objects_heap_ptr;

    static void initJSObjects();

    void* js_object = nullptr;
    std::string js_global_name;

    void initJSObject(void* prototype = nullptr);
    virtual ~JSObject();
};
