#include "AssetDumperMaterial.h"

#include "Game/T6/CommonT6.h"
#include "Game/T6/GameAssetPoolT6.h"
#include "Game/T6/GameT6.h"
#include "Game/T6/MaterialConstantsT6.h"
#include "Game/T6/TechsetConstantsT6.h"
#include "ObjWriting.h"
#include "Shader/D3D11ShaderAnalyser.h"

#include <cassert>
#include <nlohmann/json.hpp>
#include <sstream>
#include <unordered_set>

using namespace T6;
using namespace nlohmann;

namespace T6::material
{
#define KNOWN_HASH(strValue)                                                                                                                                   \
    {                                                                                                                                                          \
        Common::R_HashString(strValue, 0), strValue                                                                                                            \
    }

    static constexpr const char* SAMPLER_STR = "Sampler";
    static constexpr const char* GLOBALS_CBUFFER_NAME = "$Globals";
    static constexpr const char* PER_OBJECT_CONSTS_CBUFFER_NAME = "PerObjectConsts";

    const char* KNOWN_CONSTANT_NAMES[]{
        "AngularVelocityScale",
        "AnimSpeed",
        "Background",
        "BackgroundColor",
        "BackgroundNoise",
        "BakedLightingIntensity",
        "BloodBrightness",
        "BloodIntensity",
        "BlurAmount",
        "CapWidth",
        "Char_Size",
        "Char_Width",
        "Coarseness",
        "Color",
        "ColorAmount",
        "ColorBias",
        "Color_Map_Noise",
        "Color_Map_Scale",
        "Color_Map_Size_Scale",
        "DDXScale",
        "DDYScale",
        "DarkenAmount",
        "DarkenPower",
        "Detail_Amount",
        "Detail_Normal_Tile",
        "Diffuse_Normal_Height_Facing",
        "Dimensions",
        "DispersionAmount",
        "Dolly",
        "EdgeColor",
        "EdgeHarshness",
        "EdgeIntensity",
        "EdgeMaxDist",
        "EdgeMinDist",
        "EdgeSize",
        "Edge_Color_Multiplier",
        "Emissive_Amount",
        "EnemiesColor",
        "Exposure",
        "FPS",
        "Fade_Distance",
        "Fill_Direction",
        "Fill_Direction2",
        "FirstFrame",
        "FlareIntensity",
        "FlareScale",
        "FlattenEdges",
        "Flicker_Max",
        "Flicker_Min",
        "Flicker_Seed",
        "Flicker_Speed",
        "Font_Color",
        "Gamma",
        "GlossAmount",
        "Gloss_Amount",
        "Glow_Alt_Color",
        "Glow_Color",
        "Glow_Falloff",
        "GradientColor",
        "GradientMax",
        "GradientMin",
        "Grain_Amount",
        "Grain_Color",
        "Grid",
        "Hardness",
        "Heart_Rate_Offset",
        "Heart_Rate_Scale",
        "Highlight_1_Brightness",
        "Highlight_1_Sharpness",
        "Highlight_2_Brightness",
        "Highlight_2_Sharpness",
        "Highlight_2_Size",
        "Hightlight_1_Size",
        "Holo_Scale",
        "LastFrame",
        "Layer1Alpha",
        "Layer1Depth",
        "Layer1Offset",
        "Layer1OffsetBobbleDelay",
        "Layer1OffsetBobbleSpeedAndSize",
        "Layer1Origin",
        "Layer1Rotation",
        "Layer1Scale",
        "Layer1ScaleBobbleDelay",
        "Layer1ScaleBobbleSpeedAndSize",
        "Layer1Scroll",
        "Layer2Alpha",
        "Layer2Depth",
        "Layer2Offset",
        "Layer2OffsetBobbleDelay",
        "Layer2OffsetBobbleSpeedAndSize",
        "Layer2Origin",
        "Layer2Rotation",
        "Layer2Scale",
        "Layer2ScaleBobbleDelay",
        "Layer2ScaleBobbleSpeedAndSize",
        "Layer2Scroll",
        "Layer3Alpha",
        "Layer3Depth",
        "Layer3Offset",
        "Layer3Origin",
        "Layer3Rotation",
        "Layer3Scale",
        "Layer3Scroll",
        "Layer4Alpha",
        "Layer4Depth",
        "Layer4Offset",
        "Layer4Origin",
        "Layer4Rotation",
        "Layer4Scale",
        "Layer4Scroll",
        "LineColor",
        "LineNoise",
        "LineWidth",
        "MaxDepth",
        "MaxFlickerColor",
        "MaxPulseDepth",
        "MaxResolution",
        "Max_Color",
        "Maximum_Distance",
        "Midlayer_Depth",
        "MinDepth",
        "MinFlickerColor",
        "MinResolution",
        "MinStatic",
        "MinVelocityFraction",
        "Min_Color",
        "Min_Player_Intensity",
        "MomentumColor",
        "NegativeColor",
        "NoisePower",
        "Noise_Scale",
        "NormalHeightMultiplier",
        "Normal_Detail_Height",
        "Normal_Detail_Scale",
        "Normal_Map_Size_Scale",
        "Normal_Variance_Scale",
        "NumFrames",
        "Outline_Lookup_Scale",
        "OverallAmount",
        "OverallBrightness",
        "Overlay_Color",
        "P1",
        "P2",
        "Padding",
        "Player_Color_Multiplier",
        "Player_Lookup_Scale",
        "PositiveColor",
        "Power",
        "PulseColor",
        "PulseInterval",
        "PulseTime",
        "Pulse_Color_Multiplier",
        "Pulse_Lookup_Scale",
        "Radius",
        "ReflectionAmount",
        "Reflection_Amount",
        "Reflection_Blur",
        "Reticle_Alt_Color",
        "Reticle_Color",
        "Row_Chars_",
        "Scale",
        "ScanlineColor",
        "ScanlineIntensity",
        "ScanlineOffset",
        "ScanlinePower",
        "ScanlineSpeed",
        "ScatterAmount",
        "ScatterSize",
        "SceneNoise",
        "SparkleBrightness",
        "SparkleDensity",
        "SparklePower",
        "SparkleProbeAmount",
        "SparkleScale",
        "SparkleSpecAmount",
        "SparkleWash",
        "SpecGloss_Map_Size_Scale",
        "SpecularAmount",
        "SpecularColor",
        "Specular_Amount",
        "Specular_Decay_Threshold",
        "Speed",
        "StaticAmount",
        "StaticLookupSpeed",
        "StaticLookupX",
        "StaticScale",
        "Static_Size",
        "Static_amount",
        "TearLookupMaxX",
        "TearLookupMinX",
        "TearLookupSpeed",
        "TearMultiplier",
        "TearPower",
        "Thickness",
        "TickMarkColorAndHarshness",
        "Tint",
        "VelocityScale",
        "VignetteMultiplier",
        "VignettePower",
        "WarpAmount",
        "WarpHeight",
        "WarpScale",
        "WarpSpeed",
        "WashOut",
        "WashoutMultiply",
        "WaterDirection",
        "WaterHeight",
        "WaterRefraction",
        "WaterScale1",
        "WaterScale2",
        "WaterSpeed1",
        "WaterSpeed2",
        "Zoom",
        "alphaDissolveParms",
        "alphaRevealParms",
        "alphaRevealParms1",
        "alphaRevealParms2",
        "alphaRevealParms3",
        "alphaRevealParms4",
        "clipSpaceLookupOffset",
        "clipSpaceLookupScale",
        "cloudsFeather",
        "cloudsHeights",
        "cloudsUVMad1",
        "cloudsUVMad2",
        "cloudsUVMul1",
        "cloudsUVMul2",
        "codeMeshArg",
        "colorDetailScale",
        "colorObjMax",
        "colorObjMaxBaseBlend",
        "colorObjMin",
        "colorObjMinBaseBlend",
        "colorTint",
        "debugBumpmap",
        "debugPerformance",
        "detailScale",
        "detailScale1",
        "detailScale2",
        "detailScale3",
        "detailScale4",
        "distortionScale",
        "dofEquationScene",
        "dofEquationViewModelAndFarBlur",
        "dofLerpBias",
        "dofLerpDownBias",
        "dofLerpDownScale",
        "dofLerpScale",
        "dofLerpUpBias",
        "dofLerpUpScale",
        "dofRowDelta",
        "eyeOffsetParms",
        "falloffBeginColor",
        "falloffEndColor",
        "falloffParms",
        "featherParms",
        "flagParams",
        "framebufferRead",
        "gameTime",
        "hdrAmount",
        "inverseTransposeWorldMatrix",
        "inverseTransposeWorldViewMatrix",
        "inverseWorldMatrix",
        "inverseWorldViewMatrix",
        "motionblurDirectionAndMagnitude",
        "occlusionAmount",
        "occlusionAmount1",
        "occlusionAmount2",
        "occlusionAmount3",
        "occlusionAmount4",
        "particleCloudColor",
        "particleCloudMatrix",
        "particleCloudVelWorld",
        "resizeParams1",
        "resizeParams2",
        "scaleRGB",
        "scriptVector0",
        "scriptVector1",
        "scriptVector2",
        "scriptVector3",
        "scriptVector4",
        "scriptVector5",
        "scriptVector6",
        "scriptVector7",
        "skyBoxCloudWeights",
        "skyBoxRotationSize",
        "skyColorParms",
        "spotLightWeight",
        "treeCanopyLightingParms",
        "treeCanopyScatterColor",
        "treeCanopySwayParms",
        "ui3dUVSetup0",
        "ui3dUVSetup1",
        "ui3dUVSetup2",
        "ui3dUVSetup3",
        "ui3dUVSetup4",
        "ui3dUVSetup5",
        "uvAnimParms",
        "uvScroll",
        "viewMatrix",
        "weaponParam0",
        "weaponParam1",
        "weaponParam2",
        "weaponParam3",
        "weaponParam4",
        "weaponParam5",
        "weaponParam6",
        "weaponParam7",
        "weaponParam8",
        "weaponParam9",
        "worldViewMatrix",
        "worldViewProjectionMatrix",
    };

    const char* KNOWN_TEXTURE_DEF_NAMES[]{
        "AddMap",
        "Blip_Mask",
        "BlockNoise",
        "CS_Z_buffer",
        "Camo_Detail_Map",
        "Color_Map",
        "CompassMap",
        "Detail_Map",
        "Diffuse",
        "Diffuse_Map",
        "DpadTexture",
        "FontTextutre",
        "Grain_Map",
        "GridTexture",
        "GrimeMap",
        "Heart_Rate_Image",
        "Hologram_Diffuse",
        "Image",
        "Layer1Map",
        "Layer2Map",
        "Layer3Map",
        "Layer4Map",
        "Lookup",
        "Lookup2",
        "LookupMap",
        "Mask",
        "Noise",
        "Noise_Texture",
        "NormalDetailMap",
        "Normal_Detail_Map",
        "Normal_Map",
        "Overlay_Map",
        "Reflection_Mask",
        "Reveal_Map",
        "Rim_Color_Mask",
        "Rim_Specular_Mask",
        "Rim_Occlusion_Mask",
        "Scanline",
        "SparkleMap",
        "SpecularAndGloss",
        "SpecularAndGloss2",
        "Specular_Color_Map",
        "Specular_Gloss_Map",
        "Specular_Map",
        "SpotShadowSamplerState",
        "SpotShadowState",
        "SpriteMap",
        "Static",
        "StaticMap",
        "Static_Noise_Map",
        "SunShadowSamplerState",
        "SunShadowState",
        "Surface_Normal_Map",
        "ThermalMapMask",
        "Thermal_Gradient",
        "Thermal_Map",
        "TickMarkMaterial",
        "Tile",
        "WarpMap",
        "WaterNormalMap",
        "Weapon_Normal_Map",
        "Weapon_Specular_Map",
        "Wireframe",
        "ZBuffer_Map",
        "attenuation",
        "attenuationSampler",
        "baseLut2D",
        "baseLut2DSampler",
        "cinematicA",
        "cinematicASampler",
        "cinematicCb",
        "cinematicCbSampler",
        "cinematicCr",
        "cinematicCrSampler",
        "cinematicY",
        "cinematicYSampler",
        "codeTexture0",
        "codeTexture1",
        "codeTexture2",
        "color",
        "colorDetailMap",
        "colorDetailMapSampler",
        "colorMap",
        "colorMap1",
        "colorMap2",
        "colorMap2D",
        "colorMapPostSun",
        "colorMapPostSunSampler",
        "colorMapSampler",
        "colorMapSampler1",
        "colorMapSampler2",
        "colorSampler",
        "detailMap",
        "detailMapSampler",
        "dlightAttenuation",
        "dlightAttenuationSampler",
        "floatZ",
        "floatZSampler",
        "imageSampler",
        "lightmapSamplerSecondary",
        "lightmapSecondary",
        "lut2D",
        "lut2DSampler",
        "lut3D",
        "lut3DSampler",
        "missileCam",
        "missileCamSampler",
        "modelLighting",
        "modelLightingSampler",
        "normalMap",
        "normalMap1",
        "normalMap2",
        "normalMapSampler",
        "normalMapSampler1",
        "normalMapSampler2",
        "occlusionMap",
        "occlusionMapSampler",
        "occMap",
        "occMapSampler",
        "outdoorMap",
        "outdoorMapSampler",
        "radiantDiffuseMap",
        "rawFloatZ",
        "rawFloatZSampler",
        "reflectionProbe",
        "reflectionProbeSampler",
        "shadowmapSamplerSpot",
        "shadowmapSamplerSun",
        "shadowmapSpot",
        "shadowmapSun",
        "sonarColor",
        "sonarColorSampler",
        "sonarDepth",
        "sonarDepthSampler",
        "source",
        "specularMap",
        "specularMap1",
        "specularMap2",
        "specularMapSampler",
        "specularMapSampler1",
        "specularMapSampler2",
        "stencil",
        "stencilSampler",
        "ui3d",
        "ui3dSampler",
    };

    class MaterialConstantZoneState final : public IZoneAssetDumperState
    {
    public:
        void ExtractNamesFromZone()
        {
            if (ObjWriting::Configuration.Verbose)
                std::cout << "Building material constant name lookup...\n";

            const auto begin = std::chrono::high_resolution_clock::now();

            AddStaticKnownNames();

            for (const auto* zone : g_GameT6.GetZones())
            {
                const auto* t6AssetPools = dynamic_cast<const GameAssetPoolT6*>(zone->m_pools.get());
                if (!t6AssetPools)
                    return;

                for (const auto* techniqueSetInfo : *t6AssetPools->m_technique_set)
                {
                    const auto* techniqueSet = techniqueSetInfo->Asset();

                    for (const auto* technique : techniqueSet->techniques)
                    {
                        if (technique)
                            ExtractNamesFromTechnique(technique);
                    }
                }
            }

            const auto end = std::chrono::high_resolution_clock::now();

            if (ObjWriting::Configuration.Verbose)
            {
                const auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
                std::cout << "Built material constant name lookup in " << durationInMs.count() << "ms: " << m_constant_names_from_shaders.size()
                          << " constant names; " << m_texture_def_names_from_shaders.size() << " texture def names\n";
            }
        }

        bool GetConstantName(const unsigned hash, std::string& constantName) const
        {
            const auto existingConstantName = m_constant_names_from_shaders.find(hash);
            if (existingConstantName != m_constant_names_from_shaders.end())
            {
                constantName = existingConstantName->second;
                return true;
            }

            return false;
        }

        bool GetTextureDefName(const unsigned hash, std::string& textureDefName) const
        {
            const auto existingTextureDefName = m_texture_def_names_from_shaders.find(hash);
            if (existingTextureDefName != m_texture_def_names_from_shaders.end())
            {
                textureDefName = existingTextureDefName->second;
                return true;
            }

            return false;
        }

    private:
        void ExtractNamesFromTechnique(const MaterialTechnique* technique)
        {
            const auto existingTechnique = m_dumped_techniques.find(technique);
            if (existingTechnique != m_dumped_techniques.end())
                return;

            m_dumped_techniques.emplace(technique);

            for (auto passIndex = 0u; passIndex < technique->passCount; passIndex++)
            {
                const auto& pass = technique->passArray[passIndex];

                if (pass.vertexShader && pass.vertexShader->prog.loadDef.program)
                    ExtractNamesFromShader(pass.vertexShader->prog.loadDef.program, pass.vertexShader->prog.loadDef.programSize);

                if (pass.pixelShader && pass.pixelShader->prog.loadDef.program)
                    ExtractNamesFromShader(pass.pixelShader->prog.loadDef.program, pass.pixelShader->prog.loadDef.programSize);
            }
        }

        void ExtractNamesFromShader(const char* shader, const size_t shaderSize)
        {
            const auto shaderInfo = d3d11::ShaderAnalyser::GetShaderInfo(reinterpret_cast<const uint8_t*>(shader), shaderSize);
            if (!shaderInfo)
                return;

            const auto globalsConstantBuffer = std::find_if(shaderInfo->m_constant_buffers.cbegin(),
                                                            shaderInfo->m_constant_buffers.cend(),
                                                            [](const d3d11::ConstantBuffer& constantBuffer)
                                                            {
                                                                return constantBuffer.m_name == GLOBALS_CBUFFER_NAME;
                                                            });

            const auto perObjectConsts = std::find_if(shaderInfo->m_constant_buffers.cbegin(),
                                                      shaderInfo->m_constant_buffers.cend(),
                                                      [](const d3d11::ConstantBuffer& constantBuffer)
                                                      {
                                                          return constantBuffer.m_name == PER_OBJECT_CONSTS_CBUFFER_NAME;
                                                      });

            if (globalsConstantBuffer != shaderInfo->m_constant_buffers.end())
            {
                for (const auto& variable : globalsConstantBuffer->m_variables)
                    AddConstantName(variable.m_name);
            }

            if (perObjectConsts != shaderInfo->m_constant_buffers.end())
            {
                for (const auto& variable : perObjectConsts->m_variables)
                    AddConstantName(variable.m_name);
            }

            for (const auto& boundResource : shaderInfo->m_bound_resources)
            {
                if (boundResource.m_type == d3d11::BoundResourceType::SAMPLER || boundResource.m_type == d3d11::BoundResourceType::TEXTURE)
                {
                    if (AddTextureDefName(boundResource.m_name))
                    {
                        const auto samplerPos = boundResource.m_name.rfind(SAMPLER_STR);
                        if (samplerPos != std::string::npos)
                        {
                            auto nameWithoutSamplerStr = boundResource.m_name;
                            nameWithoutSamplerStr.erase(samplerPos, std::char_traits<char>::length(SAMPLER_STR));
                            AddTextureDefName(std::move(nameWithoutSamplerStr));
                        }
                    }
                }
            }
        }

        void AddStaticKnownNames()
        {
            for (const auto* knownConstantName : KNOWN_CONSTANT_NAMES)
                AddConstantName(knownConstantName);
            for (const auto* knownTextureDefName : KNOWN_TEXTURE_DEF_NAMES)
                AddTextureDefName(knownTextureDefName);
        }

        void AddConstantName(std::string constantName)
        {
            const auto hash = Common::R_HashString(constantName.c_str(), 0);
            if (m_constant_names_from_shaders.find(hash) != m_constant_names_from_shaders.end())
                return;

            m_constant_names_from_shaders.emplace(hash, std::move(constantName));
        }

        bool AddTextureDefName(std::string textureDefName)
        {
            const auto hash = Common::R_HashString(textureDefName.c_str(), 0);
            if (m_texture_def_names_from_shaders.find(hash) != m_texture_def_names_from_shaders.end())
                return false;

            m_texture_def_names_from_shaders.emplace(hash, std::move(textureDefName));
            return true;
        }

        std::unordered_set<const MaterialTechnique*> m_dumped_techniques;
        std::unordered_map<unsigned, std::string> m_constant_names_from_shaders;
        std::unordered_map<unsigned, std::string> m_texture_def_names_from_shaders;
    };

    class JsonDumper
    {
    public:
        explicit JsonDumper(AssetDumpingContext& context, std::ostream& stream)
            : m_stream(stream),
              m_material_constants(*context.GetZoneAssetDumperState<MaterialConstantZoneState>())
        {
        }

        void Dump(const Material* material) const
        {
            json jRoot;

            jRoot["_type"] = "material";
            jRoot["_version"] = 1;

            MaterialToJson(jRoot, material);

            m_stream << std::setw(4) << jRoot << "\n";
        }

    private:
        template<size_t SIZE> static void JsonEnumEntry(json& j, const char* key, const unsigned value, const char* (&enumValues)[SIZE])
        {
            static_assert(SIZE > 0);
            JsonEnumEntry(j, key, value, enumValues, SIZE);
        }

        static void JsonEnumEntry(json& j, const char* key, const unsigned value, const char** enumValues, const size_t enumValueCount)
        {
            assert(value < enumValueCount);

            if (value < enumValueCount)
                j[key] = enumValues[value];
            else
                j[key] = nullptr;
        }

        void MaterialToJson(json& jRoot, const Material* material) const
        {

            MaterialInfoToJson(jRoot, material);
            StateBitsEntryToJson(jRoot, material);
            jRoot["stateFlags"] = material->stateFlags;
            JsonEnumEntry(jRoot, "cameraRegion", material->cameraRegion, cameraRegionNames);
            jRoot["probeMipBits"] = material->probeMipBits;
            TechniqueSetToJson(jRoot, material->techniqueSet);
            TextureTableToJson(jRoot, material);
            ConstantTableToJson(jRoot, material);
            StateBitsTableToJson(jRoot, material);
            ThermalMaterialToJson(jRoot, material->thermalMaterial);
        }

        static void MaterialInfoToJson(json& jRoot, const Material* material)
        {
            const auto& info = material->info;

            json jGameFlags = json::array();
            for (auto bitIndex = 0u; bitIndex < sizeof(info.gameFlags) * 8u; bitIndex++)
            {
                if (info.gameFlags & (1 << bitIndex))
                {
                    if (bitIndex >= std::extent_v<decltype(gameFlagNames)>)
                    {
                        std::ostringstream ss;
                        ss << std::hex << (1 << bitIndex);
                        jGameFlags.push_back(ss.str());
                    }
                    else
                        jGameFlags.push_back(gameFlagNames[bitIndex]);
                }
            }
            jRoot["gameFlags"] = std::move(jGameFlags);

            jRoot["sortKey"] = info.sortKey;

            json jTextureAtlas;
            jTextureAtlas["rows"] = info.textureAtlasRowCount;
            jTextureAtlas["columns"] = info.textureAtlasColumnCount;
            jRoot["textureAtlas"] = std::move(jTextureAtlas);

            jRoot["surfaceTypeBits"] = info.surfaceTypeBits;
            jRoot["layeredSurfaceTypes"] = info.layeredSurfaceTypes;
            jRoot["hashIndex"] = info.hashIndex;
            jRoot["surfaceFlags"] = info.surfaceFlags;
            jRoot["contents"] = info.contents;
        }

        static void StateBitsEntryToJson(json& jRoot, const Material* material)
        {
            json jStateBitsEntry = json::array();

            for (const char c : material->stateBitsEntry)
                jStateBitsEntry.push_back(static_cast<int>(c));

            jRoot["stateBitsEntry"] = std::move(jStateBitsEntry);
        }

        static void TechniqueSetToJson(json& jRoot, const MaterialTechniqueSet* techniqueSet)
        {
            if (techniqueSet && techniqueSet->name)
                jRoot["techniqueSet"] = AssetName(techniqueSet->name);
            else
                jRoot["techniqueSet"] = nullptr;
        }

        void TextureTableToJson(json& jRoot, const Material* material) const
        {
            json jTextures = json::array();
            if (material->textureTable)
            {
                for (auto textureIndex = 0u; textureIndex < material->textureCount; textureIndex++)
                {
                    json jTexture;
                    TextureDefToJson(jTexture, &material->textureTable[textureIndex]);
                    jTextures.emplace_back(std::move(jTexture));
                }
            }
            jRoot["textures"] = std::move(jTextures);
        }

        void TextureDefToJson(json& jTexture, const MaterialTextureDef* textureDef) const
        {
            std::string textureDefName;
            if (m_material_constants.GetTextureDefName(textureDef->nameHash, textureDefName))
            {
                jTexture["name"] = textureDefName;
            }
            else
            {
                jTexture["nameHash"] = textureDef->nameHash;
                jTexture["nameStart"] = std::string(1u, textureDef->nameStart);
                jTexture["nameEnd"] = std::string(1u, textureDef->nameEnd);
            }

            JsonEnumEntry(jTexture, "semantic", textureDef->semantic, textureSemanticNames);
            jTexture["isMatureContent"] = textureDef->isMatureContent;

            json jSamplerState;
            SamplerStateToJson(jSamplerState, textureDef->samplerState);
            jTexture["samplerState"] = std::move(jSamplerState);

            if (textureDef->image && textureDef->image->name)
                jTexture["image"] = AssetName(textureDef->image->name);
            else
                jTexture["image"] = nullptr;
        }

        static void SamplerStateToJson(json& jSamplerState, const MaterialTextureDefSamplerState& samplerState)
        {
            JsonEnumEntry(jSamplerState, "filter", samplerState.filter, textureFilterNames);
            JsonEnumEntry(jSamplerState, "mipMap", samplerState.mipMap, samplerStateMipMapNames);
            jSamplerState["clampU"] = samplerState.clampU ? true : false;
            jSamplerState["clampV"] = samplerState.clampV ? true : false;
            jSamplerState["clampW"] = samplerState.clampW ? true : false;
        }

        void ConstantTableToJson(json& jRoot, const Material* material) const
        {
            json jConstants = json::array();
            if (material->constantTable)
            {
                for (auto constantIndex = 0u; constantIndex < material->constantCount; constantIndex++)
                {
                    json jConstant;
                    ConstantDefToJson(jConstant, &material->constantTable[constantIndex]);
                    jConstants.emplace_back(std::move(jConstant));
                }
            }
            jRoot["constants"] = std::move(jConstants);
        }

        void ConstantDefToJson(json& jConstant, const MaterialConstantDef* constantDef) const
        {
            const auto fragmentLength = strnlen(constantDef->name, std::extent_v<decltype(MaterialConstantDef::name)>);
            const std::string nameFragment(constantDef->name, fragmentLength);
            std::string knownConstantName;

            if (fragmentLength < std::extent_v<decltype(MaterialConstantDef::name)> || Common::R_HashString(nameFragment.c_str(), 0) == constantDef->nameHash)
            {
                jConstant["name"] = nameFragment;
            }
            else if (m_material_constants.GetConstantName(constantDef->nameHash, knownConstantName))
            {
                jConstant["name"] = knownConstantName;
            }
            else
            {
                jConstant["nameHash"] = constantDef->nameHash;
                jConstant["nameFragment"] = nameFragment;
            }

            json jLiteral;
            jLiteral.push_back(constantDef->literal.v[0]);
            jLiteral.push_back(constantDef->literal.v[1]);
            jLiteral.push_back(constantDef->literal.v[2]);
            jLiteral.push_back(constantDef->literal.v[3]);
            jConstant["literal"] = std::move(jLiteral);
        }

        static void StateBitsTableToJson(json& jRoot, const Material* material)
        {
            json jStateBits = json::array();
            if (material->stateBitsTable)
            {
                for (auto stateBitsEntryIndex = 0u; stateBitsEntryIndex < material->stateBitsCount; stateBitsEntryIndex++)
                {
                    json jStateBitsEntry;
                    StateBitsEntryToJson(jStateBitsEntry, &material->stateBitsTable[stateBitsEntryIndex].loadBits.structured);
                    jStateBits.emplace_back(std::move(jStateBitsEntry));
                }
            }
            jRoot["stateBits"] = std::move(jStateBits);
        }

        static void StateBitsEntryToJson(json& jStateBitsEntry, const GfxStateBitsLoadBitsStructured* structured)
        {
            JsonEnumEntry(jStateBitsEntry, "srcBlendRgb", std::min(structured->srcBlendRgb, std::extent_v<decltype(blendNames)> - 1u), blendNames);
            JsonEnumEntry(jStateBitsEntry, "dstBlendRgb", std::min(structured->dstBlendRgb, std::extent_v<decltype(blendNames)> - 1u), blendNames);
            JsonEnumEntry(jStateBitsEntry, "blendOpRgb", std::min(structured->blendOpRgb, std::extent_v<decltype(blendOpNames)> - 1u), blendOpNames);

            assert(structured->alphaTestDisabled || structured->alphaTest == GFXS_ALPHA_TEST_GT_0 || structured->alphaTest == GFXS_ALPHA_TEST_GE_128);
            if (structured->alphaTestDisabled)
                jStateBitsEntry["alphaTest"] = "disabled";
            else if (structured->alphaTest == GFXS_ALPHA_TEST_GT_0)
                jStateBitsEntry["alphaTest"] = "gt0";
            else if (structured->alphaTest == GFXS_ALPHA_TEST_GE_128)
                jStateBitsEntry["alphaTest"] = "ge128";

            assert(structured->cullFace == GFXS0_CULL_NONE || structured->cullFace == GFXS0_CULL_BACK || structured->cullFace == GFXS0_CULL_FRONT);
            if (structured->cullFace == GFXS0_CULL_NONE)
                jStateBitsEntry["cullFace"] = "none";
            else if (structured->cullFace == GFXS0_CULL_BACK)
                jStateBitsEntry["cullFace"] = "back";
            else if (structured->cullFace == GFXS0_CULL_FRONT)
                jStateBitsEntry["cullFace"] = "front";

            JsonEnumEntry(jStateBitsEntry, "srcBlendRgb", std::min(structured->srcBlendAlpha, std::extent_v<decltype(blendNames)> - 1u), blendNames);
            JsonEnumEntry(jStateBitsEntry, "dstBlendRgb", std::min(structured->dstBlendAlpha, std::extent_v<decltype(blendNames)> - 1u), blendNames);
            JsonEnumEntry(jStateBitsEntry, "blendOpRgb", std::min(structured->blendOpAlpha, std::extent_v<decltype(blendOpNames)> - 1u), blendOpNames);
            jStateBitsEntry["colorWriteRgb"] = static_cast<bool>(structured->colorWriteRgb);
            jStateBitsEntry["colorWriteAlpha"] = static_cast<bool>(structured->colorWriteAlpha);
            jStateBitsEntry["polymodeLine"] = static_cast<bool>(structured->polymodeLine);
            jStateBitsEntry["depthWrite"] = static_cast<bool>(structured->depthWrite);

            assert(structured->depthTestDisabled || structured->depthTest == GFXS_DEPTHTEST_ALWAYS || structured->depthTest == GFXS_DEPTHTEST_LESS
                   || structured->depthTest == GFXS_DEPTHTEST_EQUAL || structured->depthTest == GFXS_DEPTHTEST_LESSEQUAL);
            if (structured->depthTestDisabled)
                jStateBitsEntry["depthTest"] = "disabled";
            else if (structured->depthTest == GFXS_DEPTHTEST_ALWAYS)
                jStateBitsEntry["depthTest"] = "always";
            else if (structured->depthTest == GFXS_DEPTHTEST_LESS)
                jStateBitsEntry["depthTest"] = "less";
            else if (structured->depthTest == GFXS_DEPTHTEST_EQUAL)
                jStateBitsEntry["depthTest"] = "equal";
            else if (structured->depthTest == GFXS_DEPTHTEST_LESSEQUAL)
                jStateBitsEntry["depthTest"] = "lessequal";

            JsonEnumEntry(
                jStateBitsEntry, "polygonOffset", std::min(structured->polygonOffset, std::extent_v<decltype(polygonOffsetNames)> - 1u), polygonOffsetNames);

            if (structured->stencilFrontEnabled)
            {
                json jStencilFront;
                StencilToJson(
                    jStencilFront, structured->stencilFrontPass, structured->stencilFrontFail, structured->stencilFrontZFail, structured->stencilFrontFunc);
                jStateBitsEntry["stencilFront"] = std::move(jStencilFront);
            }
            else
                jStateBitsEntry["stencilFront"] = json();

            if (structured->stencilBackEnabled)
            {
                json jStencilBack;
                StencilToJson(
                    jStencilBack, structured->stencilBackPass, structured->stencilBackFail, structured->stencilBackZFail, structured->stencilBackFunc);
                jStateBitsEntry["stencilBack"] = std::move(jStencilBack);
            }
            else
                jStateBitsEntry["stencilBack"] = json();
        }

        static void StencilToJson(json& jStencil, const unsigned pass, const unsigned fail, const unsigned zFail, const unsigned func)
        {
            JsonEnumEntry(jStencil, "pass", std::min(pass, std::extent_v<decltype(stencilOpNames)> - 1u), stencilOpNames);
            JsonEnumEntry(jStencil, "fail", std::min(fail, std::extent_v<decltype(stencilOpNames)> - 1u), stencilOpNames);
            JsonEnumEntry(jStencil, "zfail", std::min(zFail, std::extent_v<decltype(stencilOpNames)> - 1u), stencilOpNames);
            JsonEnumEntry(jStencil, "func", std::min(func, std::extent_v<decltype(stencilFuncNames)> - 1u), stencilFuncNames);
        }

        static void ThermalMaterialToJson(json& jRoot, const Material* thermalMaterial)
        {
            if (thermalMaterial && thermalMaterial->info.name)
                jRoot["thermalMaterial"] = AssetName(thermalMaterial->info.name);
            else
                jRoot["thermalMaterial"] = nullptr;
        }

        static const char* AssetName(const char* input)
        {
            if (input && input[0] == ',')
                return &input[1];

            return input;
        }

        std::ostream& m_stream;
        const MaterialConstantZoneState& m_material_constants;
    };
} // namespace T6::material

std::string AssetDumperMaterial::GetFileNameForAsset(const std::string& assetName)
{
    std::string sanitizedFileName(assetName);
    if (sanitizedFileName[0] == '*')
    {
        std::replace(sanitizedFileName.begin(), sanitizedFileName.end(), '*', '_');
        const auto parenthesisPos = sanitizedFileName.find('(');
        if (parenthesisPos != std::string::npos)
            sanitizedFileName.erase(parenthesisPos);
        sanitizedFileName = "generated/" + sanitizedFileName;
    }

    std::ostringstream ss;
    ss << "materials/" << sanitizedFileName << ".json";

    return ss.str();
}

bool AssetDumperMaterial::ShouldDump(XAssetInfo<Material>* asset)
{
    return true;
}

void AssetDumperMaterial::DumpAsset(AssetDumpingContext& context, XAssetInfo<Material>* asset)
{
    const auto assetFile = context.OpenAssetFile(GetFileNameForAsset(asset->m_name));

    if (!assetFile)
        return;

    const material::JsonDumper dumper(context, *assetFile);
    dumper.Dump(asset->Asset());
}

void AssetDumperMaterial::DumpPool(AssetDumpingContext& context, AssetPool<Material>* pool)
{
    auto* materialConstantState = context.GetZoneAssetDumperState<material::MaterialConstantZoneState>();
    materialConstantState->ExtractNamesFromZone();

    AbstractAssetDumper<Material>::DumpPool(context, pool);
}
