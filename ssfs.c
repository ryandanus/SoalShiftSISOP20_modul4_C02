#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/feinard/Downloads";
char key[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
int keyloc = 10;

void encode(char *final, char *input)
{
    if (!strcmp(input, ".") || !strcmp(input, ".."))
        return;

    int key_index = 0;

    char *end = input + strlen(input);
    char extensi[1000];

    while (end > input && *end != '.')
    {     
        if(*end == '.') break;
        extensi[key_index];
        key_index++;
        --end;
    }

    if (end > input)
    {
        *end = '\0';
    }
    key_index = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        while (key_index < 88)
        {
            if (input[i] == key[key_index])
            {
                final[i] = key[(key_index + keyloc) % 88];
                break;
            }
            key_index++;
        }
    }
}

void decode(char *final, char *input)
{
    if (!strcmp(input, ".") || !strcmp(input, ".."))
        return;

    int key_index = 0;
    char extensi[1000];
    char *end = input + strlen(input);

    while (end > input && *end != '.')
    {
        if(*end == '.') break;
        extensi[key_index];
        key_index++;
        --end;
    }

    if (end > input)
    {
        *end = '\0';
    }

    for (int i = 0; i < strlen(input); i++)
    {
        while (key_index < 88)
        {
            if (input[i] == key[key_index])
            {
                final[i] = key[(key_index - keyloc) % 88];
                break;
            }
            key_index++;
        }
    }
}

// void check(char *final, char *input)
// {
//     char *temp;
//     temp = strtok(input, "/");
//     char *pwd = strrchr(input, ".");
//     while (temp != NULL)
//     {
//         for (int i = 0; i < strlen(temp) - 1; i++)
//         {
//             if (temp[i] == ".")
//             {
//                 int flag = 0;
//                 int loop = 1;
//                 char ext[100];
//                 while (i < strlen(temp) - 1)
//                 {
//                     if (temp[i] != pwd[loop])
//                     {
//                         flag++;
//                         break;
//                     }
//                     i++;
//                     loop++;
//                 }
//                 if (flag != 0)
//                 {
//                     enc(final, temp);
//                 }
//                 if (flag == 0)
//                 {
//                     char extrm[100];
//                     strcpy(extrm, temp);

//                     char *end = extrm + strlen(extrm);

//                     while (end > extrm && *end != '.')
//                     {
//                         --end;
//                     }

//                     if (end > extrm)
//                     {
//                         *end = '\0';
//                     }
//                     enc(final, extrm);
//                 }
//             }
//             if (i == strlen(temp) - 2 && temp[i] != ".")
//             {
//                 enc(final, temp);
//             }
//         }
//         temp = strtok(NULL, "/");
//     }
// }

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

static int xmp_getattr(const char *path, struct stat *stbuf)
{

    int res;
    char fpath[1000];
    char name[1000];
    sprintf(name, "%s", path);
    sprintf(fpath, "%s%s", dirpath, name);
    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    int res;
    DIR *dp;
    struct dirent *de;

    (void)offset;
    (void)fi;
    char fpath[1000];
    char name[1000];
    if (strcmp(path, "/") == 0)
    {
        sprintf(fpath, "%s", dirpath);
    }
    else
    {
        sprintf(name, "%s", path);

        sprintf(fpath, "%s%s", dirpath, name);
    }
    dp = opendir(fpath);

    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        res = filler(buf, de->d_name, &st, 0);
        if (res != 0)
            break;
    }
    closedir(dp);
    logDatabase(1, "LS", fpath);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }

    int res = 0;
    int fd = 0;

    (void)fi;

    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    res = mkdir(fpath, mode);   
    logDatabase(1, "MKDIR", fpath);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_unlink(const char *path)
{
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    res = unlink(fpath);
    
    logDatabase(0, "RM", fpath);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_rmdir(const char *path)
{
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }

    res = rmdir(fpath);
    logDatabase(0, "RMDIR", fpath);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    int res;
    char fpath[1000];
    char tpath[1000];
    if (!strcmp(from, "/"))
    {
        from = dirpath;
        sprintf(fpath, "%s", from);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, from);
    }
    if (!strcmp(to, "/"))
    {
        to = dirpath;
        sprintf(tpath, "%s", to);
    }
    else
    {
        sprintf(tpath, "%s%s", dirpath, to);
    }
    res = rename(fpath, tpath);
    logDatabase2(1, "RENAME", fpath, tpath);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    int res;
    char fpath[1000];
    char tpath[1000];
    if (!strcmp(from, "/"))
    {
        from = dirpath;
        sprintf(fpath, "%s", from);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, from);
    }
    if (!strcmp(to, "/"))
    {
        to = dirpath;
        sprintf(tpath, "%s", to);
    }
    else
    {
        sprintf(tpath, "%s%s", dirpath, to);
    }

    res = link(from, to);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }

    res = chmod(fpath, mode);
    logDatabase(1, "CHMOD", fpath);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }

    res = lchown(fpath, uid, gid);
    logDatabase(1, "CHOWM", fpath);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    int res;
    char fpath[1000];
    char name[1000];
    sprintf(name, "%s", path);
    sprintf(fpath, "%s%s", dirpath, name);
    res = truncate(fpath, size);
    if (res == -1)
        return -errno;

    return 0;
}
static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    int res;
    struct timeval tv[2];
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = ts[0].tv_nsec / 1000;
    tv[1].tv_sec = ts[1].tv_sec;
    tv[1].tv_usec = ts[1].tv_nsec / 1000;

    res = utimes(fpath, tv);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    res = open(fpath, fi->flags);
    logDatabase(1, "CAT", fpath);
    if (res == -1)
        return -errno;

    close(res);
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    (void)fi;

    fd = open(fpath, O_WRONLY);
    if (fd == -1)
        return -errno;

    logDatabase(1, "WRITE", fpath);
    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{

    (void)fi;
    char fpath[1000];

    if (!strcmp(path, "/"))
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    int res;
    logDatabase(1, "TOUCH", fpath);
    res = creat(fpath, mode);
    if (res == -1)
        return -errno;

    close(res);

    return 0;
}
static int xmp_flush(const char *path, struct fuse_file_info *fi)
{
    int res;

    (void)path;
    res = close(dup(fi->fh));
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_mknod(const char *ppath, mode_t mode, dev_t rdev)
{
    int res;

    char path[1000];
    char fpath[1000];
    strcpy(path, ppath);
    if (strcmp(path, "/") == 0)
    {
        strcpy(path, dirpath);
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    logDatabase(1, "touch/echo", fpath);
    res = mknod(fpath, mode, rdev);
    if (res == -1)
        return -errno;

    return 0;
}

static struct fuse_operations xmp_oper =
    {
        .getattr = xmp_getattr,
        .readdir = xmp_readdir,
        .read = xmp_read,
        .mkdir = xmp_mkdir,
        .mknod = xmp_mknod,
        .unlink = xmp_unlink,
        .rmdir = xmp_rmdir,
        .rename = xmp_rename,
        .link = xmp_link,
        .chmod = xmp_chmod,
        .chown = xmp_chown,
        .truncate = xmp_truncate,
        .utimens = xmp_utimens,
        .open = xmp_open,
        .write = xmp_write,
        .create = xmp_create,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
