let fs = require('fs');

// Find components
let dir = fs.readdirSync('../src');
let components = dir.reduce((components, file) =>
{
    if (file.endsWith('.h') && file != "Component.h")
    {
        let filename = `../src/${file}`;
        let file_content = fs.readFileSync(filename).toString();
        let found = file_content.match(/COMPONENT_DECLARATION\(\s*(\w+)\s*,\s*(\w+)\s*\)/);
        if (found)
        {
            let component = {
                className: found[1],
                name: found[2],
                filename: filename,
                source: file_content,
                properties: []
            }
            components.push(component);
        }
    }
    return components;
}, []);

// Find components' properties
components.forEach(component =>
{
    let regex = /COMPONENT_PROPERTY\(\s*(\w+)\s*,\s*(\w+)\s*,\s*(.+)\s*\)/g;
    let output = [];
    let matches;
    while (matches = regex.exec(component.source))
    {
        let property = {
            type: matches[1].replace('::', '_'),
            name: matches[2],
            default_value: matches[3]
        };
        component.properties.push(property);
    }
});

// Erase old generated code in the sources
components.forEach(component =>
{
    let end_tag = `// [GENERATED COMPONENT DECLARATION END]`;
    let start_index = component.source.search(/\/\/ \[GENERATED COMPONENT DECLARATION BEGIN\]/, '');
    let end_index = component.source.search(/\/\/ \[GENERATED COMPONENT DECLARATION END\]/, '');
    if (start_index != -1 && end_index != -1)
    {
        component.start_code = component.source.substring(0, start_index).trimEnd();
        component.end_code = component.source.substring(end_index + end_tag.length).trimStart();
    }
});

// Add derialize/deserialize to the header files
components.forEach(component =>
{
    let insert_block = `\n    // [GENERATED COMPONENT DECLARATION BEGIN]
public:
#if BARK_EDITOR
    Json::Value serialize() override
    {
        auto json = Component::serialize();

`;

    component.properties.forEach(property =>
    {
        insert_block += `        setJson_${property.type}(json, "${property.name}", ${property.name});\n`;
    });

    insert_block += `
        return std::move(json);
    }
#endif

    void deserialize(const Json::Value& json) override
    {
        Component::deserialize(json);

`;

    component.properties.forEach(property =>
    {
        insert_block += `        getJson_${property.type}(json, "${property.name}", ${property.default_value});\n`;
    });
    
    insert_block += `    }

    static void* js_prototype;
    void* getJSPrototype() override { return js_prototype; };
    // [GENERATED COMPONENT DECLARATION END]\n`;

    component.source = `${component.start_code}${insert_block}${component.end_code}`;
    delete component.start_code
    delete component.end_code

    fs.writeFileSync(component.filename, component.source);
    console.log(`=> ${component.filename}`);
});

let ComponentBindings_cpp = `// DO NOT EDIT - THIS FILE IS GENERATED

#include <JSBindings_Macros.h>
#include "ComponentBindings.h"
#include "Component.h"
#include "JSTypeHelper.h"

namespace onut
{
    namespace js
    {
        extern duk_context* pContext;
    }
}

using namespace onut::js;
`;



components.forEach(component =>
{
    let code = `
// ${component.className}
#include "${component.className}.h"
    
void* ${component.className}::js_prototype = nullptr;

void create${component.className}Bindings()
{
    auto ctx = pContext;

    // Constructor
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        return DUK_RET_TYPE_ERROR; // No constructor allowed
    }, 1);
    duk_push_object(ctx);

    // Destructor
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_pointer(ctx, nullptr);
        duk_put_prop_string(ctx, 0, "\\xff""\\xff""data");
        return 0;
    }, 1);
    duk_set_finalizer(ctx, -2);
`
    // zoom property
    component.properties.forEach(property =>
    {
        code += `
    // ${property.type} ${property.name}
    duk_push_string(ctx, "${property.name}");
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\\xff""\\xff""data");
        auto pComponent = (${component.className}*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pushJs_${property.type}(ctx, pComponent->${property.name});
            return 1;
        }
        return 0;
    }, 0);
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\\xff""\\xff""data");
        auto pComponent = (${component.className}*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            pComponent->${property.name} = getJs_${property.type}(ctx, ${property.default_value});
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, 
                    DUK_DEFPROP_HAVE_GETTER |
                    DUK_DEFPROP_HAVE_SETTER);
`
    });


    // Finalize
    code += `
    ${component.className}::js_prototype = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototype");

    duk_put_global_string(ctx, "${component.name}");
}
`
    ComponentBindings_cpp += code;
});
    

ComponentBindings_cpp += `
// Main function
void createComponentBindings()
{
    createCamera2DComponentBindings();
}
`;

fs.writeFileSync('../src/ComponentBindings.cpp', ComponentBindings_cpp);
console.log(`=> ../src/ComponentBindings.cpp`);
