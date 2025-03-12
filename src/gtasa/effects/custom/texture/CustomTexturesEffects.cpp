#pragma comment(lib, "d3dx9.lib")
#pragma comment ( lib, "Crypt32.lib" )
#pragma comment ( lib, "Wldap32.lib" )
#pragma comment ( lib, "Normaliz.lib" )

#include "util/EffectBase.h"
#include "util/hooks/HookMacros.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define CURL_STATICLIB
#include <curl.h>

#include <d3dx9.h>

struct MemoryStruct {
    char*   memory;
    size_t  size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) 
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);

    if (mem->memory == NULL) {
        std::cout << "Out Of Memory";
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

class CustomTexturesEffect : public EffectBase
{
    static inline IDirect3DTexture9 *image         = nullptr;
    static inline std::string        filePath      = "";
    static inline int                loadAttempts  = 0;
    static inline void               *theImage     = nullptr;
    static inline UINT               imgSize       = 0;
    static inline CURL               *handle       = nullptr;
    static inline MemoryStruct       chunk;

    const char *fileName = "";

public:
    CustomTexturesEffect (const char *fileName)
    {
        this->fileName = fileName;
    }

    void
    OnStart (EffectInstance *inst) override
    {
        if (strncmp(fileName, "http://", 7) == 0 || strncmp(fileName, "https://", 8) == 0)
        {
            SetImageFromMemory ();
            chunk.memory = (char*)malloc(1);
            chunk.size = 0;

            handle = curl_easy_init();

            curl_easy_setopt(handle, CURLOPT_URL, fileName);

            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&chunk);

            CURLcode res = curl_easy_perform(handle);

            if (res != CURLE_OK)
            {
                inst->OverrideName ("curl_easy_perform() failed");
                std::cout << "curl_easy_perform() failed : " << curl_easy_strerror(res) << std::endl;
                inst->Disable();
                return;
            }

            theImage = (void*) chunk.memory;
            imgSize = chunk.size;

            std::cout << imgSize << std::endl;

            curl_easy_cleanup(handle);
        }
        else
        {
            std::cout << "Normal" << std::endl;
            SetImageFile();
        }

        image        = nullptr;
        loadAttempts = 0;

        HOOK_ARGS (inst, Hooked_D3D9_SetRasterStage,
                   signed int (RwRaster *, int), 0x7FDFDA, 0x4CA8C4, 0x4CA9A6);
    }

    void
    OnEnd(EffectInstance *inst) override
    {

        if (chunk.memory)
        {
            free(chunk.memory);
            chunk.memory = nullptr;
        }

        if (handle)
        {
            curl_easy_cleanup(handle);
            handle = nullptr;
        }
    }

    void
    SetImageFromMemory ()
    {
        filePath = std::string (fileName);
    }

    void
    SetImageFile ()
    {
        filePath = std::string ("ChaosMod\\").append (fileName);
    }

    static signed int
    Hooked_D3D9_SetRasterStage (auto &&cb, RwRaster *raster, int stage)
    {
        signed int ret = cb ();

        IDirect3DDevice9 *device = reinterpret_cast<IDirect3DDevice9 *>(RwD3D9GetCurrentD3DDevice());
        if (!device) return ret;

        if (!image && loadAttempts++ <= 5)
        {
            int width, height;
            if (strncmp(filePath.c_str(), "http://", 7) == 0 || strncmp(filePath.c_str(), "https://", 8) == 0)
            {
                LoadTextureFromMemory (&image, &width, &height);
            }
            else
            {
                LoadTextureFromFile (filePath.c_str (), &image, nullptr, nullptr);
            }
        }

        if (image)
        {
            device->SetTexture (stage, image);
        }

        return ret;
    }

    static bool
    LoadTextureFromMemory (PDIRECT3DTEXTURE9 *out_texture,
                         int *out_width, int *out_height)
    {
        PDIRECT3DTEXTURE9 texture;
        IDirect3DDevice9 *device = reinterpret_cast<IDirect3DDevice9 *>(RwD3D9GetCurrentD3DDevice());
        HRESULT           hr
            = D3DXCreateTextureFromFileInMemory(device, theImage, imgSize, &texture);
        if (hr != S_OK) return false;

        D3DSURFACE_DESC my_image_desc;
        texture->GetLevelDesc (0, &my_image_desc);

        *out_texture = texture;
        if (out_width) *out_width = (int) my_image_desc.Width;
        if (out_height) *out_height = (int) my_image_desc.Height;
        return true;
    }

    static bool
    LoadTextureFromFile (const char *filename, PDIRECT3DTEXTURE9 *out_texture,
                         int *out_width, int *out_height)
    {
        PDIRECT3DTEXTURE9 texture;
        IDirect3DDevice9 *device = reinterpret_cast<IDirect3DDevice9 *>(RwD3D9GetCurrentD3DDevice());
        HRESULT           hr
            = D3DXCreateTextureFromFileA (device, filename, &texture);
        if (hr != S_OK) return false;

        D3DSURFACE_DESC my_image_desc;
        texture->GetLevelDesc (0, &my_image_desc);

        *out_texture = texture;
        if (out_width) *out_width = (int) my_image_desc.Width;
        if (out_height) *out_height = (int) my_image_desc.Height;
        return true;
    }
};

// clang-format off
DEFINE_EFFECT (CustomTexturesEffect, "effect_textures_its_morbin_time",         GROUP_CUSTOM_TEXTURES, "morbin.png");
DEFINE_EFFECT (CustomTexturesEffect, "effect_textures_counter_strike_source",   GROUP_CUSTOM_TEXTURES, "counter_strike_source.png");
DEFINE_EFFECT (CustomTexturesEffect, "effect_textures_custom",                  GROUP_CUSTOM_TEXTURES, "custom.png");
DEFINE_EFFECT (CustomTexturesEffect, "effect_texture_frankenstein",             GROUP_CUSTOM_TEXTURES, "tt_xfrnknstnx.png");
DEFINE_EFFECT (CustomTexturesEffect, "effect_texture_wdg",                      GROUP_CUSTOM_TEXTURES, "tt_wdgofficial.png");
DEFINE_EFFECT (CustomTexturesEffect, "effect_texture_dinoandrean",              GROUP_CUSTOM_TEXTURES, "tt_dinoandreann.png");
DEFINE_EFFECT (CustomTexturesEffect, "effect_texture_znine",                    GROUP_CUSTOM_TEXTURES, "tt_znine.png");
// clang-format on