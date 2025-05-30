#include <iostream>
using namespace std;

struct Mahasiswa {
    char nim[20];
    char nama[50];
    float ipk;
    int semester;
    int penghasilanOrtu;
    Mahasiswa* next;
};

Mahasiswa* head = nullptr;

bool stringSama(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

void salinString(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void tambahMahasiswa(Mahasiswa data) {
    Mahasiswa* baru = new Mahasiswa;
    *baru = data;
    baru->next = nullptr;

    if (!head) head = baru;
    else {
        Mahasiswa* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }
}

void inputMahasiswa() {
    Mahasiswa data;
    cout << "NIM: ";
    cin >> data.nim;
    cout << "Nama: ";
    cin.ignore();
    cin.getline(data.nama, 50);
    cout << "IPK: ";
    cin >> data.ipk;
    cout << "Semester: ";
    cin >> data.semester;
    cout << "Penghasilan Orang Tua: ";
    cin >> data.penghasilanOrtu;

    tambahMahasiswa(data);
    cout << "Data ditambahkan.\n";
}

void tampilkanMahasiswa() {
    Mahasiswa* temp = head;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->nim << " - " << temp->nama 
             << " | IPK: " << temp->ipk 
             << " | Semester: " << temp->semester 
             << " | Penghasilan Ortu: " << temp->penghasilanOrtu << endl;
        temp = temp->next;
    }
}

void simpanKeFile() {
    FILE* file = fopen("data.txt", "w");
    if (!file) {
        cout << "Gagal menyimpan file (tidak bisa buka file).\n";
        return;
    }

    Mahasiswa* temp = head;
    while (temp) {
        fprintf(file, "%s;%s;%.2f;%d;%d\n", temp->nim, temp->nama, temp->ipk, temp->semester, temp->penghasilanOrtu);
        temp = temp->next;
    }

    fclose(file);
    cout << "Data disimpan ke file data.txt\n";
}

void muatDariFile() {
    FILE* file = fopen("data.txt", "r");
    if (!file) {
        // tidak tampilkan pesan agar tidak mengganggu saat pertama kali file belum ada
        return;
    }

    Mahasiswa temp;
    while (fscanf(file, "%[^;];%[^;];%f;%d;%d\n", temp.nim, temp.nama, &temp.ipk, &temp.semester, &temp.penghasilanOrtu) != EOF) {
        tambahMahasiswa(temp);
    }

    fclose(file);
}

void tukarData(Mahasiswa* a, Mahasiswa* b) {
    char nim[20], nama[50];
    float ipk;
    int semester, penghasilan;

    // simpan data b
    salinString(nim, b->nim);
    salinString(nama, b->nama);
    ipk = b->ipk;
    semester = b->semester;
    penghasilan = b->penghasilanOrtu;

    // salin a ke b
    salinString(b->nim, a->nim);
    salinString(b->nama, a->nama);
    b->ipk = a->ipk;
    b->semester = a->semester;
    b->penghasilanOrtu = a->penghasilanOrtu;

    // salin simpanan tadi ke a
    salinString(a->nim, nim);
    salinString(a->nama, nama);
    a->ipk = ipk;
    a->semester = semester;
    a->penghasilanOrtu = penghasilan;
}

void sortIPKDescending() {
    if (!head || !head->next) return;

    bool swapped;
    Mahasiswa* ptr;
    do {
        swapped = false;
        ptr = head;
        while (ptr->next) {
            if (ptr->ipk < ptr->next->ipk) {
                tukarData(ptr, ptr->next);
                swapped = true;
            }
            ptr = ptr->next;
        }
    } while (swapped);
    cout << "Data berhasil diurutkan berdasarkan IPK (descending).\n";
}

void cariByNIM() {
    char nimDicari[20];
    cout << "Masukkan NIM yang dicari: ";
    cin >> nimDicari;

    Mahasiswa* temp = head;
    while (temp) {
        if (stringSama(temp->nim, nimDicari)) {
            cout << "Ditemukan: " << temp->nama << " | IPK: " << temp->ipk << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Data tidak ditemukan.\n";
}

void menu() {
    int pilihan;
    muatDariFile();

    do {
        cout << "\n=== Menu Beasiswa ===\n";
        cout << "1. Tambah Mahasiswa\n";
        cout << "2. Tampilkan Data\n";
        cout << "3. Urutkan Berdasarkan IPK (Descending)\n";
        cout << "4. Cari Mahasiswa berdasarkan NIM\n";
        cout << "5. Simpan & Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: inputMahasiswa(); break;
            case 2: tampilkanMahasiswa(); break;
            case 3: sortIPKDescending(); break;
            case 4: cariByNIM(); break;
            case 5: simpanKeFile(); break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 5);
}

int main() {
    menu();
    return 0;
}