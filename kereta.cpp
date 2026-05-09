/* ===========================================================================
 * PROJECT     : VISUALISASI 3D INTERIOR KERETA API EKSEKUTIF
 * KELOMPOK    : 62 Grafika Komputer
 * FILE        : kereta.cpp
 * DESKRIPSI   : Program uji scene gerbong yang menggabungkan model interior dengan pemandangan luar, animasi lingkungan,
 *               dan pencahayaan dasar. File ini digunakan untuk memvalidasi integrasi visual gerbong dan lingkungan lintasan.
 * AUTHOR      : Aisyah Nitiarahma
 * ===========================================================================
 * STRUKTUR TIM PENGEMBANG:
 * - Project Manager & Integrator : Diyani Rahayu Nur'aeni
 * - Core Programmer & Camera     : Galang Maulid Nugraha
 * - Interior Modeler             : M. Fauz Haunan Zaky
 * - Exterior & Animation         : Aisyah Nitiarahma
 * - Lighting & Material          : Agniya Azzahra
 * ===========================================================================
 * Dependensi  : OpenGL, freeGLUT, C++ Standard Library
 * Kompilasi   : g++ kereta.cpp -o output.exe -lfreeglut -lglu32 -lopengl32
 * =========================================================================== */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>

/* ---------------------------------------------------------------------------
   drawBox()
   Menggambar balok dari dua titik sudut sebagai utilitas dasar penyusun scene.
   --------------------------------------------------------------------------- */
void drawBox(float x0, float y0, float z0,
             float x1, float y1, float z1)
{
    glBegin(GL_QUADS);
    /* Bawah */
    glNormal3f(0, -1, 0);
    glVertex3f(x0, y0, z0);
    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y0, z1);
    glVertex3f(x0, y0, z1);
    /* Atas */
    glNormal3f(0, 1, 0);
    glVertex3f(x0, y1, z0);
    glVertex3f(x0, y1, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z0);
    /* Depan (Z+) */
    glNormal3f(0, 0, 1);
    glVertex3f(x0, y0, z1);
    glVertex3f(x1, y0, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x0, y1, z1);
    /* Belakang (Z-) */
    glNormal3f(0, 0, -1);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y1, z0);
    glVertex3f(x1, y1, z0);
    glVertex3f(x1, y0, z0);
    /* Kiri (X-) */
    glNormal3f(-1, 0, 0);
    glVertex3f(x0, y0, z0);
    glVertex3f(x0, y0, z1);
    glVertex3f(x0, y1, z1);
    glVertex3f(x0, y1, z0);
    /* Kanan (X+) */
    glNormal3f(1, 0, 0);
    glVertex3f(x1, y0, z0);
    glVertex3f(x1, y1, z0);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y0, z1);
    glEnd();
}

/* ---------------------------------------------------------------------------
   drawKursi()
   Menggambar satu unit kursi penumpang eksekutif pada sistem koordinat lokal.
   Fungsi ini dipanggil berulang untuk membentuk susunan kursi dalam gerbong.
   --------------------------------------------------------------------------- */
void drawKursi()
{
    /* KAKI KURSI
     * Material: baja abu-abu gelap
     * Tinggi kaki: 0.0 - 0.40 m dari lantai. */
    glColor3f(0.30f, 0.30f, 0.33f);
    drawBox(-0.22f, 0.00f, -0.25f, -0.17f, 0.40f, -0.20f); /* kaki depan-kiri */
    drawBox(0.17f, 0.00f, -0.25f, 0.22f, 0.40f, -0.20f);   /* kaki depan-kanan */
    drawBox(-0.22f, 0.00f, 0.20f, -0.17f, 0.40f, 0.25f);   /* kaki belakang-kiri */
    drawBox(0.17f, 0.00f, 0.20f, 0.22f, 0.40f, 0.25f);     /* kaki belakang-kanan */

    /* DUDUKAN KURSI
     * Rangka dudukan berada pada Y 0.40 - 0.47 m dengan warna biru tua.
     * Busa dudukan berada pada Y 0.47 - 0.53 m dengan warna biru yang lebih terang. */
    glColor3f(0.10f, 0.18f, 0.48f);
    drawBox(-0.25f, 0.40f, -0.25f, 0.25f, 0.47f, 0.25f);

    glColor3f(0.18f, 0.28f, 0.60f);
    drawBox(-0.23f, 0.47f, -0.23f, 0.23f, 0.53f, 0.23f);

    /* SANDARAN PUNGGUNG
     * Rangka sandaran berada pada rentang Z 0.20 - 0.28 m dan Y 0.47 - 1.00 m.
     * Busa sandaran lebih tipis, sedangkan headrest memakai warna krem. */
    glColor3f(0.10f, 0.18f, 0.48f);
    drawBox(-0.25f, 0.47f, 0.20f, 0.25f, 1.00f, 0.28f); /* rangka */

    glColor3f(0.18f, 0.28f, 0.60f);
    drawBox(-0.22f, 0.49f, 0.20f, 0.22f, 0.97f, 0.24f); /* busa */

    glColor3f(0.88f, 0.85f, 0.76f);
    drawBox(-0.20f, 0.88f, 0.20f, 0.20f, 1.00f, 0.23f); /* headrest */

    /* SANDARAN TANGAN
     * Posisi kiri berada pada X -0.33 hingga -0.25 m dan kanan pada X 0.25 hingga 0.33 m.
     * Tinggi armrest berada pada Y 0.50 - 0.58 m. */
    glColor3f(0.22f, 0.22f, 0.25f);
    drawBox(-0.33f, 0.50f, -0.18f, -0.25f, 0.58f, 0.22f); /* armrest kiri */
    drawBox(0.25f, 0.50f, -0.18f, 0.33f, 0.58f, 0.22f);   /* armrest kanan */
}

/* ---------------------------------------------------------------------------
   drawGerbong()
   Membangun shell gerbong lengkap dengan komponen interior utama dan elemen
   struktural yang diperlukan untuk pengujian visual scene.
   --------------------------------------------------------------------------- */
void drawGerbong()
{
    /* DIMENSI UTAMA GERBONG */
    const float WALL_L = -1.5f;
    const float WALL_R = 1.5f;
    const float FLOOR_Y = 0.0f;
    const float CEIL_Y = 2.5f;
    const float FRONT_Z = -10.0f;
    const float BACK_Z = 10.0f;
    const float TW = 0.06f;

    /* PARAMETER BUKAAN JENDELA */
    const float JY_BOT = 0.80f;
    const float JY_TOP = 1.60f;
    const float JW = 1.10f;
    const int NJ = 6;

    /* Lebar pilar antarjendela dihitung dari sisa panjang gerbong. */
    float total_jendela = NJ * JW;
    float sisa = (BACK_Z - FRONT_Z) - total_jendela;
    float pilar = sisa / (NJ + 1.0f);

    /* LANTAI */
    glColor3f(0.45f, 0.28f, 0.14f);
    drawBox(WALL_L, FLOOR_Y - TW * 2, FRONT_Z,
            WALL_R, FLOOR_Y, BACK_Z);

    /* Bidang kiri dan kanan dibuat sedikit lebih terang untuk membedakan area kursi. */
    glColor3f(0.50f, 0.32f, 0.16f);
    drawBox(WALL_L + 0.01f, FLOOR_Y + 0.001f, FRONT_Z,
            -0.50f, FLOOR_Y + 0.003f, BACK_Z);
    drawBox(0.50f, FLOOR_Y + 0.001f, FRONT_Z,
            WALL_R - 0.01f, FLOOR_Y + 0.003f, BACK_Z);

    /* Strip karpet lorong dibuat sedikit lebih tinggi agar tidak z-fighting. */
    glColor3f(0.50f, 0.08f, 0.08f);
    drawBox(-0.50f, FLOOR_Y + 0.002f, FRONT_Z,
            0.50f, FLOOR_Y + 0.006f, BACK_Z);

    /* ATAP / PLAFON */
    glColor3f(0.93f, 0.91f, 0.86f);
    drawBox(WALL_L, CEIL_Y, FRONT_Z,
            WALL_R, CEIL_Y + TW, BACK_Z);

    /* Lampu LED strip diposisikan di tengah plafon. */
    glColor3f(1.00f, 0.96f, 0.72f);
    drawBox(-0.06f, CEIL_Y - 0.01f, FRONT_Z + 0.5f,
            0.06f, CEIL_Y, BACK_Z - 0.5f);

    /* DINDING DEPAN (Z = FRONT_Z) */
    glColor3f(0.72f, 0.69f, 0.63f);
    drawBox(WALL_L, FLOOR_Y, FRONT_Z - TW,
            WALL_R, CEIL_Y, FRONT_Z);

    /* DINDING BELAKANG (Z = BACK_Z) */
    glColor3f(0.72f, 0.69f, 0.63f);
    drawBox(WALL_L, FLOOR_Y, BACK_Z,
            WALL_R, CEIL_Y, BACK_Z + TW);

    /* DINDING SAMPING DAN BUKAAN JENDELA
     * Panel samping dibagi menjadi panel bawah, panel atas, dan pilar antarjendela.
     * Kaca jendela digambar terpisah agar alpha blending dapat diterapkan dengan rapi. */
    float wallColor[3] = {0.76f, 0.73f, 0.66f};

    for (int sisi = 0; sisi < 2; sisi++)
    {
        /* xs adalah muka dinding, sedangkan xe adalah sisi luar dinding. */
        float xs = (sisi == 0) ? WALL_L : WALL_R;
        float xe = (sisi == 0) ? WALL_L - TW : WALL_R + TW;

        glColor3fv(wallColor);

        /* [A] Panel bawah: area dari lantai hingga bawah jendela */
        drawBox(xs, FLOOR_Y, FRONT_Z, xe, JY_BOT, BACK_Z);

        /* [B] Panel atas: area dari atas jendela hingga plafon */
        drawBox(xs, JY_TOP, FRONT_Z, xe, CEIL_Y, BACK_Z);

        /* [C] Pilar antarjendela */
        float zc = FRONT_Z;
        for (int i = 0; i <= NJ; i++)
        {
            float z0 = zc;
            float z1 = zc + pilar;
            if (i == NJ)
                z1 = BACK_Z;
            drawBox(xs, JY_BOT, z0, xe, JY_TOP, z1);
            zc = z1 + JW;
        }

        /* KACA JENDELA TRANSPARAN */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        float zj = FRONT_Z + pilar;
        for (int i = 0; i < NJ; i++)
        {
            glColor4f(0.70f, 0.88f, 1.00f, 0.06f);
            drawBox(xs, JY_BOT, zj, xe, JY_TOP, zj + JW);

            /* Kilap tipis di bagian atas membantu membaca permukaan kaca. */
            glColor4f(1.0f, 1.0f, 1.0f, 0.04f);
            drawBox(xs, JY_TOP - 0.12f, zj + 0.05f,
                    xe, JY_TOP - 0.02f, zj + JW - 0.05f);

            zj += JW + pilar;
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);

        /* RAK BAGASI ATAS
         * Posisi rak berada pada Y 2.0 - 2.3 dan menempel pada dinding samping. */
        glColor3f(0.65f, 0.62f, 0.57f);
        float rx0 = (sisi == 0) ? WALL_L : WALL_R - 0.30f;
        float rx1 = (sisi == 0) ? WALL_L + 0.30f : WALL_R;
        drawBox(rx0, 2.00f, FRONT_Z + 0.3f,
                rx1, 2.30f, BACK_Z - 0.3f);

        /* Penyangga rak diulang berkala sepanjang gerbong. */
        glColor3f(0.55f, 0.52f, 0.48f);
        for (float sz = FRONT_Z + 1.0f; sz < BACK_Z; sz += 2.0f)
            drawBox(rx0, 1.60f, sz, rx1, 2.00f, sz + 0.04f);
    }

    /* REL PEGANGAN TANGAN DI PLAFON */
    glColor3f(0.55f, 0.55f, 0.60f);
    drawBox(-0.52f, CEIL_Y - 0.14f, FRONT_Z + 0.4f,
            -0.46f, CEIL_Y - 0.10f, BACK_Z - 0.4f);
    drawBox(0.46f, CEIL_Y - 0.14f, FRONT_Z + 0.4f,
            0.52f, CEIL_Y - 0.10f, BACK_Z - 0.4f);
}

/* ---------------------------------------------------------------------------
   drawSemuaKursi()
   Menyusun kursi dengan konfigurasi 2-2 di sepanjang sumbu Z gerbong.
   Total susunan terdiri atas 10 baris atau 40 kursi.
   --------------------------------------------------------------------------- */
void drawSemuaKursi()
{
    /* Posisi X dipilih agar kursi tidak menabrak dinding dan tetap menyisakan lorong tengah. */
    float posX[4] = {-1.05f, -0.65f, +0.65f, +1.05f};

    float startZ = -8.50f;
    float jarakZ = 1.70f;
    int nBaris = 10;

    for (int baris = 0; baris < nBaris; baris++)
    {
        float pz = startZ + (float)baris * jarakZ;

        for (int kolom = 0; kolom < 4; kolom++)
        {
            glPushMatrix();
            glTranslatef(posX[kolom], 0.0f, pz);
            drawKursi();
            glPopMatrix();
        }
    }
}

/* ---------------------------------------------------------------------------
   pemandangan_pohon()
   Menggambar satu pohon 3D sebagai elemen berulang pada lapisan pemandangan luar.
   --------------------------------------------------------------------------- */
static void pemandangan_pohon(float px, float pz, float skala)
{
    float bx = 0.08f * skala;
    float th = 1.60f * skala;
    float cr = 0.55f * skala;

    /* Patch rumput tipis mencegah pohon tampak melayang di atas tanah. */
    float gr = cr * 1.2f;
    glColor3f(0.28f, 0.58f, 0.16f);
    glBegin(GL_QUADS);
    glVertex3f(px - gr, 0.01f, pz - gr);
    glVertex3f(px + gr, 0.01f, pz - gr);
    glVertex3f(px + gr, 0.01f, pz + gr);
    glVertex3f(px - gr, 0.01f, pz + gr);
    glEnd();

    /* Semak dibuat sebagai billboard sederhana agar tetap terbaca dari jendela. */
    float sh = 0.35f * skala;
    glColor3f(0.22f, 0.50f, 0.12f);
    glBegin(GL_QUADS);
    glVertex3f(px, 0.0f, pz - cr * 0.6f);
    glVertex3f(px, 0.0f, pz + cr * 0.6f);
    glVertex3f(px, sh, pz + cr * 0.6f);
    glVertex3f(px, sh, pz - cr * 0.6f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(px - cr * 0.6f, 0.0f, pz);
    glVertex3f(px + cr * 0.6f, 0.0f, pz);
    glVertex3f(px + cr * 0.6f, sh, pz);
    glVertex3f(px - cr * 0.6f, sh, pz);
    glEnd();

    /* Batang pohon dibentuk dari empat sisi vertikal. */
    glColor3f(0.38f, 0.22f, 0.08f);
    glBegin(GL_QUADS);
    glVertex3f(px - bx, 0.0f, pz + bx); glVertex3f(px + bx, 0.0f, pz + bx);
    glVertex3f(px + bx, th, pz + bx); glVertex3f(px - bx, th, pz + bx);
    glVertex3f(px - bx, 0.0f, pz - bx); glVertex3f(px - bx, th, pz - bx);
    glVertex3f(px + bx, th, pz - bx); glVertex3f(px + bx, 0.0f, pz - bx);
    glVertex3f(px - bx, 0.0f, pz - bx); glVertex3f(px - bx, 0.0f, pz + bx);
    glVertex3f(px - bx, th, pz + bx); glVertex3f(px - bx, th, pz - bx);
    glVertex3f(px + bx, 0.0f, pz + bx); glVertex3f(px + bx, 0.0f, pz - bx);
    glVertex3f(px + bx, th, pz - bx); glVertex3f(px + bx, th, pz + bx);
    glEnd();

    /* Mahkota dibentuk dari tiga lapisan segitiga billboard agar tetap terbaca dari jendela. */
    float cy = th * 0.55f;
    glColor3f(0.18f, 0.52f, 0.16f);
    glBegin(GL_TRIANGLES);
    glVertex3f(px, cy, pz - cr);
    glVertex3f(px, cy, pz + cr);
    glVertex3f(px, cy + 0.9f * skala, pz);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex3f(px - cr, cy, pz);
    glVertex3f(px + cr, cy, pz);
    glVertex3f(px, cy + 0.9f * skala, pz);
    glEnd();

    glColor3f(0.13f, 0.42f, 0.11f);
    glBegin(GL_TRIANGLES);
    glVertex3f(px, cy + 0.45f * skala, pz - cr * 0.7f);
    glVertex3f(px, cy + 0.45f * skala, pz + cr * 0.7f);
    glVertex3f(px, cy + 1.3f * skala, pz);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex3f(px - cr * 0.7f, cy + 0.45f * skala, pz);
    glVertex3f(px + cr * 0.7f, cy + 0.45f * skala, pz);
    glVertex3f(px, cy + 1.3f * skala, pz);
    glEnd();
}

/* ---------------------------------------------------------------------------
   pemandangan_tiang()
   Menggambar satu tiang listrik 3D sebagai elemen lintasan yang bergerak relatif terhadap kereta.
   --------------------------------------------------------------------------- */
static void pemandangan_tiang(float px, float pz, int sisi)
{
    glColor3f(0.30f, 0.28f, 0.24f);
    glBegin(GL_QUADS);
    glVertex3f(px - 0.05f, 0.0f, pz - 0.05f); glVertex3f(px + 0.05f, 0.0f, pz - 0.05f);
    glVertex3f(px + 0.05f, 3.2f, pz - 0.05f); glVertex3f(px - 0.05f, 3.2f, pz - 0.05f);
    glVertex3f(px - 0.05f, 0.0f, pz + 0.05f); glVertex3f(px - 0.05f, 3.2f, pz + 0.05f);
    glVertex3f(px + 0.05f, 3.2f, pz + 0.05f); glVertex3f(px + 0.05f, 0.0f, pz + 0.05f);
    glEnd();

    float arah = (sisi == 0) ? -0.5f : 0.5f;
    glColor3f(0.26f, 0.24f, 0.20f);
    glBegin(GL_QUADS);
    glVertex3f(px + arah * 0.0f, 2.9f, pz - 0.04f);
    glVertex3f(px + arah * 1.0f, 2.9f, pz - 0.04f);
    glVertex3f(px + arah * 1.0f, 2.98f, pz - 0.04f);
    glVertex3f(px + arah * 0.0f, 2.98f, pz - 0.04f);
    glVertex3f(px + arah * 0.0f, 2.9f, pz + 0.04f);
    glVertex3f(px + arah * 0.0f, 2.98f, pz + 0.04f);
    glVertex3f(px + arah * 1.0f, 2.98f, pz + 0.04f);
    glVertex3f(px + arah * 1.0f, 2.9f, pz + 0.04f);
    glEnd();

    glColor3f(0.15f, 0.14f, 0.12f);
    glBegin(GL_LINES);
    glVertex3f(px + arah * 0.95f, 2.93f, pz);
    glVertex3f(px + arah * 0.95f, 0.0f, pz);
    glEnd();
}

/* ---------------------------------------------------------------------------
   pemandangan_latar()
   Menggambar lapisan latar statis seperti langit, gunung, matahari, dan bidang tanah.
   --------------------------------------------------------------------------- */
static void pemandangan_latar()
{
    /* Langit dibentuk dari bidang besar yang mengelilingi area luar scene. */
    glBegin(GL_QUADS);
    glColor3f(0.22f, 0.50f, 0.88f);
    glVertex3f(-80.0f, 15.0f, -52.0f); glVertex3f(80.0f, 15.0f, -52.0f);
    glColor3f(0.65f, 0.84f, 0.97f);
    glVertex3f(80.0f, 0.3f, -52.0f); glVertex3f(-80.0f, 0.3f, -52.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.22f, 0.50f, 0.88f);
    glVertex3f(-80.0f, 15.0f, -52.0f); glVertex3f(-80.0f, 15.0f, 20.0f);
    glColor3f(0.65f, 0.84f, 0.97f);
    glVertex3f(-80.0f, 0.3f, 20.0f); glVertex3f(-80.0f, 0.3f, -52.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.22f, 0.50f, 0.88f);
    glVertex3f(80.0f, 15.0f, 20.0f); glVertex3f(80.0f, 15.0f, -52.0f);
    glColor3f(0.65f, 0.84f, 0.97f);
    glVertex3f(80.0f, 0.3f, -52.0f); glVertex3f(80.0f, 0.3f, 20.0f);
    glEnd();

    glColor3f(1.00f, 0.93f, 0.42f);
    glBegin(GL_QUADS);
    glVertex3f(14.0f, 7.0f, -51.0f); glVertex3f(16.0f, 7.0f, -51.0f);
    glVertex3f(16.0f, 9.0f, -51.0f); glVertex3f(14.0f, 9.0f, -51.0f);
    glEnd();

    glColor3f(1.00f, 0.96f, 0.68f);
    glBegin(GL_QUADS);
    glVertex3f(13.0f, 6.2f, -51.5f); glVertex3f(17.0f, 6.2f, -51.5f);
    glVertex3f(17.0f, 9.8f, -51.5f); glVertex3f(13.0f, 9.8f, -51.5f);
    glEnd();

    glColor3f(0.42f, 0.50f, 0.62f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-55.0f, 0.3f, -50.0f); glVertex3f(-20.0f, 0.3f, -50.0f);
    glVertex3f(-38.0f, 9.5f, -50.0f);
    glEnd();

    glColor3f(0.38f, 0.46f, 0.58f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-15.0f, 0.3f, -50.0f); glVertex3f(20.0f, 0.3f, -50.0f);
    glVertex3f(2.0f, 11.0f, -50.0f);
    glEnd();

    glColor3f(0.44f, 0.52f, 0.64f);
    glBegin(GL_TRIANGLES);
    glVertex3f(22.0f, 0.3f, -50.0f); glVertex3f(55.0f, 0.3f, -50.0f);
    glVertex3f(38.0f, 8.0f, -50.0f);
    glEnd();

    glColor3f(0.20f, 0.38f, 0.18f);
    glBegin(GL_TRIANGLES);
    glVertex3f(-80.0f, 0.3f, -28.0f); glVertex3f(-10.0f, 0.3f, -28.0f);
    glVertex3f(-46.0f, 6.5f, -28.0f);
    glEnd();

    glColor3f(0.22f, 0.40f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex3f(10.0f, 0.3f, -28.0f); glVertex3f(80.0f, 0.3f, -28.0f);
    glVertex3f(46.0f, 5.5f, -28.0f);
    glEnd();

    /* Bidang sawah diperluas penuh agar tidak terlihat terputus dari jendela. */
    glColor3f(0.32f, 0.56f, 0.18f);
    glBegin(GL_QUADS);
    glVertex3f(-80.0f, 0.0f, -52.0f); glVertex3f(80.0f, 0.0f, -52.0f);
    glVertex3f(80.0f, 0.0f, 20.0f); glVertex3f(-80.0f, 0.0f, 20.0f);
    glEnd();

    glColor3f(0.38f, 0.64f, 0.20f);
    glBegin(GL_QUADS);
    glVertex3f(-12.0f, 0.01f, -52.0f); glVertex3f(-1.6f, 0.01f, -52.0f);
    glVertex3f(-1.6f, 0.01f, 20.0f); glVertex3f(-12.0f, 0.01f, 20.0f);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(1.6f, 0.01f, -52.0f); glVertex3f(12.0f, 0.01f, -52.0f);
    glVertex3f(12.0f, 0.01f, 20.0f); glVertex3f(1.6f, 0.01f, 20.0f);
    glEnd();
}

/* ---------------------------------------------------------------------------
   pemandangan_burung()
   Menggambar kawanan burung dengan kepakan sayap berbasis fungsi sinus.
   --------------------------------------------------------------------------- */
static void pemandangan_burung(float t)
{
    struct Kawanan
    {
        float bx, by, bz, speed, phase;
    } kawanan[3] = {
        {-40.0f, 5.5f, -18.0f, 4.2f, 0.0f},
        {-40.0f, 7.2f, -25.0f, 3.6f, 1.8f},
        {-40.0f, 4.8f, -12.0f, 5.0f, 3.5f},
    };

    glDisable(GL_LIGHTING);
    glColor3f(0.08f, 0.06f, 0.05f);

    for (int k = 0; k < 3; k++)
    {
        float tx = fmod(t * kawanan[k].speed + kawanan[k].phase, 95.0f) - 10.0f;
        float bx0 = kawanan[k].bx + tx * 1.8f;
        float by0 = kawanan[k].by + sin(t * 1.5f + kawanan[k].phase) * 0.3f;
        float bz0 = kawanan[k].bz;

        float offsetX[5] = {0.0f, 2.2f, -1.8f, 4.0f, -3.5f};
        float offsetY[5] = {0.0f, 0.6f, 0.4f, 1.2f, 0.8f};

        for (int j = 0; j < 5; j++)
        {
            float bx = bx0 + offsetX[j];
            float by = by0 + offsetY[j];
            float bz = bz0;

            /* Kepakan sayap memakai sinus agar gerak tampak periodik dan halus. */
            float flap = sin(t * 8.0f + j * 0.8f) * 0.4f;

            glBegin(GL_QUADS);
            glVertex3f(bx - 0.18f, by, bz);
            glVertex3f(bx + 0.18f, by, bz);
            glVertex3f(bx + 0.18f, by + 0.08f, bz);
            glVertex3f(bx - 0.18f, by + 0.08f, bz);
            glEnd();

            glBegin(GL_TRIANGLES);
            glVertex3f(bx, by + 0.04f, bz);
            glVertex3f(bx - 0.55f, by + 0.04f + flap, bz);
            glVertex3f(bx - 0.20f, by + 0.04f, bz);
            glEnd();

            glBegin(GL_TRIANGLES);
            glVertex3f(bx, by + 0.04f, bz);
            glVertex3f(bx + 0.55f, by + 0.04f + flap, bz);
            glVertex3f(bx + 0.20f, by + 0.04f, bz);
            glEnd();
        }
    }
}

/* ---------------------------------------------------------------------------
   pemandangan_sawah_bergulir()
   Menggambar petak sawah berulang yang bergeser mengikuti offset pemandangan.
   --------------------------------------------------------------------------- */
static void pemandangan_sawah_bergulir(float offset)
{
    float lebar_petak = 8.0f;
    int n_petak = 16;
    float rentang = n_petak * lebar_petak;

    for (int i = 0; i < n_petak; i++)
    {
        float pz0 = -70.0f + i * lebar_petak;
        float pz = pz0 + fmod(offset, rentang);
        if (pz > 25.0f)
            pz -= rentang;
        float pz1 = pz + lebar_petak - 0.3f;

        if (i % 2 == 0)
            glColor3f(0.30f, 0.58f, 0.15f);
        else
            glColor3f(0.40f, 0.70f, 0.20f);

        glBegin(GL_QUADS);
        glVertex3f(-2.0f, 0.02f, pz);
        glVertex3f(-14.0f, 0.02f, pz);
        glVertex3f(-14.0f, 0.02f, pz1);
        glVertex3f(-2.0f, 0.02f, pz1);
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(2.0f, 0.02f, pz);
        glVertex3f(14.0f, 0.02f, pz);
        glVertex3f(14.0f, 0.02f, pz1);
        glVertex3f(2.0f, 0.02f, pz1);
        glEnd();

        /* Pematang memberi pemisah antarpeta sawah saat bergerak. */
        glColor3f(0.62f, 0.52f, 0.30f);
        glBegin(GL_QUADS);
        glVertex3f(-2.0f, 0.025f, pz1);
        glVertex3f(-14.0f, 0.025f, pz1);
        glVertex3f(-14.0f, 0.025f, pz1 + 0.3f);
        glVertex3f(-2.0f, 0.025f, pz1 + 0.3f);
        glEnd();
        glBegin(GL_QUADS);
        glVertex3f(2.0f, 0.025f, pz1);
        glVertex3f(14.0f, 0.025f, pz1);
        glVertex3f(14.0f, 0.025f, pz1 + 0.3f);
        glVertex3f(2.0f, 0.025f, pz1 + 0.3f);
        glEnd();
    }
}

/* ---------------------------------------------------------------------------
   drawKipasAC()
   Menggambar kipas plafon dengan empat bilah yang dapat diputar untuk animasi.
   --------------------------------------------------------------------------- */
void drawKipasAC(float rotDeg)
{
    glColor3f(0.82f, 0.80f, 0.78f);
    drawBox(-0.08f, 2.42f, -0.08f, 0.08f, 2.48f, 0.08f);

    glPushMatrix();
    glTranslatef(0.0f, 2.44f, 0.0f);
    glRotatef(rotDeg, 0.0f, 1.0f, 0.0f);

    glColor3f(0.75f, 0.73f, 0.70f);

    glBegin(GL_QUADS);
    glVertex3f(0.05f, 0.0f, 0.0f);
    glVertex3f(0.05f, 0.0f, 0.55f);
    glVertex3f(-0.05f, 0.0f, 0.55f);
    glVertex3f(-0.05f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.05f, 0.0f, 0.0f);
    glVertex3f(0.05f, 0.0f, -0.55f);
    glVertex3f(-0.05f, 0.0f, -0.55f);
    glVertex3f(-0.05f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.05f);
    glVertex3f(0.55f, 0.0f, 0.05f);
    glVertex3f(0.55f, 0.0f, -0.05f);
    glVertex3f(0.0f, 0.0f, -0.05f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.05f);
    glVertex3f(-0.55f, 0.0f, 0.05f);
    glVertex3f(-0.55f, 0.0f, -0.05f);
    glVertex3f(0.0f, 0.0f, -0.05f);
    glEnd();
    glPopMatrix();
}

/* ---------------------------------------------------------------------------
   drawPemandangan()
   Menggambar seluruh pemandangan luar, termasuk awan, sawah, burung, pohon, dan tiang listrik.
   --------------------------------------------------------------------------- */
void drawPemandangan(float offset, float awanOff, float bT)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    /* LATAR digambar lebih dahulu sebagai lapisan paling belakang. */
    pemandangan_latar();

    /* Awan digerakkan dengan kecepatan berbeda untuk memberi efek paralaks ringan. */
    glColor3f(0.96f, 0.96f, 0.97f);
    float ax = awanOff;
    glBegin(GL_QUADS);
    glVertex3f(-25.0f + ax, 8.0f, -51.0f); glVertex3f(-12.0f + ax, 8.0f, -51.0f);
    glVertex3f(-12.0f + ax, 10.2f, -51.0f); glVertex3f(-25.0f + ax, 10.2f, -51.0f);
    glEnd();

    float ax2 = awanOff * 0.6f;
    glBegin(GL_QUADS);
    glVertex3f(-2.0f + ax2, 9.5f, -51.0f); glVertex3f(9.0f + ax2, 9.5f, -51.0f);
    glVertex3f(9.0f + ax2, 11.5f, -51.0f); glVertex3f(-2.0f + ax2, 11.5f, -51.0f);
    glEnd();

    float ax3 = awanOff * 0.8f;
    glBegin(GL_QUADS);
    glVertex3f(22.0f + ax3, 7.5f, -51.0f); glVertex3f(33.0f + ax3, 7.5f, -51.0f);
    glVertex3f(33.0f + ax3, 9.5f, -51.0f); glVertex3f(22.0f + ax3, 9.5f, -51.0f);
    glEnd();

    glColor3f(0.92f, 0.92f, 0.94f);
    glBegin(GL_QUADS);
    glVertex3f(-55.0f + awanOff * 1.2f, 10.5f, -51.0f);
    glVertex3f(-45.0f + awanOff * 1.2f, 10.5f, -51.0f);
    glVertex3f(-45.0f + awanOff * 1.2f, 12.0f, -51.0f);
    glVertex3f(-55.0f + awanOff * 1.2f, 12.0f, -51.0f);
    glEnd();

    pemandangan_sawah_bergulir(offset);
    pemandangan_burung(bT);

    /* Pohon disusun dalam tiga lapisan kedalaman untuk memberi efek paralaks. */
    /* Objek luar di-spawn dari jarak jauh dan di-reset setelah lewat kamera
     * agar perpindahan loop tidak tampak mendadak. */
    {
        /* Lapisan 1: pohon dekat dengan kecepatan scroll penuh. */
        const int N1 = 20;
        float spasi1 = 5.0f;
        float rentang1 = N1 * spasi1; /* 100 m, cukup panjang untuk satu siklus. */

        float posX_kiri1[N1] = {-3.2f, -4.5f, -5.0f, -3.8f, -4.2f, -3.5f, -4.8f, -3.0f, -5.3f, -4.0f,
                                -3.3f, -4.6f, -5.1f, -3.7f, -4.3f, -3.9f, -5.2f, -4.0f, -3.6f, -4.9f};
        float posX_kanan1[N1] = {3.2f, 4.5f, 5.0f, 3.8f, 4.2f, 3.5f, 4.8f, 3.0f, 5.3f, 4.0f,
                                 3.3f, 4.6f, 5.1f, 3.7f, 4.3f, 3.9f, 5.2f, 4.0f, 3.6f, 4.9f};
        float skala1[N1] = {1.0f, 1.3f, 0.9f, 1.2f, 1.1f, 1.4f, 0.8f, 1.0f, 0.9f, 1.2f,
                            1.1f, 1.0f, 1.3f, 0.9f, 1.2f, 1.5f, 1.0f, 1.1f, 1.2f, 0.8f};

        for (int i = 0; i < N1; i++)
        {
            float pz0 = -70.0f + i * spasi1;
            float pz = pz0 + fmod(offset, rentang1);
            if (pz > 25.0f)
                pz -= rentang1;

            pemandangan_pohon(posX_kiri1[i], pz, skala1[i]);
            pemandangan_pohon(posX_kanan1[i], pz, skala1[i]);
        }

        /* Lapisan 2: pohon menengah dengan kecepatan scroll 75 persen. */
        const int N2 = 18;
        float spasi2 = 5.5f;
        float rentang2 = N2 * spasi2;
        float off2 = fmod(offset * 0.75f, rentang2);

        float posX_kiri2[N2] = {-6.5f, -8.0f, -7.2f, -9.0f, -6.8f, -8.5f, -7.5f, -6.2f, -9.5f,
                                -7.8f, -8.2f, -6.0f, -7.0f, -9.2f, -6.7f, -8.8f, -7.3f, -6.4f};
        float posX_kanan2[N2] = {6.5f, 8.0f, 7.2f, 9.0f, 6.8f, 8.5f, 7.5f, 6.2f, 9.5f,
                                 7.8f, 8.2f, 6.0f, 7.0f, 9.2f, 6.7f, 8.8f, 7.3f, 6.4f};
        float skala2[N2] = {1.5f, 1.8f, 1.3f, 2.0f, 1.6f, 1.4f, 1.7f, 1.2f, 1.9f,
                            1.5f, 1.6f, 1.3f, 1.8f, 1.4f, 1.7f, 2.0f, 1.5f, 1.3f};

        for (int i = 0; i < N2; i++)
        {
            float pz = -70.0f + i * spasi2 + off2;
            if (pz > 25.0f)
                pz -= rentang2;

            pemandangan_pohon(posX_kiri2[i], pz, skala2[i]);
            pemandangan_pohon(posX_kanan2[i], pz, skala2[i]);
        }

        /* Lapisan 3: pohon jauh dengan kecepatan scroll 50 persen. */
        const int N3 = 14;
        float spasi3 = 7.5f;
        float rentang3 = N3 * spasi3;
        float off3 = fmod(offset * 0.50f, rentang3);

        float posX_kiri3[N3] = {-12.0f, -15.0f, -11.5f, -17.0f, -13.5f, -16.0f, -12.5f,
                                -14.0f, -18.0f, -11.0f, -13.0f, -16.5f, -14.5f, -17.5f};
        float posX_kanan3[N3] = {12.0f, 15.0f, 11.5f, 17.0f, 13.5f, 16.0f, 12.5f,
                                 14.0f, 18.0f, 11.0f, 13.0f, 16.5f, 14.5f, 17.5f};
        float skala3[N3] = {2.2f, 2.5f, 2.0f, 2.8f, 2.3f, 2.1f, 2.6f,
                            2.4f, 2.9f, 2.0f, 2.7f, 2.2f, 2.5f, 2.3f};

        for (int i = 0; i < N3; i++)
        {
            float pz = -70.0f + i * spasi3 + off3;
            if (pz > 25.0f)
                pz -= rentang3;

            pemandangan_pohon(posX_kiri3[i], pz, skala3[i]);
            pemandangan_pohon(posX_kanan3[i], pz, skala3[i]);
        }
    }

    /* Tiang listrik disusun berkala di kedua sisi gerbong. */
    {
        const int N_TIANG = 8;
        float spasi = 8.0f;
        float rentang = N_TIANG * spasi;

        for (int i = 0; i < N_TIANG; i++)
        {
            float pz0 = -50.0f + i * spasi;
            float pz = pz0 + fmod(offset, rentang);
            if (pz > 15.0f)
                pz -= rentang;

            pemandangan_tiang(-2.3f, pz, 0);
            pemandangan_tiang(2.3f, pz, 1);

            /* Kabel dihubungkan ke tiang berikutnya untuk menjaga kontinuitas visual. */
            float pz_next = pz + spasi;
            glColor3f(0.18f, 0.17f, 0.15f);
            glBegin(GL_LINES);
            glVertex3f(-2.3f - 0.5f, 2.93f, pz);
            glVertex3f(-2.3f - 0.5f, 2.93f, pz_next);
            glVertex3f(2.3f + 0.5f, 2.93f, pz);
            glVertex3f(2.3f + 0.5f, 2.93f, pz_next);
            glEnd();
        }
    }

    glEnable(GL_LIGHTING);
}

/* ---------------------------------------------------------------------------
   setupPencahayaan()
   Mengonfigurasi pencahayaan interior untuk mode uji scene gerbong dan pemandangan.
   --------------------------------------------------------------------------- */
void setupPencahayaan()
{
    glEnable(GL_LIGHTING);

    /* LIGHT0: lampu plafon utama (kuning hangat, dari atas) */
    glEnable(GL_LIGHT0);
    GLfloat pos0[] = {0.0f, 2.4f, 0.0f, 1.0f};
    GLfloat amb0[] = {0.28f, 0.26f, 0.20f, 1.0f};
    GLfloat dif0[] = {0.85f, 0.82f, 0.70f, 1.0f};
    GLfloat spec0[] = {0.40f, 0.38f, 0.30f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

    /* LIGHT1: cahaya dari jendela kiri (siang hari, biru langit) */
    glEnable(GL_LIGHT1);
    GLfloat pos1[] = {-4.0f, 1.2f, 0.0f, 1.0f};
    GLfloat amb1[] = {0.04f, 0.07f, 0.12f, 1.0f};
    GLfloat dif1[] = {0.35f, 0.50f, 0.75f, 1.0f};
    GLfloat spec1[] = {0.08f, 0.12f, 0.18f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

    /* Material spekular dasar dipakai untuk mengaktifkan respons cahaya. */
    GLfloat mat_spec[] = {0.35f, 0.35f, 0.35f, 1.0f};
    GLfloat mat_shine[] = {18.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

/* ---------------------------------------------------------------------------
   MODE UJI MANDIRI
   Bagian ini menyediakan entry point lokal untuk memeriksa integrasi gerbong,
   pemandangan luar, dan animasi tanpa bergantung pada file integrasi akhir.
   --------------------------------------------------------------------------- */

/* State kamera dan animasi untuk mode uji mandiri. */
float camYaw = 0.0f;
float camPitch = 0.0f;
float camZ = 8.0f;
float getaran = 0.0f;
float shakeT = 0.0f;
float scenOffset = 0.0f;
float awanOffset = 0.0f;
float burungt = 0.0f;
float kipasRot = 0.0f;

void display_test()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Posisi kamera berada di tengah lorong untuk memudahkan inspeksi scene. */
    float eyeX = 0.0f;
    float eyeY = 1.6f + getaran;
    float eyeZ = camZ;

    float rad = camYaw * 3.14159f / 180.0f;
    float pitR = camPitch * 3.14159f / 180.0f;

    gluLookAt(
        eyeX, eyeY, eyeZ,
        eyeX + sin(rad) * 10.0f,
        eyeY + sin(pitR) * 10.0f,
        eyeZ - cos(rad) * 10.0f,
        0.0f, 1.0f, 0.0f);

    /* Pemandangan luar digambar lebih dahulu agar berada di lapisan paling belakang. */
    drawPemandangan(scenOffset, awanOffset, burungt);

    /* Interior dirender setelah pemandangan agar pencahayaan bekerja pada objek gerbong. */
    setupPencahayaan();
    drawGerbong();
    drawSemuaKursi();

    /* Tiga unit kipas dirender di plafon pada posisi Z yang berbeda. */
    glPushMatrix();
    drawKipasAC(kipasRot);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -4.5f);
    drawKipasAC(kipasRot + 90.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 4.5f);
    drawKipasAC(kipasRot + 180.0f);
    glPopMatrix();

    glutSwapBuffers();
}

void timer_test(int v)
{
    shakeT += 1.0f / 60.0f;
    /* Pemandangan bergulir otomatis untuk mensimulasikan kereta yang sedang melaju. */
    scenOffset += 0.22f; /* Satuan mendekati perpindahan per frame pada mode uji. */

    /* Awan bergerak lebih lambat untuk memberi kesan kedalaman. */
    awanOffset += 0.018f;
    if (awanOffset > 160.0f)
        awanOffset = 0.0f;

    /* Variabel waktu burung dipakai untuk translasi dan kepakan sayap. */
    burungt += 1.0f / 60.0f;

    /* Sudut kipas diakumulasi setiap frame untuk animasi rotasi cepat. */
    kipasRot += 8.0f;
    if (kipasRot >= 360.0f)
        kipasRot -= 360.0f;

    getaran = 0.010f * sin(shakeT * 20.0f)
            + 0.005f * sin(shakeT * 41.0f);
    glutPostRedisplay();
    glutTimerFunc(16, timer_test, 0);
}

/* Input keyboard khusus untuk rotasi kamera. */
void keyboard_test(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT)
        camYaw -= 3.0f;
    if (key == GLUT_KEY_RIGHT)
        camYaw += 3.0f;
    if (key == GLUT_KEY_UP)
        camPitch = camPitch > -30.0f ? camPitch - 2.0f : camPitch;
    if (key == GLUT_KEY_DOWN)
        camPitch = camPitch < 30.0f ? camPitch + 2.0f : camPitch;
    glutPostRedisplay();
}

/* Input keyboard biasa untuk pergerakan maju dan mundur di lorong. */
void keyboard_normal(unsigned char key, int x, int y)
{
    if (key == 'w' || key == 'W')
        camZ -= 0.3f;
    if (key == 's' || key == 'S')
        camZ += 0.3f;

    /* Clamp posisi menjaga kamera tetap berada di dalam batas gerbong. */
    if (camZ < -9.0f)
        camZ = -9.0f;
    if (camZ > 9.0f)
        camZ = 9.0f;
    glutPostRedisplay();
}

void reshape_test(int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (double)w / h, 0.05, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1100, 620);
    glutCreateWindow("[JEK] Interior Kereta - Test Mode");

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.55f, 0.78f, 0.95f, 1.0f); /* Biru langit dipakai sebagai warna fallback latar jendela. */

    glutDisplayFunc(display_test);
    glutReshapeFunc(reshape_test);
    glutSpecialFunc(keyboard_test);
    glutKeyboardFunc(keyboard_normal);
    glutTimerFunc(0, timer_test, 0);

    printf("====================================================\n");
    printf("  [Aisyah] Interior Kereta + Pemandangan Luar\n");
    printf("  Panah KIRI/KANAN : putar kamera\n");
    printf("  Panah ATAS/BAWAH : tengok atas/bawah\n");
    printf("  W / S            : jalan maju / mundur di lorong\n");
    printf("  Pemandangan      : scroll otomatis (kereta melaju)\n");
    printf("====================================================\n");
    printf("\n  CATATAN INTEGRASI TIM:\n");
    printf("  Fungsi yang siap dipakai:\n");
    printf("    drawPemandangan(offset) -> gambar sebelum interior\n");
    printf("    drawGerbong()           -> shell gerbong\n");
    printf("    drawSemuaKursi()        -> kursi 2-2 eksekutif\n");
    printf("    setupPencahayaan()      -> lampu interior\n");
    printf("====================================================\n");

    glutMainLoop();
    return 0;
}
