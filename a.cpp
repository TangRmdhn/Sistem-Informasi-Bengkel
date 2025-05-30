// bengkel.cpp - Modified with countdown functionality
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;
string Perbaikan[7] = {"Ganti Oli","Turun Mesin","CVT","Komstir","Servis","Pengereman","Ganti Ban"};
int WaktuPerbaikan[7] = {10,240,20,15,30,20,20};

struct Kendaraan{
    string merk;
    string jenis;
    string warna;
    string NoPol;
};

struct Kerusakan{
    int jenisRusak;
    int banyaksparepart;
    int sparepart[5];
};

struct Customer{
    string nama;
    string no_telp;
    string alamat;
    string email;
    Kendaraan motor;
    Kerusakan perbaikan;
    int totalharga;
    int detikEstimasi;        // Tambahan: estimasi waktu dalam detik
    time_t waktuMulai;        // Tambahan: waktu mulai perbaikan
    int keterangan;           // Tambahan: 0=Tidak diperbaiki, 1=sedang diperbaiki, 2=antrian, 3=Proses pembayaran
}; Customer cust[100];

struct Warehouse{
    string barang;
    int QTY;
    int hargajual;
};

Warehouse stok[19];
                     
int DataCust = 0;

int Antrian[100] = {false};
int OnQueue = 0;

int Dikerjakan[3] = {false};
int OnWork = 0;

int Pembayaran[10] = {false};
int OnPayment = 0;

string namaFile = "DataCustomer.txt";

// Fungsi countdown
int hitungCountdown(int durasiDetik, time_t waktuMulai) {
    time_t waktuSekarang = time(0);
    int selisih = static_cast<int>(waktuSekarang - waktuMulai);
    int sisa = durasiDetik - selisih;

    // Pastikan tidak negatif
    if (sisa < 0) sisa = 0;

    return sisa;
}

string GantiUnderscore(string nama){
    for (int i = 0; i < nama.length(); i++){
        if(nama[i] == '_'){
            nama[i] = ' ';
        }
    }
    return nama;
}

string GantiSpasi(string nama){
    for (int i = 0; i < nama.length(); i++){
        if(nama[i] == ' '){
            nama[i] = '_';
        }
    }
    return nama;
}

void OpenFile(){
    string nama;
    string no_telp;
    string alamat;
    string email;
    int totalharga;
    int detikEstimasi;
    time_t waktuMulai;
    int keterangan;

    string merk;
    string jenis;
    string warna;
    string NoPol;

    int jenisRusak;
    int banyaksparepart;
    int sparepart[5];

    ifstream Database(namaFile);
    if (Database.is_open()){
        while (Database >> nama >> no_telp >> alamat >> email >> totalharga >> detikEstimasi >> waktuMulai >> keterangan >> merk >> jenis >> warna >> NoPol >> jenisRusak >> banyaksparepart) {
            
            // Membaca array sparepart sebanyak banyaksparepart
            for (int i = 0; i < banyaksparepart; ++i) {
                Database >> sparepart[i];
            }

            // Simpan ke struct
            cust[DataCust].nama = nama;
            cust[DataCust].no_telp = no_telp;
            cust[DataCust].alamat = alamat;
            cust[DataCust].email = email;
            cust[DataCust].totalharga = totalharga;
            cust[DataCust].detikEstimasi = detikEstimasi;
            cust[DataCust].waktuMulai = waktuMulai;
            cust[DataCust].keterangan = keterangan;

            cust[DataCust].motor.merk = merk;
            cust[DataCust].motor.jenis = jenis;
            cust[DataCust].motor.warna = warna;
            cust[DataCust].motor.NoPol = NoPol;

            cust[DataCust].perbaikan.jenisRusak = jenisRusak;
            cust[DataCust].perbaikan.banyaksparepart = banyaksparepart;

            for (int i = 0; i < banyaksparepart; ++i) {
                cust[DataCust].perbaikan.sparepart[i] = sparepart[i];
            }

            DataCust++;
        }
        Database.close();
    } else {
        cout << "File tidak ada" << endl;
    }
}

void SaveFile() {
    ofstream Database(namaFile);
    if (Database.is_open()) {
        for (int i = 0; i < DataCust; ++i) {
            // Tulis data customer dan kendaraannya
            Database << cust[i].nama << " "
                     << cust[i].no_telp << " "
                     << cust[i].alamat << " "
                     << cust[i].email << " "
                     << cust[i].totalharga << " "
                     << cust[i].detikEstimasi << " "
                     << cust[i].waktuMulai << " "
                     << cust[i].keterangan << " "
                     << cust[i].motor.merk << " "
                     << cust[i].motor.jenis << " "
                     << cust[i].motor.warna << " "
                     << cust[i].motor.NoPol << " "
                     << cust[i].perbaikan.jenisRusak << " "
                     << cust[i].perbaikan.banyaksparepart;

            // Tulis sparepart-nya sebanyak yang digunakan
            for (int j = 0; j < cust[i].perbaikan.banyaksparepart; ++j) {
                Database << " " << cust[i].perbaikan.sparepart[j];
            }

            Database << endl;  // ganti baris antar customer
        }

        Database.close();
        cout << "Data berhasil disimpan ke file.\n";
    } else {
        cout << "Gagal membuka file untuk menyimpan data.\n";
    }
}

void SaveStok() {
    ofstream FileStok("Stok.txt");
    if (FileStok.is_open()) {
        for (int i = 0; i < 19; ++i) {
            FileStok << GantiSpasi(stok[i].barang) << " "
                     << stok[i].QTY << " "
                     << stok[i].hargajual << endl;
        }
        FileStok.close();
        cout << "Data stok berhasil disimpan.\n";
    } else {
        cout << "Gagal membuka file Stok.txt\n";
    }
}

void OpenStok() {
    ifstream FileStok("Stok.txt");
    if (FileStok.is_open()) {
        for (int i = 0; i < 19; ++i) {
            string namaBarang;
            FileStok >> namaBarang >> stok[i].QTY >> stok[i].hargajual;
            stok[i].barang = GantiUnderscore(namaBarang);
        }
        FileStok.close();
        cout << "Data stok berhasil dibaca.\n";
    } else {
        cout << "File Stok.txt tidak ditemukan.\n";
    }
}

void SaveStatus() {
    ofstream fileWork("Dikerjakan.txt");
    if (fileWork.is_open()) {
        fileWork << OnWork << endl;
        for(int i = 0; i < OnWork; i++){
            fileWork << Dikerjakan[i];
        }
        fileWork.close();
    } else {
        cout << "Gagal menyimpan file Dikerjakan.txt\n";
    }

    ofstream fileQueue("Antrian.txt");
    if (fileQueue.is_open()) {
        fileQueue << OnQueue << endl;
        for (int i = 0; i < OnQueue; i++) {
            fileQueue << Antrian[i] << endl;
        }
        fileQueue.close();
    } else {
        cout << "Gagal menyimpan file Antrian.txt\n";
    }
    
    ofstream filePayment("Pembayaran.txt");
    if (filePayment.is_open()) {
        filePayment << OnPayment << endl;
        for (int i = 0; i < OnPayment; i++) {
            filePayment << Pembayaran[i] << endl;
        }
        filePayment.close();
    } else {
        cout << "Gagal pembayan file Antrian.txt\n";
    }
}

void LoadStatus() {
    ifstream fileWork("Dikerjakan.txt");
    if (fileWork.is_open()) {
        while(fileWork >> Dikerjakan[OnWork]){
            OnWork++;
        }
        fileWork.close();
    } else {
        cout << "File Dikerjakan.txt tidak ditemukan.\n";
    }

    ifstream fileQueue("Antrian.txt");
    if (fileQueue.is_open()) {
        while(fileQueue >> Antrian[OnQueue]){
            OnQueue++;
        }
        fileQueue.close();
    } else {
        cout << "File Antrian.txt tidak ditemukan.\n";
    }

    ifstream filePayment("Pembayaran.txt");
    if (filePayment.is_open()) {
        while(filePayment >> Pembayaran[OnPayment]){
            OnPayment++;
        }
        filePayment.close();
    } else {
        cout << "File Pembayaran.txt tidak ditemukan.\n";
    }
}

void Register(){
    int IDkerusakan;
    cin.ignore();
    cout << "Nama : "; getline(cin, cust[DataCust].nama);
    cout << "No Telepon : "; cin >> cust[DataCust].no_telp;
    cout << "Alamat : "; cin >> cust[DataCust].alamat;
    cout << "Email : "; cin >> cust[DataCust].email;

    cout << "Merk : "; cin >> cust[DataCust].motor.merk;
    cout << "Jenis : "; cin >> cust[DataCust].motor.jenis;
    cout << "Warna : "; cin >> cust[DataCust].motor.warna;
    cout << "No Polisi : "; cin >> cust[DataCust].motor.NoPol;
    
    for(int i = 0; i < 7; i++){
        cout << i+1 << ". " << Perbaikan[i] << endl;
    }
    cout << "\nJenis Kerusakan : "; cin >> IDkerusakan;
    IDkerusakan -= 1;
    cust[DataCust].perbaikan.jenisRusak = IDkerusakan;

    int totalSparepart;
    for(int i = 0; i < 19; i++){
        cout << i+1 << ". " << stok[i].barang << endl;
    }
    cout << "Butuh berapa Sparepart : "; cin >> totalSparepart;
    cust[DataCust].perbaikan.banyaksparepart = totalSparepart;
    for(int i = 0; i < totalSparepart; i++){
        int pil;
        cout << "Masukan sparepart " << i+1 << " : "; cin >> pil;
        pil -= 1;
        cust[DataCust].perbaikan.sparepart[i] = pil;
        cust[DataCust].totalharga += stok[pil].hargajual;
        stok[pil].QTY--;
    }

    int jasa = 20000;
    cust[DataCust].totalharga += jasa;

    // Set estimasi waktu dalam detik (konversi dari menit)
    cust[DataCust].detikEstimasi = WaktuPerbaikan[IDkerusakan] * 60;
    cust[DataCust].waktuMulai = time(0); // Set waktu mulai sekarang

    if(OnWork < 3){
        Dikerjakan[OnWork] = DataCust;
        OnWork += 1;
        cust[DataCust].keterangan = 1; // Sedang diperbaiki
    } else {
        Antrian[OnQueue] = DataCust;
        OnQueue += 1;
        cust[DataCust].keterangan = 2; // Antrian
    }
    DataCust += 1;
}

void CetakStruk(int id) {
    cout << "\n===== Struk Servis =====\n";
    cout << "Nama: " << cust[id].nama << endl;
    cout << "No Polisi: " << cust[id].motor.NoPol << endl;
    cout << "Jenis Perbaikan: " << Perbaikan[cust[id].perbaikan.jenisRusak] << endl;
    cout << "Total Harga: Rp" << cust[id].totalharga << endl;
    cout << "=========================\n\n";
}

void ListDikerjakan(){
    cout << "Kendaraan yang sedang dikerjakan : " << endl << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnWork; i++){
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.NoPol << "|" << endl;
    }
}

void Finish(){
    int pilih;
    cout << "\nKendaraan Mana yang finish : "; cin >> pilih;
    int IdMotor = Dikerjakan[pilih-1];

    Pembayaran[OnPayment] = IdMotor;
    OnPayment++;
    cust[IdMotor].keterangan = 3; // Proses pembayaran
    
    if(OnQueue == 0){
        for(int i = pilih-1; i < OnWork-1; i++){
            Dikerjakan[i] = Dikerjakan[i+1];
        }
        OnWork -= 1;
    } else if(OnQueue > 0){
        // Pindahkan dari antrian ke dikerjakan
        int newWork = Antrian[0];
        Dikerjakan[pilih-1] = newWork;
        cust[newWork].keterangan = 1; // Sedang diperbaiki
        cust[newWork].waktuMulai = time(0); // Update waktu mulai
        
        // Shift antrian
        for(int i = 0; i < OnQueue-1; i++){
            Antrian[i] = Antrian[i+1];
        }
        OnQueue -= 1;
    }
}

int WaktuMinimumDikerjakan() {
    int minWaktu = 0;
    for (int i = 0; i < OnWork; i++) {
        int id = Dikerjakan[i];
        int waktuServis = WaktuPerbaikan[cust[id].perbaikan.jenisRusak];
        if(i == 0){
            minWaktu = waktuServis;
            continue;
        }
        if(minWaktu > waktuServis){
            minWaktu = waktuServis;
        }
    }
    return(minWaktu);
}

void EstimasiDikerjakan() {
    cout << "Kendaraan yang sedang dikerjakan : " << endl << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << setw(19) << "Jenis Kerusakan" << "|" << setw(19) << "Estimasi (Menit)" << "|"  << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << right << setfill('=') << setw(20) << "|" << right << setfill('=') << setw(20) << "|" << endl;
    for (int i = 0; i < OnWork; i++) {
        int id = Dikerjakan[i];
        int waktuServis = WaktuPerbaikan[cust[id].perbaikan.jenisRusak];
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[id].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[id].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[id].motor.NoPol << "|" << left << setfill(' ') << setw(19) << Perbaikan[cust[id].perbaikan.jenisRusak] << "|" << left << setfill(' ') << setw(19) << waktuServis << "|" << endl;
    }
}

int EstimasiAntrianRekursif(int jumlah, int antri, int waktuAwal) {
    if (antri == jumlah) return 0;
    int id = Antrian[antri];
    int waktuServis = WaktuPerbaikan[cust[id].perbaikan.jenisRusak];
    cout << cust[id].motor.NoPol << " estimasi waktu: " << waktuAwal << " - " << (waktuAwal + waktuServis) << " menit\n";
    return waktuServis + EstimasiAntrianRekursif(jumlah, antri + 1, waktuAwal + waktuServis);
}

void TampilkanEstimasi() {
    cout << "\n=== Estimasi Waktu Servis ===\n";

    if (OnWork > 0) {
        cout << ">> Sedang Dikerjakan (Paralel dari waktu 0):\n";
        EstimasiDikerjakan();
    }

    if (OnQueue > 0) {
        int mulaiAntrian = WaktuMinimumDikerjakan();
        cout << "\n>> Dalam Antrian (Mulai dari waktu: " << mulaiAntrian << " menit):\n";
        EstimasiAntrianRekursif(OnQueue, 0, mulaiAntrian);
    }
}

void swap(Warehouse &a, Warehouse &b) {
    Warehouse temp = a;
    a = b;
    b = temp;
}

bool compare(Warehouse a, Warehouse b, string kriteria) {
    if (kriteria == "nama")
        return a.barang < b.barang;
    else if (kriteria == "QTY")
        return a.QTY < b.QTY;
    else if (kriteria == "harga")
        return a.hargajual < b.hargajual;
    return false;
}

int partition(Warehouse arr[], int low, int high, string kriteria) {
    Warehouse pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compare(arr[j], pivot, kriteria)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(Warehouse arr[], int low, int high, string kriteria) {
    if (low < high) {
        int pi = partition(arr, low, high, kriteria);
        quickSort(arr, low, pi - 1, kriteria);
        quickSort(arr, pi + 1, high, kriteria);
    }
}

bool Gudang(){
    int BarangGudang = 19;
    char pilih;

    cout << "Sparepart di Gudang : " << endl << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Nama Sparepart" << left << setfill(' ') << setw(20) << "|Jumlah Barang" << left << setfill(' ') << setw(20) << "|Harga Jual" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < BarangGudang; i++){
        cout << "| " << left << setfill(' ') << setw(3) << i+1 << "|" << left << setfill(' ') << setw(19) << stok[i].barang << "|" << left << setfill(' ') << setw(19) << stok[i].QTY << "|"  << left << setfill(' ') << setw(19) << stok[i].hargajual << "|" << endl;
    }

    cout << "1. Sort Nama Sparepart\n";
    cout << "2. Sort Jumlah Sparepart\n";
    cout << "3. Sort Harga Sparepart\n";
    cout << "4. Keluar\n";

    cout << "Pilih : "; cin >> pilih;
    switch(pilih){
        case '1':
            quickSort(stok, 0, BarangGudang-1, "nama");
        break;
        case '2':
            quickSort(stok, 0, BarangGudang-1, "QTY");
        break;
        case '3':
            quickSort(stok, 0, BarangGudang-1, "harga");
        break;
        case '4':
            return false; 
        break;
        default:
        break;
    }
    return true;
}

void ListAntrian(){
    cout << "Antrian servis : " << endl << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnQueue; i++){
        int IdMotor = Antrian[i];
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[IdMotor].motor.NoPol << "|" << endl;
    }
}

void ListPembayaran(){
    cout << "Kendaraan yang harus dibayar : " << endl << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnPayment; i++){
        int IdMotor = Pembayaran[i];
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[IdMotor].motor.NoPol << "|" << endl;
    }
}

void Bayar(){
    int pilih;
    cout << "\nKendaraan Mana yang selesai pembayaran : "; cin >> pilih;
    int IdMotor = Pembayaran[pilih-1];
    
    CetakStruk(IdMotor);
    
    // Update keterangan menjadi selesai/tidak diperbaiki
    cust[IdMotor].keterangan = 0;
    
    // Hapus dari array pembayaran
    for(int i = pilih-1; i < OnPayment-1; i++){
        Pembayaran[i] = Pembayaran[i+1];
    }
    OnPayment--;
}

int CariCust(string nopol){
    for(int i = 0; i < DataCust; i++){
        if(nopol == cust[i].motor.NoPol){
            return i;
        }
    }
    return -1;
}

// Fungsi untuk menampilkan status keterangan
string getStatusKeterangan(int keterangan) {
    switch(keterangan) {
        case 0: return "Tidak diperbaiki/Selesai";
        case 1: return "Sedang diperbaiki";
        case 2: return "Dalam antrian";
        case 3: return "Proses pembayaran";
        default: return "Status tidak diketahui";
    }
}

// Fungsi customer untuk cek status motor
void SearchCust(){
    string nopol;
    cout << "Masukan Nomor Polisi : "; cin >> nopol;
    int IdCust = CariCust(nopol);
    
    if(IdCust == -1){
        cout << "Tidak ada Nomor Polisi tersebut dalam database\n";
        return;
    }
    
    cout << "\n===== STATUS KENDARAAN =====\n";
    cout << "Nama Pemilik: " << cust[IdCust].nama << endl;
    cout << "No Polisi: " << cust[IdCust].motor.NoPol << endl;
    cout << "Merk: " << cust[IdCust].motor.merk << endl;
    cout << "Jenis: " << cust[IdCust].motor.jenis << endl;
    cout << "Jenis Perbaikan: " << Perbaikan[cust[IdCust].perbaikan.jenisRusak] << endl;
    cout << "Status: " << getStatusKeterangan(cust[IdCust].keterangan) << endl;
    
    // Tampilkan countdown jika sedang diperbaiki
    if(cust[IdCust].keterangan == 1) {
        int sisaDetik = hitungCountdown(cust[IdCust].detikEstimasi, cust[IdCust].waktuMulai);
        int menit = sisaDetik / 60;
        int detik = sisaDetik % 60;
        
        cout << "Sisa waktu perbaikan: " << menit << " menit " << detik << " detik" << endl;
        
        if(sisaDetik <= 0) {
            cout << "** PERBAIKAN SUDAH SELESAI! Silakan menuju ke kasir untuk pembayaran **" << endl;
        }
    }
    
    cout << "Total Biaya: Rp" << cust[IdCust].totalharga << endl;
    cout << "=============================\n\n";
}

void CustUI(){
    int pilih;
    while(true){
        cout << "\n=== MENU CUSTOMER ===\n";
        cout << "1. Cek Status Kendaraan\n";
        cout << "2. Keluar\n";
        cout << "Pilihan: "; cin >> pilih;
        
        switch(pilih) {
            case 1:
                SearchCust();
                break;
            case 2:
                return;
            default:
                cout << "Pilihan tidak valid!\n";
                break;
        }
    }
}

void AdminUI(){
    int pilih;
    while(true){
        cout << "1. Pendaftaran Motor\n";
        cout << "3. Gudang\n";
        cout << "4. Estimasi Waktu\n";
        cout << "6. List Dikerjakan\n";
        cout << "7. List Antrian\n";
        cout << "8. List Pembayaran\n";
        cout << "9. Save & Exit\n";
        cout << "Pilih Menu : "; cin >> pilih;
        switch (pilih){
            case 1:
                Register();
            break;
            case 2:

            break;
            case 3:
                while (Gudang()){}
            break;
            case 4:
                TampilkanEstimasi();
            break;
            case 6:
                if(OnWork == 0){
                    cout << "Tidak ada yang sedang dikerjakan" << endl;
                } else {
                    ListDikerjakan();
                    cout << "[0] Keluar" << endl;
                    cout << "[1] Selesaikan pekerjaan" << endl;
                    cout << "Pilih : "; cin >> pilih;
                    if(pilih == 1){
                        Finish();
                    }
                }
            break;
            case 7:
                if(OnPayment == 0){
                    cout << "Tidak ada pekerjaan yang selesai" << endl;
                } else {           
                    ListAntrian();
                }
            break;
            case 8:
                if(OnPayment == 0){
                    cout << "Tidak ada pekerjaan yang selesai" << endl;
                } else {            
                    ListPembayaran();
                    cout << "[0] Keluar" << endl;
                    cout << "[1] Pembayaran" << endl;
                    cout << "Pilih : "; cin >> pilih;
                    if(pilih == 1){
                        Bayar();
                    }
                }
            break;
            case 9:
                return;
            break;
            default:
            break;
        }
    }
}

int main(){
    int pilih;
    OpenFile();
    OpenStok();
    LoadStatus();

    AdminUI();
    CustUI();

    SaveFile();
    SaveStok();
    SaveStatus();
    return 0;
    
}