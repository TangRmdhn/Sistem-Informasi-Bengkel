// bengkel.cpp
#include <iostream>
#include <iomanip>
#include <fstream>

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
    int ket;                // 0 : Sudah Selesai | 1 : Sedang dikerjakan | 2 : Antrian | 3 : Pembayaran 
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
    int ket;

    string nama;
    string no_telp;
    string alamat;
    string email;
    int totalharga;

    string merk;
    string jenis;
    string warna;
    string NoPol;

    int jenisRusak;
    int banyaksparepart;
    int sparepart[5];

    ifstream Database(namaFile);
    if (Database.is_open()){
        while (Database >> ket >> nama >> no_telp >> alamat >> email >> totalharga >> merk >> jenis >> warna >> NoPol >> jenisRusak >> banyaksparepart) {
            
            // Membaca array sparepart sebanyak banyaksparepart
            for (int i = 0; i < banyaksparepart; ++i) {
                Database >> sparepart[i];
            }

            // Simpan ke struct
            cust[DataCust].ket = ket;

            cust[DataCust].nama = nama;
            cust[DataCust].no_telp = no_telp;
            cust[DataCust].alamat = alamat;
            cust[DataCust].email = email;
            cust[DataCust].totalharga = totalharga;

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
            Database << cust[i].ket << " "
                     << cust[i].nama << " "
                     << cust[i].no_telp << " "
                     << cust[i].alamat << " "
                     << cust[i].email << " "
                     << cust[i].totalharga << " "
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
        for(int i = 0; i < OnWork; i++) {
            fileWork << Dikerjakan[i] << endl;
        }
        fileWork.close();
    } else {
        cout << "Gagal menyimpan file Dikerjakan.txt\n";
    }

    ofstream fileQueue("Antrian.txt");
    if (fileQueue.is_open()) {
        for (int i = 0; i < OnQueue; i++) {
            fileQueue << Antrian[i] << endl;
        }
        fileQueue.close();
    } else {
        cout << "Gagal menyimpan file Antrian.txt\n";
    }
    
    ofstream filePayment("Pembayaran.txt");
    if (filePayment.is_open()) {
        for (int i = 0; i < OnPayment; i++) {
            filePayment << Pembayaran[i] << endl;
        }
        filePayment.close();
    } else {
        cout << "Gagal menyimpan file Pembayaran.txt\n";
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

    int que;
    ifstream fileQueue("Antrian.txt");
    if (fileQueue.is_open()) {
        while(fileQueue >> que){
            Antrian[OnQueue] = que;
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

    if(OnWork < 3){
        Dikerjakan[OnWork] = DataCust;
        OnWork += 1;
        cust[DataCust].ket = 1;
    } else {
        Antrian[OnQueue] = DataCust;
        OnQueue += 1;
        cust[DataCust].ket = 2;
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
    cout << setfill('=') << setw(67) << " " << endl;
    cout << "|" << right << setfill(' ') << setw(26) << ' ' << "Sedang Dikerjakan" << right << setfill(' ') << setw(22) << "|" << endl;
    cout << "|" << right << setfill('=') << setw(65) << "|" << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnWork; i++){
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.NoPol << "|" << endl;
    }
    cout << right << setfill('=') << setw(67) << " " << endl;
}

void ListAntrian(){
    cout << setfill('=') << setw(67) << " " << endl;
    cout << "|" << right << setfill(' ') << setw(27) << ' ' << "Antrian Service" << right << setfill(' ') << setw(23) << "|" << endl;
    cout << "|" << right << setfill('=') << setw(65) << "|" << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnQueue; i++){
        int IdMotor = Antrian[i];
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[IdMotor].motor.NoPol << "|" << endl;
    }
    cout << right << setfill('=') << setw(67) << " " << endl;
}

void ListPembayaran(){
    cout << setfill('=') << setw(67) << " " << endl;
    cout << "|" << right << setfill(' ') << setw(20) << ' ' << "Kendaraan yang sudah selesai" << right << setfill(' ') << setw(17) << "|" << endl;
    cout << "|" << right << setfill('=') << setw(65) << "|" << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnPayment; i++){
        int IdMotor = Pembayaran[i];
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[IdMotor].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[IdMotor].motor.NoPol << "|" << endl;
    }
    cout << right << setfill('=') << setw(67) << " " << endl;
}

void Finish(){
    int pilih;

    cout << "\nMotor Mana yang sudah selesai : "; cin >> pilih;

    int IdMotor = pilih-1;

    cust[Pembayaran[IdMotor]].ket = 3;

    Pembayaran[OnPayment] = Dikerjakan[IdMotor];
    OnPayment++;
    
    if(OnQueue == 0){
        for(int i = IdMotor; i < 3; i++){
            Dikerjakan[i] = Dikerjakan[i+1];
        }
        OnWork -= 1;
    } else if(OnQueue > 0){
        Dikerjakan[IdMotor] = Antrian[0];
        OnQueue -= 1;   
    }

    for(int i = 0; i < OnQueue; i++){
        Antrian[i] = Antrian[i+1];
    }
}

void Payment(){
    int pilih;

    cout << "\nKendaraan Mana yang ingin dibayar : "; cin >> pilih;
    int index = pilih-1;
    int idCustomer = Pembayaran[index];

    cust[idCustomer].ket = 0;

    CetakStruk(idCustomer);

    for(int i = index; i < OnPayment-1; i++){
        Pembayaran[i] = Pembayaran[i + 1];
    }
    
    OnPayment--;
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
        // cout << cust[id].motor.NoPol << " estimasi waktu: 0 - " << waktuServis << " menit\n";

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

    cout << setfill('=') << setw(67) << " " << endl;
    cout << "|" << right << setfill(' ') << setw(29) << ' ' << "Sparepart" << right << setfill(' ') << setw(27) << "|" << endl;
    cout << "|" << right << setfill('=') << setw(65) << "|" << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Nama Sparepart" << left << setfill(' ') << setw(20) << "|Jumlah Barang" << left << setfill(' ') << setw(20) << "|Harga Jual" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < BarangGudang; i++){
        cout << "| " << left << setfill(' ') << setw(3) << i+1 << "|" << left << setfill(' ') << setw(19) << stok[i].barang << "|" << left << setfill(' ') << setw(19) << stok[i].QTY << "|"  << left << setfill(' ') << setw(19) << stok[i].hargajual << "|" << endl;
    }
    cout << right << setfill('=') << setw(67) << " " << endl;

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

void Bayar(){
    int pilih;
    cout << "\nKendaraan Mana yang finish : "; cin >> pilih;

    CetakStruk(pilih-1);
}

void AdminUI(){
    int pilih;
    while(true){
        system("cls");
        cout << "=========== Main Menu ===========\n\n";
        cout << "1. Pendaftaran Motor\n";
        cout << "2. Gudang\n";
        cout << "3. Estimasi Waktu\n";
        cout << "4. List Dikerjakan\n";
        cout << "5. List Antrian\n";
        cout << "6. List Pembayaran\n";
        cout << "7. Save & Exit\n";
        cout << "Pilih Menu : "; cin >> pilih;
        switch (pilih){
            case 1:
                Register();
            break;
            case 2:
                while (Gudang()){}
            break;
            case 3:
                TampilkanEstimasi();
            break;
            case 4:
                if(OnWork == 0){
                    cout << "Tidak ada yang sedang dikerjakan" << endl;
                } else {
                    int pilih2;
                    ListDikerjakan();
                    cout << "[0] Keluar" << endl;
                    cout << "[1] Selesaikan pekerjaan" << endl;
                    cout << "Pilih : "; cin >> pilih2;
                    if(pilih2 == 1){
                        Finish();
                    }
                }
            break;
            case 5:
                if(OnPayment == 0){
                    cout << "Tidak ada pekerjaan yang selesai" << endl;
                } else {           
                    ListAntrian();
                }
            break;
            case 6:
                if(OnPayment == 0){
                    cout << "Tidak ada pekerjaan yang selesai" << endl;
                } else {
                    int pilih2;
                    ListPembayaran();
                    cout << "[0] Keluar" << endl;
                    cout << "[1] Pembayaran" << endl;
                    cout << "Pilih : "; cin >> pilih2;
                    if(pilih2 == 1){
                        Payment();
                    }
                }
            break;
            case 7:
                return;
            break;
            default:
            break;
        }
    }
}

// Fungsi untuk mencari posisi customer dalam antrian
int CariPosisiAntrian(int IdCust) {
    for(int i = 0; i < OnQueue; i++) {
        if(IdCust == Antrian[i]) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk mencari posisi customer dalam list yang sedang dikerjakan
int CariPosisiDikerjakan(int IdCust) {
    for(int i = 0; i < OnWork; i++) {
        if(IdCust == Dikerjakan[i]) {
            return i;
        }
    }
    return -1;
}

// Fungsi estimasi waktu yang diperbaiki
void EstimasiCust(int IdCust) {
    system("cls");
    int ket = cust[IdCust].ket;
    int waktu = 0;
    
    cout << "=== Estimasi Waktu Servis ===\n";
    cout << "Nama\t\t\t: " << cust[IdCust].nama << endl;
    cout << "No Polisi\t\t: " << cust[IdCust].motor.NoPol << endl;
    cout << "Jenis Perbaikan\t\t: " << Perbaikan[cust[IdCust].perbaikan.jenisRusak] << endl;
    cout << "Status\t\t\t: ";
    
    if(ket == 1) {
        // Sedang dikerjakan
        cout << "Sedang Dikerjakan" << endl;
        waktu = WaktuPerbaikan[cust[IdCust].perbaikan.jenisRusak];
        cout << "Estimasi waktu selesai: 0 - " << waktu << " menit" << endl;
        cout << "Motor Anda sedang dalam proses perbaikan." << endl;
        
    } else if(ket == 2) {
        // Dalam antrian
        cout << "Dalam Antrian" << endl;
        
        // Waktu minimum dari yang sedang dikerjakan
        int waktuMinimum = WaktuMinimumDikerjakan();
        
        // Cari posisi dalam antrian
        int posisiAntrian = CariPosisiAntrian(IdCust);
        
        if(posisiAntrian != -1) {
            // Hitung waktu tunggu dari antrian sebelumnya
            waktu = waktuMinimum;
            for(int i = 0; i < posisiAntrian; i++) {
                int idAntrian = Antrian[i];
                waktu += WaktuPerbaikan[cust[idAntrian].perbaikan.jenisRusak];
            }
            
            // Tambahkan waktu untuk mengerjakan motor customer ini
            int waktuPerbaikan = WaktuPerbaikan[cust[IdCust].perbaikan.jenisRusak];
            
            cout << "\nPosisi dalam antrian\t\t: " << (posisiAntrian + 1) << endl;
            cout << "Estimasi mulai dikerjakan\t: " << waktu << " menit lagi" << endl;
            cout << "Estimasi selesai\t\t: " << (waktu + waktuPerbaikan) << " menit lagi" << endl;
        } else {
            cout << "Error: Motor tidak ditemukan dalam antrian!" << endl;
        }
        
    } else if(ket == 3) {
        // Selesai, menunggu pembayaran
        cout << "Selesai - Menunggu Pembayaran" << endl;
        cout << "Kendaraan sudah selesai diperbaiki!" << endl;
        cout << "Silahkan ke kasir untuk pembayaran." << endl;
        cout << "Total yang harus dibayar: Rp" << cust[IdCust].totalharga << endl;
        
    } else if(ket == 0) {
        // Sudah selesai dan dibayar
        cout << "Sudah Selesai" << endl;
        cout << "Kendaraan Anda telah selesai diperbaiki dan dibayar." << endl;
        cout << "Terima kasih telah menggunakan layanan kami!" << endl;
        
    } else {
        cout << "Status tidak dikenal" << endl;
    }
    
    cout << setfill('=') << setw(40) << ' ' << endl;
}

// Fungsi untuk menampilkan estimasi semua customer (untuk admin)
void TampilkanSemuaEstimasi() {
    cout << "\n=== Estimasi Waktu Semua Customer ===\n";
    
    if(OnWork > 0) {
        cout << "\n>> Sedang Dikerjakan:\n";
        for(int i = 0; i < OnWork; i++) {
            int id = Dikerjakan[i];
            int waktu = WaktuPerbaikan[cust[id].perbaikan.jenisRusak];
            cout << (i+1) << ". " << cust[id].motor.NoPol 
                 << " (" << cust[id].nama << ") - " 
                 << Perbaikan[cust[id].perbaikan.jenisRusak]
                 << " | Estimasi: 0-" << waktu << " menit" << endl;
        }
    }
    
    if(OnQueue > 0) {
        cout << "\n>> Dalam Antrian:\n";
        int waktuMulai = WaktuMinimumDikerjakan();
        int waktuAkumulasi = waktuMulai;
        
        for(int i = 0; i < OnQueue; i++) {
            int id = Antrian[i];
            int waktuPerbaikan = WaktuPerbaikan[cust[id].perbaikan.jenisRusak];
            
            cout << (i+1) << ". " << cust[id].motor.NoPol 
                 << " (" << cust[id].nama << ") - " 
                 << Perbaikan[cust[id].perbaikan.jenisRusak]
                 << " | Mulai: " << waktuAkumulasi 
                 << " | Selesai: " << (waktuAkumulasi + waktuPerbaikan) << " menit" << endl;
            
            waktuAkumulasi += waktuPerbaikan;
        }
    }
    
    if(OnPayment > 0) {
        cout << "\n>> Menunggu Pembayaran:\n";
        for(int i = 0; i < OnPayment; i++) {
            int id = Pembayaran[i];
            cout << (i+1) << ". " << cust[id].motor.NoPol 
                 << " (" << cust[id].nama << ") - " 
                 << "Total: Rp" << cust[id].totalharga << endl;
        }
    }
    
    cout << "\n====================================\n";
}

// Fungsi untuk mencari nomor polisi yang diperbaiki
int CariEmail(string email) {
    
    for(int i = 0; i < DataCust; i++) {
        string emailCust = cust[i].email;
        if(email == emailCust) {
            return i;
        }
    }
    return -1;
}

// Fungsi customer UI yang diperbaiki
void CustUI() {
    string email;
    cout << "\n=== Cek Status Kendaraan ===\n";
    cout << "Masukan Email: "; 
    cin >> email;
    
    int IdCust = CariEmail(email);
    if(IdCust == -1) {
        cout << "\nTidak ada kendaraan dengan Email tersebut dalam sistem.\n";
        cout << "Pastikan Email yang Anda masukkan benar.\n";
    } else {
        EstimasiCust(IdCust);
    }
    
    cout << "\nTekan Enter untuk kembali ke menu utama...";
    cin.ignore();
    cin.get();
}

int main(){
    string user = "admin", pass = "admin";
    char pilih;
    OpenFile();
    OpenStok();
    LoadStatus();

    while(true){
        system("cls");
        cout << "Masuk sebagai : \n";
        cout << "[1] Admin\n";
        cout << "[2] Customer\n";
        cout << "Pilih : "; cin >> pilih;
        switch(pilih){
            case '1':
                system("cls");
                cout << "=========== LOGIN ===========\n\n";
                cout << "Masukan Username : "; cin >> user;
                cout << "Masukan Password : "; cin >> pass;
                if(user == "admin" && pass == "admin"){
                    AdminUI();
                }
            break;
            case '2':
                CustUI();
            break;
            default:
                cout << "\nInput tidak valid\n\n";
                system("pause");
            continue;
        }
    }


    // SaveFile();
    // SaveStok();
    // SaveStatus();
    return 0;
    
}