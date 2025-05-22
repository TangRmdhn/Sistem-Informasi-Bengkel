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
    int jenis;
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
}; Customer cust[100];

struct Warehouse{
    string barang;
    int QTY;
    int hargajual;
};

Warehouse stok[19] = {{"Oli", 10, 60000},
                     {"Filter Oli", 5, 40000},
                     {"Klep", 10, 40000},
                     {"Radiator", 5, 40000},
                     {"SetBlock Mesin", 5, 2000000},
                     {"Transmision Set", 5, 1000000},
                     {"Gearset", 5, 300000},
                     {"V Belt", 15, 40000},
                     {"Per CVT", 15, 40000},
                     {"Roller CVT", 15, 40000},
                     {"Brake Pad", 20, 40000},
                     {"Kaliper", 5, 950000},
                     {"Brake Disc", 10, 300000},
                     {"Selang Rem", 15, 100000},
                     {"Ban", 20, 200000},
                     {"Speedometer", 5, 40000},
                     {"Baut M6", 20, 2000},
                     {"Baut M4", 20, 2000},
                     {"Boshing", 10, 35000}};
                     
int DataCust = 0;

int Antrian[100] = {false};
int OnQueue = 0;

int Dikerjakan[3] = {false};
int OnWork = 0;

int Pembayaran[10] = {false};
int OnPayment = 0;

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
    cust[DataCust].perbaikan.jenis = IDkerusakan;

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
    }

    int jasa = 20000;

    cust[DataCust].totalharga += jasa;

    if(OnWork < 3){
        Dikerjakan[OnWork] = DataCust;
        OnWork += 1;
    } else {
        Antrian[OnQueue] = DataCust;
        OnQueue += 1;
    }
    DataCust += 1;
}

void Finish(){
    int pilih;
    cout << "Kendaraan yang sedang dikerjakan : " << endl << endl;
    cout << "| NO " << left << setfill(' ') << setw(20) << "|Merk" << left << setfill(' ') << setw(20) << "|Jenis" << left << setfill(' ') << setw(20) << "|No Polisi" << "|" << endl;
    cout << "|====" << left << setfill('=') << setw(20) << "|" << "|" << right << setfill('=') << setw(20) << "|"  << right << setfill('=') << setw(20) << "|" << endl;
    for(int i = 0; i < OnWork; i++){
        cout << "| " << i+1 << "  |" << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.merk << "|" << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.jenis << "|"  << left << setfill(' ') << setw(19) << cust[Dikerjakan[i]].motor.NoPol << "|" << endl;
    }
    cout << "\nKendaraan Mana yang finish : "; cin >> pilih;
    
    int IdMotor = pilih-1;

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

// int CountEstimate(int i, int waktu){
//     if(i == 0){
//         return waktu + cust[i].perbaikan.waktu;
//     }else {
//         waktu += CountEstimate(i, waktu)
//     }
// }

// void EstimateTime(){
//     int waktu;
//     for(int i = 0; i < OnWork+OnQueue; i++){
//         waktu += cust[i].perbaikan.waktu;
//     }
// }

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

void Gudang(){
    int BarangGudang = 3;
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
            return;
        break;
        default:
        break;
    }
}

int main(){
    int pilih;
    while(true){
        cout << "1. Daftar Motor\n";
        cout << "2. Finish\n";
        cout << "3. Gudang\n";
        cout << "Pilih Menu : "; cin >> pilih;
        switch (pilih){
            case 1:
                Register();
            break;
            case 2:
                if(OnWork == 0){
                    cout << "Tidak ada yang sedang dikerjakan" << endl;
                } else {
                    Finish();
                }
            break;
            case 3:
                while (true){
                    Gudang();
                }
            break;
            default:
            break;
        }
    }
}
