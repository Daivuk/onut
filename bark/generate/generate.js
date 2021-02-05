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
    // Find those with flags
    {
        let regex = /COMPONENT_PROPERTY\(\s*([\w\:]+)\s*,\s*(\w+)\s*,\s*(.+)\s*,\s*([\w|\s]+)\s*\)/g;
        let output = [];
        let matches;
        while (matches = regex.exec(component.source))
        {
            let flags = [];
            if (matches[4]) flags = matches[4].split('|');
            flags = flags.map(flag => flag.trim());

            let property = {
                type: matches[1].replace('::', '_'),
                name: matches[2],
                default_value: matches[3],
                flags: flags
            };

            component.properties.push(property);
        }
    }

    // Find those without flags
    {
        let regex = /COMPONENT_PROPERTY\(\s*([\w\:]+)\s*,\s*(\w+)\s*,\s*(.+)\s*\)/g;
        let output = [];
        let matches;
        while (matches = regex.exec(component.source))
        {
            let name = matches[2];
            if (!component.properties.find(property => property.name == name))
            {
                let property = {
                    type: matches[1].replace('::', '_'),
                    name: name,
                    default_value: matches[3],
                    flags: []
                };

                component.properties.push(property);
            }
        }
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
        insert_block += `        ${property.name} = getJson_${property.type}(json, "${property.name}", ${property.default_value});\n`;
    });
    
    insert_block += `    }

    static void* js_prototype;
    void* getJSPrototype() override { return js_prototype; };
    // [GENERATED COMPONENT DECLARATION END]\n`;

    component.source = `${component.start_code}${insert_block}${component.end_code}`;
    delete component.start_code
    delete component.end_code

    component.source = component.source.replace(/\r?\n/g, "\r\n");
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
    component.properties.forEach(property =>
    {
        let is_read_only = property.flags.indexOf("PROP_READ_ONLY") != -1;
        let has_cpp_getter = property.flags.indexOf("PROP_CPP_GETTER") != -1;
        let has_cpp_setter = property.flags.indexOf("PROP_CPP_SETTER") != -1;

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
            pushJs_${property.type}(ctx, ${has_cpp_getter ? `pComponent->get_${property.name}()` : `pComponent->${property.name}`});
            return 1;
        }
        return 0;
    }, 0);
`
    if (!is_read_only) code +=
`    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t
    {
        duk_push_this(ctx);
        duk_get_prop_string(ctx, -1, "\\xff""\\xff""data");
        auto pComponent = (${component.className}*)duk_to_pointer(ctx, -1);
        if (pComponent)
        {
            ${has_cpp_setter ? `pComponent->set_${property.name}(getJs_${property.type}(ctx, ${property.default_value}))` : `pComponent->${property.name} = getJs_${property.type}(ctx, ${property.default_value})`};
        }
        return 0;
    }, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
`
        else code +=
`    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
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
`
components.forEach(component =>
{
    ComponentBindings_cpp += `    create${component.className}Bindings();
`
});
    ComponentBindings_cpp += `}
`;

fs.writeFileSync('../src/ComponentBindings.cpp', ComponentBindings_cpp.replace(/\r?\n/g, "\r\n"));
console.log(`=> ../src/ComponentBindings.cpp`);



let ComponentFactory_cpp = `// DO NOT EDIT - THIS FILE IS GENERATED

#include "ComponentFactory.h"
#include "Component.h"
#include <map>
#include <functional>
#include <onut/Log.h>

${
    components.reduce((code, component)=>
    {
        code += `#include "${component.className}.h"
`
        return code;
    }, "")
}
#include "ScriptComponent.h"

static std::map<std::string, std::function<ComponentRef()>> factories;

void initComponentFactory()
{
${
    components.reduce((code, component)=>
    {
        code += `    factories["${component.name}"] = []()->ComponentRef {return OMake<${component.className}>();};
`
        return code;
    }, "")
}
    factories["Script"] = []()->ComponentRef {return OMake<ScriptComponent>();};
}

void shutdownComponentFactory()
{
    factories.clear();
}

ComponentRef createComponentByName(const std::string& name)
{
    auto it = factories.find(name);
    if (it == factories.end())
    {
        OLogE("No Component found: " + name);
        return nullptr;
    }
    auto component = it->second();
    component->initJSObject(component->getJSPrototype());
    return component;
}
`

fs.writeFileSync('../src/ComponentFactory.cpp', ComponentFactory_cpp.replace(/\r?\n/g, "\r\n"));
console.log(`=> ../src/ComponentFactory.cpp`);


let ComponentGetterBindings_cpp = `#include "Component.h"
#include "Entity.h"
#include <JSBindings_Macros.h>

using namespace onut::js;

void createComponentGetterBindings(duk_context* ctx)
{
#define IMPL_COMPONENT_GETTER(__prop_name__, __comp_name__) \\
    duk_push_string(ctx, #__prop_name__); \\
    duk_push_c_function(ctx, [](duk_context *ctx)->duk_ret_t \\
    { \\
        duk_push_this(ctx); \\
        duk_get_prop_string(ctx, -1, "\\xff""\\xff""data"); \\
        auto pEntity = (Entity*)duk_to_pointer(ctx, -1); \\
        if (pEntity) \\
        { \\
            auto it = pEntity->components.find(#__comp_name__); \\
            if (it == pEntity->components.end()) \\
            { \\
                OLogE("Entity has no component named: " #__comp_name__); \\
                return 0; \\
            } \\
            auto pComponent = it->second.get(); \\
            duk_push_heapptr(ctx, pComponent->js_object); \\
            return 1; \\
        } \\
        return 0; \\
    }, 0); \\
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

${
        components.reduce((output, component) =>
        {
            let nameLowerCase = component.name.charAt(0).toLowerCase() + component.name.slice(1);
            output += `    IMPL_COMPONENT_GETTER(${nameLowerCase}, ${component.name});
`
            return output;
        }, "")
    }}
`;

fs.writeFileSync('../src/ComponentGetterBindings.cpp', ComponentGetterBindings_cpp.replace(/\r?\n/g, "\r\n"));
console.log(`=> ../src/ComponentGetterBindings.cpp`);


let bark_d_ts = `// DO NOT EDIT - THIS FILE IS GENERATED

// Entity
declare class Entity {
    name: string;

    getPosition(): Vector3;
    getWorldPosition(): Vector3;
    setPosition(position: Vector3);

    getTransform(): Matrix;
    getWorldTransform(): Matrix;
    setTransform(transform: Matrix);

    getComponent(name: string): object;
    addComponent(name: string): object;
    removeComponent(name: string);
    disableComponent(name: string);
    enableComponent(name: string);

    add(child: Entity);
    insert(before: Entity, child: Entity);
    remove(child: Entity);
    removeAll();
    getChildCount(): number;
    getChild(index: number): Entity;
    getChild(name: string, deepSearch: boolean = true): UEntityI;
    getParent(): Entity;

    copy(): Entity;

    // Quick access to default components
`
components.forEach(component =>
{
    let nameLowerCase = component.name.charAt(0).toLowerCase() + component.name.slice(1);
    bark_d_ts += `    ${nameLowerCase}: ${component.name};
`
});

bark_d_ts += `}
declare function findEntity(name: string): Entity;
declare function destroyEntity(entity: Entity);

// Scene
declare function loadScene(name: string);

// Components
`

function typeToJS(type)
{
    switch (type)
    {
        case "bool": return "boolean";
        case "int": return "int";
        case "float": return "number";
        case "std_string": return "string";
        case "OTextureRef": return "Texture";
        case "OTiledMapRef": return "TiledMap";
        case "OSoundRef": return "Sound";
        case "OSpriteAnimRef": return "SpriteAnim";
        case "OModelRef": return "Model";
        case "OShaderRef": return "Shader";
        default: return type;
    }
}

components.forEach(component =>
{
    bark_d_ts += `declare class ${component.name}
{
`
    component.properties.forEach(property =>
    {
        bark_d_ts += `    ${property.name}: ${typeToJS(property.type)};
`
    });

    bark_d_ts += `}

`
})

fs.writeFileSync('../typings/bark.d.ts', bark_d_ts.replace(/\r?\n/g, "\r\n"));
console.log(`=> ../typings/bark.d.ts`);
