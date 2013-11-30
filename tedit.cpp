#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <stdlib.h>
using namespace std;

const int linechar=80;

struct filestruct {
	string filename;
	string text;
	unsigned char *start; // Start of text buffer
	unsigned char *gap; // Start of gap
	unsigned char *rest; // End of gap
	unsigned char *end; // End of text buffer

	int toppos; // Text position for current top screen line
	int topline; // Line number for top of screen
	int margin; // Position for leftmost column on screen

	int linepos; // Text position for current line
	int line; // Current document line
	int col; // Current document column
	int lastcol; // Remembered column from last horizontal navigation
	int anchor; // Anchor position for selection

	int refresh; // Flag to trigger screen redraw
	int lineupdate; // Flag to trigger redraw of current line
	int dirty; // Dirty flag is set when the editor buffer has been changed

	int isnewfile; // File is a new file

    int openfile();
    int newfile();
	
} *file;
    ofstream outfile;
    ifstream infile;
    
 int filestruct::openfile()
{
    int  i;
    //char file;
        
    cout <<"\t\t-------------------------------------" <<endl;
    cout <<"\t\t|             OPEN FILE             |" <<endl;
    cout <<"\t\t-------------------------------------" <<endl;  
    cout <<"\t\t|Open file| "; getline(cin,filename);
    cout <<"\t\t-----------" <<endl;

    infile.open((filename).c_str());
    
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
    filename.empty();
}
    

int filestruct::newfile()
{
    
    cout <<"\t\t-------------------------------------" <<endl;
    cout <<"\t\t|              NEW FILE             |" <<endl;
    cout <<"\t\t-------------------------------------" <<endl;  
    cout <<"\t\t|New File| "; getline(cin,filename);
    cout <<"\t\t----------" <<endl;
    
       
    getline(cin,text, '$' );
       
    if(! outfile)
    {
         cout <<"error writing to file" <<endl;
         return -1;
    }
    outfile.open((filename+".txt").c_str());
    outfile << text <<endl;
    outfile.close();
    
}


int main()
{
    char     options;  
    file= new filestruct;
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
                            file->openfile();
                        }
                   break;
                   case 'n':
                        {
                            file->newfile();
                        }
                   break;
    }
    
    
    }
    while(options != 'e');
    
    return 0;
}
