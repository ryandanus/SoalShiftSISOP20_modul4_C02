# SoalShiftSISOP20_modul4_C02


### Kelompok 
- Feinard             05111840000081
- Ryan Danu Saputra   05111840000144

SOAL 
  - [Nomor 4](#nomor-4)

JAWABAN
  - [4](#jawaban-4)


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




### Jawaban 4c
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
