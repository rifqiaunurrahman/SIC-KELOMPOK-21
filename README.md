Arduino IDE dengan ESP32:
Sistem ini memanfaatkan ESP32, mikrokontroler berbasis WiFi dan Bluetooth yang dapat diprogram menggunakan Arduino IDE. Mikrokontroler ini mengendalikan berbagai perangkat dan sensor, serta berkomunikasi dengan beberapa platform dan layanan.

Komponen yang Digunakan:

Buzzer: Komponen ini menghasilkan bunyi atau sinyal audio sebagai respons terhadap perintah dari ESP32.
Sensor Ultrasonic: Mengukur jarak antara sensor dan objek yang terdeteksi dengan menggunakan gelombang ultrasonik. Data jarak ini digunakan untuk memicu notifikasi atau aksi lainnya.
Display I2C: Menampilkan informasi secara visual pada layar LCD yang terhubung menggunakan antarmuka I2C. Ini berguna untuk menampilkan data seperti jarak yang diukur atau status sistem.
Telegram Bot: Menggunakan Telegram untuk mengirim notifikasi. Bot ini memiliki dua jenis notifikasi:
Notifikasi Siaga: Memberikan peringatan atau informasi penting ketika kondisi tidak normal terdeteksi.
Notifikasi Bahaya: Memberikan peringatan darurat jika situasi berpotensi berbahaya.
Fitur Menu: Terdapat tiga fitur menu menarik, yaitu:
/start: Menyambut pengguna baru dengan pesan selamat datang.
/check distance: Menyediakan informasi mengenai jarak yang terukur oleh sensor ultrasonic.
/weather: Mengecek dan menampilkan informasi cuaca beberapa jam ke depan.
MQTT Server:
MQTT (Message Queuing Telemetry Transport) adalah protokol komunikasi ringan yang digunakan untuk menghubungkan ESP32 dengan server atau aplikasi lain. Server MQTT akan menerima data dari ESP32 dan menyebarkannya ke aplikasi lain jika diperlukan.

Python, MongoDB, dan Streamlit:

Python digunakan untuk menulis skrip yang mengelola MongoDB (database NoSQL) dan Streamlit (platform untuk membuat aplikasi web interaktif).
MongoDB menyimpan data dari sensor, notifikasi, dan informasi lain yang dikumpulkan oleh ESP32.
Streamlit digunakan untuk membuat antarmuka pengguna yang menampilkan data dari MongoDB secara real-time.
Tahapan Menjalankan Sistem
Arduino IDE dan ESP32:

Instalasi Arduino IDE: Unduh dan instal Arduino IDE dari situs resminya.
Konfigurasi ESP32: Tambahkan board ESP32 ke Arduino IDE dengan mengakses "Board Manager" dan menginstal ESP32.
Pemrograman: Tulis kode dalam Arduino IDE untuk mengendalikan buzzer, sensor ultrasonic, display I2C, serta integrasi dengan Telegram dan MQTT.
Upload Kode: Upload kode ke ESP32 melalui USB.
MQTT Server (HiveMQ):

Daftar di HiveMQ: Daftar dan buat akun di HiveMQ untuk menggunakan broker MQTT mereka.
Konfigurasi: Set up broker MQTT dengan detail koneksi yang diperlukan (alamat broker, port, dll.) dan integrasikan dengan kode ESP32.
MongoDB dan Python di VS Code:

Instal MongoDB: Unduh dan instal MongoDB dari situs resminya.
Konfigurasi Database: Buat database dan koleksi di MongoDB untuk menyimpan data.
Python Environment: Pastikan Python dan pip terinstal, lalu instal pustaka yang diperlukan (misalnya, pymongo untuk MongoDB).
Pengembangan di VS Code: Gunakan VS Code untuk menulis skrip Python yang menghubungkan ke MongoDB dan mengelola data.
Streamlit:

Instal Streamlit: Install Streamlit dengan perintah pip install streamlit.
Pengembangan Aplikasi: Buat aplikasi Streamlit di Python untuk menampilkan data dari MongoDB secara interaktif.
Menjalankan Aplikasi: Jalankan aplikasi Streamlit menggunakan perintah streamlit run nama_file.py di terminal.
Telegram Bot:

Buat Bot Telegram: Gunakan BotFather di Telegram untuk membuat bot baru dan mendapatkan token API.
Integrasi Bot: Tulis kode di Arduino IDE untuk menghubungkan ESP32 dengan bot Telegram, memungkinkan pengiriman notifikasi dengan dua jenis notifikasi dan fitur menu.
Uji Bot: Pastikan bot berfungsi dengan baik dan mengirimkan pesan sesuai perintah dan fitur yang tersedia.
