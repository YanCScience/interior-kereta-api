/* ===========================================================================
 * PROJECT     : VISUALISASI 3D INTERIOR KERETA API EKSEKUTIF
 * KELOMPOK    : 62 Grafika Komputer
 * FILE        : final_kereta.cpp
 * DESKRIPSI   : File integrasi akhir yang menggabungkan interior, eksterior, animasi, pencahayaan, HUD,
 *               dan navigasi kamera dalam satu executable. File ini menjadi artefak presentasi final proyek.
 * AUTHOR      : Seluruh Anggota Tim (Diintegrasikan oleh Diyani Rahayu Nur'aeni)
 * ===========================================================================
 * STRUKTUR TIM PENGEMBANG:
 * - Project Manager & Integrator : Diyani Rahayu Nur'aeni
 * - Core Programmer & Camera     : Galang Maulid Nugraha
 * - Interior Modeler             : M. Fauz Haunan Zaky
 * - Exterior & Animation         : Aisyah Nitiarahma
 * - Lighting & Material          : Agniya Azzahra
 * ===========================================================================
 * Dependensi  : OpenGL, freeGLUT, C++ Standard Library
 * Kompilasi   : g++ final_kereta.cpp -o output.exe -lfreeglut -lglu32 -lopengl32
 * =========================================================================== */

#define _CRT_SECURE_NO_WARNINGS

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

// =============================================================================
//  KONSTANTA DAN STATE GLOBAL
// =============================================================================
const int   WIN_W = 1100;
const int   WIN_H = 700;
const float PI = 3.14159265f;

// Batas gerak kamera di area lorong
const float BATAS_X_MIN = -0.5f; 
const float BATAS_X_MAX = 0.5f;
const float BATAS_Z_MIN = -9.0f;
const float BATAS_Z_MAX = 9.0f;

// State kamera
float camX = 0.0f;
float camY = 1.6f;
float camZ = 8.0f;
float yaw = 180.0f;
float pitch = 0.0f;
bool  mouseTekan = false;
int   mousePrevX = 0;
int   mousePrevY = 0;
const float SENSITIVITAS = 0.25f;

// State animasi
float scenOffset = 0.0f;  
float awanOffset = 0.0f;  
float burungt    = 0.0f;  
float kipasRot   = 0.0f;  
float shakeT     = 0.0f;
float getaran    = 0.0f;

// State fitur
bool lampuHidup = true;

// =============================================================================
//  UTILITAS BERSAMA
// =============================================================================
inline float toRad(float deg) { return deg * PI / 180.0f; }

void drawBox(float x0, float y0, float z0, float x1, float y1, float z1) {
    glBegin(GL_QUADS);
        glNormal3f( 0,-1, 0);
        glVertex3f(x0,y0,z0); glVertex3f(x1,y0,z0); glVertex3f(x1,y0,z1); glVertex3f(x0,y0,z1);
        glNormal3f( 0, 1, 0);
        glVertex3f(x0,y1,z0); glVertex3f(x0,y1,z1); glVertex3f(x1,y1,z1); glVertex3f(x1,y1,z0);
        glNormal3f( 0, 0, 1);
        glVertex3f(x0,y0,z1); glVertex3f(x1,y0,z1); glVertex3f(x1,y1,z1); glVertex3f(x0,y1,z1);
        glNormal3f( 0, 0,-1);
        glVertex3f(x0,y0,z0); glVertex3f(x0,y1,z0); glVertex3f(x1,y1,z0); glVertex3f(x1,y0,z0);
        glNormal3f(-1, 0, 0);
        glVertex3f(x0,y0,z0); glVertex3f(x0,y0,z1); glVertex3f(x0,y1,z1); glVertex3f(x0,y1,z0);
        glNormal3f( 1, 0, 0);
        glVertex3f(x1,y0,z0); glVertex3f(x1,y1,z0); glVertex3f(x1,y1,z1); glVertex3f(x1,y0,z1);
    glEnd();
}

void setMaterial(float r, float g, float b,
                 float specR, float specG, float specB,
                 float shininess) {
    GLfloat ambient[]  = { r * 0.25f, g * 0.25f, b * 0.25f, 1.0f };
    GLfloat diffuse[]  = { r,         g,         b,         1.0f };
    GLfloat specular[] = { specR,     specG,     specB,     1.0f };
    GLfloat shine[]    = { shininess };
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
}

void drawText2D(float x, float y, const char* str, void* font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos2f(x, y);
    for (const char* c = str; *c != '\0'; ++c) glutBitmapCharacter(font, *c);
}

// =============================================================================
//  INTERIOR GERBONG
// =============================================================================
void drawKursi() {
    // Kaki kursi dan sandaran tangan menggunakan material dengan komponen specular lebih tinggi.
    setMaterial(0.30f, 0.30f, 0.33f, 0.60f, 0.60f, 0.68f, 72.0f);
    glColor3f(0.30f, 0.30f, 0.33f);
    drawBox(-0.22f, 0.00f, -0.25f,  -0.17f, 0.40f, -0.20f); 
    drawBox( 0.17f, 0.00f, -0.25f,   0.22f, 0.40f, -0.20f); 
    drawBox(-0.22f, 0.00f,  0.20f,  -0.17f, 0.40f,  0.25f); 
    drawBox( 0.17f, 0.00f,  0.20f,   0.22f, 0.40f,  0.25f); 

    setMaterial(0.10f, 0.18f, 0.48f, 0.05f, 0.07f, 0.12f, 10.0f);
    glColor3f(0.10f, 0.18f, 0.48f); drawBox(-0.25f, 0.40f, -0.25f,   0.25f, 0.47f,  0.25f);
    setMaterial(0.18f, 0.28f, 0.60f, 0.06f, 0.08f, 0.14f, 12.0f);
    glColor3f(0.18f, 0.28f, 0.60f); drawBox(-0.23f, 0.47f, -0.23f,   0.23f, 0.53f,  0.23f);
    setMaterial(0.10f, 0.18f, 0.48f, 0.05f, 0.07f, 0.12f, 10.0f);
    glColor3f(0.10f, 0.18f, 0.48f); drawBox(-0.25f, 0.47f,  0.20f,   0.25f, 1.00f,  0.28f); 
    setMaterial(0.18f, 0.28f, 0.60f, 0.06f, 0.08f, 0.14f, 12.0f);
    glColor3f(0.18f, 0.28f, 0.60f); drawBox(-0.22f, 0.49f,  0.20f,   0.22f, 0.97f,  0.24f); 
    setMaterial(0.88f, 0.85f, 0.76f, 0.14f, 0.13f, 0.10f, 8.0f);
    glColor3f(0.88f, 0.85f, 0.76f); drawBox(-0.20f, 0.88f,  0.20f,   0.20f, 1.00f,  0.23f); 
    setMaterial(0.22f, 0.22f, 0.25f, 0.45f, 0.45f, 0.50f, 44.0f);
    glColor3f(0.22f, 0.22f, 0.25f);
    drawBox(-0.33f, 0.50f, -0.18f,  -0.25f, 0.58f,  0.22f); 
    drawBox( 0.25f, 0.50f, -0.18f,   0.33f, 0.58f,  0.22f); 
}

void drawSemuaKursi() {
    float posX[] = {-1.05f, -0.65f, +0.65f, +1.05f};
    float startZ = -8.50f, jarakZ = 1.70f;
    for (int baris = 0; baris < 10; baris++) {
        float pz = startZ + (float)baris * jarakZ;
        for (int kolom = 0; kolom < 4; kolom++) {
            glPushMatrix();
                glTranslatef(posX[kolom], 0.0f, pz);
                drawKursi();
            glPopMatrix();
        }
    }
}

void drawGerbong() {
    // Struktur gerbong mencakup lantai, dinding, plafon, jendela, dan rak bagasi.
    const float WALL_L = -1.5f, WALL_R = 1.5f, FLOOR_Y = 0.0f, CEIL_Y = 2.5f;
    const float FRONT_Z = -10.0f, BACK_Z = 10.0f, TW = 0.06f;
    const float JY_BOT = 0.80f, JY_TOP = 1.60f, JW = 1.10f;
    const int NJ = 6;
    float pilar = ((BACK_Z - FRONT_Z) - (NJ * JW)) / (NJ + 1.0f);

    glColor3f(0.45f, 0.28f, 0.14f); drawBox(WALL_L, FLOOR_Y - TW * 2, FRONT_Z, WALL_R, FLOOR_Y, BACK_Z);
    glColor3f(0.50f, 0.32f, 0.16f); drawBox(WALL_L + 0.01f, FLOOR_Y + 0.001f, FRONT_Z, -0.50f, FLOOR_Y + 0.003f, BACK_Z);
    glColor3f(0.50f, 0.32f, 0.16f); drawBox( 0.50f, FLOOR_Y + 0.001f, FRONT_Z, WALL_R - 0.01f, FLOOR_Y + 0.003f, BACK_Z);
    glColor3f(0.50f, 0.08f, 0.08f); drawBox(-0.50f, FLOOR_Y + 0.002f, FRONT_Z, 0.50f, FLOOR_Y + 0.006f, BACK_Z);

    glColor3f(0.93f, 0.91f, 0.86f); drawBox(WALL_L, CEIL_Y, FRONT_Z, WALL_R, CEIL_Y + TW, BACK_Z);
    if(lampuHidup) glColor3f(1.00f, 0.96f, 0.72f); else glColor3f(0.3f, 0.3f, 0.3f);
    drawBox(-0.06f, CEIL_Y - 0.01f, FRONT_Z + 0.5f, 0.06f, CEIL_Y, BACK_Z - 0.5f);

    glColor3f(0.72f, 0.69f, 0.63f);
    drawBox(WALL_L, FLOOR_Y, FRONT_Z - TW, WALL_R, CEIL_Y, FRONT_Z);
    drawBox(WALL_L, FLOOR_Y, BACK_Z, WALL_R, CEIL_Y, BACK_Z + TW);

    for (int sisi = 0; sisi < 2; sisi++) {
        float xs = (sisi == 0) ? WALL_L : WALL_R;
        float xe = (sisi == 0) ? WALL_L - TW : WALL_R + TW;
        glColor3f(0.76f, 0.73f, 0.66f);
        drawBox(xs, FLOOR_Y, FRONT_Z, xe, JY_BOT, BACK_Z);
        drawBox(xs, JY_TOP, FRONT_Z, xe, CEIL_Y, BACK_Z);
        float zc = FRONT_Z;
        for (int i = 0; i <= NJ; i++) {
            float z1 = (i == NJ) ? BACK_Z : zc + pilar;
            drawBox(xs, JY_BOT, zc, xe, JY_TOP, z1);
            zc = z1 + JW;
        }
        
        glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
        float zj = FRONT_Z + pilar;
        for (int i = 0; i < NJ; i++) {
            glColor4f(0.70f, 0.88f, 1.00f, 0.06f); drawBox(xs, JY_BOT, zj, xe, JY_TOP, zj + JW);
            glColor4f(1.0f, 1.0f, 1.0f, 0.04f); drawBox(xs, JY_TOP - 0.12f, zj + 0.05f, xe, JY_TOP - 0.02f, zj + JW - 0.05f);
            zj += JW + pilar;
        }
        glDepthMask(GL_TRUE); glDisable(GL_BLEND);

        glColor3f(0.65f, 0.62f, 0.57f);
        float rx0 = (sisi == 0) ? WALL_L : WALL_R - 0.30f, rx1 = (sisi == 0) ? WALL_L + 0.30f: WALL_R;
        drawBox(rx0, 2.00f, FRONT_Z + 0.3f, rx1, 2.30f, BACK_Z - 0.3f);
    }
}

// =============================================================================
//  EKSTERIOR DAN ANIMASI
// =============================================================================
void drawKipasAC(float rotDeg) {
    setMaterial(0.82f, 0.80f, 0.78f, 0.25f, 0.25f, 0.25f, 22.0f);
    glColor3f(0.82f, 0.80f, 0.78f); drawBox(-0.08f, 2.42f, -0.08f,  0.08f, 2.48f,  0.08f);
    glPushMatrix();
        glTranslatef(0.0f, 2.44f, 0.0f); glRotatef(rotDeg, 0.0f, 1.0f, 0.0f);
        setMaterial(0.75f, 0.73f, 0.70f, 0.18f, 0.18f, 0.18f, 18.0f);
        glColor3f(0.75f, 0.73f, 0.70f);
        glBegin(GL_QUADS);
            glVertex3f(0.05f, 0.0f, 0.0f); glVertex3f(0.05f, 0.0f, 0.55f); glVertex3f(-0.05f, 0.0f, 0.55f); glVertex3f(-0.05f, 0.0f, 0.0f);
            glVertex3f(0.05f, 0.0f, 0.0f); glVertex3f(0.05f, 0.0f,-0.55f); glVertex3f(-0.05f, 0.0f,-0.55f); glVertex3f(-0.05f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.05f); glVertex3f(0.55f, 0.0f, 0.05f); glVertex3f(0.55f, 0.0f,-0.05f); glVertex3f(0.0f, 0.0f,-0.05f);
            glVertex3f(0.0f, 0.0f, 0.05f); glVertex3f(-0.55f,0.0f, 0.05f); glVertex3f(-0.55f,0.0f,-0.05f); glVertex3f(0.0f, 0.0f,-0.05f);
        glEnd();
    glPopMatrix();
}

void drawSemuaKipasAC() {
    glPushMatrix(); drawKipasAC(kipasRot); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.0f, -4.5f); drawKipasAC(kipasRot + 90.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.0f,  4.5f); drawKipasAC(kipasRot + 180.0f); glPopMatrix();
}

void drawPohonLuar(float x, float z, float tinggiBatang, float tinggiDaun) {
    glColor3f(0.38f, 0.22f, 0.10f);
    drawBox(x - 0.12f, 0.0f, z - 0.12f, x + 0.12f, tinggiBatang, z + 0.12f);

    glColor3f(0.18f, 0.48f, 0.16f);
    glBegin(GL_TRIANGLES);
        glVertex3f(x, tinggiBatang + tinggiDaun, z);
        glVertex3f(x - 1.0f, tinggiBatang - 0.15f, z);
        glVertex3f(x + 1.0f, tinggiBatang - 0.15f, z);

        glVertex3f(x, tinggiBatang + tinggiDaun * 0.75f, z);
        glVertex3f(x - 1.25f, tinggiBatang + 0.35f, z);
        glVertex3f(x + 1.25f, tinggiBatang + 0.35f, z);
    glEnd();
}

void drawTiangListrik(float x, float z, float tinggi) {
    glColor3f(0.42f, 0.36f, 0.28f);
    drawBox(x - 0.08f, 0.0f, z - 0.08f, x + 0.08f, tinggi, z + 0.08f);
    drawBox(x - 0.85f, tinggi - 0.30f, z - 0.05f, x + 0.85f, tinggi - 0.18f, z + 0.05f);

    glColor3f(0.18f, 0.18f, 0.18f);
    glBegin(GL_LINES);
        glVertex3f(x + 0.85f, tinggi - 0.24f, z);
        glVertex3f(x + 3.6f, tinggi - 0.18f, z - 3.6f);
        glVertex3f(x - 0.85f, tinggi - 0.24f, z);
        glVertex3f(x - 3.6f, tinggi - 0.18f, z - 3.6f);
    glEnd();
}

void drawPemandangan(float offset, float awanOff, float bT) {
    glDisable(GL_LIGHTING); glDisable(GL_CULL_FACE);

    // Langit dan tanah digambar lebih dahulu sebagai lapisan latar terjauh.
    glBegin(GL_QUADS);
        glColor3f(0.22f, 0.50f, 0.88f); glVertex3f(-80.0f, 15.0f, -52.0f); glVertex3f(80.0f, 15.0f, -52.0f);
        glColor3f(0.65f, 0.84f, 0.97f); glVertex3f(80.0f,  0.3f, -52.0f);  glVertex3f(-80.0f, 0.3f, -52.0f);
        glColor3f(0.32f, 0.56f, 0.18f); glVertex3f(-80.0f, 0.0f, -52.0f); glVertex3f(80.0f, 0.0f, -52.0f); glVertex3f(80.0f, 0.0f, 20.0f); glVertex3f(-80.0f, 0.0f, 20.0f);
    glEnd();

    // Awan bergerak lebih lambat untuk menghasilkan efek paralaks.
    glColor3f(0.96f, 0.96f, 0.97f);
    glBegin(GL_QUADS);
        glVertex3f(-25.0f + awanOff, 8.0f, -51.0f); glVertex3f(-12.0f + awanOff, 8.0f, -51.0f); glVertex3f(-12.0f + awanOff,10.2f, -51.0f); glVertex3f(-25.0f + awanOff,10.2f, -51.0f);
        glVertex3f(-2.0f + awanOff*0.6f, 9.5f, -51.0f); glVertex3f( 9.0f + awanOff*0.6f, 9.5f, -51.0f); glVertex3f( 9.0f + awanOff*0.6f,11.5f, -51.0f); glVertex3f(-2.0f + awanOff*0.6f,11.5f, -51.0f);
    glEnd();

    // Petak sawah diulang secara periodik untuk mensimulasikan gerak maju kereta.
    float rentang = 16 * 8.0f;
    for (int i = 0; i < 16; i++) {
        float pz = -70.0f + i * 8.0f + fmod(offset, rentang);
        if (pz > 25.0f) pz -= rentang;
        float pz1 = pz + 7.7f;
        if (i % 2 == 0) glColor3f(0.30f, 0.58f, 0.15f); else glColor3f(0.40f, 0.70f, 0.20f);
        glBegin(GL_QUADS);
            glVertex3f(-2.0f, 0.02f, pz); glVertex3f(-14.0f, 0.02f, pz); glVertex3f(-14.0f, 0.02f, pz1); glVertex3f(-2.0f, 0.02f, pz1);
            glVertex3f( 2.0f, 0.02f, pz); glVertex3f( 14.0f, 0.02f, pz); glVertex3f( 14.0f, 0.02f, pz1); glVertex3f( 2.0f, 0.02f, pz1);
        glEnd();
    }

    // Pohon dan tiang listrik bergerak mundur lebih cepat daripada awan.
    float rentangLuar = 6 * 15.0f;
    for (int i = 0; i < 6; i++) {
        float pz = -72.0f + i * 15.0f + fmod(offset * 1.15f, rentangLuar);
        if (pz > 25.0f) pz -= rentangLuar;
        drawPohonLuar(-9.2f, pz, 2.3f, 2.6f);
        drawPohonLuar( 9.6f, pz + 6.0f, 2.0f, 2.3f);
    }

    float rentangTiang = 5 * 20.0f;
    for (int i = 0; i < 5; i++) {
        float pz = -76.0f + i * 20.0f + fmod(offset * 1.35f, rentangTiang);
        if (pz > 28.0f) pz -= rentangTiang;
        drawTiangListrik(-5.8f, pz, 5.6f);
    }

    // Burung dianimasikan dengan perpindahan posisi dan osilasi sinusoidal pada sayap.
    glColor3f(0.08f, 0.06f, 0.05f);
    struct Burung {
        float bx, by, bz, speed, phase;
    };
    Burung kawanan[] = {
        { -40.0f, 5.5f, -18.0f, 4.2f, 0.0f },
        { -40.0f, 7.2f, -25.0f, 3.6f, 1.8f },
        { -40.0f, 4.8f, -12.0f, 5.0f, 3.5f }
    };
    for (int k = 0; k < 3; k++) {
        float tx = fmod(bT * kawanan[k].speed + kawanan[k].phase, 95.0f) - 10.0f;
        float bx0 = kawanan[k].bx + tx * 1.8f, by0 = kawanan[k].by + sin(bT * 1.5f + kawanan[k].phase) * 0.3f, bz0 = kawanan[k].bz;
        float offsetX[] = { 0.0f, 2.2f, -1.8f, 4.0f, -3.5f };
        float offsetY[] = { 0.0f, 0.6f, 0.4f, 1.2f, 0.8f };
        for (int j = 0; j < 5; j++) {
            float bx = bx0 + offsetX[j], by = by0 + offsetY[j], flap = sin(bT * 8.0f + j * 0.8f) * 0.4f;
            glBegin(GL_QUADS); glVertex3f(bx - 0.18f, by, bz0); glVertex3f(bx + 0.18f, by, bz0); glVertex3f(bx + 0.18f, by + 0.08f, bz0); glVertex3f(bx - 0.18f, by + 0.08f, bz0); glEnd();
            glBegin(GL_TRIANGLES); glVertex3f(bx, by + 0.04f, bz0); glVertex3f(bx - 0.55f, by + 0.04f + flap, bz0); glVertex3f(bx - 0.20f, by + 0.04f, bz0); glEnd();
            glBegin(GL_TRIANGLES); glVertex3f(bx, by + 0.04f, bz0); glVertex3f(bx + 0.55f, by + 0.04f + flap, bz0); glVertex3f(bx + 0.20f, by + 0.04f, bz0); glEnd();
        }
    }
    glEnable(GL_LIGHTING);
}

// =============================================================================
//  PEMBARUAN STATE ANIMASI
// =============================================================================
void updateAnimasiKereta() {
    // Seluruh variabel waktu dan offset diperbarui secara terpusat.
    shakeT += 1.0f / 60.0f;
    scenOffset += 0.22f;
    awanOffset += 0.018f;
    if (awanOffset > 160.0f) awanOffset = 0.0f;
    burungt += 1.0f / 60.0f;
    kipasRot += 8.0f;
    if (kipasRot >= 360.0f) kipasRot -= 360.0f;
    getaran = 0.010f * sin(shakeT * 20.0f) + 0.005f * sin(shakeT * 41.0f);
}

// =============================================================================
//  PENCAHAYAAN DAN MATERIAL
// =============================================================================
void setupPencahayaan() {
    if(lampuHidup) {
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        GLfloat ambNormal[] = { 0.22f, 0.22f, 0.24f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambNormal);

        // LIGHT0: Plafon (Kuning Hangat)
        glEnable(GL_LIGHT0);
        GLfloat pos0[]  = {  0.0f,  2.4f,  0.0f,  1.0f }; 
        GLfloat amb0[]  = {  0.25f, 0.22f, 0.15f, 1.0f }; 
        GLfloat dif0[]  = {  0.90f, 0.85f, 0.65f, 1.0f }; 
        GLfloat spec0[] = {  0.50f, 0.45f, 0.30f, 1.0f }; 
        glLightfv(GL_LIGHT0, GL_POSITION, pos0); glLightfv(GL_LIGHT0, GL_AMBIENT,  amb0); glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif0); glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

        // LIGHT1: Cahaya Jendela (Biru Langit)
        glEnable(GL_LIGHT1);
        GLfloat pos1[]  = { -4.0f,  1.2f,  0.0f,  1.0f }; 
        GLfloat amb1[]  = {  0.03f, 0.06f, 0.10f, 1.0f }; 
        GLfloat dif1[]  = {  0.30f, 0.50f, 0.80f, 1.0f }; 
        GLfloat spec1[] = {  0.05f, 0.10f, 0.15f, 1.0f }; 
        glLightfv(GL_LIGHT1, GL_POSITION, pos1); glLightfv(GL_LIGHT1, GL_AMBIENT,  amb1); glLightfv(GL_LIGHT1, GL_DIFFUSE,  dif1); glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

        // LIGHT2: Bounce Light Karpet
        glEnable(GL_LIGHT2);
        GLfloat pos2[]  = {  0.0f,  0.1f,  0.0f,  1.0f }; 
        GLfloat amb2[]  = {  0.08f, 0.02f, 0.02f, 1.0f }; 
        GLfloat dif2[]  = {  0.20f, 0.05f, 0.05f, 1.0f }; 
        GLfloat spec2[] = {  0.0f,  0.0f,  0.0f,  1.0f }; 
        glLightfv(GL_LIGHT2, GL_POSITION, pos2); glLightfv(GL_LIGHT2, GL_AMBIENT,  amb2); glLightfv(GL_LIGHT2, GL_DIFFUSE,  dif2); glLightfv(GL_LIGHT2, GL_SPECULAR, spec2);
    } else {
        glDisable(GL_LIGHT0); glDisable(GL_LIGHT1); glDisable(GL_LIGHT2);
        glEnable(GL_LIGHTING);
        GLfloat ambGelap[] = { 0.15f, 0.12f, 0.10f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambGelap);
    }
}

// =============================================================================
//  HUD, KAMERA, DAN INPUT
// =============================================================================
void drawHUD() {
    bool wasLit = glIsEnabled(GL_LIGHTING); glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.60f);
    glBegin(GL_QUADS); glVertex2f(0, WIN_H); glVertex2f(420, WIN_H); glVertex2f(420, WIN_H - 120); glVertex2f(0, WIN_H - 120); glEnd();
    glDisable(GL_BLEND);

    glColor3f(1.0f, 1.0f, 0.85f);
    int y = WIN_H - 22;
    drawText2D(10, (float)y, "=== SIMULASI KERETA API EKSEKUTIF ==="); y -= 16;
    drawText2D(10, (float)y, "W/S: Maju-Mundur  |  A/D: Geser Kiri-Kanan"); y -= 16;
    drawText2D(10, (float)y, "Mouse Drag: Lihat Sekeliling  |  F: Toggle Lampu"); y -= 16;
    drawText2D(10, (float)y, "ESC: Keluar Program"); y -= 16;

    char buf[128];
    sprintf(buf, "Status Lampu: %s  |  Kamera Z: %.1f", lampuHidup ? "ON" : "OFF", camZ);
    glColor3f(0.8f, 1.0f, 0.8f);
    drawText2D(10, (float)y, buf);

    glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
    if (wasLit) glEnable(GL_LIGHTING);
}

void applyCameraView() {
    // Kamera first-person dibentuk dari posisi pemain serta orientasi yaw dan pitch.
    float cosYaw = cos(toRad(yaw)), sinYaw = sin(toRad(yaw)), cosPitch = cos(toRad(pitch)), sinPitch = sin(toRad(pitch));
    float lookX = camX + cosYaw * cosPitch, lookY = camY + sinPitch, lookZ = camZ + sinYaw * cosPitch;
    gluLookAt(camX, camY + getaran, camZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);
}

void terapkanBatasLorong(float kandidatX, float kandidatZ) {
    // Kamera dibatasi agar tetap berada di area lorong tengah.
    if (kandidatX >= BATAS_X_MIN && kandidatX <= BATAS_X_MAX) camX = kandidatX;
    if (kandidatZ >= BATAS_Z_MIN && kandidatZ <= BATAS_Z_MAX) camZ = kandidatZ;
}

void keyboardDown(unsigned char key, int x, int y) {
    const float SPEED = 0.12f;
    float dx = cos(toRad(yaw)), dz = sin(toRad(yaw));
    float newX = camX, newZ = camZ;

    switch (key) {
        case 'w': case 'W': newX += dx * SPEED; newZ += dz * SPEED; break;
        case 's': case 'S': newX -= dx * SPEED; newZ -= dz * SPEED; break;
        case 'a': case 'A': newX += dz * SPEED; newZ -= dx * SPEED; break;
        case 'd': case 'D': newX -= dz * SPEED; newZ += dx * SPEED; break;
        case 'f': case 'F': lampuHidup = !lampuHidup; break;
        case 27: exit(0); break;
    }

    terapkanBatasLorong(newX, newZ);

    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) { mouseTekan = (state == GLUT_DOWN); mousePrevX = x; mousePrevY = y; }
}

void mouseMotion(int x, int y) {
    if (!mouseTekan) return;
    yaw += (x - mousePrevX) * SENSITIVITAS;
    pitch -= (y - mousePrevY) * SENSITIVITAS;
    if (pitch > 80.0f) pitch = 80.0f; if (pitch < -80.0f) pitch = -80.0f;
    mousePrevX = x; mousePrevY = y;
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1; glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(65.0, (double)w / h, 0.05, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

// =============================================================================
//  INTEGRASI APLIKASI
// =============================================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    applyCameraView();

    // Urutan render menjaga pemandangan luar tetap berada di belakang interior.
    // 1. Pemandangan luar
    // 2. Pencahayaan interior
    // 3. Struktur gerbong + kursi
    drawPemandangan(scenOffset, awanOffset, burungt);
    setupPencahayaan();
    drawGerbong();
    drawSemuaKursi();
    drawSemuaKipasAC();
    drawHUD();

    glutSwapBuffers();
}

void timerCallback(int v) {
    updateAnimasiKereta();
    glutPostRedisplay();
    glutTimerFunc(16, timerCallback, 0);
}

void initOpenGLState() {
    // State dasar OpenGL dipisahkan agar fungsi main() tetap ringkas.
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.55f, 0.78f, 0.95f, 1.0f);
}

void registerGlutCallbacks() {
    // Seluruh callback GLUT didaftarkan di satu lokasi untuk menjaga konsistensi inisialisasi.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, timerCallback, 0);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Project UAS Grafkom - Interior Kereta Api Eksekutif (Kelompok 62)");

    initOpenGLState();
    registerGlutCallbacks();

    glutMainLoop();
    return 0;
}
