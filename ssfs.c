#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

static const char *dirpath = "/home/feinard/Downloads";
char key[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
int keygen = 10;
char ext[1000] = "\0";
int id = 0;

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

char *lastPart(char *str)
{
    if (!strcmp(str, "/"))
        return NULL;
    return strrchr(str, '/') + 1;
}

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

char *datapath(char *path, char *str1, const char *str2)
{
    strcpy(path, str1);
    if (!strcmp(str2, "/"))
        return path;
    if (str2[0] != '/')
    {
        path[strlen(path) + 1] = '\0';
        path[strlen(path)] = '/';
    }
    sprintf(path, "%s%s", path, str2);
    return path;
}

int checkEncFolder(char *str)
{
    int ans;
    char *fi = strtok(str, "/");
    ans = 0;
    while (fi)
    {
        char sub[1024];
        strcpy(sub,fi);
        if (!strcmp(sub, "encv1_"))
            ans |= 1;
        fi = strtok(NULL, "/");
    }
    return ans;
}

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

void encrypt1(char *str, int flag)
{
    struct stat add;
    stat(str, &add);
    if (!S_ISDIR(add.st_mode))
        return;
    loopAllEnc1(str, flag);
}

void logDatabase(int warning, char *type, char *path)
{
    printf("logDatabase STARTED............\n");
    char printLoc[1000];
    memset(printLoc, 0, sizeof(printLoc));
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
    memset(printLoc, 0, sizeof(printLoc));
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
    res = lstat(checkPath(fpath), stbuf);

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
    dp = opendir(checkPath(fpath));
    int flag = checkEncFolder(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;

        memset(&st, 0, sizeof(st));
        char name[1000];
        strcpy(name,de->d_name);
        if (flag == 1)
        {
            if(de->d_type == DT_REG)
            {
                decrypt(name,1);
            }
            else if(de->d_type == DT_DIR && strcpy(de->d_name,".") != 0 && strcpy(de->d_name,"..") !=0)
            {
                decrypt(name,0);
            }
            res = filler(buf, name, &st, 0);
            if (res != 0)
                break;
        }
        else
        {
            res = filler(buf, name, &st, 0);
            if (res != 0)
                break;
        }
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

    fd = open(checkPath(fpath), O_RDONLY);
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
    res = mkdir(checkPath(fpath), mode);
    char cek_substr[1024];
    if (lastPart(fpath) == 0)
        return 0;
    char filePath[1000000];
    strcpy(filePath, lastPart(fpath));
    findSubstring(filePath, cek_substr, 0, 6);
    if (strcmp(cek_substr, "encv1_") == 0) //folder encrypt1
    {
        encrypt1(fpath, 1);
    }
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
    res = unlink(checkPath(fpath));

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

    res = rmdir(checkPath(fpath));
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
    res = rename(checkPath(fpath),checkPath(tpath));
    logDatabase2(1, "RENAME", fpath, tpath);
    if (res == -1)
        return -errno;
    int fromCount = 0, toCount = 0;
    char cek_substr[1024], cek2[1024];
    if (lastPart(fpath) == 0)
        return 0;
    char filePath[1000000];
    strcpy(filePath, lastPart(fpath));
    findSubstring(filePath, cek_substr, 0, 6);
    if (strcmp(cek_substr, "encv1_") == 0) 
    {
        fromCount = 1;
    }
    else if (strcmp(cek_substr, "encv2_") == 0) 
    {
        fromCount = 2;
    }

    if (lastPart(tpath) == 0)
        return 0;

    strcpy(filePath, lastPart(tpath));
    findSubstring(filePath, cek_substr, 0, 6);
    if (strcmp(cek2, "encv1_") == 0) 
    {
        toCount = 1;
    }
    else if (strcmp(cek2, "encv2_") == 0) 
    {
        toCount = 2;
    }

    if (fromCount == 0 && toCount == 1)
        encrypt1(tpath, 1);
    else if (fromCount == 0 && toCount == 2)
        encrypt1(tpath, -1);
    else if (fromCount == 2 && toCount != 1)
        encrypt1(tpath, -1);

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

    res = chmod(checkPath(fpath), mode);
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

    res = lchown(checkPath(fpath), uid, gid);
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
    res = truncate(checkPath(fpath), size);
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

    res = utimes(checkPath(fpath), tv);
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
    res = open(checkPath(fpath), fi->flags);
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

    fd = open(checkPath(fpath), O_WRONLY);
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
    checkPath(fpath);
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
