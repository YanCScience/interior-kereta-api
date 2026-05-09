/* ===========================================================================
 * PROJECT     : VISUALISASI 3D INTERIOR KERETA API EKSEKUTIF
 * KELOMPOK    : 62 Grafika Komputer
 * FILE        : interior_kereta.cpp
 * DESKRIPSI   : Modul model interior gerbong yang memuat geometri kursi eksekutif, dinding, jendela, rak bagasi,
 *               dan mode uji mandiri. File ini digunakan untuk memvalidasi proporsi dan tata letak interior.
 * AUTHOR      : M. Fauz Haunan Zaky
 * ===========================================================================
 * STRUKTUR TIM PENGEMBANG:
 * - Project Manager & Integrator : Diyani Rahayu Nur'aeni
 * - Core Programmer & Camera     : Galang Maulid Nugraha
 * - Interior Modeler             : M. Fauz Haunan Zaky
 * - Exterior & Animation         : Aisyah Nitiarahma
 * - Lighting & Material          : Agniya Azzahra
 * ===========================================================================
 * Dependensi  : OpenGL, freeGLUT, C++ Standard Library
 * Kompilasi   : g++ interior_kereta.cpp -o output.exe -lfreeglut -lglu32 -lopengl32
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
   Menggambar balok dari dua titik sudut dan digunakan sebagai elemen dasar
   penyusun seluruh geometri interior.
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
   Fungsi ini diasumsikan dipanggil setelah transformasi posisi kursi diterapkan.
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
   Membangun shell gerbong lengkap pada sistem koordinat interior standar.
   Komponen utama mencakup lantai, dinding, plafon, jendela, rak bagasi, dan rel plafon.
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
    glColor3f(0.20f, 0.20f, 0.20f);
    drawBox(WALL_L, FLOOR_Y - TW, FRONT_Z,
            WALL_R, FLOOR_Y, BACK_Z);

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

        /* KACA JENDELA SEMI-TRANSPARAN */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        float zj = FRONT_Z + pilar;
        for (int i = 0; i < NJ; i++)
        {
            /* Bingkai tipis membantu siluet jendela tetap terbaca. */
            glColor3f(0.12f, 0.12f, 0.12f);
            float ox = (sisi == 0) ? -0.005f : 0.005f;
            drawBox(xs + ox, JY_BOT - 0.02f, zj - 0.02f,
                    xe - ox, JY_TOP + 0.02f, zj + JW + 0.02f);

            glColor4f(0.55f, 0.73f, 0.92f, 0.22f);
            drawBox(xs, JY_BOT, zj, xe, JY_TOP, zj + JW);

            /* Refleksi tipis memberikan kesan permukaan kaca. */
            glColor4f(1.0f, 1.0f, 1.0f, 0.08f);
            drawBox(xs, JY_BOT + 0.55f, zj + 0.06f,
                    xe, JY_TOP - 0.04f, zj + JW - 0.06f);

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
    /* Posisi X keempat kursi per baris. */
    float posX[4] = {-1.40f, -0.80f, +0.80f, +1.40f};

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
   setupPencahayaan()
   Mengonfigurasi pencahayaan interior untuk mode uji mandiri geometri gerbong.
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
   Bagian ini menyediakan entry point lokal untuk memeriksa model interior
   tanpa mengubah file integrasi utama.
   --------------------------------------------------------------------------- */

/* State kamera dan animasi sederhana untuk mode uji mandiri. */
float camYaw = 0.0f;
float camPitch = 0.0f;
float getaran = 0.0f;
float shakeT = 0.0f;

void display_test()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /* Kamera ditempatkan di tengah lorong dan menghadap ke depan gerbong. */
    float eyeX = 0.0f;
    float eyeY = 1.6f + getaran;
    float eyeZ = 8.0f;

    float rad = camYaw * 3.14159f / 180.0f;
    float pitR = camPitch * 3.14159f / 180.0f;

    gluLookAt(
        eyeX, eyeY, eyeZ,
        eyeX + sin(rad) * 10.0f,
        eyeY + sin(pitR) * 10.0f,
        eyeZ - cos(rad) * 10.0f,
        0.0f, 1.0f, 0.0f);

    setupPencahayaan();

    /* Render interior utama. */
    drawGerbong();
    drawSemuaKursi();

    /* Latar langit sederhana membantu pembacaan bukaan jendela saat pengujian. */
    glDisable(GL_LIGHTING);
    glColor3f(0.55f, 0.75f, 0.93f);
    glBegin(GL_QUADS);
    glVertex3f(-20.0f, 0.0f, -50.0f);
    glVertex3f(20.0f, 0.0f, -50.0f);
    glVertex3f(20.0f, 20.0f, -50.0f);
    glVertex3f(-20.0f, 20.0f, -50.0f);
    glEnd();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void timer_test(int v)
{
    shakeT += 1.0f / 60.0f;
    getaran = 0.010f * sin(shakeT * 20.0f) + 0.005f * sin(shakeT * 41.0f);
    glutPostRedisplay();
    glutTimerFunc(16, timer_test, 0);
}

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

void reshape_test(int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (double)w / h, 0.05, 200.0);
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
    glClearColor(0.10f, 0.10f, 0.10f, 1.0f);

    glutDisplayFunc(display_test);
    glutReshapeFunc(reshape_test);
    glutSpecialFunc(keyboard_test);
    glutTimerFunc(0, timer_test, 0);

    printf("====================================================\n");
    printf("  [M. Fauz] Interior Kereta - Test Standalone\n");
    printf("  Panah KIRI/KANAN : putar kamera\n");
    printf("  Panah ATAS/BAWAH : tengok atas/bawah\n");
    printf("====================================================\n");
    printf("\n  CATATAN INTEGRASI TIM:\n");
    printf("  Fungsi yang siap dipakai:\n");
    printf("    drawGerbong()     -> panggil dari display() utama\n");
    printf("    drawSemuaKursi()  -> panggil dari display() utama\n");
    printf("    setupPencahayaan()-> panggil di awal display()\n");
    printf("====================================================\n");

    glutMainLoop();
    return 0;
}
