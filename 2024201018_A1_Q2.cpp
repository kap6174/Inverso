#include <iostream>
#include <algorithm>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


using namespace std;

void disp(string msg)
{
    write(STDERR_FILENO, msg.c_str(), strlen(msg.c_str()));
}

void parseargs(int argcount, char *argvector[], string &newFile, string &oldFile, string &dir)
{
    if(argcount < 4)
    {
        string errmsg = "Arguments not given properly, usage: "+string(argvector[0])+" <old file> <new file> <dir>\n";
        write(STDERR_FILENO, errmsg.c_str(), strlen(errmsg.c_str()));
        exit(1);
    }
    newFile = argvector[1];
    oldFile = argvector[2];
    dir = argvector[3];

}


int openfile(string &inputFile)
{
    int filedes = open(inputFile.c_str(), O_RDONLY);
    if(filedes == -1)
    {
        perror("Error opening file");
        exit(1);
    }
    return filedes;
}

bool checkDir(string &dir)
{
    struct stat st;
    if(stat(dir.c_str(), &st) != 0)
    {
        return false;
    }
    else if(st.st_mode & S_IFDIR)
        return true;

    return false;
}


int checkReversal(int &newfiledescriptor, int &oldfiledescriptor, long long &newFile_size, long long &oldFile_size)
{
    struct stat stOld;
    struct stat stNew;
    unsigned long long chunkSize = 4096;

    if (fstat(newfiledescriptor, &stNew) == -1 || fstat(oldfiledescriptor, &stOld) == -1)
    {
        perror("Error getting file size");
        exit(1);
    }

    newFile_size = stNew.st_size;
    oldFile_size = stOld.st_size;

    char checkOld[1];
    lseek(newfiledescriptor, -1, SEEK_END);
    read(newfiledescriptor, checkOld, 1);
    if (checkOld[0] == '\n')
    {
        newFile_size--;
    }

    char checkNew[1];
    lseek(oldfiledescriptor, -1, SEEK_END);
    read(oldfiledescriptor, checkNew, 1);
    if (checkNew[0] == '\n')
    {
        oldFile_size--;
    }

    if (newFile_size != oldFile_size)
        return 0;

    ssize_t readbytes1;
    ssize_t readbytes2;

    char *buff1 = new char[chunkSize];
    char *buff2 = new char[chunkSize];

    lseek(oldfiledescriptor, 0, SEEK_SET); //Setting newfile descriptor to initial position

    for (long long offset = newFile_size - chunkSize; offset >= 0; offset -= chunkSize)
    {
        lseek(newfiledescriptor, offset, SEEK_SET);
        readbytes1 = read(newfiledescriptor, buff1, chunkSize);
        if (readbytes1 < 0)
        {
            perror("Error reading the file!");
            break;
        }

        reverse(buff1, buff1 + readbytes1);

        readbytes2 = read(oldfiledescriptor, buff2, chunkSize);
        if (readbytes2 < 0)
        {
            perror("Error reading the file!");
            break;
        }

        for (long long i = 0; i < readbytes1; i++)
        {
            if (buff1[i] != buff2[i])
                return 0;
        }
    }

    long long rem = newFile_size % chunkSize;
    if (rem != 0)
    {
        lseek(newfiledescriptor, 0, SEEK_SET);
        readbytes1 = read(newfiledescriptor, buff1, rem);
        if (readbytes1 < 0)
        {
            perror("Error reading the file!");
            exit(1);
        }

        reverse(buff1, buff1 + readbytes1);

        readbytes2 = read(oldfiledescriptor, buff2, rem);
        if (readbytes2 < 0)
        {
            perror("Error reading the file!");
            exit(1);
        }

        for (long long i = 0; i < readbytes1; i++)
        {
            if (buff1[i] != buff2[i])
                return 0;
        }
    }

    return 1;
}



void permissionPrint(string path, string name)
{
    struct stat st;
    if(stat(path.c_str(), &st) == -1)
    {
        perror("Error getting file stats");
        return;
    }
    string msg;

    if(st.st_mode & S_IRUSR)
    {
        msg = "User has read permissions on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "User has read permissions on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IWUSR)
    {
        msg = "User has write permission on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "User has write permission on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IXUSR)
    {
        msg = "User has execute permission on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "User has execute permission on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IRGRP)
    {
        msg = "Group has read permissions on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Group has read permissions on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IWGRP)
    {
        msg = "Group has write permission on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Group has write permission on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IXGRP)
    {
        msg = "Group has execute permission on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Group has execute permission on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IROTH)
    {
        msg = "Others have read permissions on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Others have read permissions on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IWOTH)
    {
        msg = "Others have write permission on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Others have write permission on " + name + ": No\n";
        disp(msg);
    }
    if(st.st_mode & S_IXOTH)
    {
        msg = "Others have execute permission on " + name + ": Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Others have execute permission on " + name + ": No\n";
        disp(msg);
    }
}

int main(int argcount, char *argvector[])
{
    string newFile;
    string oldFile;
    string dir;
    long long newFile_size, oldFile_size;

    //Parsing the command line arguments
    parseargs(argcount, argvector, newFile, oldFile, dir);  
 
    
    //Opening the new file
    int newfiledescriptor = openfile(newFile);
    //Opening the old file
    int oldfiledescriptor = openfile(oldFile);

    
    string msg;

    if(checkDir(dir))
    {
        msg = "Directory is created: Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Directory is created: No\n";
        disp(msg);
    }
    
    if(checkReversal(newfiledescriptor, oldfiledescriptor, newFile_size, oldFile_size))
    {
        msg = "Whether file contents are reversed in newfile: Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Whether file contents are reversed in newfile: No\n";
        disp(msg);
    }

    if(newFile_size ==  oldFile_size)
    {
        msg = "Both Files Sizes are Same : Yes\n";
        disp(msg);
    }
    else
    {
        msg = "Both Files Sizes are Same : No\n";
        disp(msg);
    }

    permissionPrint(newFile, "newfile");
    permissionPrint(oldFile, "oldfile");
    permissionPrint(dir, "dir");

    close(newfiledescriptor);
    close(oldfiledescriptor);
    

    return 0;
}