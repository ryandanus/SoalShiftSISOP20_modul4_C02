# SoalShiftSISOP20_modul4_C02


### Kelompok 
- Feinard             05111840000081
- Ryan Danu Saputra   05111840000144

SOAL 
  - [Nomor 1](#nomor-1)
  - [Nomor 4](#nomor-4)

JAWABAN
  - [1](#jawaban-1)
  - [4](#jawaban-4)

## Nomor 1
### Soal 1 :
Enkripsi versi 1:
a. Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
b. Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.
d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
e. Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.
```9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO```
Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10
“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg
Note : Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt.
f. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.


## Nomor 4
### Soal 4 :
Log system:

Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
Sisanya, akan dicatat dengan level INFO.
Format untuk logging yaitu:

[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]


LEVEL    : Level logging
yy   	 : Tahun dua digit
mm    	 : Bulan dua digit
dd    	 : Hari dua digit
HH    	 : Jam dua digit
MM    	 : Menit dua digit
SS    	 : Detik dua digit
CMD     	 : System call yang terpanggil
DESC      : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::)

Contoh format logging nantinya seperti:

INFO::200419-18:29:28::MKDIR::/iz1
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg



### Jawaban 1
Source Code : [4.c](https://github.com/ryandanus/SoalShiftSISOP20_modul4_C02/blob/master/sshc.c)
untuk menyelesaikan permasalahan nomer 1 dibuatlah beberapa fungsi dengan memiliki fungsi utama yaitu pengecekan path dan remake 
```
void findSubstring(char *s, char *sub, int p, int l)
{
    int c = 0;
    while (c < l)
    {
        sub[c] = s[p + c];
        c++;
    }
    sub[c] = '\0';
}
```
findSubstring untuk melakukan peengecekan melalui subtsring 
```
char *encrypt(char *str, bool check)
{
    int i, j, k = 0;
    char *ext = strrchr(str, '.');
    if (check && ext != NULL)
        k = strlen(ext);
    for (i = 0; i < strlen(str) - k; i++)
    {
        for (j = 0; j < 87; j++)
        {
            if (str[i] == key[j])
            {
                str[i] = key[(j + keygen) % 87];
                break;
            }
        }
    }
    return str;
}

char *decrypt(char *str, bool check)
{
    int i, j, k = 0;
    char *ext = strrchr(str, '.');
    if (check && ext != NULL)
        k = strlen(ext);
    for (i = 0; i < strlen(str) - k; i++)
    {
        for (j = 0; j < 87; j++)
        {
            if (str[i] == key[j])
            {
                str[i] = key[(j + 87 - keygen) % 87];
                break;
            }
        }
    }
    return str;
}
```
fungsi tersebut berfungsi untuk enkripsi dan dekripsi dimana melakukan pengondisian
`char *ext = strrchr(str, '.'); ` untuk tidak membaca file setelah (.) 
contoh : jika ada file bernama sisop.zip maka tulisan ".zip" akan ke-skip

dan menggunakan caesar cipher 
```
if (str[i] == key[j])
            {
                str[i] = key[(j + 87 - keygen) % 87];
                break;
            }
```

Untuk semua pemanggilan lokasi diperlukan pengubahan. Jika ditemukan encv1_ maka diperlukan untuk enkripsi nama sehingga program tidak salah membaca nama file
```c
char *checkPath(char *str)
{
    bool encr;
    int start, id;
    encr = 0;
    start = 1;
    id = strchr(str + start, '/') - str - 1;
    char currentPath[1024];
    while (id < strlen(str))
    {
        strcpy(currentPath, "");
        strncpy(currentPath, str + start, id - start + 1);
        currentPath[id - start + 1] = '\0';
        if (encr)
        {
            encrypt(currentPath, 0);
            strncpy(str + start, currentPath, id - start + 1);
        }
        if (!encr && strstr(str + start, "encv1_") == str + start)
            encr = 1;
        start = id + 2;
        id = strchr(str + start, '/') - str - 1;
    }
    id = strlen(str);
    id--;
    strncpy(currentPath, str + start, id - start + 1);
    currentPath[id - start + 1] = '\0';
    if (encr)
    {
        encrypt(currentPath, 1);
        strncpy(str + start, currentPath, id - start + 1);
    }
    return str;
}
```

lalu terdapat fungsi dibawah ini untuk mengecek di datapath apakah terenkripsi/tidak
```
void loopAllEnc1(char *str, int flag)
{
    struct dirent *dp;
    DIR *dir = opendir(str);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char path[2000000], name[1000000], newname[1000000];
            datapath(path, str, dp->d_name);
            strcpy(name, dp->d_name);
            if (flag == 1)
                datapath(newname, str, encrypt(name, 1));
            else if (flag == -1)
                datapath(newname, str, decrypt(name, 1));
            if (dp->d_type == DT_REG)
                rename(path, newname);
            else if (dp->d_type == DT_DIR)
            {
                rename(path, newname);
                loopAllEnc1(newname, flag);
            }
        }
    }
}

```
Untuk flag = 1, menandakan bahwa program harus mengenkripsi nama file yang ada. Jika flag = -1, maka program harus melakukan dekripsi. Hal ini terjadi apabila ada pengubahan path menjadi dekripsi/ diluar folder encv1_



### Jawaban 4
Source Code : [4.c](https://github.com/ryandanus/SoalShiftSISOP20_modul4_C02/blob/master/sshc.c)

```c

void logDatabase(int warning, char *type, char *path)
{
    printf("logDatabase STARTED............\n");
    char printLoc[1000];
    memset(printLoc, NULL, sizeof(printLoc));
    if (warning == 1)
    {
        strcat(printLoc, "INFO::");
    }
    else
    {
        strcat(printLoc, "WARNING::");
    }

    char hours[3], minutes[3], seconds[3], day[3], month[3], year[3];

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    sprintf(hours, "%02d", (local->tm_hour));
    sprintf(minutes, "%02d", (local->tm_min));
    sprintf(seconds, "%02d", (local->tm_sec));

    sprintf(day, "%02d", (local->tm_mday) % 32);
    sprintf(month, "%02d", local->tm_mon + 1);
    sprintf(year, "%02d", (local->tm_year + 1900) % 100);

    strcat(printLoc, year);
    strcat(printLoc, month);
    strcat(printLoc, day);
    strcat(printLoc, "-");
    strcat(printLoc, hours);
    strcat(printLoc, ":");
    strcat(printLoc, minutes);
    strcat(printLoc, ":");
    strcat(printLoc, seconds);
    strcat(printLoc, "::");
    strcat(printLoc, type);
    strcat(printLoc, "::");
    strcat(printLoc, path);
    FILE *logfile;
    logfile = fopen("/home/feinard/fs.log", "a+");
    if (logfile == NULL)
    {
        printf("FILE ERROR\n");
        return;
    }
    printf("%s\n", printLoc);
    fprintf(logfile, "%s\n", printLoc);
    printf("DONE\n");
    fclose(logfile);
    return;
}
void logDatabase2(int warning, char *type, char *path, char *path2)
{
    printf("logDatabase STARTED............\n");
    char printLoc[1000];
    memset(printLoc, NULL, sizeof(printLoc));
    if (warning == 1)
    {
        strcat(printLoc, "INFO::");
    }
    else
    {
        strcat(printLoc, "WARNING::");
    }

    char hours[3], minutes[3], seconds[3], day[3], month[3], year[3];

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    sprintf(hours, "%02d", (local->tm_hour));
    sprintf(minutes, "%02d", (local->tm_min));
    sprintf(seconds, "%02d", (local->tm_sec));

    sprintf(day, "%02d", (local->tm_mday) % 32);
    sprintf(month, "%02d", local->tm_mon + 1);
    sprintf(year, "%02d", (local->tm_year + 1900) % 100);

    strcat(printLoc, year);
    strcat(printLoc, month);
    strcat(printLoc, day);
    strcat(printLoc, "-");
    strcat(printLoc, hours);
    strcat(printLoc, ":");
    strcat(printLoc, minutes);
    strcat(printLoc, ":");
    strcat(printLoc, seconds);
    strcat(printLoc, "::");
    strcat(printLoc, type);
    strcat(printLoc, "::");
    strcat(printLoc, path);
    strcat(printLoc, "::");
    strcat(printLoc, path2);
    FILE *logfile;
    logfile = fopen("/home/feinard/fs.log", "a+");
    if (logfile == NULL)
    {
        printf("FILE ERROR\n");
        return;
    }
    printf("%s\n", printLoc);
    fprintf(logfile, "%s\n", printLoc);
    printf("DONE\n");
    fclose(logfile);
    return;
}
```

### penjelasan 

Untuk setiap pemanggilan fungsi, dipanggil logDatabase baik yang pertama maupun kedua. Untuk logDatabase1, pertama dilakukan pengecekan warning. Int warning ini berasal dari pemanggilan rm atau rmdir,selain itu warning = 0 . Setelah itu dilakukan pemanggilan time.h dengan localtime untuk mengambil waktu yang nantinya disimpan di fs.log 

  ```c
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  ```
  Setelah itu disimpan ddidalam string dan dilakukan sprintf ke variable printloc
  ```c
  sprintf(hours, "%02d", (local->tm_hour));
  sprintf(minutes, "%02d", (local->tm_min));
  sprintf(seconds, "%02d", (local->tm_sec));

  sprintf(day, "%02d", (local->tm_mday) % 32);
  sprintf(month, "%02d", local->tm_mon + 1);
  sprintf(year, "%02d", (local->tm_year + 1900) % 100);

  strcat(printLoc, year);
  strcat(printLoc, month);
  strcat(printLoc, day);
  strcat(printLoc, "-");
  strcat(printLoc, hours);
  strcat(printLoc, ":");
  strcat(printLoc, minutes);
  strcat(printLoc, ":");
  strcat(printLoc, seconds);
  strcat(printLoc, "::");
  strcat(printLoc, type);
  strcat(printLoc, "::");
  strcat(printLoc, path);
  strcat(printLoc, "::");
  strcat(printLoc, path2);
  ```
Setelah itu dilakukan pembukaan file dari /home/[user]/fs.log dengan permision a dan dilakukan fprintf printloc dan path yang ada. Untuk logDatabase2 ditambah dengan path kedua.
Contoh pemanggilan fungsi logDatabase1
```c
logDatabase(1, "touch/echo", fpath);
```
Contoh pemanggilan fungsi logDatabase2
```c
    logDatabase2(1, "RENAME", fpath, tpath);
```    

agar keluar sesuai dengan contoh soal
```
INFO::200419-18:29:28::MKDIR::/iz1
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg
```
# Kendala
