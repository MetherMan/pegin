// Renders game assets through the client's own Render.dll, like the character select screen.
// Run from a GameClient folder: render_engine_check.exe <out-prefix> [bench-frames]
// Writes <out-prefix>-<tick>.bmp for fixed animation ticks (select-screen state, no fog mode),
// <out-prefix>-fog-900.bmp with world-style linear fog, and prints ms/frame.
// Used by tools/verify_render_mixed_vp.py to compare the original and patched Render.dll.
#include <math.h>
// Package.h misspells its memory macro; VC6 accepted the resulting default-int member.
#define MEMORY_UNER_MGR(x)
#include "Render.h"
#include <stdio.h>
#include <stdlib.h>

typedef BOOL (__cdecl *LoadPackageFn)(const char*);
typedef IComponent* (__cdecl *MakeComponentFn)(const char*);

static const int W = 800, H = 600;

static void SaveWindow(HWND wnd, const char* path)
{
    HDC dc = GetDC(wnd), mem = CreateCompatibleDC(dc);
    HBITMAP bmp = CreateCompatibleBitmap(dc, W, H);
    SelectObject(mem, bmp);
    PrintWindow(wnd, mem, 3);  // PW_CLIENTONLY | PW_RENDERFULLCONTENT
    BITMAPINFOHEADER bih = { sizeof(bih), W, H, 1, 24, BI_RGB };
    int stride = (W * 3 + 3) & ~3;
    BYTE* bits = (BYTE*)malloc(stride * H);
    GetDIBits(mem, bmp, 0, H, bits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);
    BITMAPFILEHEADER bfh = { 0x4d42, (DWORD)(sizeof(bfh) + sizeof(bih) + stride * H), 0, 0, sizeof(bfh) + sizeof(bih) };
    FILE* f = fopen(path, "wb");
    fwrite(&bfh, sizeof(bfh), 1, f); fwrite(&bih, sizeof(bih), 1, f); fwrite(bits, stride * H, 1, f); fclose(f);
    free(bits); DeleteObject(bmp); DeleteDC(mem); ReleaseDC(wnd, dc);
}

int main(int argc, char** argv)
{
    const char* prefix = argc > 1 ? argv[1] : "render";
    int benchFrames = argc > 2 ? atoi(argv[2]) : 300;
    HMODULE common = LoadLibraryA("Common.dll");
    LoadPackageFn loadPackage = (LoadPackageFn)GetProcAddress(common, "?LoadComponentPackage@@YAHPBD@Z");
    MakeComponentFn make = (MakeComponentFn)GetProcAddress(common, "?MakeComponent@@YAPAVIComponent@@PBD@Z");
    if (!loadPackage || !make || !loadPackage("Render.dll")) { printf("{\"error\":\"package\"}\n"); return 2; }

    WNDCLASSA wc = {}; wc.lpfnWndProc = DefWindowProcA; wc.hInstance = GetModuleHandle(0); wc.lpszClassName = "LaqiaRenderCheck";
    RegisterClassA(&wc);
    RECT r = { 0, 0, W, H }; AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    HWND wnd = CreateWindowA("LaqiaRenderCheck", "render check", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0,
                             r.right - r.left, r.bottom - r.top, 0, 0, wc.hInstance, 0);

    IW3DScene* scene = (IW3DScene*)make("Scene");
    if (!scene || !scene->Init(wnd, W, H, 32, FALSE)) { printf("{\"error\":\"scene\"}\n"); return 3; }
    scene->SetProjection(0.785398f, 0.1f, 300.0f);
    scene->LookAt(Vector3(0.207f, 1.5f, 0.2f), Vector3(0.207f, 1.2f, 4.011f), Vector3(0.0f, 1.0f, 0.0f));

    IW3DLight* light = (IW3DLight*)make("Light");
    light->SetPosition(Vector3(0.0f, 10.0f, 3.49f));
    light->SetDirectional();
    light->SetDiffuse(0xFFFFDA); light->SetAmbient(0x382E1E); light->SetSpecular(0xFFFFFF);
    light->SetDirection(Vector3(0.0f, -1.0f, -1.0f));

    IW3DModel* back = (IW3DModel*)make("Model");
    IW3DModel* lamp = (IW3DModel*)make("Model");
    if (!back->Load("Object", "stage_always_01.mod") || !lamp->Load("Object", "stage_lighton_always_01.mod"))
    { printf("{\"error\":\"stage\"}\n"); return 4; }

    IW3DAnimationMgr* anims = (IW3DAnimationMgr*)make("AnimationMgr");
    SAction* intro = anims->LoadAnimation("Body/Animation", "intro_m.ani");
    if (!intro) { printf("{\"error\":\"animation\"}\n"); return 5; }
    SAction action = *intro; action.nStart = 2; action.nEnd = 75;
    // Default male armour set: every part is a skinned mesh driven by intro_m.ani.
    const char* parts[] = { "High/ma_tor_a001_1.mod", "High/ma_leg_a001_1.mod", "High/ma_gun_a001_1.mod",
                            "High/ma_boo_a001_1.mod", "High/co_hea_0000.mod", "High/co_hir_0000.mod" };
    IW3DModel* body[6] = {};
    int loaded = 0;
    for (int i = 0; i < 6; i++)
    {
        body[i] = (IW3DModel*)make("Model");
        if (body[i]->Load("Body", parts[i])) { body[i]->SetAction(&action); loaded++; }
        else { body[i]->Release(); body[i] = 0; }
    }
    if (!loaded) { printf("{\"error\":\"body\"}\n"); return 6; }

    Matrix4 stage; stage.SetIdentity();
    Matrix4 character; character.SetWorld(Vector3(0.207f, 0.604f, 4.011f), Vector3(0.0f, 1.0f, 0.0f).GetQuaternion());

    struct Frame { static void Draw(IW3DScene* s, IW3DLight* l, IW3DModel* b, IW3DModel* m, IW3DModel** parts,
                                    const Matrix4& st, const Matrix4& ch, DWORD tick)
    {
        s->BeginFrame();
        l->SetLight(0);
        b->SetWorld(st); b->Render(tick, TRUE);
        m->SetWorld(st); m->Render(tick, TRUE);
        BOOL blended = FALSE;
        for (int i = 0; i < 6; i++)
        {
            if (!parts[i]) continue;
            parts[i]->SetApplyWorld(FALSE);
            parts[i]->SetWorld(ch);
            if (!blended) { parts[i]->SetVertexBlend(tick); blended = TRUE; }
            parts[i]->Render(tick, TRUE);
            parts[i]->SetApplyWorld(TRUE);
        }
        s->EndFrame();
    } };

    MSG msg;
    const DWORD ticks[] = { 0, 400, 900, 1500, 2300 };
    for (int t = 0; t < 5; t++)
    {
        for (int repeat = 0; repeat < 3; repeat++)
        {
            while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) DispatchMessageA(&msg);
            Frame::Draw(scene, light, back, lamp, body, stage, character, ticks[t]);
        }
        char path[260]; sprintf(path, "%s-%lu.bmp", prefix, ticks[t]);
        SaveWindow(wnd, path);
    }

    // The world sets linear fog every frame (CW3DWorld::Draw); compare that path too.
    scene->SetFog(0x303848, 3.0f, 12.0f);
    for (int repeat = 0; repeat < 3; repeat++)
    {
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) DispatchMessageA(&msg);
        Frame::Draw(scene, light, back, lamp, body, stage, character, 900);
    }
    char fogPath[260]; sprintf(fogPath, "%s-fog-900.bmp", prefix);
    SaveWindow(wnd, fogPath);

    LARGE_INTEGER f, a, b; QueryPerformanceFrequency(&f); QueryPerformanceCounter(&a);
    for (int i = 0; i < benchFrames; i++)
    {
        while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) DispatchMessageA(&msg);
        Frame::Draw(scene, light, back, lamp, body, stage, character, (DWORD)(i * 16));
    }
    QueryPerformanceCounter(&b);
    printf("{\"parts\":%d,\"frames\":%d,\"ms_per_frame\":%.3f}\n", loaded, benchFrames,
           (b.QuadPart - a.QuadPart) * 1000.0 / f.QuadPart / benchFrames);
    return 0;
}
