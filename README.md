# ESP8266-WiFi-Prank


/*
=====================================================================
Program Captive Portal + Prank Audio dengan ESP8266 & LittleFS
=====================================================================

Deskripsi:
Program ini menggunakan ESP8266 untuk membuat Access Point palsu 
(AP Mode) dengan tampilan web (captive portal). Halaman web yang 
ditampilkan berisi gambar `indihome.png` dan tombol "Hubungkan". 
Saat tombol ditekan, otomatis memutar audio `prank02.mp3` yang 
disimpan di LittleFS.

Komponen & Library:
- ESP8266WiFi.h       : Membuat Access Point ESP8266.
- ESP8266WebServer.h  : Menyediakan web server (port 80).
- DNSServer.h         : Mengarahkan semua request ke captive portal.
- LittleFS.h          : Menyimpan file statis (gambar & audio).
- ESP8266mDNS.h       : Mendukung akses hostname lokal.
- WiFiUdp.h + ArduinoOTA.h : Mendukung update firmware OTA.

Konfigurasi:
- `ssid`  : Nama Access Point (AP) palsu.
- `indihome.png` : Logo/gambar pada web prank, disimpan di LittleFS.
- `prank02.mp3`  : File audio prank , disimpan di LittleFS.

Fitur Utama:
1. **Access Point + Captive Portal**
   - ESP8266 membuat AP.
   - Semua request diarahkan ke halaman utama (captive portal).
   - Halaman menampilkan gambar `indihome.png` dan tombol interaktif.

2. **Halaman Web Prank**
   - User diarahkan ke halaman dengan tombol "Hubungkan".
   - Saat tombol ditekan → audio `prank02.mp3` diputar → tombol berubah menjadi "Terhubung".

3. **Streaming File**
   - Web server melayani file `indihome.png` (gambar) dan `prank02.mp3` (audio) langsung dari LittleFS.
   - Mendukung streaming audio dengan header `Range` agar file besar bisa diputar lancar.

4. **OTA Update**
   - Program mendukung update firmware lewat WiFi menggunakan ArduinoOTA.

Tujuan:
- Membuat alat prank berbasis captive portal sederhana.
- Menggunakan ESP8266 sebagai server web mini dengan file statis dari LittleFS.
- Menampilkan kombinasi gambar + audio prank ketika user terhubung ke AP palsu.

Catatan:
⚠️ Program ini hanya untuk **eksperimen pribadi & edukasi**. 
Dilarang digunakan untuk menyerang atau menipu jaringan publik 
karena melanggar hukum.
=====================================================================
*/

