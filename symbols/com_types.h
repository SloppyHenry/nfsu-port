// Automatisch erzeugt von tools/types/com.py (WP-04): DirectX-COM-Schnittstellen als C-Strukturen.
// Jede Schnittstelle: Objekt { lpVtbl } und Vtable-Struktur mit Methodennamen in Slot-Reihenfolge.
#pragma once

typedef struct IUnknown IUnknown;
typedef struct IDirect3D9 IDirect3D9;
typedef struct IDirect3DDevice9 IDirect3DDevice9;
typedef struct IDirect3DSwapChain9 IDirect3DSwapChain9;
typedef struct IDirect3DResource9 IDirect3DResource9;
typedef struct IDirect3DBaseTexture9 IDirect3DBaseTexture9;
typedef struct IDirect3DTexture9 IDirect3DTexture9;
typedef struct IDirect3DCubeTexture9 IDirect3DCubeTexture9;
typedef struct IDirect3DVolumeTexture9 IDirect3DVolumeTexture9;
typedef struct IDirect3DSurface9 IDirect3DSurface9;
typedef struct IDirect3DVertexBuffer9 IDirect3DVertexBuffer9;
typedef struct IDirect3DIndexBuffer9 IDirect3DIndexBuffer9;
typedef struct IDirect3DStateBlock9 IDirect3DStateBlock9;
typedef struct IDirect3DVertexDeclaration9 IDirect3DVertexDeclaration9;
typedef struct IDirect3DVertexShader9 IDirect3DVertexShader9;
typedef struct IDirect3DPixelShader9 IDirect3DPixelShader9;
typedef struct IDirect3DQuery9 IDirect3DQuery9;
typedef struct IDirectSound IDirectSound;
typedef struct IDirectSound8 IDirectSound8;
typedef struct IDirectSoundBuffer IDirectSoundBuffer;
typedef struct IDirectSoundBuffer8 IDirectSoundBuffer8;
typedef struct IDirectSound3DBuffer IDirectSound3DBuffer;
typedef struct IDirectSound3DListener IDirectSound3DListener;
typedef struct IDirectSoundNotify IDirectSoundNotify;
typedef struct IKsPropertySet IKsPropertySet;
typedef struct IDirectInput8A IDirectInput8A;
typedef struct IDirectInputDevice8A IDirectInputDevice8A;
typedef struct IDirectInputEffect IDirectInputEffect;
typedef struct IDirectInputA IDirectInputA;
typedef struct IDirectInputDeviceA IDirectInputDeviceA;
typedef struct IDirectDraw IDirectDraw;
typedef struct IDirectDraw2 IDirectDraw2;
typedef struct IDirectDraw7 IDirectDraw7;

typedef struct IUnknownVtbl {
    void * QueryInterface; // +0x0 
    void * AddRef; // +0x4 
    void * Release; // +0x8 
} IUnknownVtbl;
struct IUnknown { const IUnknownVtbl * lpVtbl; };
typedef struct IDirect3D9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * RegisterSoftwareDevice; // +0xC 
    void * GetAdapterCount; // +0x10 
    void * GetAdapterIdentifier; // +0x14 
    void * GetAdapterModeCount; // +0x18 
    void * EnumAdapterModes; // +0x1C 
    void * GetAdapterDisplayMode; // +0x20 
    void * CheckDeviceType; // +0x24 
    void * CheckDeviceFormat; // +0x28 
    void * CheckDeviceMultiSampleType; // +0x2C 
    void * CheckDepthStencilMatch; // +0x30 
    void * CheckDeviceFormatConversion; // +0x34 
    void * GetDeviceCaps; // +0x38 
    void * GetAdapterMonitor; // +0x3C 
    void * CreateDevice; // +0x40 
} IDirect3D9Vtbl;
struct IDirect3D9 { const IDirect3D9Vtbl * lpVtbl; };
typedef struct IDirect3DDevice9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * TestCooperativeLevel; // +0xC 
    void * GetAvailableTextureMem; // +0x10 
    void * EvictManagedResources; // +0x14 
    void * GetDirect3D; // +0x18 
    void * GetDeviceCaps; // +0x1C 
    void * GetDisplayMode; // +0x20 
    void * GetCreationParameters; // +0x24 
    void * SetCursorProperties; // +0x28 
    void * SetCursorPosition; // +0x2C 
    void * ShowCursor; // +0x30 
    void * CreateAdditionalSwapChain; // +0x34 
    void * GetSwapChain; // +0x38 
    void * GetNumberOfSwapChains; // +0x3C 
    void * Reset; // +0x40 
    void * Present; // +0x44 
    void * GetBackBuffer; // +0x48 
    void * GetRasterStatus; // +0x4C 
    void * SetDialogBoxMode; // +0x50 
    void * SetGammaRamp; // +0x54 
    void * GetGammaRamp; // +0x58 
    void * CreateTexture; // +0x5C 
    void * CreateVolumeTexture; // +0x60 
    void * CreateCubeTexture; // +0x64 
    void * CreateVertexBuffer; // +0x68 
    void * CreateIndexBuffer; // +0x6C 
    void * CreateRenderTarget; // +0x70 
    void * CreateDepthStencilSurface; // +0x74 
    void * UpdateSurface; // +0x78 
    void * UpdateTexture; // +0x7C 
    void * GetRenderTargetData; // +0x80 
    void * GetFrontBufferData; // +0x84 
    void * StretchRect; // +0x88 
    void * ColorFill; // +0x8C 
    void * CreateOffscreenPlainSurface; // +0x90 
    void * SetRenderTarget; // +0x94 
    void * GetRenderTarget; // +0x98 
    void * SetDepthStencilSurface; // +0x9C 
    void * GetDepthStencilSurface; // +0xA0 
    void * BeginScene; // +0xA4 
    void * EndScene; // +0xA8 
    void * Clear; // +0xAC 
    void * SetTransform; // +0xB0 
    void * GetTransform; // +0xB4 
    void * MultiplyTransform; // +0xB8 
    void * SetViewport; // +0xBC 
    void * GetViewport; // +0xC0 
    void * SetMaterial; // +0xC4 
    void * GetMaterial; // +0xC8 
    void * SetLight; // +0xCC 
    void * GetLight; // +0xD0 
    void * LightEnable; // +0xD4 
    void * GetLightEnable; // +0xD8 
    void * SetClipPlane; // +0xDC 
    void * GetClipPlane; // +0xE0 
    void * SetRenderState; // +0xE4 
    void * GetRenderState; // +0xE8 
    void * CreateStateBlock; // +0xEC 
    void * BeginStateBlock; // +0xF0 
    void * EndStateBlock; // +0xF4 
    void * SetClipStatus; // +0xF8 
    void * GetClipStatus; // +0xFC 
    void * GetTexture; // +0x100 
    void * SetTexture; // +0x104 
    void * GetTextureStageState; // +0x108 
    void * SetTextureStageState; // +0x10C 
    void * GetSamplerState; // +0x110 
    void * SetSamplerState; // +0x114 
    void * ValidateDevice; // +0x118 
    void * SetPaletteEntries; // +0x11C 
    void * GetPaletteEntries; // +0x120 
    void * SetCurrentTexturePalette; // +0x124 
    void * GetCurrentTexturePalette; // +0x128 
    void * SetScissorRect; // +0x12C 
    void * GetScissorRect; // +0x130 
    void * SetSoftwareVertexProcessing; // +0x134 
    void * GetSoftwareVertexProcessing; // +0x138 
    void * SetNPatchMode; // +0x13C 
    void * GetNPatchMode; // +0x140 
    void * DrawPrimitive; // +0x144 
    void * DrawIndexedPrimitive; // +0x148 
    void * DrawPrimitiveUP; // +0x14C 
    void * DrawIndexedPrimitiveUP; // +0x150 
    void * ProcessVertices; // +0x154 
    void * CreateVertexDeclaration; // +0x158 
    void * SetVertexDeclaration; // +0x15C 
    void * GetVertexDeclaration; // +0x160 
    void * SetFVF; // +0x164 
    void * GetFVF; // +0x168 
    void * CreateVertexShader; // +0x16C 
    void * SetVertexShader; // +0x170 
    void * GetVertexShader; // +0x174 
    void * SetVertexShaderConstantF; // +0x178 
    void * GetVertexShaderConstantF; // +0x17C 
    void * SetVertexShaderConstantI; // +0x180 
    void * GetVertexShaderConstantI; // +0x184 
    void * SetVertexShaderConstantB; // +0x188 
    void * GetVertexShaderConstantB; // +0x18C 
    void * SetStreamSource; // +0x190 
    void * GetStreamSource; // +0x194 
    void * SetStreamSourceFreq; // +0x198 
    void * GetStreamSourceFreq; // +0x19C 
    void * SetIndices; // +0x1A0 
    void * GetIndices; // +0x1A4 
    void * CreatePixelShader; // +0x1A8 
    void * SetPixelShader; // +0x1AC 
    void * GetPixelShader; // +0x1B0 
    void * SetPixelShaderConstantF; // +0x1B4 
    void * GetPixelShaderConstantF; // +0x1B8 
    void * SetPixelShaderConstantI; // +0x1BC 
    void * GetPixelShaderConstantI; // +0x1C0 
    void * SetPixelShaderConstantB; // +0x1C4 
    void * GetPixelShaderConstantB; // +0x1C8 
    void * DrawRectPatch; // +0x1CC 
    void * DrawTriPatch; // +0x1D0 
    void * DeletePatch; // +0x1D4 
    void * CreateQuery; // +0x1D8 
} IDirect3DDevice9Vtbl;
struct IDirect3DDevice9 { const IDirect3DDevice9Vtbl * lpVtbl; };
typedef struct IDirect3DSwapChain9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * Present; // +0xC 
    void * GetFrontBufferData; // +0x10 
    void * GetBackBuffer; // +0x14 
    void * GetRasterStatus; // +0x18 
    void * GetDisplayMode; // +0x1C 
    void * GetDevice; // +0x20 
    void * GetPresentParameters; // +0x24 
} IDirect3DSwapChain9Vtbl;
struct IDirect3DSwapChain9 { const IDirect3DSwapChain9Vtbl * lpVtbl; };
typedef struct IDirect3DResource9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC 
    void * SetPrivateData; // +0x10 
    void * GetPrivateData; // +0x14 
    void * FreePrivateData; // +0x18 
    void * SetPriority; // +0x1C 
    void * GetPriority; // +0x20 
    void * PreLoad; // +0x24 
    void * GetType; // +0x28 
} IDirect3DResource9Vtbl;
struct IDirect3DResource9 { const IDirect3DResource9Vtbl * lpVtbl; };
typedef struct IDirect3DBaseTexture9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * SetLOD; // +0x2C 
    void * GetLOD; // +0x30 
    void * GetLevelCount; // +0x34 
    void * SetAutoGenFilterType; // +0x38 
    void * GetAutoGenFilterType; // +0x3C 
    void * GenerateMipSubLevels; // +0x40 
} IDirect3DBaseTexture9Vtbl;
struct IDirect3DBaseTexture9 { const IDirect3DBaseTexture9Vtbl * lpVtbl; };
typedef struct IDirect3DTexture9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * SetLOD; // +0x2C IDirect3DBaseTexture9
    void * GetLOD; // +0x30 IDirect3DBaseTexture9
    void * GetLevelCount; // +0x34 IDirect3DBaseTexture9
    void * SetAutoGenFilterType; // +0x38 IDirect3DBaseTexture9
    void * GetAutoGenFilterType; // +0x3C IDirect3DBaseTexture9
    void * GenerateMipSubLevels; // +0x40 IDirect3DBaseTexture9
    void * GetLevelDesc; // +0x44 
    void * GetSurfaceLevel; // +0x48 
    void * LockRect; // +0x4C 
    void * UnlockRect; // +0x50 
    void * AddDirtyRect; // +0x54 
} IDirect3DTexture9Vtbl;
struct IDirect3DTexture9 { const IDirect3DTexture9Vtbl * lpVtbl; };
typedef struct IDirect3DCubeTexture9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * SetLOD; // +0x2C IDirect3DBaseTexture9
    void * GetLOD; // +0x30 IDirect3DBaseTexture9
    void * GetLevelCount; // +0x34 IDirect3DBaseTexture9
    void * SetAutoGenFilterType; // +0x38 IDirect3DBaseTexture9
    void * GetAutoGenFilterType; // +0x3C IDirect3DBaseTexture9
    void * GenerateMipSubLevels; // +0x40 IDirect3DBaseTexture9
    void * GetLevelDesc; // +0x44 
    void * GetCubeMapSurface; // +0x48 
    void * LockRect; // +0x4C 
    void * UnlockRect; // +0x50 
    void * AddDirtyRect; // +0x54 
} IDirect3DCubeTexture9Vtbl;
struct IDirect3DCubeTexture9 { const IDirect3DCubeTexture9Vtbl * lpVtbl; };
typedef struct IDirect3DVolumeTexture9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * SetLOD; // +0x2C IDirect3DBaseTexture9
    void * GetLOD; // +0x30 IDirect3DBaseTexture9
    void * GetLevelCount; // +0x34 IDirect3DBaseTexture9
    void * SetAutoGenFilterType; // +0x38 IDirect3DBaseTexture9
    void * GetAutoGenFilterType; // +0x3C IDirect3DBaseTexture9
    void * GenerateMipSubLevels; // +0x40 IDirect3DBaseTexture9
    void * GetLevelDesc; // +0x44 
    void * GetVolumeLevel; // +0x48 
    void * LockBox; // +0x4C 
    void * UnlockBox; // +0x50 
    void * AddDirtyBox; // +0x54 
} IDirect3DVolumeTexture9Vtbl;
struct IDirect3DVolumeTexture9 { const IDirect3DVolumeTexture9Vtbl * lpVtbl; };
typedef struct IDirect3DSurface9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * GetContainer; // +0x2C 
    void * GetDesc; // +0x30 
    void * LockRect; // +0x34 
    void * UnlockRect; // +0x38 
    void * GetDC; // +0x3C 
    void * ReleaseDC; // +0x40 
} IDirect3DSurface9Vtbl;
struct IDirect3DSurface9 { const IDirect3DSurface9Vtbl * lpVtbl; };
typedef struct IDirect3DVertexBuffer9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * Lock; // +0x2C 
    void * Unlock; // +0x30 
    void * GetDesc; // +0x34 
} IDirect3DVertexBuffer9Vtbl;
struct IDirect3DVertexBuffer9 { const IDirect3DVertexBuffer9Vtbl * lpVtbl; };
typedef struct IDirect3DIndexBuffer9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC IDirect3DResource9
    void * SetPrivateData; // +0x10 IDirect3DResource9
    void * GetPrivateData; // +0x14 IDirect3DResource9
    void * FreePrivateData; // +0x18 IDirect3DResource9
    void * SetPriority; // +0x1C IDirect3DResource9
    void * GetPriority; // +0x20 IDirect3DResource9
    void * PreLoad; // +0x24 IDirect3DResource9
    void * GetType; // +0x28 IDirect3DResource9
    void * Lock; // +0x2C 
    void * Unlock; // +0x30 
    void * GetDesc; // +0x34 
} IDirect3DIndexBuffer9Vtbl;
struct IDirect3DIndexBuffer9 { const IDirect3DIndexBuffer9Vtbl * lpVtbl; };
typedef struct IDirect3DStateBlock9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC 
    void * Capture; // +0x10 
    void * Apply; // +0x14 
} IDirect3DStateBlock9Vtbl;
struct IDirect3DStateBlock9 { const IDirect3DStateBlock9Vtbl * lpVtbl; };
typedef struct IDirect3DVertexDeclaration9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC 
    void * GetDeclaration; // +0x10 
} IDirect3DVertexDeclaration9Vtbl;
struct IDirect3DVertexDeclaration9 { const IDirect3DVertexDeclaration9Vtbl * lpVtbl; };
typedef struct IDirect3DVertexShader9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC 
    void * GetFunction; // +0x10 
} IDirect3DVertexShader9Vtbl;
struct IDirect3DVertexShader9 { const IDirect3DVertexShader9Vtbl * lpVtbl; };
typedef struct IDirect3DPixelShader9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC 
    void * GetFunction; // +0x10 
} IDirect3DPixelShader9Vtbl;
struct IDirect3DPixelShader9 { const IDirect3DPixelShader9Vtbl * lpVtbl; };
typedef struct IDirect3DQuery9Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetDevice; // +0xC 
    void * GetType; // +0x10 
    void * GetDataSize; // +0x14 
    void * Issue; // +0x18 
    void * GetData; // +0x1C 
} IDirect3DQuery9Vtbl;
struct IDirect3DQuery9 { const IDirect3DQuery9Vtbl * lpVtbl; };
typedef struct IDirectSoundVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * CreateSoundBuffer; // +0xC 
    void * GetCaps; // +0x10 
    void * DuplicateSoundBuffer; // +0x14 
    void * SetCooperativeLevel; // +0x18 
    void * Compact; // +0x1C 
    void * GetSpeakerConfig; // +0x20 
    void * SetSpeakerConfig; // +0x24 
    void * Initialize; // +0x28 
} IDirectSoundVtbl;
struct IDirectSound { const IDirectSoundVtbl * lpVtbl; };
typedef struct IDirectSound8Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * CreateSoundBuffer; // +0xC IDirectSound
    void * GetCaps; // +0x10 IDirectSound
    void * DuplicateSoundBuffer; // +0x14 IDirectSound
    void * SetCooperativeLevel; // +0x18 IDirectSound
    void * Compact; // +0x1C IDirectSound
    void * GetSpeakerConfig; // +0x20 IDirectSound
    void * SetSpeakerConfig; // +0x24 IDirectSound
    void * Initialize; // +0x28 IDirectSound
    void * VerifyCertification; // +0x2C 
} IDirectSound8Vtbl;
struct IDirectSound8 { const IDirectSound8Vtbl * lpVtbl; };
typedef struct IDirectSoundBufferVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetCaps; // +0xC 
    void * GetCurrentPosition; // +0x10 
    void * GetFormat; // +0x14 
    void * GetVolume; // +0x18 
    void * GetPan; // +0x1C 
    void * GetFrequency; // +0x20 
    void * GetStatus; // +0x24 
    void * Initialize; // +0x28 
    void * Lock; // +0x2C 
    void * Play; // +0x30 
    void * SetCurrentPosition; // +0x34 
    void * SetFormat; // +0x38 
    void * SetVolume; // +0x3C 
    void * SetPan; // +0x40 
    void * SetFrequency; // +0x44 
    void * Stop; // +0x48 
    void * Unlock; // +0x4C 
    void * Restore; // +0x50 
} IDirectSoundBufferVtbl;
struct IDirectSoundBuffer { const IDirectSoundBufferVtbl * lpVtbl; };
typedef struct IDirectSoundBuffer8Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetCaps; // +0xC IDirectSoundBuffer
    void * GetCurrentPosition; // +0x10 IDirectSoundBuffer
    void * GetFormat; // +0x14 IDirectSoundBuffer
    void * GetVolume; // +0x18 IDirectSoundBuffer
    void * GetPan; // +0x1C IDirectSoundBuffer
    void * GetFrequency; // +0x20 IDirectSoundBuffer
    void * GetStatus; // +0x24 IDirectSoundBuffer
    void * Initialize; // +0x28 IDirectSoundBuffer
    void * Lock; // +0x2C IDirectSoundBuffer
    void * Play; // +0x30 IDirectSoundBuffer
    void * SetCurrentPosition; // +0x34 IDirectSoundBuffer
    void * SetFormat; // +0x38 IDirectSoundBuffer
    void * SetVolume; // +0x3C IDirectSoundBuffer
    void * SetPan; // +0x40 IDirectSoundBuffer
    void * SetFrequency; // +0x44 IDirectSoundBuffer
    void * Stop; // +0x48 IDirectSoundBuffer
    void * Unlock; // +0x4C IDirectSoundBuffer
    void * Restore; // +0x50 IDirectSoundBuffer
    void * SetFX; // +0x54 
    void * AcquireResources; // +0x58 
    void * GetObjectInPath; // +0x5C 
} IDirectSoundBuffer8Vtbl;
struct IDirectSoundBuffer8 { const IDirectSoundBuffer8Vtbl * lpVtbl; };
typedef struct IDirectSound3DBufferVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetAllParameters; // +0xC 
    void * GetConeAngles; // +0x10 
    void * GetConeOrientation; // +0x14 
    void * GetConeOutsideVolume; // +0x18 
    void * GetMaxDistance; // +0x1C 
    void * GetMinDistance; // +0x20 
    void * GetMode; // +0x24 
    void * GetPosition; // +0x28 
    void * GetVelocity; // +0x2C 
    void * SetAllParameters; // +0x30 
    void * SetConeAngles; // +0x34 
    void * SetConeOrientation; // +0x38 
    void * SetConeOutsideVolume; // +0x3C 
    void * SetMaxDistance; // +0x40 
    void * SetMinDistance; // +0x44 
    void * SetMode; // +0x48 
    void * SetPosition; // +0x4C 
    void * SetVelocity; // +0x50 
} IDirectSound3DBufferVtbl;
struct IDirectSound3DBuffer { const IDirectSound3DBufferVtbl * lpVtbl; };
typedef struct IDirectSound3DListenerVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetAllParameters; // +0xC 
    void * GetDistanceFactor; // +0x10 
    void * GetDopplerFactor; // +0x14 
    void * GetOrientation; // +0x18 
    void * GetPosition; // +0x1C 
    void * GetRolloffFactor; // +0x20 
    void * GetVelocity; // +0x24 
    void * SetAllParameters; // +0x28 
    void * SetDistanceFactor; // +0x2C 
    void * SetDopplerFactor; // +0x30 
    void * SetOrientation; // +0x34 
    void * SetPosition; // +0x38 
    void * SetRolloffFactor; // +0x3C 
    void * SetVelocity; // +0x40 
    void * CommitDeferredSettings; // +0x44 
} IDirectSound3DListenerVtbl;
struct IDirectSound3DListener { const IDirectSound3DListenerVtbl * lpVtbl; };
typedef struct IDirectSoundNotifyVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * SetNotificationPositions; // +0xC 
} IDirectSoundNotifyVtbl;
struct IDirectSoundNotify { const IDirectSoundNotifyVtbl * lpVtbl; };
typedef struct IKsPropertySetVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * Get; // +0xC 
    void * Set; // +0x10 
    void * QuerySupport; // +0x14 
} IKsPropertySetVtbl;
struct IKsPropertySet { const IKsPropertySetVtbl * lpVtbl; };
typedef struct IDirectInput8AVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * CreateDevice; // +0xC 
    void * EnumDevices; // +0x10 
    void * GetDeviceStatus; // +0x14 
    void * RunControlPanel; // +0x18 
    void * Initialize; // +0x1C 
    void * FindDevice; // +0x20 
    void * EnumDevicesBySemantics; // +0x24 
    void * ConfigureDevices; // +0x28 
} IDirectInput8AVtbl;
struct IDirectInput8A { const IDirectInput8AVtbl * lpVtbl; };
typedef struct IDirectInputDevice8AVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetCapabilities; // +0xC 
    void * EnumObjects; // +0x10 
    void * GetProperty; // +0x14 
    void * SetProperty; // +0x18 
    void * Acquire; // +0x1C 
    void * Unacquire; // +0x20 
    void * GetDeviceState; // +0x24 
    void * GetDeviceData; // +0x28 
    void * SetDataFormat; // +0x2C 
    void * SetEventNotification; // +0x30 
    void * SetCooperativeLevel; // +0x34 
    void * GetObjectInfo; // +0x38 
    void * GetDeviceInfo; // +0x3C 
    void * RunControlPanel; // +0x40 
    void * Initialize; // +0x44 
    void * CreateEffect; // +0x48 
    void * EnumEffects; // +0x4C 
    void * GetEffectInfo; // +0x50 
    void * GetForceFeedbackState; // +0x54 
    void * SendForceFeedbackCommand; // +0x58 
    void * EnumCreatedEffectObjects; // +0x5C 
    void * Escape; // +0x60 
    void * Poll; // +0x64 
    void * SendDeviceData; // +0x68 
    void * EnumEffectsInFile; // +0x6C 
    void * WriteEffectToFile; // +0x70 
    void * BuildActionMap; // +0x74 
    void * SetActionMap; // +0x78 
    void * GetImageInfo; // +0x7C 
} IDirectInputDevice8AVtbl;
struct IDirectInputDevice8A { const IDirectInputDevice8AVtbl * lpVtbl; };
typedef struct IDirectInputEffectVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * Initialize; // +0xC 
    void * GetEffectGuid; // +0x10 
    void * GetParameters; // +0x14 
    void * SetParameters; // +0x18 
    void * Start; // +0x1C 
    void * Stop; // +0x20 
    void * GetEffectStatus; // +0x24 
    void * Download; // +0x28 
    void * Unload; // +0x2C 
    void * Escape; // +0x30 
} IDirectInputEffectVtbl;
struct IDirectInputEffect { const IDirectInputEffectVtbl * lpVtbl; };
typedef struct IDirectInputAVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * CreateDevice; // +0xC 
    void * EnumDevices; // +0x10 
    void * GetDeviceStatus; // +0x14 
    void * RunControlPanel; // +0x18 
    void * Initialize; // +0x1C 
    void * FindDevice; // +0x20 
    void * CreateDeviceEx; // +0x24 
} IDirectInputAVtbl;
struct IDirectInputA { const IDirectInputAVtbl * lpVtbl; };
typedef struct IDirectInputDeviceAVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * GetCapabilities; // +0xC 
    void * EnumObjects; // +0x10 
    void * GetProperty; // +0x14 
    void * SetProperty; // +0x18 
    void * Acquire; // +0x1C 
    void * Unacquire; // +0x20 
    void * GetDeviceState; // +0x24 
    void * GetDeviceData; // +0x28 
    void * SetDataFormat; // +0x2C 
    void * SetEventNotification; // +0x30 
    void * SetCooperativeLevel; // +0x34 
    void * GetObjectInfo; // +0x38 
    void * GetDeviceInfo; // +0x3C 
    void * RunControlPanel; // +0x40 
    void * Initialize; // +0x44 
    void * CreateEffect; // +0x48 
    void * EnumEffects; // +0x4C 
    void * GetEffectInfo; // +0x50 
    void * GetForceFeedbackState; // +0x54 
    void * SendForceFeedbackCommand; // +0x58 
    void * EnumCreatedEffectObjects; // +0x5C 
    void * Escape; // +0x60 
    void * Poll; // +0x64 
    void * SendDeviceData; // +0x68 
    void * EnumEffectsInFile; // +0x6C 
    void * WriteEffectToFile; // +0x70 
} IDirectInputDeviceAVtbl;
struct IDirectInputDeviceA { const IDirectInputDeviceAVtbl * lpVtbl; };
typedef struct IDirectDrawVtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * Compact; // +0xC 
    void * CreateClipper; // +0x10 
    void * CreatePalette; // +0x14 
    void * CreateSurface; // +0x18 
    void * DuplicateSurface; // +0x1C 
    void * EnumDisplayModes; // +0x20 
    void * EnumSurfaces; // +0x24 
    void * FlipToGDISurface; // +0x28 
    void * GetCaps; // +0x2C 
    void * GetDisplayMode; // +0x30 
    void * GetFourCCCodes; // +0x34 
    void * GetGDISurface; // +0x38 
    void * GetMonitorFrequency; // +0x3C 
    void * GetScanLine; // +0x40 
    void * GetVerticalBlankStatus; // +0x44 
    void * Initialize; // +0x48 
    void * RestoreDisplayMode; // +0x4C 
    void * SetCooperativeLevel; // +0x50 
    void * SetDisplayMode; // +0x54 
    void * WaitForVerticalBlank; // +0x58 
} IDirectDrawVtbl;
struct IDirectDraw { const IDirectDrawVtbl * lpVtbl; };
typedef struct IDirectDraw2Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * Compact; // +0xC IDirectDraw
    void * CreateClipper; // +0x10 IDirectDraw
    void * CreatePalette; // +0x14 IDirectDraw
    void * CreateSurface; // +0x18 IDirectDraw
    void * DuplicateSurface; // +0x1C IDirectDraw
    void * EnumDisplayModes; // +0x20 IDirectDraw
    void * EnumSurfaces; // +0x24 IDirectDraw
    void * FlipToGDISurface; // +0x28 IDirectDraw
    void * GetCaps; // +0x2C IDirectDraw
    void * GetDisplayMode; // +0x30 IDirectDraw
    void * GetFourCCCodes; // +0x34 IDirectDraw
    void * GetGDISurface; // +0x38 IDirectDraw
    void * GetMonitorFrequency; // +0x3C IDirectDraw
    void * GetScanLine; // +0x40 IDirectDraw
    void * GetVerticalBlankStatus; // +0x44 IDirectDraw
    void * Initialize; // +0x48 IDirectDraw
    void * RestoreDisplayMode; // +0x4C IDirectDraw
    void * SetCooperativeLevel; // +0x50 IDirectDraw
    void * SetDisplayMode; // +0x54 
    void * WaitForVerticalBlank; // +0x58 IDirectDraw
    void * GetAvailableVidMem; // +0x5C 
} IDirectDraw2Vtbl;
struct IDirectDraw2 { const IDirectDraw2Vtbl * lpVtbl; };
typedef struct IDirectDraw7Vtbl {
    void * QueryInterface; // +0x0 IUnknown
    void * AddRef; // +0x4 IUnknown
    void * Release; // +0x8 IUnknown
    void * Compact; // +0xC IDirectDraw
    void * CreateClipper; // +0x10 IDirectDraw
    void * CreatePalette; // +0x14 IDirectDraw
    void * CreateSurface; // +0x18 IDirectDraw
    void * DuplicateSurface; // +0x1C IDirectDraw
    void * EnumDisplayModes; // +0x20 IDirectDraw
    void * EnumSurfaces; // +0x24 IDirectDraw
    void * FlipToGDISurface; // +0x28 IDirectDraw
    void * GetCaps; // +0x2C IDirectDraw
    void * GetDisplayMode; // +0x30 IDirectDraw
    void * GetFourCCCodes; // +0x34 IDirectDraw
    void * GetGDISurface; // +0x38 IDirectDraw
    void * GetMonitorFrequency; // +0x3C IDirectDraw
    void * GetScanLine; // +0x40 IDirectDraw
    void * GetVerticalBlankStatus; // +0x44 IDirectDraw
    void * Initialize; // +0x48 IDirectDraw
    void * RestoreDisplayMode; // +0x4C IDirectDraw
    void * SetCooperativeLevel; // +0x50 IDirectDraw
    void * SetDisplayMode; // +0x54 IDirectDraw2
    void * WaitForVerticalBlank; // +0x58 IDirectDraw
    void * GetAvailableVidMem; // +0x5C IDirectDraw2
    void * GetSurfaceFromDC; // +0x60 
    void * RestoreAllSurfaces; // +0x64 
    void * TestCooperativeLevel; // +0x68 
    void * GetDeviceIdentifier; // +0x6C 
    void * StartModeTest; // +0x70 
    void * EvaluateMode; // +0x74 
} IDirectDraw7Vtbl;
struct IDirectDraw7 { const IDirectDraw7Vtbl * lpVtbl; };
