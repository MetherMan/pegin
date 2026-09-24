// Direct3D 8 vertex-processing check behind tools/patch_render_mixed_vp.py.
// Draws an engine-like skinned mesh (indexed vertex blending, UBYTE4 indices, 60 world
// matrices, managed WRITEONLY buffers) and static lit geometry on SOFTWARE, MIXED and
// HARDWARE devices, compares the pixels and times the frames. On GPUs reporting
// MaxVertexBlendMatrixIndex=0 (e.g. NVIDIA) HARDWARE breaks skinning, while MIXED with
// software mode around skinned draws (what CW3DModel::Render does) matches SOFTWARE.
// Build (x86): cl /O2 /EHsc /MT tools\d3d8_vp_check.cpp user32.lib gdi32.lib
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>

typedef HRESULT (__stdcall *Fn)(...);
static void** vt(void* o) { return *(void***)o; }
#define CALL(obj, idx, T, ...) ((T)vt(obj)[idx])(obj, __VA_ARGS__)
#define CALL0(obj, idx, T) ((T)vt(obj)[idx])(obj)

struct PP { UINT w, h; DWORD fmt; UINT count; DWORD ms; DWORD swap; HWND wnd; BOOL windowed; BOOL autoDS; DWORD dsfmt; DWORD flags; UINT refresh; UINT interval; };
struct Mode { UINT w, h, refresh; DWORD fmt; };
struct Mat { float m[16]; };
struct Light { DWORD type; float diffuse[4], specular[4], ambient[4], pos[3], dir[3], range, falloff, a0, a1, a2, theta, phi; };
struct Material { float diffuse[4], ambient[4], specular[4], emissive[4], power; };
struct LockedRect { INT pitch; void* bits; };
struct SkinV { float p[3]; BYTE idx[4]; float n[3]; float u, v; };
struct StatV { float p[3]; float n[3]; float u, v; };

typedef HRESULT (__stdcall *CreateDeviceFn)(void*, UINT, DWORD, HWND, DWORD, PP*, void**);
typedef HRESULT (__stdcall *GetCapsFn)(void*, UINT, DWORD, BYTE*);
typedef HRESULT (__stdcall *GetModeFn)(void*, UINT, Mode*);
typedef ULONG (__stdcall *ReleaseFn)(void*);
typedef HRESULT (__stdcall *CreateVBFn)(void*, UINT, DWORD, DWORD, DWORD, void**);
typedef HRESULT (__stdcall *CreateIBFn)(void*, UINT, DWORD, DWORD, DWORD, void**);
typedef HRESULT (__stdcall *LockFn)(void*, UINT, UINT, BYTE**, DWORD);
typedef HRESULT (__stdcall *UnlockFn)(void*);
typedef HRESULT (__stdcall *RSFn)(void*, DWORD, DWORD);
typedef HRESULT (__stdcall *XFFn)(void*, DWORD, const Mat*);
typedef HRESULT (__stdcall *VSFn)(void*, DWORD);
typedef HRESULT (__stdcall *StreamFn)(void*, UINT, void*, UINT);
typedef HRESULT (__stdcall *IndicesFn)(void*, void*, UINT);
typedef HRESULT (__stdcall *DIPFn)(void*, DWORD, UINT, UINT, UINT, UINT);
typedef HRESULT (__stdcall *ClearFn)(void*, DWORD, const void*, DWORD, DWORD, float, DWORD);
typedef HRESULT (__stdcall *VoidFn)(void*);
typedef HRESULT (__stdcall *PresentFn)(void*, const RECT*, const RECT*, HWND, const void*);
typedef HRESULT (__stdcall *GetBBFn)(void*, UINT, DWORD, void**);
typedef HRESULT (__stdcall *LockRectFn)(void*, LockedRect*, const RECT*, DWORD);
typedef HRESULT (__stdcall *SetLightFn)(void*, DWORD, const Light*);
typedef HRESULT (__stdcall *LightEnableFn)(void*, DWORD, BOOL);
typedef HRESULT (__stdcall *SetMaterialFn)(void*, const Material*);

enum { W = 320, H = 240, BONES = 60 };

static Mat ident() { Mat r = {}; r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1; return r; }
static Mat mul(const Mat& a, const Mat& b) { Mat r = {}; for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) for (int k = 0; k < 4; k++) r.m[i*4+j] += a.m[i*4+k] * b.m[k*4+j]; return r; }
static Mat rotY(float t) { Mat r = ident(); r.m[0] = cosf(t); r.m[2] = -sinf(t); r.m[8] = sinf(t); r.m[10] = cosf(t); return r; }
static Mat rotZ(float t) { Mat r = ident(); r.m[0] = cosf(t); r.m[1] = sinf(t); r.m[4] = -sinf(t); r.m[5] = cosf(t); return r; }
static Mat trans(float x, float y, float z) { Mat r = ident(); r.m[12] = x; r.m[13] = y; r.m[14] = z; return r; }
static Mat persp() { float f = 1.0f / tanf(0.5f), zn = 0.5f, zf = 100; Mat r = {}; r.m[0] = f * H / W; r.m[5] = f; r.m[10] = zf / (zf - zn); r.m[11] = 1; r.m[14] = -zn * zf / (zf - zn); return r; }

struct Scene { void* vbSkin; void* ibSkin; void* vbStat; void* ibStat; UINT skinVerts, skinTris, statVerts, statTris; };

static bool fill(void* dev, Scene& s, DWORD usage, int grid, int statGrid) {
    // Skinned ribbon: vertices along X bound to 60 bones by column.
    s.skinVerts = (grid + 1) * (grid + 1); s.skinTris = grid * grid * 2;
    if (FAILED(CALL(dev, 23, CreateVBFn, s.skinVerts * sizeof(SkinV), usage, 0x1116, 1, &s.vbSkin))) return false;
    SkinV* v; CALL(s.vbSkin, 11, LockFn, 0, 0, (BYTE**)&v, 0);
    for (int y = 0; y <= grid; y++) for (int x = 0; x <= grid; x++) {
        SkinV& o = v[y * (grid + 1) + x];
        o.p[0] = -3 + 6.0f * x / grid; o.p[1] = -1.5f + 3.0f * y / grid; o.p[2] = 0;
        int b = x * BONES / (grid + 1); o.idx[0] = (BYTE)b; o.idx[1] = o.idx[2] = o.idx[3] = 0;
        o.n[0] = 0; o.n[1] = 0; o.n[2] = -1; o.u = (float)x / grid; o.v = (float)y / grid;
    }
    CALL0(s.vbSkin, 12, UnlockFn);
    CALL(dev, 24, CreateIBFn, s.skinTris * 3 * 4, usage, 102, 1, &s.ibSkin);
    DWORD* ix; CALL(s.ibSkin, 11, LockFn, 0, 0, (BYTE**)&ix, 0);
    for (int y = 0; y < grid; y++) for (int x = 0; x < grid; x++) {
        DWORD a = y * (grid + 1) + x, b = a + 1, c = a + grid + 1, d = c + 1;
        *ix++ = a; *ix++ = c; *ix++ = b; *ix++ = b; *ix++ = c; *ix++ = d;
    }
    CALL0(s.ibSkin, 12, UnlockFn);
    // Static terrain-like grid.
    s.statVerts = (statGrid + 1) * (statGrid + 1); s.statTris = statGrid * statGrid * 2;
    CALL(dev, 23, CreateVBFn, s.statVerts * sizeof(StatV), usage, 0x112, 1, &s.vbStat);
    StatV* sv; CALL(s.vbStat, 11, LockFn, 0, 0, (BYTE**)&sv, 0);
    for (int y = 0; y <= statGrid; y++) for (int x = 0; x <= statGrid; x++) {
        StatV& o = sv[y * (statGrid + 1) + x];
        o.p[0] = -4 + 8.0f * x / statGrid; o.p[1] = -2.5f; o.p[2] = 1 + 6.0f * y / statGrid + 0.2f * sinf(x * 0.3f);
        o.n[0] = 0; o.n[1] = 1; o.n[2] = 0; o.u = (float)x / statGrid; o.v = (float)y / statGrid;
    }
    CALL0(s.vbStat, 12, UnlockFn);
    CALL(dev, 24, CreateIBFn, s.statTris * 3 * 4, usage, 102, 1, &s.ibStat);
    CALL(s.ibStat, 11, LockFn, 0, 0, (BYTE**)&ix, 0);
    for (int y = 0; y < statGrid; y++) for (int x = 0; x < statGrid; x++) {
        DWORD a = y * (statGrid + 1) + x, b = a + 1, c = a + statGrid + 1, d = c + 1;
        *ix++ = a; *ix++ = c; *ix++ = b; *ix++ = b; *ix++ = c; *ix++ = d;
    }
    CALL0(s.ibStat, 12, UnlockFn);
    return true;
}

static HRESULT drawSkin(void* dev, Scene& s, bool mixed, float t) {
    if (mixed) CALL(dev, 50, RSFn, 153, TRUE);   // D3DRS_SOFTWAREVERTEXPROCESSING, as the engine does
    CALL(dev, 50, RSFn, 151, 256);               // D3DVBF_0WEIGHTS
    CALL(dev, 50, RSFn, 167, TRUE);              // INDEXEDVERTEXBLENDENABLE
    for (int i = 0; i < BONES; i++) {
        Mat m = mul(rotZ(0.02f * i * sinf(t)), trans(0, 0.8f * sinf(i * 0.2f + t), 4 + 0.05f * i));
        CALL(dev, 37, XFFn, 256 + i, &m);
    }
    CALL(dev, 76, VSFn, 0x1116);
    CALL(dev, 83, StreamFn, 0, s.vbSkin, sizeof(SkinV));
    CALL(dev, 85, IndicesFn, s.ibSkin, 0);
    HRESULT hr = CALL(dev, 71, DIPFn, 4, 0, s.skinVerts, 0, s.skinTris);
    CALL(dev, 50, RSFn, 151, 0);
    CALL(dev, 50, RSFn, 167, FALSE);
    if (mixed) CALL(dev, 50, RSFn, 153, FALSE);
    return hr;
}

static HRESULT drawStatic(void* dev, Scene& s, float t) {
    Mat w = rotY(0.1f * sinf(t));
    CALL(dev, 37, XFFn, 256, &w);
    CALL(dev, 76, VSFn, 0x112);
    CALL(dev, 83, StreamFn, 0, s.vbStat, sizeof(StatV));
    CALL(dev, 85, IndicesFn, s.ibStat, 0);
    return CALL(dev, 71, DIPFn, 4, 0, s.statVerts, 0, s.statTris);
}

static void setup(void* dev) {
    Mat v = ident(), p = persp();
    CALL(dev, 37, XFFn, 2, &v); CALL(dev, 37, XFFn, 3, &p);
    Light l = {}; l.type = 3; l.diffuse[0] = 1; l.diffuse[1] = 0.8f; l.diffuse[2] = 0.6f; l.dir[0] = 0.3f; l.dir[1] = -0.5f; l.dir[2] = 1;
    CALL(dev, 44, SetLightFn, 0, &l); CALL(dev, 46, LightEnableFn, 0, TRUE);
    Material m = {}; m.diffuse[0] = m.diffuse[1] = m.diffuse[2] = m.diffuse[3] = 1; m.ambient[0] = 0.2f; m.ambient[1] = 0.3f; m.ambient[2] = 0.4f;
    CALL(dev, 42, SetMaterialFn, &m);
    CALL(dev, 50, RSFn, 137, TRUE); CALL(dev, 50, RSFn, 139, 0xff404040); CALL(dev, 50, RSFn, 22, 1); CALL(dev, 50, RSFn, 7, 1);
}

static std::vector<DWORD> grab(void* dev) {
    std::vector<DWORD> px(W * H);
    void* bb = 0; CALL(dev, 16, GetBBFn, 0, 0, &bb);
    LockedRect lr; if (SUCCEEDED(CALL(bb, 9, LockRectFn, &lr, NULL, 0x10))) {
        for (int y = 0; y < H; y++) memcpy(&px[y * W], (BYTE*)lr.bits + y * lr.pitch, W * 4);
        CALL0(bb, 10, UnlockFn);
    } else printf("  LockRect failed\n");
    CALL0(bb, 2, ReleaseFn);
    return px;
}

static void* makeDevice(void* d3d, HWND wnd, DWORD flags) {
    Mode mode; CALL(d3d, 8, GetModeFn, 0, &mode);
    PP pp = {}; pp.w = W; pp.h = H; pp.fmt = mode.fmt; pp.count = 1; pp.swap = 1; pp.wnd = wnd; pp.windowed = TRUE;
    pp.autoDS = TRUE; pp.dsfmt = 80; pp.flags = 1; pp.interval = 0;
    void* dev = 0; HRESULT hr = CALL(d3d, 15, CreateDeviceFn, 0, 1, wnd, flags, &pp, &dev);
    if (FAILED(hr)) { printf("CreateDevice(0x%x) failed 0x%08lx\n", flags, hr); return 0; }
    return dev;
}

static void release(Scene& s) { CALL0(s.vbSkin, 2, ReleaseFn); CALL0(s.ibSkin, 2, ReleaseFn); CALL0(s.vbStat, 2, ReleaseFn); CALL0(s.ibStat, 2, ReleaseFn); }

static std::vector<DWORD> render(void* d3d, HWND wnd, DWORD flags, DWORD usage, const char* label, HRESULT* skinHr) {
    std::vector<DWORD> none;
    void* dev = makeDevice(d3d, wnd, flags); if (!dev) return none;
    Scene s = {}; if (!fill(dev, s, usage, 40, 60)) { printf("%s: buffer creation failed\n", label); CALL0(dev, 2, ReleaseFn); return none; }
    setup(dev);
    CALL(dev, 36, ClearFn, 0, NULL, 3, 0xff102030, 1.0f, 0);
    CALL0(dev, 34, VoidFn);
    HRESULT h1 = drawStatic(dev, s, 0.7f);
    HRESULT h2 = drawSkin(dev, s, flags == 0x80, 0.7f);
    HRESULT h3 = drawStatic(dev, s, 0.7f);
    CALL0(dev, 35, VoidFn);
    printf("%-34s static=0x%08lx skinned=0x%08lx static-after=0x%08lx\n", label, h1, h2, h3);
    if (skinHr) *skinHr = h2;
    std::vector<DWORD> px = grab(dev);
    release(s); CALL0(dev, 2, ReleaseFn);
    return px;
}

static double bench(void* d3d, HWND wnd, DWORD flags, DWORD usage, int statGrid, int skinGrid, int frames) {
    void* dev = makeDevice(d3d, wnd, flags); if (!dev) return -1;
    Scene s = {}; fill(dev, s, usage, skinGrid, statGrid); setup(dev);
    LARGE_INTEGER f, a, b; QueryPerformanceFrequency(&f);
    for (int i = 0; i < frames + 5; i++) {
        if (i == 5) QueryPerformanceCounter(&a);
        CALL(dev, 36, ClearFn, 0, NULL, 3, 0xff102030, 1.0f, 0);
        CALL0(dev, 34, VoidFn);
        for (int k = 0; k < 6; k++) drawStatic(dev, s, i * 0.01f + k);
        drawSkin(dev, s, flags == 0x80, i * 0.01f);
        CALL0(dev, 35, VoidFn);
        CALL(dev, 15, PresentFn, NULL, NULL, NULL, NULL);
    }
    QueryPerformanceCounter(&b);
    release(s); CALL0(dev, 2, ReleaseFn);
    return (b.QuadPart - a.QuadPart) * 1000.0 / f.QuadPart / frames;
}

static void compare(const std::vector<DWORD>& a, const std::vector<DWORD>& b, const char* label) {
    if (a.empty() || b.empty()) { printf("%s: no image\n", label); return; }
    int diff = 0, big = 0, maxd = 0, covered = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if ((a[i] & 0xffffff) != 0x102030) covered++;
        int d = 0; for (int c = 0; c < 24; c += 8) { int x = abs((int)((a[i] >> c) & 255) - (int)((b[i] >> c) & 255)); if (x > d) d = x; }
        if (d) diff++; if (d > 24) big++; if (d > maxd) maxd = d;
    }
    printf("%s: covered=%d differing=%d (>24: %d) max=%d\n", label, covered, diff, big, maxd);
}

int main() {
    WNDCLASSA wc = {}; wc.lpfnWndProc = DefWindowProcA; wc.hInstance = GetModuleHandle(0); wc.lpszClassName = "vp";
    RegisterClassA(&wc);
    HWND wnd = CreateWindowA("vp", "vp", WS_OVERLAPPEDWINDOW, 0, 0, W + 16, H + 39, 0, 0, wc.hInstance, 0);
    HMODULE m = LoadLibraryA("d3d8.dll");
    void* (__stdcall *create)(UINT) = (void* (__stdcall*)(UINT))GetProcAddress(m, "Direct3DCreate8");
    void* d3d = create(220);
    BYTE caps[512] = {}; CALL(d3d, 13, GetCapsFn, 0, 1, caps);
    printf("caps: VertexProcessingCaps=0x%lx MaxActiveLights=%lu MaxVertexBlendMatrices=%lu MaxVertexBlendMatrixIndex=%lu VS=0x%08lx DevCaps=0x%lx\n",
        *(DWORD*)(caps + 156), *(DWORD*)(caps + 160), *(DWORD*)(caps + 168), *(DWORD*)(caps + 172), *(DWORD*)(caps + 196), *(DWORD*)(caps + 28));
    std::vector<DWORD> ref = render(d3d, wnd, 0x20, 8, "SOFTWARE (current game)", 0);
    std::vector<DWORD> mixed = render(d3d, wnd, 0x80, 8, "MIXED, engine buffers", 0);
    std::vector<DWORD> mixedSw = render(d3d, wnd, 0x80, 0x18, "MIXED, +SOFTWAREPROCESSING buffers", 0);
    std::vector<DWORD> hw = render(d3d, wnd, 0x40, 8, "HARDWARE (all GPU)", 0);
    compare(ref, mixed, "SOFTWARE vs MIXED engine buffers");
    compare(ref, mixedSw, "SOFTWARE vs MIXED +SW buffers");
    compare(ref, hw, "SOFTWARE vs HARDWARE");
    int frames = 120;
    printf("bench ms/frame (6x %d-vertex static + skinned):\n", 301 * 301);
    printf("  SOFTWARE           %.2f\n", bench(d3d, wnd, 0x20, 8, 300, 60, frames));
    printf("  MIXED engine bufs  %.2f\n", bench(d3d, wnd, 0x80, 8, 300, 60, frames));
    printf("  MIXED +SW bufs     %.2f\n", bench(d3d, wnd, 0x80, 0x18, 300, 60, frames));
    CALL0(d3d, 2, ReleaseFn);
    return 0;
}
