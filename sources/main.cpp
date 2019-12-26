//Copyright 2019 <yurygavrilov47>
#include "FTPfiles.h"

int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        path path_to_ftp("C:\\Users\\user\\Desktop\\qq\\ftp");
        FTPfiles Test(path_to_ftp);
    } else {
        path path_to_ftp(argv[1]);
        FTPfiles Test(path_to_ftp);
    }
    return 0;
}
