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
void printPercentage(long long writtenbytes, int &inputfiledescriptor)
{
    long long size;
    static long long totalwrittenbytes = 0;
    struct stat st;
    int checkfilesize = fstat(inputfiledescriptor, &st);
    if(checkfilesize == -1)
    {
        perror("Error getting file size");
        return;
    }

    char check[1];
    lseek(inputfiledescriptor, -1, SEEK_END);
    read(inputfiledescriptor, check, 1);
    if(check[0] == '\n')
        size = st.st_size - 1;
    else
        size = st.st_size;
    

    totalwrittenbytes += writtenbytes;

    int percent = (totalwrittenbytes * 100) / size;
    string msg = "\rProgress: " + to_string(percent) + "%";
    disp(msg);
    fsync(STDERR_FILENO);

}
void parseargs(int argcount, char *argvector[], string &inputFile, int &flag, int &startIndex, int &endIndex)
{
    
    if(argcount < 3)
    {
        string errmsg = "Arguments not given properly, usage: "+string(argvector[0])+" <input file> <flag> [start index] [end index]\n";
        write(STDERR_FILENO, errmsg.c_str(), strlen(errmsg.c_str()));
        exit(1);
    }
    inputFile = argvector[1];
    flag = stoi(argvector[2]);

    if(flag == 0)
    {
        if(argcount > 3)
        {
            string errmsg = "Arguments not given properly\n";
            write(STDERR_FILENO, errmsg.c_str(), strlen(errmsg.c_str()));
            exit(1);
        }   
    }

    if(flag == 1)
    {
        if(argcount < 5)
        {
            disp("Flag is 1 please give start index and end index\n");
            exit(1);
        }
        startIndex = stoi(argvector[3]);
        endIndex = stoi(argvector[4]);
    }

}
bool checkDir(string dir)
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
void createDir(const char* dirname)
{
    if(mkdir(dirname, 0700) == -1)
    {
        perror("Error Creating Directory!");
        exit(1);
    }
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

int createOutputFile(string &inputFile, int flag)
{
    int p = inputFile.find_last_of("/");
    string outputfilename = "Assignment1/" + to_string(flag) + "_" + inputFile.substr(p + 1);

    int filedes = open(outputfilename.c_str(), O_CREAT | O_WRONLY, 0600);
    if(filedes == -1)
    {
        perror("Error creating file");
        exit(1);
    }
    return filedes;
}
void reverseFile_0(int &inputfiledescriptor, int &outputfiledescriptor)
{
    struct stat st;
    long long size;
    unsigned long long chunkSize = 4096;
    int checkfilesize = fstat(inputfiledescriptor, &st);
    if(checkfilesize == -1)
    {
        return;
    }

    char check[1];
    lseek(inputfiledescriptor, -1, SEEK_END);
    read(inputfiledescriptor, check, 1);
    if(check[0] == '\n')
        size = st.st_size - 1;
    else
        size = st.st_size;

    ssize_t readbytes;
    ssize_t writtenbytes;

    char *buff = new char[chunkSize];
    for(long long offset = size - chunkSize; offset >= 0; offset -= chunkSize)
    {
        lseek(inputfiledescriptor, offset, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, chunkSize);
        if(readbytes < 0)
        {
            perror("Error reading the file!");
            break;
        }

        reverse(buff, buff + readbytes);

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            break;
        }
        

    }
    long long rem = size % chunkSize;
    if(rem != 0)
    {
        lseek(inputfiledescriptor, 0, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, rem);
        if(readbytes < 0)
        {
            perror("Error reading the file!");
            exit(1);
        }

        reverse(buff, buff + readbytes);

        writtenbytes = write(outputfiledescriptor, buff, readbytes);
        
        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            exit(1);
        }
    }
}
void reverseTillStartIndex(int &inputfiledescriptor, int &outputfiledescriptor, int &startIndex)
{
    struct stat st;
    unsigned long long chunkSize = 4096;
    int checkfilesize = fstat(inputfiledescriptor, &st);
    if(checkfilesize == -1)
    {
        perror("Error getting file size");
        return;
    }
    long long size = startIndex;
    ssize_t readbytes;
    ssize_t writtenbytes;
    char *buff = new char[chunkSize];
    for(long long offset = size - chunkSize; offset >= 0; offset -= chunkSize)
    {
        lseek(inputfiledescriptor, offset, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, chunkSize);
        if(readbytes < 0)
        {
            perror("Error reading the file!");
            break;
        }

        reverse(buff, buff + readbytes);

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            break;
        }

    }
    long long rem = size % chunkSize;
    if(rem != 0)
    {
        lseek(inputfiledescriptor, 0, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, rem);
        
        if(readbytes < 0)
        {
            perror("Error reading the file!");
            exit(1);
        }

        reverse(buff, buff + readbytes);

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            exit(1);
        }
    }
}
void notreversing(int &inputfiledescriptor, int &outputfiledescriptor, int &startIndex, int &endIndex)
{
    struct stat st;
    unsigned long long chunkSize = 4096;
    int checkfilesize = fstat(inputfiledescriptor, &st);
    if(checkfilesize == -1)
    {
        perror("Error getting file size");
        return;
    }

    long long size = endIndex - startIndex + 1;
    ssize_t readbytes;
    ssize_t writtenbytes;
    
    char *buff = new char[chunkSize];
    for(long long offset = endIndex - chunkSize + 1; offset >= startIndex; offset -= chunkSize)
    {
        lseek(inputfiledescriptor, offset, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, chunkSize);
        if(readbytes < 0)
        {
            perror("Error reading the file!");
            break;
        }

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            break;
        }

    }
    long long rem = size % chunkSize;
    if(rem != 0)
    {
        lseek(inputfiledescriptor, startIndex, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, rem);

        if(readbytes < 0)
        {
            perror("Error reading the file!");
            exit(1);
        }

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            exit(1);
        }
    }

}
void reverseTillEndIndex(int &inputfiledescriptor, int &outputfiledescriptor, int &endIndex)
{
    struct stat st;
    long long totalFileSize;
    unsigned long long chunkSize = 4096;
    int checkfilesize = fstat(inputfiledescriptor, &st);
    if(checkfilesize == -1)
    {
        perror("Error getting file size");
        return;
    }
    char check[1];
    lseek(inputfiledescriptor, -1, SEEK_END);
    read(inputfiledescriptor, check, 1);
    if(check[0] == '\n')
        totalFileSize = st.st_size - 1;
    else
        totalFileSize = st.st_size;
    long long size = totalFileSize - (endIndex + 1);
    ssize_t readbytes;
    ssize_t writtenbytes;

    char *buff = new char[chunkSize];
    for(long long offset = totalFileSize - chunkSize; offset >= endIndex + 1; offset -= chunkSize)
    {
        lseek(inputfiledescriptor, offset, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, chunkSize);
        if(readbytes < 0)
        {
            perror("Error reading the file!");
            break;
        }

        reverse(buff, buff + readbytes);

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            break;
        }

    }
    long long rem = size % chunkSize;
    if(rem != 0)
    {
        lseek(inputfiledescriptor, endIndex + 1, SEEK_SET);
        readbytes = read(inputfiledescriptor, buff, rem);

        if(readbytes < 0)
        {
            perror("Error reading the file!");
            exit(1);
        }

        reverse(buff, buff + readbytes);

        writtenbytes = write(outputfiledescriptor, buff, readbytes);

        printPercentage(writtenbytes, inputfiledescriptor);

        if(writtenbytes != readbytes)
        {
            perror("Error writing to file");
            exit(1);
        }
    }
}
void reverseFile_1(int &inputfiledescriptor, int &outputfiledescriptor, int &startIndex, int &endIndex)
{
    reverseTillStartIndex(inputfiledescriptor, outputfiledescriptor, startIndex);
    notreversing(inputfiledescriptor, outputfiledescriptor, startIndex, endIndex);
    reverseTillEndIndex(inputfiledescriptor, outputfiledescriptor, endIndex);

}

int main(int argcount, char *argvector[])
{
    struct stat st;
    long long size;
    string inputFile;    
    int startIndex = 0;
    int endIndex = 0;
    int flag = 0;

    //Parsing the command line arguments
    parseargs(argcount, argvector, inputFile, flag, startIndex, endIndex);  

    //Check for Wrong Flags
    if(flag != 0 and flag != 1)
    {
        string msg = "Incorrect Flag Given\n";
        disp(msg);
        return 1;
    }

    //Check for Wrong index combinations
    if(flag == 1)
    {
        if(startIndex > endIndex)
        {
            string msg = "Incorrect Index Given\n";
            disp(msg);
            return 1;
        }
    }  
    //Opening the input file
    int inputfiledescriptor = openfile(inputFile);

    //Empty File Check with check for if the file ends \n or not
    if(fstat(inputfiledescriptor, &st) == -1)
    {
        return 1;
    }
    char check[1];
    lseek(inputfiledescriptor, -1, SEEK_END);
    read(inputfiledescriptor, check, 1);
    if(check[0] == '\n')
        size = st.st_size - 2; 
    else
        size = st.st_size - 1;
    if(size < 0)
    {
        string msg = "Empty File\n";
        disp(msg);
        return 1;   
    }
    if(flag == 1)
    {
        if(startIndex < 0 or endIndex > size)
        {
            string msg = "Index Out Of Bounds\n";
            disp(msg);
            return 1;
        }
    }
    //Creating the Assignment1 Directory
    if(checkDir("Assignment1") == false)
        createDir("Assignment1");


    //Creating the output file
    int outputfiledescriptor = createOutputFile(inputFile, flag);

    //Reversing File based on flag
    if(flag == 0)
        reverseFile_0(inputfiledescriptor, outputfiledescriptor);
    else
        reverseFile_1(inputfiledescriptor, outputfiledescriptor, startIndex, endIndex);

    disp("\n");
    
    close(inputfiledescriptor);
    close(outputfiledescriptor);
    return 0;
}