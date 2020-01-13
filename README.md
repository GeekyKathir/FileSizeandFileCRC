# FileSizeandFileCRC

List all the files in the mentioned folder (allow user to enter the folder name at command prompt) and its sub-folders in the below format
    Sr. No., FileName, Folder Path, Size, Date Created, Date Modified, FileCRC
Sort the above table in a FileSizeSort.CSV file based on the File Size
Sort the above table in a FileCrcSort.CSV file based on the File CRC

Notes:
The File CRC is computed using a 32-bit CRC with seed value = 0xFFFFFFFF and polynomial = 0x973afb51 (given polynomial) 
Write a class for CRC computation and use it in the main() function
