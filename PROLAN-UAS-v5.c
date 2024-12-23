#include <stdio.h>
#include <string.h>
#include <math.h>

#define DATABASE 10 // Jumlah maksimum produk dalam database

// Struktur untuk menyimpan data produk
struct Produk {
    char kode[15];
    char nama[50];
    char kategori[30];
    double harga;
    int stok;
};

// Fungsi untuk menampilkan semua produk
void tampilkanProduk(struct Produk produk[], int n) {
    printf("%-5s %-25s %-15s %-5s %10s\n", "Kode", "Nama", "Kategori", "Harga", "Stok");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5s %-25s %-15s %-5.f %10d\n", produk[i].kode, produk[i].nama, produk[i].kategori, produk[i].harga, produk[i].stok);
    }
}

// Fungsi untuk menyimpan data produk ke dalam file
void simpanData(struct Produk produk[], int n, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("\nGagal Menyimpan File...\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%d\n", produk[i].kode, produk[i].nama, produk[i].kategori, produk[i].harga, produk[i].stok);
    }
    fclose(file);
    printf("\nData tersimpan ke %s...\n", filename);
}

// Fungsi untuk memuat data produk dari file
int muatData(struct Produk produk[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("\nGagal Memuat File...\n");
        return 0;
    }
    int count = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%lf,%d\n", produk[count].kode, produk[count].nama, produk[count].kategori, &produk[count].harga, &produk[count].stok) != EOF) {
        count++;
        if (count >= DATABASE) break; // Batasi data yang dimuat hingga jumlah maksimum
    }
    fclose(file);
    printf("\n %d data dimuat dari %s\n", count, filename);
    return count;
}

// Fungsi untuk menggabungkan sub-array dalam merge sort
void merge(struct Produk produk[], int left, int mid, int right, int (*cmp)(struct Produk, struct Produk)) {
    int n1 = mid - left + 1, n2 = right - mid;
    struct Produk L[n1], R[n2];

    // Salin data ke array sementara
    for (int i = 0; i < n1; i++) L[i] = produk[left + i];
    for (int j = 0; j < n2; j++) R[j] = produk[mid + 1 + j];

    // Gabungkan kembali array sementara ke array utama
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) produk[k++] = (cmp(L[i], R[j]) <= 0) ? L[i++] : R[j++];
    while (i < n1) produk[k++] = L[i++];
    while (j < n2) produk[k++] = R[j++];
}

// Fungsi rekursif merge sort
void mergeSort(struct Produk produk[], int left, int right, int (*cmp)(struct Produk, struct Produk)) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(produk, left, mid, cmp); // Urutkan bagian kiri
        mergeSort(produk, mid + 1, right, cmp); // Urutkan bagian kanan
        merge(produk, left, mid, right, cmp); // Gabungkan keduanya
    }
}

// Fungsi pembanding berdasarkan berbagai atribut
int compareKode(struct Produk a, struct Produk b) { return strcmp(a.kode, b.kode); }
int compareNama(struct Produk a, struct Produk b) { return strcmp(a.nama, b.nama); }
int compareKategori(struct Produk a, struct Produk b) { return strcmp(a.kategori, b.kategori); }

// Fungsi Jump Search untuk pencarian data string
int jumpSearchString(struct Produk produk[], int n, const char *key, int (*cmp)(struct Produk, const char *)) {
    int jump = sqrt(n);
    int prev = 0;

    // Lompat dengan interval tertentu hingga mendekati data
    while (prev < n && cmp(produk[prev], key) < 0) {
        prev += jump;
    }

    // Lakukan pencarian linier pada interval
    for (int i = fmax(0, prev - jump); i < n && i <= prev; i++) {
        if (cmp(produk[i], key) == 0) {
            return i; // Kembalikan indeks data jika ditemukan
        }
    }
    return -1; // Kembalikan -1 jika data tidak ditemukan
}

// Fungsi pembanding untuk pencarian data string
int compareKodeString(struct Produk a, const char *key) { return strcmp(a.kode, key); }
int compareNamaString(struct Produk a, const char *key) { return strcmp(a.nama, key); }
int compareKategoriString(struct Produk a, const char *key) { return strcmp(a.kategori, key); }

int main() {
    int n = 10; // Jumlah data awal
    struct Produk produk[DATABASE] = {
        {"P008", "Tas Ransel", "Aksesoris", 65000, 10},
        {"P003", "Sneakers", "Sepatu", 80000, 15},
        {"P007", "Celana Jeans", "Pakaian", 50000, 20},
        {"P002", "Blazer", "Pakaian", 70000, 50},
        {"P006", "Topi Baseball", "Aksesoris", 25000, 25},
        {"P001", "Jaket Hoodie", "Pakaian", 55000, 8},
        {"P005", "Sepatu Formal", "Sepatu", 60000, 12},
        {"P009", "Jam Tangan", "Aksesoris", 75000, 5},
        {"P004", "Kacamata Hitam", "Aksesoris", 15000, 18},
        {"P010", "Kaos Kaki", "Pakaian", 10000, 100}
    };
    
    const char *filename = "Produk-Toko.txt";
    int isSorted = 0; // Menandakan apakah data sudah diurutkan atau belum
	int pilihan;

    // Menu utama
    do {
        printf("\n------ Menu Manajemen Produk ------\n");
        printf("1. Lihat Produk\n");
        printf("2. Urutkan Produk\n");
        printf("3. Cari Produk\n");
        printf("4. Simpan Data\n");
        printf("5. Muat Data\n");
        printf("0. Exit\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        if (pilihan == 0) {
            printf("Keluar...\n");
            break;
        }

        if (pilihan == 1) {
            tampilkanProduk(produk, n); // Menampilkan data produk
        } else if (pilihan == 2) {
            printf("\nUrutkan berdasarkan: \n");
			printf("1. Kode\n");
			printf("2. Nama\n");
			printf("3. Kategori\n");
			printf("Pilih: ");
            int subPilihan;
            scanf("%d", &subPilihan);

            // Urutkan data berdasarkan pilihan
            switch (subPilihan) {
                case 1:
                    mergeSort(produk, 0, n - 1, compareKode);
                    printf("\nData Produk diurutkan berdasarkan Kode:\n");
                    break;
                case 2:
                    mergeSort(produk, 0, n - 1, compareNama);
                    printf("\nData Produk diurutkan berdasarkan Nama:\n");
                    break;
                case 3:
                    mergeSort(produk, 0, n - 1, compareKategori);
                    printf("\nData Produk diurutkan berdasarkan Kategori:\n");
                    break;
                default:
                    printf("Pilihan tidak valid!\n");
                    continue;
            }
            
            isSorted = 1; // Tandai data sudah diurutkan
            tampilkanProduk(produk, n); // Tampilkan data setelah diurutkan
            
        } else if (pilihan == 3) {
            if (!isSorted) {
                printf("Data belum diurutkan! Harap urutkan terlebih dahulu.\n");
                continue;
            }

            printf("\nCari berdasarkan: \n");
			printf("1. Kode\n");
			printf("2. Nama\n");
			printf("3. Kategori\n");
			printf("Pilih: ");
            int subPilihan;
            scanf("%d", &subPilihan);

            char key[50]; // Kata kunci untuk pencarian
            int index; // Indeks data yang ditemukan

            // Cari data berdasarkan pilihan
            switch (subPilihan) {
                case 1:
                    printf("Masukkan Kode Produk: ");
                    scanf("%s", key);
                    index = jumpSearchString(produk, n, key, compareKodeString);
                    if (index != -1) {
                    	printf("\nHasil Pencarian:\n");
                    	printf("%-5s %-25s %-15s %-5s %10s\n", "Kode", "Nama", "Kategori", "Harga", "Stok");
                    	printf("----------------------------------------------------------------\n");
                    	printf("%-5s %-25s %-15s %-5.f %10d\n", produk[index].kode, produk[index].nama, produk[index].kategori, produk[index].harga, produk[index].stok);
            		} else {
                		printf("Data tidak ditemukan!\n");
            		}
                    break;
                case 2:
                    printf("Masukkan Nama Produk: ");
                    getchar();
                    fgets(key, sizeof(key), stdin);
                    key[strcspn(key, "\n")] = '\0'; // Hilangkan karakter newline
                    index = jumpSearchString(produk, n, key, compareNamaString);
                    if (index != -1) {
                    	printf("\nHasil Pencarian:\n");
                    	printf("%-5s %-25s %-15s %-5s %10s\n", "Kode", "Nama", "Kategori", "Harga", "Stok");
                    	printf("----------------------------------------------------------------\n");
                    	printf("%-5s %-25s %-15s %-5.f %10d\n", produk[index].kode, produk[index].nama, produk[index].kategori, produk[index].harga, produk[index].stok);
            		} else {
                		printf("Data tidak ditemukan!\n");
            		}
                    break;
                case 3:
                    printf("Masukkan Kategori Produk: ");
                    getchar();
                    fgets(key, sizeof(key), stdin);
                    key[strcspn(key, "\n")] = '\0'; // Hilangkan karakter newline
                    index = jumpSearchString(produk, n, key, compareKategoriString);
                    if (index != -1) {
                        printf("\nHasil Pencarian:\n");
                        printf("%-5s %-25s %-15s %-5s %10s\n", "Kode", "Nama", "Kategori", "Harga", "Stok");
                        printf("----------------------------------------------------------------\n");
                
                        int left = index;
                        while (left >= 0 && strcmp(produk[left].kategori, key) == 0) {
                            left--;  // Cari batas kiri
                        }
                        left++;  // Kembali ke elemen valid pertama

                        int right = index;
                        while (right < n && strcmp(produk[right].kategori, key) == 0) {
                            right++;  // Cari batas kanan
                        }
                        for (int i = left; i < right; i++) {
                            printf("%-5s %-25s %-15s %-5.f %10d\n", produk[i].kode, produk[i].nama, produk[i].kategori, produk[i].harga, produk[i].stok);
                        }
                	} else {
                    	printf("Data tidak ditemukan!\n");
                	}
                    break;
                default:
                    printf("Pilihan tidak valid!\n");
                    continue;
            	} 
			} else if (pilihan == 4) {
            simpanData(produk, n, filename); // Simpan data ke file
        } else if (pilihan == 5) {
            n = muatData(produk, filename); // Muat data dari file
        } else {
            printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 0);

    return 0;
}
