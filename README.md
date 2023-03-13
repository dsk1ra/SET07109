### Description:
***
When you run makefile file it runs csource.c file and generates csourse.exe and csourse.obj. 
After that it runs csourse.exe and passing chosen file as second argument. 
When it runs it gets:
- total number of lines
- total number of characters
- total number of comments
- total number of variables
and outputs it into the console.
Then finds all defined variebles of int, char and double types, finds how many times has each of those have been referenced and puts this information into .s file.
It takes the name of the .c input file and puts it into .s output file.

### Installation
***
- Open Developers Console
- cd to project folder
- enter "nmake all" to run the project
- enter "nmake clear_obj" to delete .obj file
 
 ### Author
 Denys Skira

 ### Links
https://github.com/dsk1ra/SET07109