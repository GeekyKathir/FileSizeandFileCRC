/******************************************************************************************
Name:       Kathir N S

Objective:  1.  Program to list out the files in a directory and its sub-directory
                in the below format:
                Sr. No., FileName, Folder Path, Size, Date Created, Date Modified, FileCRC
            2.  Sort the above table in a FileSizeSort.CSV file based on the File Size
            3.  Sort the above table in a FileCrcSort.CSV file based on the File CRC

******************************************************************************************/
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <bits/stdc++.h>

using namespace std;

typedef unsigned int Uint32;
typedef unsigned char byte;

// Structure to collect the data of the file
struct fileData {

    string  fileName;
    string  filePath;
    string  fileCreatedTime;
    string  fileModifiedTime;
    long    fileSize;
    string  fileType;
    Uint32  fileCRC;

};

void listFile(const char *);
string decimalToHex(Uint32);

// Global Declaration
//Total files in the Directory
int iteration=0;
//To get dynamic memory for the finalData
vector<fileData> finalData;


/***************************************************************************************
Name:   Compare the Sizes
***************************************************************************************/
bool compareSize(fileData a, fileData b)
{
	//For descending order replace with a.fileSize >b.fileSize
	return (a.fileSize < b.fileSize);
}


/**************************************************************************************
Name:   Compare the FileCRC
**************************************************************************************/
bool compareCRC(fileData a, fileData b)
{
    //For descending order replace with a.fileCRC >b.fileCRC
	return (a.fileCRC < b.fileCRC);

}


/**************************************************************************************
Name:   Decimal to Hexadecimal conversion
**************************************************************************************/
string decimalToHex(Uint32 decimalCRC){

    //To convert the Decimal value to Hexadecimal value
    stringstream decimalHex;
    decimalHex<<hex<<decimalCRC;

    return decimalHex.str();
}


/**************************************************************************************
Name:          CRC32
Description:   generateTable() is used to generate the CRC table for the given Polynomial.
               calcCrcFile() is used to find the CRC of a file for the input directory in string

**************************************************************************************/
class CRC32
{
	public:

		static void generateTable(Uint32 tableCRC[256])
		{
		    //Required Polynomial
		    Uint32 polynomial = 0x973afb51;
			Uint32 defCRC;

			for(int i = 0; i < 256; ++i)
			{
				defCRC = i;

				for(int j = 8; j > 0; --j)
				{
					if(defCRC & 1){

                        defCRC = (defCRC >> 1) ^ polynomial;
					}
					else{

                        defCRC >>= 1;
					}

				}

				tableCRC[i] = defCRC;
			}
		}

		static Uint32 calcCrcFile(string fileName, Uint32* tempTable)
		{
			ifstream myFile( fileName.c_str(), std::ios::binary );
			if (!myFile)
				return 0;

			Uint32 crc = crcInit;
			const int bufSize = 256;
			char buffer[bufSize];

			int read = 0;
			while (myFile.read(buffer,bufSize)){
				read += bufSize;
				for (int i = 0; i < bufSize; ++i){
					calculate(buffer[i], crc, tempTable);
				}
			}

			//To Clear the eof/bad bits
			myFile.clear();
			//No of bytes to be read
			myFile.seekg(0, std::ios::end);
			std::ios::pos_type pos = myFile.tellg();
			myFile.seekg(read, std::ios::beg);
			int count = pos - myFile.tellg();
			//reading
			myFile.read(buffer,count);

			for (int i = 0; i < count; ++i){
				calculate(buffer[i], crc, tempTable);
			}

			return (crc ^ crcOut);
		}

		inline static void calculate(const byte in, Uint32& crc, Uint32* table)
		{
			crc = ( crc >> 8 ) ^ table[( in )^( crc & 0xFF )];
		}

	private:

		static const Uint32 crcInit = 0xFFFFFFFF;
		static const Uint32 crcOut = 0xFFFFFFFF;
};


/***************************************************************************
Name:           main
Description:    userDir-get input from the user for preferred directory

***************************************************************************/
int main()
{
    //Getting Preferred Directory from the user
    string userDir;
    cout<<"Enter the preferred directory : ";
    getline(cin,userDir);
    const char *charDir=userDir.c_str();

    //To check whether Entered Directory by User is Valid or not
    struct stat buffer;

    //Boolean condition to return Directory valid or not
    bool dirPresent= ( stat(charDir, &buffer) == 0);

    while(!dirPresent){
        //Ask user to enter the correct directory
        cout<<"Please enter a valid directory: ";
        getline(cin,userDir);
        dirPresent= ( stat(charDir, &buffer) == 0);
    }
    listFile(charDir);

    /**To find the CRC of a file**/
    Uint32 crcTable[256];
	CRC32::generateTable(crcTable);

	for (int j=0; j<iteration; j++){

        string virtualPath=finalData[j].filePath;
        //Resize to remove the '*' at the end of the directory
	    virtualPath.resize(virtualPath.size()-1);
        //Replacing the '\' with '/' to pass the path in calcCrcFile
	    for(int k=0;k<virtualPath.size();k++){

            if(virtualPath[k]=='\\'){
                virtualPath[k]='/';
            }
        }
        //crcPath combines the directory with filename to find the CRC of a particular file
	    string crcPath=virtualPath+(finalData[j].fileName);

        //storing the value of CRC in structure array
        finalData[j].fileCRC=CRC32::calcCrcFile(crcPath, crcTable);

        //Decimal to Hexadecimal conversion
        string decHexPrint=decimalToHex(finalData[j].fileCRC);

        //Printing the list of files for user reference
        cout<<"\n";
        cout<<j+1<<"\t";
        cout<<finalData[j].fileName<<"\t";
        cout<<finalData[j].filePath<<"\t";
        cout<<finalData[j].fileSize<<"KB"<<"\t";
        cout<<finalData[j].fileCreatedTime<<"\t";
        cout<<finalData[j].fileModifiedTime<<"\t";
        cout<<decHexPrint<<endl;


	}

    /**Sorting the content based on the file size**/
    sort(finalData.begin(), finalData.end(), compareSize);
    cout<<"\n\nFile sorting based on size completed successfully\n";

    /**Writing the Sorted Table in .csv file**/
    //ofstream is used to create and write in the file
    ofstream myFileSize;
    myFileSize.open("FileSizeSort.csv");
    //Storing the data in .csv as per required format
    myFileSize<<"Sr.no,FileName,FolderPath,Size(KB),Date Created,Date Modified,FileCRC"<<endl;
    cout<<"FileSizeSort.csv Created and Writing in .csv\n";

    //Writing in FileSizesort.csv
    for(int i=0; i<iteration; i++){

        //Decimal to Hexadecimal conversion
        string decHexSize=decimalToHex(finalData[i].fileCRC);

        myFileSize<<i+1<<","<<finalData[i].fileName<<","<<finalData[i].filePath<<",";
        myFileSize<<finalData[i].fileSize<<",";
        myFileSize<<finalData[i].fileCreatedTime<<","<<finalData[i].fileModifiedTime<<",";
        myFileSize<<decHexSize<<endl;
    }
    myFileSize.close();
    cout<<"FileSizeSort.csv successfully saved\n"<<endl;


    /**Sorting the content based on the file CRC**/
    sort(finalData.begin(), finalData.end(), compareCRC);
    cout<<"\n\nFile sorting based on CRC completed successfully\n";

    /**Writing the Sorted Table in .csv file**/
    ofstream myFileCrc;
    myFileCrc.open("FileCrcSort.csv");
    myFileCrc<<"Sr.no,FileName,FolderPath,Size(KB),Date Created,Date Modified,FileCRC"<<endl;

    cout<<"FileCrcSort.csv Created and Writing in .csv\n";

    //Writing in FileCrcSort.csv
    for(int n=0; n<iteration; n++){

        //Decimal to Hexadecimal conversion
        string decHexCrc=decimalToHex(finalData[n].fileCRC);

        myFileCrc<<n+1<<","<<finalData[n].fileName<<","<<finalData[n].filePath<<",";
        myFileCrc<<finalData[n].fileSize<<",";
        myFileCrc<<finalData[n].fileCreatedTime<<","<<finalData[n].fileModifiedTime<<",";
        myFileCrc<<decHexCrc<<endl;
    }
    myFileCrc.close();
    cout<<"FileCrcSort.csv successfully saved\n"<<endl;

    return 0;
}


/**************************************************************************
Name:           listFile
Description:    DIR - A structure representing a directory stream
                pDIR is a path of Directory
                entry is used to read files in the directory
                d_name is used to access the filename
**************************************************************************/
void listFile(const char *getcharDir){

    DIR *pDIR;
    dirent *entry;

    pDIR=opendir(getcharDir);

    if(opendir(getcharDir)){

        entry=readdir(pDIR);

        while(readdir(pDIR)){

            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){

                struct stat fileInfo;

                //Converting DIR to string
                string  tempPath=pDIR->dd_name;
                //Converting the const char to string
                string  tempFile=entry->d_name;
                //Resize to eliminate the '*' at the end of the directory
                tempPath.resize(tempPath.size()-1);
                //Appending the Directory with File or Folder name
                tempPath=tempPath+tempFile;

                const char *absolutePath=tempPath.c_str();

                if  (stat(absolutePath,&fileInfo)==0){


                    /**Finding whether directory contain folder or file**/
                    string tempType;

                    //Condition to find the folder
                    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR) {

                        //Recursion to find the files in folder
                        listFile(absolutePath);

                    }

                    else {

                        tempType="File";

                        /**Trimming the whitespace at the end of the file created time and modified time**/
                        string tempTrim;
                        string modifiedTime=ctime(&fileInfo.st_mtime);
                        tempTrim=modifiedTime;
                        tempTrim.resize(tempTrim.size()-1);
                        modifiedTime=tempTrim;

                        string createdTime;
                        createdTime=ctime(&fileInfo.st_ctime);
                        tempTrim=createdTime;
                        tempTrim.resize(tempTrim.size()-1);
                        createdTime=tempTrim;


                        /**To find the Name, Path, Type, Size, Date created and modified of the File**/

                        long sizeFile=fileInfo.st_size/1024;

                        //condition to dynamically allocate the space when it find new file in vector
                        finalData.push_back(fileData());

                        finalData[iteration].fileName=tempFile;
                        finalData[iteration].filePath=pDIR->dd_name;
                        finalData[iteration].fileType=tempType;
                        finalData[iteration].fileSize=sizeFile;
                        finalData[iteration].fileCreatedTime=createdTime;
                        finalData[iteration].fileModifiedTime=modifiedTime;

                        //To check the total No of files
                        iteration=iteration+1;

                    }

                }


            }

        }

        closedir(pDIR);

    }
}
