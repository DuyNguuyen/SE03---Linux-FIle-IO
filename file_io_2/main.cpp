/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: home
 *
 * Created on April 7, 2023, 6:44 PM
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ftw.h>
#include <cstring>
#define BUF_SIZE 1024

using namespace std;

//Check if path is exist or not
bool check_path(const char* path);

//Total sum of all file use by function sum()
long long total;
//Function to call when ftw() access to each object
int sum(const char *filename, const struct stat *buf, int type);

int main(int argc, char** argv) {
    if (argc < 2){
        cout << "Please select action\n";
        return 1;
    }
    
    char action = **(argv + 1);

    switch (action){
        //Copy file
        case '0':
        {
            if (argc != 4){
                cout << "Invalid parameter\n";
                return 1;
            }
            
            if (check_path(argv[2])){
                FILE *src, *dst;
                size_t in, out;
                char buf[BUF_SIZE];

                if ((src = fopen(argv[2], "r")) == NULL){
                    cout << "Can not open source file\n";
                    return 1;
                }
                
                if ((dst = fopen(argv[3], "w")) == NULL){
                    cout << "Can not open destination file\n";
                    return 1;
                }

                while (1) {
                    in = fread(buf, 1, BUF_SIZE, src);
                    if (in == 0){
                        cout << "Read file end\n";
                        break;
                    }
                    //out = fprintf(dst, buf);
                    out = fwrite(buf, 1, in, dst);  
                    if (out == 0){
                        cout << "Writing end\n";
                        break;        
                    }
                }
                
                fclose(src);
                fclose(dst);
            } else {
                cout << "Invalid source path\n";
            }
            
            break;
        }
        //Check path is exist or not
        case '1':
        {
            if (argc != 3){
                cout << "Invalid parameter\n";
                return 1;
            }
            
            if (check_path(argv[2])){
                cout << argv[2] << " is a valid path\n";
            } else {
                cout << argv[2] << " is an invalid path\n";
            }
            
            break;
        }
        //Get parent directory
        case '2':
        {
            if (argc != 3){
                cout << "Invalid parameter\n";
                return 1;
            }
            
            if (check_path(argv[2])){
                //Get absolute path of given path
                char *path = realpath(argv[2], NULL);
                
                //Trim character / at end of path if exist
                int pLength = strlen(path);
                if (path[pLength - 1] == '/'){
                    path[pLength - 1] = '\0';
                }
                
                //Remove file name
                for (int i = pLength; i--; i>-1){
                    if (path[i] == '/'){
                        path[i] = '\0';
                        break;
                    }
                }
                
                cout << "Parent directory of your path is:\n" << path << "\n";
            } else {
                cout << argv[2] << " is an invalid path\n";
            }
            
            break;
        }
        //Delete file
        case '3':
        {
            if (argc != 3){
                cout << "Invalid parameter\n";
                return 1;
            }
            
            if (check_path(argv[2])){
                if (remove(argv[2]) == 0){
                    cout << "File delete successfully\n";
                } else{
                    cout << "Error! Unable to delete the file\n";
                }
            } else{
                cout << argv[2] << " is an invalid path\n";
            }
            
            break;
        }
        //Get file name and size
        case '4':
        {
            if (argc != 3){
                cout << "Invalid parameter\n";
                return 1;
            }
            
            struct stat buf;
            if(stat(argv[2], &buf) == 0){
                char *path = realpath(argv[2], NULL);
                
                int pLength = strlen(path);
                if (path[pLength - 1] == '/'){
                    path[pLength - 1] = '\0';
                }
                
                //Get file name after the last '/' character from path
                char* filename;
                for (int i = pLength; i--; i > -1){
                    if (path[i] == '/'){
                        filename = path + i + 1;
                        break;
                    }
                }
                cout << "File name: " << filename << "\n";
                
                if(S_ISDIR(buf.st_mode)){
                    //Caculate size of folder
                    if (ftw(argv[2], &sum, 1)) {
                        cout << "Error when caculating size of file\n";
                    } else{
                        cout << "Size: " << total << "bytes\n";
                    }
                }else{
                    cout << "Size: " << buf.st_size << " bytes\n";
                }
            } else {
                cout << argv[2] << " is an invalid path\n";
            }
            break;
        }
        default:
        {
            cout << "Invalid action, please try again\n";
            return 1;
        }
    }
    
    return 0;
}

bool check_path(const char* path)
{
    struct stat buf;
    if(stat(path, &buf) == 0){
        return true;
    } else {
        return false;
    }
}

int sum(const char *filename, const struct stat *buf, int type) {
    if(type == FTW_F){
        total += buf->st_size;
    }
    
    return 0;
}

