#define LIGHT_PASS \
"float3 LightPass(float3 pos, float3 normal, float3 lpos, float lradius, float3 lcolor)\n" \
"{\n" \
"    float3 dir = lpos - pos;\n" \
"    float dis = length(dir);\n" \
"    float disSqr = dis * dis;\n" \
"    disSqr /= lradius * lradius;\n" \
"    float dotNormal = dot(normal, dir) / dis;\n" \
"    dotNormal = 1 - (1 - dotNormal) * (1 - dotNormal);\n" \
"    float intensity = clamp(1 - disSqr, 0, 1);\n" \
"    dotNormal = clamp(dotNormal, 0, 1);\n" \
"    intensity *= dotNormal;\n" \
"    return lcolor * intensity;\n" \
"}\n"

static const char* SHADER_SRC_2D_VS = ""
    "input float2 inPosition;\n"
    "input float2 inTexCoord;\n"
    "input float4 inColor;\n"
    "\n"
    "output float2 outTexCoord;\n"
    "output float4 outColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    oPosition = mul(float4(inPosition.xy, 0.0, 1.0), oViewProjection);\n"
    "    outTexCoord = inTexCoord;\n"
    "    outColor = inColor;\n"
    "}\n"
"";

static const char* SHADER_SRC_2D_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "input float4 inColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 diffuse = texDiffuse(inTexCoord);\n"
    "    oColor = diffuse * inColor;\n"
    "}\n"
"";

static const char* SHADER_SRC_3D_VS = ""
    "extern float3 sunDir;\n"
    "extern float3 sunColor;\n"
    "extern float3 ambient;\n"
    "extern float4 l0_posRadius;\n"
    "extern float3 l0_color;\n"
    "extern float4 l1_posRadius;\n"
    "extern float3 l1_color;\n"
    "extern float4 l2_posRadius;\n"
    "extern float3 l2_color;\n"
    "\n"
    "input float3 inPosition;\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "\n"
    "output float3 outNormal;\n"
    "output float4 outColor;\n"
    "output float2 outTexCoord;\n"
    "\n"
    LIGHT_PASS
    "\n"
    "void main()\n"
    "{\n"
    "    oPosition = mul(float4(inPosition, 1.0), oViewProjection);\n"
    "    outNormal = normalize(mul(oModel, float4(inNormal, 0.0)).xyz);\n"
    "    float3 worldPos = mul(oModel, float4(inPosition, 1.0)).xyz;\n"
    "    outColor.rgb = ambient + lerp(inColor.rgb * sunColor * (dot(sunDir, outNormal) * 0.5 + 0.5), inColor.rgb, 0);\n"
    "    outColor.rgb += LightPass(worldPos, outNormal, l0_posRadius.xyz, l0_posRadius.w, l0_color);\n"
    "    outColor.rgb += LightPass(worldPos, outNormal, l1_posRadius.xyz, l1_posRadius.w, l1_color);\n"
    "    outColor.rgb += LightPass(worldPos, outNormal, l2_posRadius.xyz, l2_posRadius.w, l2_color);\n"
    "    outColor.a = inColor.a;\n"
    "    outTexCoord = inTexCoord;\n"
    "}\n"
"";

static const char* SHADER_SRC_3D_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "extern float alphaTestBias;\n"
    "\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 diffuse = texDiffuse(inTexCoord);\n"
    "    if (diffuse.a < alphaTestBias) discard;\n"
    "    oColor = diffuse * inColor;\n"
    "}\n"
"";

static const char* SHADER_TRANSPARENT_VS = ""
    "input float3 inPosition;\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "\n"
    "output float3 outNormal;\n"
    "output float4 outColor;\n"
    "output float2 outTexCoord;\n"
    "\n"
    LIGHT_PASS
    "\n"
    "void main()\n"
    "{\n"
    "    oPosition = mul(float4(inPosition, 1.0), oViewProjection);\n"
    "    outNormal = normalize(mul(oModel, float4(inNormal, 0.0)).xyz);\n"
    //"    float3 worldPos = mul(oModel, float4(inPosition, 1.0)).xyz;\n"
    "    outColor.rgb = float3(.5, .5, .5) + lerp(inColor.rgb * float3(1, 1, 1) * (dot(float3(0, 0, 1), outNormal) * 0.5 + 0.5), inColor.rgb, 0);\n"
    //"    outColor.rgb += LightPass(worldPos, outNormal, l0_posRadius.xyz, l0_posRadius.w, l0_color);\n"
    //"    outColor.rgb += LightPass(worldPos, outNormal, l1_posRadius.xyz, l1_posRadius.w, l1_color);\n"
    //"    outColor.rgb += LightPass(worldPos, outNormal, l2_posRadius.xyz, l2_posRadius.w, l2_color);\n"
    "    outColor.a = inColor.a;\n"
    "    outTexCoord = inTexCoord;\n"
    "}\n"
"";

static const char* SHADER_TRANSPARENT_PS = ""
    "Texture0 texDiffuse;\n"
    "Texture1 texNormal;\n"
    "Texture2 texMaterial;\n"
    "\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 diffuse = texDiffuse(inTexCoord);\n"
    "    oColor = diffuse * inColor;\n"
    "}\n"
"";

static const char* SHADER_SRC_EFFECTS_VS = ""
    "input float2 inPosition;\n"
    "\n"
    "output float2 outTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    oPosition = float4(inPosition.xy, 0.0, 1.0);\n"
    "    outTexCoord.x = (inPosition.x + 1.0) / 2.0;\n"
    "    outTexCoord.y = 1.0 - (inPosition.y + 1.0) / 2.0;\n"
    "}\n"
"";

static const char* SHADER_SRC_EFFECTS_FLIP_Y_VS = ""
    "input float2 inPosition;\n"
    "\n"
    "output float2 outTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    oPosition = float4(inPosition.xy, 0.0, 1.0);\n"
    "    outTexCoord.x = (inPosition.x + 1.0) / 2.0;\n"
    "    outTexCoord.y = (inPosition.y + 1.0) / 2.0;\n"
    "}\n"
"";

static const char* SHADER_SRC_SEPIA_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "extern float3 tone;\n"
    "extern float desaturate;\n"
    "extern float sepiaAmount;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 colorInput = texDiffuse(inTexCoord);\n"
    "    float3 sepia = colorInput.rgb;\n"
    "\n"
    "    // calculating amounts of input, grey and sepia colors to blend and combine\n"
    "    float grey = dot(sepia, float3(0.2126, 0.7152, 0.0722));\n"
    "\n"
    "    float3 grayscale = (grey * desaturate) + (colorInput.rgb * (1.0 - desaturate));\n"
    "\n"
    "    sepia = lerp(sepia, grayscale, sepiaAmount);\n"
    "    sepia *= tone;\n"
    "\n"
    "    colorInput.rgb = lerp(grayscale, sepia, sepiaAmount);\n"
    "\n"
    "    // returning the final color\n"
    "    oColor = colorInput;\n"
    "}\n"
"";

static const char* SHADER_SRC_VIGNETTE_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "\n"
    "extern float2 kernelSize;\n"
    "extern float amount;\n"
    "\n"
    "float4 VignettePass(float4 colorInput, float2 tex)\n"
    "{\n"
    "    tex = tex * 2.0 - 1.0;\n"
    "    float dis = length(tex);\n"
    "    dis = dis / 1.4142135623730950488016887242097;\n"
    "    dis = pow(dis, 3.0);\n"
    "    colorInput.rgb *= pow(1.0 - dis * amount, 2.0);\n"
    "    return colorInput;\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 c0 = texDiffuse(inTexCoord);\n"
    "    oColor = VignettePass(c0, inTexCoord);\n"
    "}\n"
"";

static const char* SHADER_SRC_CARTOON_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "\n"
    "extern float3 tone;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 diffuse = texDiffuse(inTexCoord);\n"
    "    diffuse.rgb = round(diffuse.rgb * tone) / tone;\n"
    "    oColor = diffuse;\n"
    "}\n"
"";

static const char* SHADER_SRC_CRT_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "extern float2 screen_size;\n"
    "extern float2 pixel;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "\n"
    "const float3 rgbfilter[8] = {\n"
    "    float3(103 / 255, 103 / 255, 24 / 255), float3(12 / 255, 34 / 255, 101 / 255), float3(64 / 255, 63 / 255, 13 / 255), float3(20 / 255, 18 / 255, 63 / 255),\n"
    "    float3(64 / 255, 63 / 255, 13 / 255), float3(20 / 255, 18 / 255, 63 / 255), float3(103 / 255, 103 / 255, 24 / 255), float3(12 / 255, 34 / 255, 101 / 255)\n"
    "};\n"
    "\n"
    "const float clampDistance = 2.0;\n"
    "const float curvature = 0.85;\n"
    "\n"
    "void main()\n"
    "{\n"
    "// Calculate the new texture coordinatess\n"
    "float2 screenPos = inTexCoord * 2 - 1;\n"
    "float size = length(screenPos);\n"
    "float2 texCoord = normalize(screenPos);\n"
    "float clamped_size = (1 - pow(clamp(1 - (size / clampDistance), 0.0, 1.0), curvature)) * clampDistance;\n"
    "texCoord *= clamped_size;\n"
    "texCoord = texCoord / 2 + 0.5;\n"

    "    float4 diffuse = texDiffuse(texCoord);\n"
    "    float x = mod(floor(inTexCoord.x * screen_size.x), 4);\n"
    "    float y = mod(floor(inTexCoord.y * screen_size.y), 2);\n"
    "    float3 cathodes = rgbfilter[y * 4 + x] * 2.5;\n"

    "// Apply RGB catodes\n"
    "diffuse.rgb = lerp(\n"
    "    diffuse.rgb * cathodes * 2.88,\n"
    "    diffuse.rgb, 0.6);\n"

    "// Add a vignete in the corners\n"
    "size *= 0.7071067812;\n"
    "size = pow(size, 5.0);\n"
    "diffuse.rgb *= 1.0 - size;\n"

    "    oColor = diffuse;\n"
    "}\n"
"";

static const char* SHADER_SRC_BLURH_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "extern float2 kernelSize;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 color = float4(0.0, 0.0, 0.0, 0.0);\n"
    "\n"
    "    color += texDiffuse(float2(inTexCoord.x - 6.0 * kernelSize.x, inTexCoord.y)) * 0.002216;\n"
    "    color += texDiffuse(float2(inTexCoord.x - 5.0 * kernelSize.x, inTexCoord.y)) * 0.008764;\n"
    "    color += texDiffuse(float2(inTexCoord.x - 4.0 * kernelSize.x, inTexCoord.y)) * 0.026995;\n"
    "    color += texDiffuse(float2(inTexCoord.x - 3.0 * kernelSize.x, inTexCoord.y)) * 0.064759;\n"
    "    color += texDiffuse(float2(inTexCoord.x - 2.0 * kernelSize.x, inTexCoord.y)) * 0.120985;\n"
    "    color += texDiffuse(float2(inTexCoord.x - 1.0 * kernelSize.x, inTexCoord.y)) * 0.176033;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y)) * 0.199471;\n"
    "    color += texDiffuse(float2(inTexCoord.x + 6.0 * kernelSize.x, inTexCoord.y)) * 0.002216;\n"
    "    color += texDiffuse(float2(inTexCoord.x + 5.0 * kernelSize.x, inTexCoord.y)) * 0.008764;\n"
    "    color += texDiffuse(float2(inTexCoord.x + 4.0 * kernelSize.x, inTexCoord.y)) * 0.026995;\n"
    "    color += texDiffuse(float2(inTexCoord.x + 3.0 * kernelSize.x, inTexCoord.y)) * 0.064759;\n"
    "    color += texDiffuse(float2(inTexCoord.x + 2.0 * kernelSize.x, inTexCoord.y)) * 0.120985;\n"
    "    color += texDiffuse(float2(inTexCoord.x + 1.0 * kernelSize.x, inTexCoord.y)) * 0.176033;\n"
    "\n"
    "    oColor = color;\n"
    "}\n"
"";

static const char* SHADER_SRC_BLURV_PS = ""
    "Texture0 texDiffuse;\n"
    "\n"
    "extern float2 kernelSize;\n"
    "\n"
    "input float2 inTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 color = float4(0.0, 0.0, 0.0, 0.0);\n"
    "\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y - 6.0 * kernelSize.y)) * 0.002216;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y - 5.0 * kernelSize.y)) * 0.008764;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y - 4.0 * kernelSize.y)) * 0.026995;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y - 3.0 * kernelSize.y)) * 0.064759;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y - 2.0 * kernelSize.y)) * 0.120985;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y - 1.0 * kernelSize.y)) * 0.176033;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y)) * 0.199471;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y + 6.0 * kernelSize.y)) * 0.002216;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y + 5.0 * kernelSize.y)) * 0.008764;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y + 4.0 * kernelSize.y)) * 0.026995;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y + 3.0 * kernelSize.y)) * 0.064759;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y + 2.0 * kernelSize.y)) * 0.120985;\n"
    "    color += texDiffuse(float2(inTexCoord.x, inTexCoord.y + 1.0 * kernelSize.y)) * 0.176033;\n"
    "\n"
    "    oColor = color;\n"
    "}\n"
"";

static const char* SHADER_GBUFFER_VS = ""
    "input float3 inPosition;\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "\n"
    "output float3 outNormal;\n"
    "output float4 outColor;\n"
    "output float2 outTexCoord;\n"
    "output float2 outDepth;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    oPosition = mul(float4(inPosition, 1.0), oViewProjection);\n"
    "    outNormal = normalize(mul(oModel, float4(inNormal, 0.0)).xyz);\n"
    "    outColor = inColor;\n"
    "    outTexCoord = inTexCoord;\n"
    "    outDepth = oPosition.zw;\n"
    "}\n"
"";

static const char* SHADER_GBUFFER_PS = ""
    "Texture0 texDiffuse;\n"
    "Texture1 texNormal;\n"
    "Texture2 texMaterial;\n"
    "\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "input float2 inDepth;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 diffuse = texDiffuse(inTexCoord);\n"
    "    float4 normal = texNormal(inTexCoord);\n"
    "    float4 material = texMaterial(inTexCoord);\n"
    "    oColor0 = diffuse;\n"
    "    oColor1 = float4(inNormal.rgb * 0.5 + 0.5, 1.0);\n"
    "    oColor2 = float4(inDepth.x / inDepth.y, 0.0, 0.0, 1.0);\n"
    "}\n"
"";

static const char* SHADER_GBUFFER_ALPHA_TEST_PS = ""
    "Texture0 texDiffuse;\n"
    "Texture1 texNormal;\n"
    "Texture2 texMaterial;\n"
    "\n"
    "input float3 inNormal;\n"
    "input float4 inColor;\n"
    "input float2 inTexCoord;\n"
    "input float2 inDepth;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    float4 diffuse = texDiffuse(inTexCoord);\n"
    "    if (diffuse.a < 0.3) discard;\n"
    "    float4 normal = texNormal(inTexCoord);\n"
    "    float4 material = texMaterial(inTexCoord);\n"
    "    oColor0 = diffuse;\n"
    "    oColor1 = float4(inNormal.rgb * 0.5 + 0.5, 1.0);\n"
    "    oColor2 = float4(inDepth.x / inDepth.y, 0.0, 0.0, 1.0);\n"
    "}\n"
"";

static const char* SHADER_DEFERRED_OMNI_PS = ""
    "input float2 inUV;\n"
    "input float4 inColor;\n"
    "\n"
    "Texture0 texDiffuse\n"
    "{\n"
    "    filter = nearest;\n"
    "    repeat = clamp;\n"
    "}\n"
    "\n"
    "Texture1 texNormal\n"
    "{\n"
    "    filter = nearest;\n"
    "    repeatY = clamp;\n"
    "}\n"
    "\n"
    "Texture2 texDepth\n"
    "{\n"
    "    filter = nearest;\n"
    "    repeatY = clamp;\n"
    "}\n"
    "\n"
    "Texture3 texMaterial\n"
    "{\n"
    "    filter = nearest;\n"
    "    repeatY = clamp;\n"
    "}\n"
    "\n"
    "extern matrix invViewProj;\n"
    "extern float4 lPosAndRadius;\n"
    "extern float3 lColor;\n"
    "\n"
    LIGHT_PASS
    "\n"
    "void main()\n"
    "{\n"
    "    float4 tdiffuse = texDiffuse(inUV);\n"
    "    float4 tnormal = texNormal(inUV);\n"
    "    float4 tdepth = texDepth(inUV);\n"
    "    float4 tmaterial = texMaterial(inUV);\n"
    "\n"
    "    // Position\n"
    "    float4 position = float4(float2(inUV.x, 1 - inUV.y) * 2.0 - 1.0, tdepth.r, 1.0);\n"
    "    position = mul(position, invViewProj);\n"
    "    position /= position.w;\n"
    "\n"
    "    // normal\n"
    "    float3 normal = tnormal.xyz * 2.0 - 1.0;\n"
    "\n"
    "    oColor = tdiffuse;\n"
    "    oColor.rgb *= LightPass(position.xyz, normal, lPosAndRadius.xyz, lPosAndRadius.w, lColor);\n"
    "}\n"
"";

#define SSAO_PS(__samples__) \
"input float2 inUV;\n" \
"input float4 inColor;\n" \
"\n" \
"Texture0 texDiffuse\n" \
"{\n" \
"    filter = nearest;\n" \
"    repeat = clamp;\n" \
"}\n" \
"\n" \
"Texture1 texNormal\n" \
"{\n" \
"    filter = nearest;\n" \
"    repeat = clamp;\n" \
"}\n" \
"\n" \
"Texture2 texDepth\n" \
"{\n" \
"    filter = nearest;\n" \
"    repeat = clamp;\n" \
"}\n" \
"\n" \
"extern matrix invViewProj;\n" \
"extern float4 settings;\n" \
"\n" \
"const float SAMPLES = " #__samples__ ";\n" \
"const float SCALE = 2.5;\n" \
"const float3 MOD3 = float3(0.1031, 0.11369, 0.13787);\n" \
"const float GOLDEN_ANGLE = 2.4;\n" \
"\n" \
"float4 getPosition(float2 uv)\n" \
"{\n" \
"    float depth = texDepth(uv).r;\n" \
"    float4 position = float4(float2(uv.x, 1 - uv.y) * 2.0 - 1.0, depth, 1.0);\n" \
"    position = mul(position, invViewProj);\n" \
"    return float4(position.xyz / position.w, depth / position.w);\n" \
"}\n" \
"\n" \
"float doAmbientOcclusion(float2 tcoord, float2 uv, float3 p, float3 cnorm)\n" \
"{\n" \
"    float4 pos = getPosition(tcoord + uv);\n" \
"    float3 diff = pos.xyz - p;\n" \
"    float l = length(diff);\n" \
"    float3 v = diff / l;\n" \
"    float d = l * SCALE;\n" \
"    float ao = max(0.0, dot(cnorm, v) - settings.x) * (1.0 / (1.0 + d));\n" \
"    ao *= smoothstep(settings.z, settings.z * 0.5, l);\n" \
"    return ao;\n" \
"}\n" \
"\n" \
"float hash12(float2 p)\n" \
"{\n" \
"    float3 p3 = frac(float3(p.xyx) * MOD3);\n" \
"    p3 += dot(p3, p3.yzx + 19.19);\n" \
"    return frac((p3.x + p3.y) * p3.z);\n" \
"}\n" \
"\n" \
"float spiralAO(float2 uv, float3 p, float3 n, float rad)\n" \
"{\n" \
"    float ao = 0.0;\n" \
"    float inv = 1.0 / SAMPLES;\n" \
"    float radius = 0.0;\n" \
"\n" \
"    float rotatePhase = hash12(uv * 100.0) * 6.28;\n" \
"    float rStep = inv * rad;\n" \
"    float2 spiralUV;\n" \
"\n" \
"    for (float i = 0.0; i < SAMPLES; i++)\n" \
"    {\n" \
"        spiralUV.x = sin(rotatePhase);\n" \
"        spiralUV.y = cos(rotatePhase);\n" \
"        radius += rStep;\n" \
"        ao += doAmbientOcclusion(uv, spiralUV * radius, p, n);\n" \
"        rotatePhase += GOLDEN_ANGLE;\n" \
"    }\n" \
"    ao *= inv;\n" \
"    return ao;\n" \
"}\n" \
"\n" \
"void main()\n" \
"{\n" \
"    float4 tdiffuse = texDiffuse(inUV);\n" \
"    float4 tnormal = texNormal(inUV);\n" \
"\n" \
"    // Position\n" \
"    float4 position = getPosition(inUV);\n" \
"\n" \
"    // normal\n" \
"    float3 normal = tnormal.xyz * 2.0 - 1.0;\n" \
"\n" \
"    float rad = settings.y / position.w;\n" \
"    float ao = spiralAO(inUV, position.xyz, normal, rad);\n" \
"    ao *= settings.w;\n" \
"\n" \
"    oColor = float4(0.0, 0.0, 0.0, ao * tdiffuse.a);\n" \
"}\n"

static const char* SHADER_SSAO_LOW_PS = SSAO_PS(8);
static const char* SHADER_SSAO_MEDIUM_PS = SSAO_PS(16);
static const char* SHADER_SSAO_HIGH_PS = SSAO_PS(32);
