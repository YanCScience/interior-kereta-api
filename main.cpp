/* ===========================================================================
 * PROJECT     : VISUALISASI 3D INTERIOR KERETA API EKSEKUTIF
 * KELOMPOK    : 62 Grafika Komputer
 * FILE        : main.cpp
 * DESKRIPSI   : Program utama simulasi 3D yang mengintegrasikan interior gerbong, pemandangan luar, pencahayaan, HUD,
 *               dan navigasi kamera first-person. File ini menjadi entry point interaktif berbasis freeGLUT.
 * AUTHOR      : Galang Maulid Nugraha
 * ===========================================================================
 * STRUKTUR TIM PENGEMBANG:
 * - Project Manager & Integrator : Diyani Rahayu Nur'aeni
 * - Core Programmer & Camera     : Galang Maulid Nugraha
 * - Interior Modeler             : M. Fauz Haunan Zaky
 * - Exterior & Animation         : Aisyah Nitiarahma
 * - Lighting & Material          : Agniya Azzahra
 * ===========================================================================
 * Dependensi  : OpenGL, freeGLUT, C++ Standard Library
 * Kompilasi   : g++ main.cpp -o output.exe -lfreeglut -lglu32 -lopengl32
 * =========================================================================== */
#define _CRT_SECURE_NO_WARNINGS


#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstring>

// =============================================================================
//  KONSTANTA
// =============================================================================
const int   WIN_W = 1100;
const int   WIN_H = 700;
const float PI = 3.14159265f;

// Ukuran gerbong (digunakan juga sebagai batas gerak kamera)
const float GERBONG_PANJANG = 20.0f;   // total panjang gerbong (sumbu Z)
const float GERBONG_LEBAR = 4.0f;   // lebar gerbong (sumbu X)
const float GERBONG_TINGGI = 3.0f;   // tinggi gerbong (sumbu Y)

// Batas gerak kamera (sedikit lebih kecil dari ukuran gerbong)
const float BATAS_X_MIN = -GERBONG_LEBAR / 2.0f + 0.4f;
const float BATAS_X_MAX = GERBONG_LEBAR / 2.0f - 0.4f;
const float BATAS_Z_MIN = -GERBONG_PANJANG / 2.0f + 0.4f;
const float BATAS_Z_MAX = GERBONG_PANJANG / 2.0f - 0.4f;
const float CAM_Y_TETAP = 1.65f;   // tinggi mata penumpang

// =============================================================================
//  VARIABEL GLOBAL - KAMERA
// =============================================================================
float camX = 0.0f;           // posisi kamera X
float camY = CAM_Y_TETAP;   // posisi kamera Y
float camZ = 3.0f;           // posisi kamera Z
float yaw = 180.0f;          // rotasi horizontal (derajat)
float pitch = 0.0f;          // rotasi vertikal   (derajat)

// Status input mouse
bool  mouseTekan = false;
int   mousePrevX = 0;
int   mousePrevY = 0;
const float SENSITIVITAS = 0.25f;

// =============================================================================
//  VARIABEL GLOBAL - ANIMASI
// =============================================================================
float pohonOffset = 0.0f;   // offset animasi pohon (bergerak mundur)
float waktu = 0.0f;   // counter waktu untuk efek sin (vibrasi)

// =============================================================================
//  VARIABEL GLOBAL - FITUR
// =============================================================================
bool lampuHidup = true;   // toggle lampu interior

// =============================================================================
//  UTILITAS
// =============================================================================

// Konversi derajat -> radian
inline float toRad(float deg) { return deg * PI / 180.0f; }

// Gambar kotak solid (wrapper ringkas)
void drawBox(float sx, float sy, float sz)
{
    glPushMatrix();
    glScalef(sx, sy, sz);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Cetak teks 2D di posisi (x, y) layar
void drawText2D(float x, float y, const char* str, void* font = GLUT_BITMAP_HELVETICA_12)
{
    glRasterPos2f(x, y);
    for (const char* c = str; *c != '\0'; ++c)
        glutBitmapCharacter(font, *c);
}

// =============================================================================
//  MENGGAMBAR KURSI TUNGGAL
//  Dipanggil dengan glTranslate terlebih dahulu ke posisi yang diinginkan.
//  Kursi menghadap ke arah -Z (penumpang duduk menghadap depan gerbong).
// =============================================================================
void drawKursi()
{
    // ---------- Warna dasar komponen ----------
    const float WARNA_BESI[3] = { 0.30f, 0.30f, 0.32f };
    const float WARNA_KAIN[3] = { 0.52f, 0.08f, 0.10f };   // merah tua
    const float WARNA_KAIN_GELAP[3] = { 0.40f, 0.06f, 0.08f };

    // -- Kaki-kaki kursi (empat penopang vertikal disimulasikan dengan kubus) --
    glColor3fv(WARNA_BESI);
    float kp[][2] = { {-0.20f,-0.22f},{0.20f,-0.22f},{-0.20f,0.22f},{0.20f,0.22f} };
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(kp[i][0], 0.22f, kp[i][1]);
        drawBox(0.05f, 0.44f, 0.05f);
        glPopMatrix();
    }

    // -- Penyangga bawah (melintang kiri-kanan & depan-belakang) --
    glPushMatrix(); glTranslatef(0.0f, 0.08f, 0.0f); drawBox(0.40f, 0.04f, 0.04f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.08f, 0.0f); drawBox(0.04f, 0.04f, 0.44f); glPopMatrix();

    // -- Dudukan (alas kursi) --
    glColor3fv(WARNA_KAIN);
    glPushMatrix();
    glTranslatef(0.0f, 0.46f, 0.0f);
    drawBox(0.56f, 0.09f, 0.54f);
    glPopMatrix();

    // -- Sandaran punggung (di belakang dudukan, tidak overlap) --
    glColor3fv(WARNA_KAIN);
    glPushMatrix();
    glTranslatef(0.0f, 0.88f, 0.28f);   // maju ke z+0.28 (tepat di bibir belakang dudukan)
    drawBox(0.56f, 0.78f, 0.10f);
    glPopMatrix();

    // -- Headrest (sandaran kepala) --
    glColor3fv(WARNA_KAIN_GELAP);
    glPushMatrix();
    glTranslatef(0.0f, 1.30f, 0.27f);
    drawBox(0.44f, 0.24f, 0.12f);
    glPopMatrix();

    // -- Sandaran tangan kiri --
    glColor3fv(WARNA_BESI);
    glPushMatrix();
    glTranslatef(-0.30f, 0.62f, 0.0f);
    drawBox(0.06f, 0.28f, 0.50f);
    glPopMatrix();

    // -- Sandaran tangan kanan --
    glPushMatrix();
    glTranslatef(0.30f, 0.62f, 0.0f);
    drawBox(0.06f, 0.28f, 0.50f);
    glPopMatrix();
}

// =============================================================================
//  MENGGAMBAR SEMUA KURSI DI DALAM GERBONG
//  Layout: kursi berpasangan kiri & kanan, lorong kosong di tengah.
// =============================================================================
void drawSemuaKursi()
{
    // Posisi X kursi (kiri dan kanan lorong)
    const float X_KIRI = -1.15f;
    const float X_KANAN = 1.15f;

    // Posisi Z deretan kursi (dari belakang ke depan gerbong)
    float barisZ[] = { -8.5f, -6.5f, -4.5f, -2.5f, -0.5f, 1.5f, 3.5f, 5.5f, 7.5f };
    int jumlahBaris = 9;

    for (int i = 0; i < jumlahBaris; i++) {
        // Kursi kiri (Y=0.08 agar tidak menembus lantai)
        glPushMatrix();
        glTranslatef(X_KIRI, 0.08f, barisZ[i]);
        drawKursi();
        glPopMatrix();

        // Kursi kanan
        glPushMatrix();
        glTranslatef(X_KANAN, 0.08f, barisZ[i]);
        drawKursi();
        glPopMatrix();
    }
}

// =============================================================================
//  MENGGAMBAR JENDELA (tertanam di dinding kiri/kanan)
//  Dipanggil setelah glTranslate ke posisi jendela.
//  Parameter sisiKanan: true = dinding kanan (+X), false = dinding kiri (-X).
// =============================================================================
void drawJendela(bool sisiKanan)
{
    // Jendela tipis ke arah X (tertanam di dinding vertikal)
    float nx = sisiKanan ? 1.0f : -1.0f;  // arah normal dinding

    // -- Frame / bingkai jendela (putih abu) --
    glColor3f(0.82f, 0.82f, 0.82f);

    // Bingkai atas
    glPushMatrix();
    glTranslatef(0.0f, 0.33f, 0.0f);
    drawBox(0.08f * nx, 0.07f, 0.86f);   // panjang Z = lebar jendela
    glPopMatrix();
    // Bingkai bawah
    glPushMatrix();
    glTranslatef(0.0f, -0.33f, 0.0f);
    drawBox(0.08f * nx, 0.07f, 0.86f);
    glPopMatrix();
    // Bingkai kiri (arah Z negatif)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.41f);
    drawBox(0.08f * nx, 0.60f, 0.06f);
    glPopMatrix();
    // Bingkai kanan (arah Z positif)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.41f);
    drawBox(0.08f * nx, 0.60f, 0.06f);
    glPopMatrix();

    // -- Kaca jendela (biru muda semi-transparan) --
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.55f, 0.75f, 0.95f, 0.35f);
    glPushMatrix();
    drawBox(0.05f, 0.58f, 0.76f);
    glPopMatrix();
    glDisable(GL_BLEND);

    // -- Tirai bawah (kuning muda) --
    glColor3f(0.95f, 0.92f, 0.70f);
    glPushMatrix();
    glTranslatef(0.0f, -0.20f, 0.0f);
    drawBox(0.06f, 0.22f, 0.75f);
    glPopMatrix();
}

// =============================================================================
//  MENGGAMBAR GERBONG (INTERIOR LENGKAP)
// =============================================================================
void drawGerbong()
{
    float P = GERBONG_PANJANG;
    float L = GERBONG_LEBAR;
    float T = GERBONG_TINGGI;

    // ----------------------------------------------------------------
    // LANTAI — kayu coklat hangat
    // ----------------------------------------------------------------
    glColor3f(0.56f, 0.42f, 0.28f);
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.0f);
    drawBox(L, 0.10f, P);
    glPopMatrix();

    // Papan lantai (garis-garis dekoratif)
    glColor3f(0.50f, 0.37f, 0.24f);
    for (int i = -9; i <= 9; i++) {
        glPushMatrix();
        glTranslatef(0.0f, -0.0f, i * 1.0f);
        drawBox(L - 0.02f, 0.02f, 0.04f);
        glPopMatrix();
    }

    // Karpet lorong tengah (hijau gelap)
    glColor3f(0.18f, 0.38f, 0.18f);
    glPushMatrix();
    glTranslatef(0.0f, 0.01f, 0.0f);
    drawBox(0.85f, 0.02f, P - 0.3f);
    glPopMatrix();

    // ----------------------------------------------------------------
    // ATAP — putih gading
    // ----------------------------------------------------------------
    glColor3f(0.92f, 0.92f, 0.90f);
    glPushMatrix();
    glTranslatef(0.0f, T + 0.075f, 0.0f);
    drawBox(L, 0.15f, P);
    glPopMatrix();

    // List plafon (pinggiran dekoratif)
    glColor3f(0.85f, 0.85f, 0.82f);
    // kiri
    glPushMatrix(); glTranslatef(-L / 2 + 0.20f, T - 0.05f, 0.0f); drawBox(0.40f, 0.08f, P); glPopMatrix();
    // kanan
    glPushMatrix(); glTranslatef(L / 2 - 0.20f, T - 0.05f, 0.0f); drawBox(0.40f, 0.08f, P); glPopMatrix();

    // ----------------------------------------------------------------
    // DINDING KIRI (-X)
    // ----------------------------------------------------------------
    glColor3f(0.87f, 0.83f, 0.76f);
    glPushMatrix();
    glTranslatef(-L / 2 - 0.075f, T / 2, 0.0f);
    drawBox(0.15f, T, P);
    glPopMatrix();

    // ----------------------------------------------------------------
    // DINDING KANAN (+X)
    // ----------------------------------------------------------------
    glPushMatrix();
    glTranslatef(L / 2 + 0.075f, T / 2, 0.0f);
    drawBox(0.15f, T, P);
    glPopMatrix();

    // ----------------------------------------------------------------
    // DINDING DEPAN (-Z)
    // ----------------------------------------------------------------
    glColor3f(0.80f, 0.76f, 0.70f);
    glPushMatrix();
    glTranslatef(0.0f, T / 2, -P / 2 - 0.075f);
    drawBox(L + 0.3f, T, 0.15f);
    glPopMatrix();

    // ----------------------------------------------------------------
    // DINDING BELAKANG (+Z)
    // ----------------------------------------------------------------
    glPushMatrix();
    glTranslatef(0.0f, T / 2, P / 2 + 0.075f);
    drawBox(L + 0.3f, T, 0.15f);
    glPopMatrix();

    // ----------------------------------------------------------------
    // PANEL BAWAH JENDELA (dado) — kiri & kanan
    // ----------------------------------------------------------------
    glColor3f(0.70f, 0.65f, 0.55f);
    // kiri
    glPushMatrix(); glTranslatef(-L / 2, 0.65f, 0.0f); drawBox(0.12f, 1.20f, P - 0.5f); glPopMatrix();
    // kanan
    glPushMatrix(); glTranslatef(L / 2, 0.65f, 0.0f); drawBox(0.12f, 1.20f, P - 0.5f); glPopMatrix();

    // ----------------------------------------------------------------
    // JENDELA — kiri & kanan, 6 pasang
    // ----------------------------------------------------------------
    float jendelaZ[] = { -8.0f, -5.0f, -2.0f, 1.0f, 4.0f, 7.0f };
    for (int i = 0; i < 6; i++) {
        // Jendela kiri
        glPushMatrix();
        glTranslatef(-L / 2, 1.80f, jendelaZ[i]);
        drawJendela(false);
        glPopMatrix();

        // Jendela kanan
        glPushMatrix();
        glTranslatef(L / 2, 1.80f, jendelaZ[i]);
        drawJendela(true);
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // RAK BAGASI ATAS (overhead compartment)
    // ----------------------------------------------------------------
    glColor3f(0.72f, 0.67f, 0.58f);
    // kiri
    glPushMatrix();
    glTranslatef(-L / 2 + 0.52f, T - 0.45f, 0.0f);
    drawBox(0.60f, 0.40f, P - 0.6f);
    glPopMatrix();
    // kanan
    glPushMatrix();
    glTranslatef(L / 2 - 0.52f, T - 0.45f, 0.0f);
    drawBox(0.60f, 0.40f, P - 0.6f);
    glPopMatrix();

    // Pintu rak bagasi (list horizontal)
    glColor3f(0.60f, 0.55f, 0.48f);
    for (int i = -4; i <= 4; i++) {
        float rz = i * 2.2f;
        // kiri
        glPushMatrix();
        glTranslatef(-L / 2 + 0.28f, T - 0.45f, rz);
        drawBox(0.05f, 0.38f, 1.90f);
        glPopMatrix();
        // kanan
        glPushMatrix();
        glTranslatef(L / 2 - 0.28f, T - 0.45f, rz);
        drawBox(0.05f, 0.38f, 1.90f);
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // LAMPU ATAP (strip lampu LED)
    // ----------------------------------------------------------------
    if (lampuHidup)
        glColor3f(1.0f, 0.98f, 0.88f);
    else
        glColor3f(0.5f, 0.5f, 0.5f);

    float lampuZ[] = { -8.0f, -4.0f, 0.0f, 4.0f, 8.0f };
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(0.0f, T - 0.04f, lampuZ[i]);
        drawBox(0.50f, 0.10f, 1.60f);
        glPopMatrix();
    }
}

// =============================================================================
//  MENGGAMBAR POHON TUNGGAL
//  Dipanggil setelah glTranslate ke posisi pohon.
// =============================================================================
void drawPohon()
{
    // -- Batang --
    glColor3f(0.45f, 0.28f, 0.10f);
    glPushMatrix();
    glTranslatef(0.0f, 0.90f, 0.0f);
    drawBox(0.30f, 1.80f, 0.30f);
    glPopMatrix();

    // -- Tiga lapisan daun (kerucut bertumpuk) --
    glColor3f(0.10f, 0.52f, 0.12f);
    glPushMatrix();
    glTranslatef(0.0f, 2.20f, 0.0f);
    glutSolidCone(1.10f, 1.60f, 8, 4);
    glPopMatrix();

    glColor3f(0.12f, 0.60f, 0.14f);
    glPushMatrix();
    glTranslatef(0.0f, 2.85f, 0.0f);
    glutSolidCone(0.85f, 1.30f, 8, 4);
    glPopMatrix();

    glColor3f(0.15f, 0.68f, 0.18f);
    glPushMatrix();
    glTranslatef(0.0f, 3.35f, 0.0f);
    glutSolidCone(0.60f, 1.00f, 8, 4);
    glPopMatrix();
}

// =============================================================================
//  MENGGAMBAR PEMANDANGAN LUAR (bergerak animasi)
// =============================================================================
void drawPemandangan()
{
    // ----------------------------------------------------------------
    // TANAH / PADANG RUMPUT
    // ----------------------------------------------------------------
    glColor3f(0.28f, 0.58f, 0.20f);
    glPushMatrix();
    glTranslatef(0.0f, -0.60f, pohonOffset);
    drawBox(80.0f, 0.20f, 150.0f);
    glPopMatrix();

    // ----------------------------------------------------------------
    // REL KERETA (dua jalur paralel)
    // ----------------------------------------------------------------
    // Bantalan rel (kayu coklat)
    glColor3f(0.50f, 0.35f, 0.18f);
    float bantalanStep = 1.2f;
    for (int i = -30; i <= 30; i++) {
        float bz = i * bantalanStep + fmod(pohonOffset, bantalanStep);
        glPushMatrix();
        glTranslatef(0.0f, -0.48f, bz);
        drawBox(3.00f, 0.10f, 0.28f);
        glPopMatrix();
    }

    // Rel kiri & kanan (besi abu gelap)
    glColor3f(0.25f, 0.25f, 0.28f);
    for (int r = -1; r <= 1; r += 2) {
        glPushMatrix();
        glTranslatef(r * 0.76f, -0.44f, pohonOffset);
        drawBox(0.12f, 0.08f, 150.0f);
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // POHON-POHON (kiri gerbong, sisi -X)
    // ----------------------------------------------------------------
    float pohonDataKiri[][2] = {
        {-7.0f, -44.0f}, {-9.5f, -32.0f}, {-8.2f, -18.0f}, {-7.8f,  -6.0f},
        {-9.0f,   6.0f}, {-8.5f,  18.0f}, {-7.5f,  30.0f}, {-9.2f,  42.0f}
    };
    int nPohonKiri = 8;
    for (int i = 0; i < nPohonKiri; i++) {
        float pz = pohonDataKiri[i][1] + pohonOffset;
        // Loop: pohon muncul kembali dari depan setelah melewati belakang
        pz = fmod(pz + 100.0f, 100.0f) - 60.0f;
        glPushMatrix();
        glTranslatef(pohonDataKiri[i][0], 0.0f, pz);
        drawPohon();
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // POHON-POHON (kanan gerbong, sisi +X)
    // ----------------------------------------------------------------
    float pohonDataKanan[][2] = {
        { 7.5f, -40.0f}, { 9.0f, -26.0f}, { 8.0f, -12.0f}, { 7.2f,  2.0f},
        { 9.5f,  14.0f}, { 8.3f,  26.0f}, { 7.8f,  38.0f}, { 9.1f, -52.0f}
    };
    int nPohonKanan = 8;
    for (int i = 0; i < nPohonKanan; i++) {
        float pz = pohonDataKanan[i][1] + pohonOffset;
        pz = fmod(pz + 100.0f, 100.0f) - 60.0f;
        glPushMatrix();
        glTranslatef(pohonDataKanan[i][0], 0.0f, pz);
        drawPohon();
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // TIANG LISTRIK / SINYAL (kiri gerbong)
    // ----------------------------------------------------------------
    float tiangData[] = { -50.0f, -30.0f, -10.0f, 10.0f, 30.0f };
    for (int i = 0; i < 5; i++) {
        float tz = tiangData[i] + pohonOffset;
        tz = fmod(tz + 100.0f, 120.0f) - 70.0f;

        // Tiang utama (vertikal)
        glColor3f(0.40f, 0.40f, 0.44f);
        glPushMatrix();
        glTranslatef(-5.5f, 2.0f, tz);
        drawBox(0.20f, 4.00f, 0.20f);
        glPopMatrix();

        // Palang horizontal
        glPushMatrix();
        glTranslatef(-4.5f, 4.0f, tz);
        drawBox(2.00f, 0.15f, 0.15f);
        glPopMatrix();

        // Isolator (bola kecil kuning)
        glColor3f(0.90f, 0.85f, 0.20f);
        glPushMatrix();
        glTranslatef(-3.6f, 4.1f, tz);
        glutSolidSphere(0.12f, 6, 4);
        glPopMatrix();
    }

    // ----------------------------------------------------------------
    // GUNUNG / BUKIT DI KEJAUHAN
    // ----------------------------------------------------------------
    glColor3f(0.38f, 0.55f, 0.35f);
    float gunungX[] = { -30.0f, -15.0f, 15.0f, 30.0f, -45.0f, 45.0f };
    float gunungH[] = { 10.0f,   8.0f,  9.0f,  7.0f,  11.0f,  8.5f };
    float gunungR[] = { 8.0f,   6.0f,  7.0f,  5.5f,   9.0f,  7.0f };
    int nGunung = 6;
    for (int i = 0; i < nGunung; i++) {
        glPushMatrix();
        glTranslatef(gunungX[i], -0.5f, -70.0f);
        // Kerucut besar untuk gunung
        glutSolidCone(gunungR[i], gunungH[i], 10, 5);
        glPopMatrix();
    }
}

// =============================================================================
//  SETUP PENCAHAYAAN (dipanggil setiap frame)
// =============================================================================
void setupLighting()
{
    if (lampuHidup) {
        glEnable(GL_LIGHTING);

        // Ambien global (cahaya basal)
        GLfloat ambGlobal[] = { 0.35f, 0.33f, 0.30f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGlobal);

        // ---- LIGHT0 : Lampu atap interior (titik cahaya, hangat) ----
        glEnable(GL_LIGHT0);
        GLfloat pos0[] = { 0.0f, 2.8f, 0.0f, 1.0f };   // w=1 => positional
        GLfloat dif0[] = { 0.90f, 0.88f, 0.75f, 1.0f };
        GLfloat spec0[] = { 0.50f, 0.50f, 0.45f, 1.0f };
        GLfloat att = 0.015f;
        glLightfv(GL_LIGHT0, GL_POSITION, pos0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att);

        // ---- LIGHT1 : Cahaya luar (matahari dari jendela, biru langit) ----
        glEnable(GL_LIGHT1);
        GLfloat pos1[] = { 8.0f, 4.0f, 0.0f, 0.0f };   // w=0 => directional
        GLfloat dif1[] = { 0.45f, 0.55f, 0.75f, 1.0f };
        GLfloat amb1[] = { 0.10f, 0.12f, 0.18f, 1.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, pos1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);

    }
    else {
        // Lampu mati: hanya ambien redup
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        GLfloat ambGelap[] = { 0.15f, 0.12f, 0.10f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGelap);
    }

    // Material default (override per objek oleh glColor via GL_COLOR_MATERIAL)
    GLfloat matSpec[] = { 0.30f, 0.30f, 0.28f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0f);
}

// =============================================================================
//  HUD — Informasi kontrol di pojok layar
// =============================================================================
void drawHUD()
{
    // Simpan state lighting agar tidak berpengaruh ke teks
    bool wasLit = (glIsEnabled(GL_LIGHTING) == GL_TRUE);
    glDisable(GL_LIGHTING);

    // Masuk mode proyeksi ortografis 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // --- Panel latar (semi-transparan hitam) ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.50f);
    glBegin(GL_QUADS);
    glVertex2f(0, WIN_H);
    glVertex2f(390, WIN_H);
    glVertex2f(390, WIN_H - 110);
    glVertex2f(0, WIN_H - 110);
    glEnd();
    glDisable(GL_BLEND);

    // --- Teks kontrol ---
    glColor3f(1.0f, 1.0f, 0.85f);
    int y = WIN_H - 22;
    auto put = [&](const char* s) {
        drawText2D(10, (float)y, s, GLUT_BITMAP_HELVETICA_12);
        y -= 16;
        };

    put("=== SIMULASI KERETA API EKSEKUTIF ===");
    put("W/S: Maju-Mundur  |  A/D: Geser kiri-kanan");
    put("J/L: Putar kamera |  I/K: Lihat atas-bawah");
    put("Mouse drag: Putar bebas  |  F: Toggle lampu");
    put("ESC: Keluar");

    char buf[80];
    sprintf(buf, "Lampu: %s  |  Pos: (%.1f, %.1f)  Yaw: %.0f deg",
        lampuHidup ? "ON " : "OFF", camX, camZ, yaw);
    glColor3f(0.8f, 1.0f, 0.8f);
    drawText2D(10, (float)y, buf, GLUT_BITMAP_HELVETICA_12);

    // Kembalikan ke mode 3D
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    if (wasLit) glEnable(GL_LIGHTING);
}

// =============================================================================
//  FUNGSI DISPLAY — dipanggil setiap frame oleh GLUT
// =============================================================================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ---- Hitung vektor arah pandang kamera ----
    float cosYaw = cos(toRad(yaw));
    float sinYaw = sin(toRad(yaw));
    float cosPitch = cos(toRad(pitch));
    float sinPitch = sin(toRad(pitch));

    float lookX = camX + cosYaw * cosPitch;
    float lookY = camY + sinPitch;
    float lookZ = camZ + sinYaw * cosPitch;

    // ---- Efek getaran kamera (simulasi kereta bergerak di rel) ----
    float vibrasiY = sinf(waktu * 7.0f) * 0.012f;  // naik-turun kecil
    float vibrasiX = sinf(waktu * 4.5f) * 0.006f;  // kiri-kanan sangat kecil

    // ---- Pasang kamera ----
    gluLookAt(
        camX + vibrasiX, camY + vibrasiY, camZ,   // posisi eye
        lookX, lookY, lookZ,           // titik fokus
        0.0f, 1.0f, 0.0f            // up vector
    );

    // ---- Pencahayaan (posisi lampu dalam world-space) ----
    setupLighting();

    // ---- Gambar scene ----
    drawGerbong();
    drawSemuaKursi();
    drawPemandangan();

    // ---- HUD (setelah semua 3D selesai) ----
    drawHUD();

    glutSwapBuffers();
}

// =============================================================================
//  FUNGSI RESHAPE — dipanggil saat ukuran jendela berubah
// =============================================================================
void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        65.0,             // field of view vertikal
        (double)w / h,    // aspect ratio
        0.05,             // near clip
        250.0             // far clip
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// =============================================================================
//  FUNGSI TIMER — pembaruan animasi ~60 FPS
// =============================================================================
void timerCallback(int value)
{
    // Kecepatan animasi pohon (satuan unit/frame)
    const float KECEPATAN_KERETA = 0.12f;

    // Gerakkan offset pohon ke arah positif Z (ilusi kereta maju ke -Z)
    pohonOffset += KECEPATAN_KERETA;
    if (pohonOffset > 10000.0f) pohonOffset = 0.0f;   // reset agar tidak overflow

    waktu += 0.04f;   // increment waktu untuk efek sin (vibrasi)

    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);   // jadwal frame berikutnya
}

// =============================================================================
//  COLLISION DETECTION — cek apakah posisi (x,z) berada di dalam kursi
// =============================================================================
bool isInsideKursi(float x, float z)
{
    const float X_KIRI = -1.15f;
    const float X_KANAN = 1.15f;
    float barisZ[] = { -8.5f,-6.5f,-4.5f,-2.5f,-0.5f,1.5f,3.5f,5.5f,7.5f };
    const int jumlah = 9;
    const float HALF_W = 0.42f;  // setengah lebar collision box
    const float HALF_D = 0.45f;  // setengah kedalaman collision box

    for (int i = 0; i < jumlah; i++) {
        if (x >= X_KIRI - HALF_W && x <= X_KIRI + HALF_W &&
            z >= barisZ[i] - HALF_D && z <= barisZ[i] + HALF_D) return true;
        if (x >= X_KANAN - HALF_W && x <= X_KANAN + HALF_W &&
            z >= barisZ[i] - HALF_D && z <= barisZ[i] + HALF_D) return true;
    }
    return false;
}

// =============================================================================
//  FUNGSI KEYBOARD — tombol karakter biasa
// =============================================================================
void keyboardDown(unsigned char key, int /*x*/, int /*y*/)
{
    const float SPEED = 0.12f;

    // Arah gerak berdasarkan yaw saat ini
    float dx = cos(toRad(yaw));
    float dz = sin(toRad(yaw));

    float newX = camX, newZ = camZ;

    switch (key)
    {
        // ---- Gerak translasi ----
    case 'w': case 'W':   newX += dx * SPEED;  newZ += dz * SPEED;  break;
    case 's': case 'S':   newX -= dx * SPEED;  newZ -= dz * SPEED;  break;
    case 'a': case 'A':   newX += dz * SPEED;  newZ -= dx * SPEED;  break;
    case 'd': case 'D':   newX -= dz * SPEED;  newZ += dx * SPEED;  break;

        // ---- Rotasi keyboard (alternatif mouse) ----
    case 'j': case 'J':   yaw -= 2.5f;  break;
    case 'l': case 'L':   yaw += 2.5f;  break;
    case 'i': case 'I':   pitch += 2.0f;  break;
    case 'k': case 'K':   pitch -= 2.0f;  break;

        // ---- Toggle lampu ----
    case 'f': case 'F':   lampuHidup = !lampuHidup;  break;

        // ---- Keluar ----
    case 27:   exit(0);  break;

    default: break;
    }

    // Batasi sudut pitch agar kamera tetap stabil
    if (pitch > 80.0f) pitch = 80.0f;
    if (pitch < -80.0f) pitch = -80.0f;

    // Batas gerbong + collision kursi
    // Cek X dan Z secara terpisah agar bisa "sliding" di pinggir kursi
    float tryX = newX, tryZ = newZ;

    // Coba gerak X dulu
    if (tryX >= BATAS_X_MIN && tryX <= BATAS_X_MAX && !isInsideKursi(tryX, camZ))
        camX = tryX;

    // Coba gerak Z dulu
    if (tryZ >= BATAS_Z_MIN && tryZ <= BATAS_Z_MAX && !isInsideKursi(camX, tryZ))
        camZ = tryZ;

    glutPostRedisplay();
}

// =============================================================================
//  FUNGSI MOUSE — tombol mouse
// =============================================================================
void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        mouseTekan = (state == GLUT_DOWN);
        mousePrevX = x;
        mousePrevY = y;
    }
}

// =============================================================================
//  FUNGSI MOUSE — gerakan saat tombol ditekan
// =============================================================================
void mouseMotion(int x, int y)
{
    if (!mouseTekan) return;

    int dx = x - mousePrevX;
    int dy = y - mousePrevY;

    yaw += dx * SENSITIVITAS;
    pitch -= dy * SENSITIVITAS;

    if (pitch > 80.0f) pitch = 80.0f;
    if (pitch < -80.0f) pitch = -80.0f;

    mousePrevX = x;
    mousePrevY = y;

    glutPostRedisplay();
}

// =============================================================================
//  INISIALISASI OpenGL
// =============================================================================
void initGL()
{
    // Warna latar (biru langit)
    glClearColor(0.52f, 0.80f, 0.98f, 1.0f);

    glEnable(GL_DEPTH_TEST);        // depth buffer
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_COLOR_MATERIAL);    // glColor mempengaruhi material
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(GL_SMOOTH);        // shading Gouraud

    glEnable(GL_NORMALIZE);         // normalkan normal setelah scaling

    // Sembunyikan kursor di dalam jendela
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}

// =============================================================================
//  FUNGSI MAIN
// =============================================================================
int main(int argc, char* argv[])
{
    printf("==================================================\n");
    printf("  SIMULASI INTERIOR KERETA API EKSEKUTIF 3D\n");
    printf("  Grafika Komputer - OpenGL + freeglut\n");
    printf("==================================================\n");
    printf("  KONTROL:\n");
    printf("  W/S        : Maju / Mundur\n");
    printf("  A/D        : Geser Kiri / Kanan\n");
    printf("  J/L        : Putar kamera kiri / kanan\n");
    printf("  I/K        : Lihat ke atas / bawah\n");
    printf("  Klik+drag  : Putar kamera bebas (mouse)\n");
    printf("  F          : Toggle lampu interior ON/OFF\n");
    printf("  ESC        : Keluar\n");
    printf("==================================================\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(80, 60);
    glutCreateWindow("Simulasi Interior Kereta Api Eksekutif - Grafika Komputer");

    initGL();

    // Registrasi callback GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, timerCallback, 0);

    glutMainLoop();
    return 0;
}

