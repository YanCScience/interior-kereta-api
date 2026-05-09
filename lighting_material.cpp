/* ===========================================================================
 * PROJECT     : VISUALISASI 3D INTERIOR KERETA API EKSEKUTIF
 * KELOMPOK    : 62 Grafika Komputer
 * FILE        : lighting_material.cpp
 * DESKRIPSI   : Modul utilitas material dan pencahayaan interior untuk OpenGL fixed-function pipeline.
 *               File ini menyediakan helper material serta konfigurasi sumber cahaya utama gerbong.
 * AUTHOR      : Agniya Azzahra
 * ===========================================================================
 * STRUKTUR TIM PENGEMBANG:
 * - Project Manager & Integrator : Diyani Rahayu Nur'aeni
 * - Core Programmer & Camera     : Galang Maulid Nugraha
 * - Interior Modeler             : M. Fauz Haunan Zaky
 * - Exterior & Animation         : Aisyah Nitiarahma
 * - Lighting & Material          : Agniya Azzahra
 * ===========================================================================
 * Dependensi  : OpenGL, freeGLUT, C++ Standard Library
 * Kompilasi   : g++ lighting_material.cpp -o output.exe -lfreeglut -lglu32 -lopengl32
 * =========================================================================== */

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

/* ---------------------------------------------------------------------------
   setMaterial()
   Mengatur properti material objek sebelum proses rendering dilakukan.
   Nilai shininess mengikuti rentang standar OpenGL dari 0.0 hingga 128.0.
   --------------------------------------------------------------------------- */
void setMaterial(float r,     float g,     float b,
                 float specR, float specG, float specB,
                 float shininess)
{
    GLfloat ambient[]  = { r * 0.25f, g * 0.25f, b * 0.25f, 1.0f };
    GLfloat diffuse[]  = { r,         g,         b,         1.0f };
    GLfloat specular[] = { specR,     specG,     specB,     1.0f };
    GLfloat shine[]    = { shininess };
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
}

/* ---------------------------------------------------------------------------
   setupPencahayaan()
   Mengonfigurasi tiga sumber cahaya interior yang mewakili lampu plafon,
   cahaya masuk dari jendela, dan ambient fill dekat lantai.
   --------------------------------------------------------------------------- */
void setupPencahayaan()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);       /* Menjaga vektor normal tetap ternormalisasi setelah transformasi. */
    glEnable(GL_COLOR_MATERIAL);  /* Mengizinkan glColor memengaruhi kanal ambient dan diffuse. */
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    /* LIGHT0: Lampu plafon utama dengan temperatur warna hangat. */
    glEnable(GL_LIGHT0);
    GLfloat pos0[]  = {  0.0f,  2.4f,  0.0f,  1.0f }; /* Posisi lampu di tengah plafon. */
    GLfloat amb0[]  = {  0.25f, 0.22f, 0.15f, 1.0f }; /* Komponen ambient hangat. */
    GLfloat dif0[]  = {  0.90f, 0.85f, 0.65f, 1.0f }; /* Komponen diffuse lampu utama. */
    GLfloat spec0[] = {  0.50f, 0.45f, 0.30f, 1.0f }; /* Komponen specular berwarna keemasan. */
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  amb0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

    /* LIGHT1: Cahaya terarah dari area jendela sisi kiri. */
    glEnable(GL_LIGHT1);
    GLfloat pos1[]  = { -4.0f,  1.2f,  0.0f,  1.0f }; /* Posisi sumber cahaya luar ruang. */
    GLfloat amb1[]  = {  0.03f, 0.06f, 0.10f, 1.0f }; /* Ambient biru redup. */
    GLfloat dif1[]  = {  0.30f, 0.50f, 0.80f, 1.0f }; /* Diffuse biru langit. */
    GLfloat spec1[] = {  0.05f, 0.10f, 0.15f, 1.0f }; /* Specular ringan untuk highlight jendela. */
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  amb1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  dif1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

    /* LIGHT2: Ambient fill rendah untuk menjaga detail area lantai tetap terbaca. */
    glEnable(GL_LIGHT2);
    GLfloat pos2[]  = {  0.0f,  0.1f,  0.0f,  1.0f }; /* Sumber cahaya dekat lantai. */
    GLfloat amb2[]  = {  0.08f, 0.02f, 0.02f, 1.0f }; /* Ambient merah redup. */
    GLfloat dif2[]  = {  0.20f, 0.05f, 0.05f, 1.0f }; /* Diffuse merah tua lembut. */
    GLfloat spec2[] = {  0.0f,  0.0f,  0.0f,  1.0f }; /* Tanpa kontribusi specular tambahan. */
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT2, GL_AMBIENT,  amb2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  dif2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec2);
}
