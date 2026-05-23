Visualisasi 3D Interior Kereta Api Eksekutif 🚂
Repositori ini mengandungi projek akhir untuk kursus Grafika Komputer oleh Kumpulan 62. Projek ini merupakan satu simulasi interaktif 3D bagi ruang dalaman (interior) sebuah kereta api eksekutif yang dibina sepenuhnya menggunakan kod prosedur C++ dan OpenGL (Fixed-Function Pipeline). Projek ini mensasarkan ilusi pergerakan realistik menggunakan teknik manipulasi optik tanpa memerlukan sebarang aset model 3D pihak ketiga.

🌟 Ciri-ciri Utama (Key Features)
- Ilusi Pergerakan (Vection) & Parallax Scrolling: Mensimulasikan pergerakan kereta api melalui teknik Optical Flow pada pemandangan luar tingkap. Aset seperti pokok, tiang elektrik, sawah, dan awan bergerak pada tahap kelajuan yang berbeza untuk menghasilkan ilusi kedalaman ruang (depth) secara berterusan (looping).
- Pemodelan Prosedural Tanpa Aset Luar: Keseluruhan struktur gerabak, rak bagasi, pintu, dan lebih daripada 40 kerusi penumpang dibina dari asas primitif geometri OpenGL (GL_QUADS, GL_LINES) menggunakan teknik pembinaan matriks secara hierarki (instancing).
- Sistem Pencahayaan Dinamik (Multi-Source Lighting): Konfigurasi sistem pencahayaan yang mensimulasikan tiga jenis cahaya serentak: GL_LIGHT0 (lampu hangat siling) dan GL_LIGHT1 (cahaya biru dari luar tingkap) berserta pengiraan parameter pantulan material untuk memberikan kesan realistik.
- Simulasi Getaran Kamera (Screen Shake): Menggunakan manipulasi matematik iaitu superposisi gelombang sinus pada paksi kamera untuk memberikan rasa getaran mekanikal fizikal ketika berada di dalam kereta api yang meluncur.
- Navigasi Interaktif & Pengesanan Perlanggaran (AABB): Kawalan penjelajahan First-Person interaktif melalui papan kekunci. Sistem dilengkapi dengan perlindungan Axis-Aligned Bounding Box (AABB) yang disertakan mekanik sliding, bagi menghalang pandangan daripada menembusi dinding atau tempat duduk.
- Paparan Maklumat (HUD) 2D: Pelapisan separa lutsinar yang dibina dengan unjuran ortografik (gluOrtho2D) untuk memaparkan status lampu serta telemetri kedudukan pemain secara masa nyata.

💻 Teknologi yang Digunakan
Bahasa Pengaturcaraan: C/C++ 
API Grafik: OpenGL (Legacy / Fixed-Function Pipeline) 
Perpustakaan (Library): FreeGLUT / GLUT 
Pengkompil (Compiler): MinGW / GCC 64-bit (disyorkan melalui MSYS2) 

👥 Developers (Kelompok 62)
Projek ini dibangunkan secara berkolaborasi dengan pembahagian modul kepakaran masing-masing:
Diyani Rahayu Nur'aeni — Pengurus Projek & Integrator Sistem 
M. Fauz Haunan Zaky — Pemodel Dalaman / Interior Modeler (Seni Bina Prosedural) 
Aisyah Nitiarahma — Logik Animasi & Eksterior (Kesan Parallax & Animasi Skunder) 
Agniya Azzahra — Pakar Pencahayaan & Material (Atmosfera & Warna) 
Galang Maulid Nugraha — Pengaturcara Teras (Fizik, Navigasi Kamera & Optimasi Performa) 

🎮 Controls
Setelah aplikasi dijalankan, anda boleh menggunakan papan kekunci dan tetikus untuk berinteraksi di dalam dunia maya kereta api:
W / S : Bergerak maju / mundur menyusuri lorong 
A / D : Menggelongsor ke pandangan kiri / kanan 
J / L (atau Pergerakan Mouse): Pusingkan arah pandangan ke kiri / kanan 
I / K (atau Pergerakan Mouse): Dongak kepala ke atas / menunduk 
F : Menghidupkan atau mematikan cahaya lampu utama di siling (Toggle Lighting) 
ESC : Menutup simulasi 

🚀 Cara Menjalankan Program (Kompilasi)
Pastikan sistem anda telah terpasang sistem pengkompil C++ bersama pustaka OpenGL dan FreeGLUT. Jika anda menggunakan OS Windows, adalah sangat disyorkan menggunakan persekitaran MSYS2 (MinGW64).

Melalui Terminal Visual Studio Code (atau Command Prompt):
Buka terminal tepat di dalam direktori fail anda, dan jalankan arahan pautan kompilasi berikut:

Bash
g++ final_kereta.cpp -o final_kereta.exe -lfreeglut -lglu32 -lopengl32 -lm

(Nota: Untuk pengkompilan di persekitaran Linux/Mac, sila gunakan tanda penghubung -lGL -lGLU -lglut -lm) 

Setelah kompilasi berjaya disiapkan (fail final_kereta.exe akan dijana tanpa mesej ralat merah di terminal), jalankannya dengan arahan:

Bash
./final_kereta.exe

Jika program tidak dapat dimulakan akibat notis ketiadaan DLL, pastikan fail-fail sokongan (seperti freeglut.dll, libstdc++-6.dll, libgcc_s_seh-1.dll, dan libwinpthread-1.dll) berada tepat berhampiran bersebelahan fail .exe anda (di folder yang sama).
