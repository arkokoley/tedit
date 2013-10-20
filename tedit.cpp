#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <stdlib.h>
using namespace std;

int openfile();
int newfile();

    string   fileopen;
    string   createfile;
    string   text;
    ofstream outfile;
    ifstream infile;
    
int main()
{
    char     options;
    
       
    do
    {
    system("cls");
    cout <<"\t\t-------------------------------------" <<endl;
    cout <<"\t\t| [o]pen file | [N]ew file | [e]xit |" <<endl;
    cout <<"\t\t-------------------------------------" <<endl;
    options =getch();
    
    switch(options)
    {
                   case 'o':
                        {
                            openfile();
                        }
                   break;
                   case 'n':
                        {
                            newfile();
                        }
                   break;
    }
    
    
    }
    while(options != 'e');
    
    return 0;
}

int openfile()
{
    int  i;
    //char file;
        
    cout <<"\t\t-------------------------------------" <<endl;
    cout <<"\t\t|             OPEN FILE             |" <<endl;
    cout <<"\t\t-------------------------------------" <<endl;  
    cout <<"\t\t|Open file| "; getline(cin,fileopen);
    cout <<"\t\t-----------" <<endl;

    infile.open((fileopen).c_str());
    
    if(!infile)
    {
         cout <<"error reading file..." <<endl;
         cin.ignore(256,'\n');
         return -1;
    }
    
    
    while(! infile.eof())
    {
            getline(infile, text);
            cout << text <<endl;
    }            
    infile.close();
    cin.ignore(256,'\n');
    fileopen.empty();
}
    

int newfile()
{
    
    cout <<"\t\t-------------------------------------" <<endl;
    cout <<"\t\t|              NEW FILE             |" <<endl;
    cout <<"\t\t-------------------------------------" <<endl;  
    cout <<"\t\t|New File| "; getline(cin,createfile);
    cout <<"\t\t----------" <<endl;
    
       
    getline(cin,text, '$' );
       
    if(! outfile)
    {
         cout <<"error writing to file" <<endl;
         return -1;
    }
    outfile.open((createfile+".txt").c_str());
    outfile << text <<endl;
    outfile.close();
    
}
