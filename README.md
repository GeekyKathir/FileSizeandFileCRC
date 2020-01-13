# FileSizeandFileCRC

1. List all the files in the mentioned folder (allow user to enter the folder name at command prompt) and its sub-folders in the format      Sr. No., FileName, Folder Path, Size, Date Created, Date Modified, FileCRC
2. Sort the above table in a FileSizeSort.CSV file based on the File Size
3. Sort the above table in a FileCrcSort.CSV file based on the File CRC

Notes:
1. The File CRC is computed using a 32-bit CRC with seed value = 0xFFFFFFFF and polynomial = 0x973afb51 (given polynomial) 
2. Write a class for CRC computation and use it in the main() function
