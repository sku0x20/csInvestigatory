----BRANCH MANAGEMENT SYSTEM----

**All features/functions are working, NOTHING IS Dummy**

As the name suggests this project aim's to be used 
in the branches of a company. The project is capable of 
uploading and retriving data from the company's database
if local data is corrupted or misplaced.
This project provides extra security to employess as well as 
admin by hashing the passwords.

----Getting Started----
The poject consistes of mainly two executables:-
1. adminApp.exe
2. userApp.exe

**adminApp.exe**
This exe is designed for the admin. To use this executable
admin have to login by providing the password.
After Successful login, the executable checks the internet
avalibility and uploads or retrives data from main database.
This exe makes 3 binary files:-
1. config:-
	This contains information for user id and stores new users if 
	internet is not available.
2. users:-
	Contains information of users.
3. userPass:-
	Contains hashed passwords of users.

-While adding new user its id is automatically generated and after filleing 
all necessary information app prompts user for entering his password for login.
-Admin can only edit income of the user after entering the id.
-Esc can be used for loging out.

**userApp.exe**
This exe is designed to be used by employess for cheching their information and 
editing the information like address, phoneno, etc.
At first user have to login to the application by providing their id and password.
-Editing can only be done when the internet is available.
-Employees can logout by pressing Esc.

----Prerequisites----
Things needed to run this project smoothly:-
1. libmysql.dll
2. libQrCode.dll
3. libsoci_core_4_0.dll
4. libsoci_mysql_4_0.dll

----Credits & Thanks----
1. SOCI - The C++ Database Access Library(https://github.com/SOCI/soci)
		For providing such a user-friendly library for accessing databases. 
		
2. PicoSHA2 - SHA256 hash generator in C++(https://github.com/okdshin/PicoSHA2)
		For providing the necessary header for generating sha256 hashes.
		
3. QR-Code-generator - QR Code generator library(https://github.com/nayuki/QR-Code-generator)
		For providing the helpful library for generating qr codes.
		
----Acknowledgments----
1. Thanking Google, Stackoverflow and the other online forums for helping me when I 
   am lost and depressed.
   
NOTE: 1. This project only works under Windows OS due to native cosole support by Microsft 
      consoles APIs.
      2. Use ; to end address
      3. Works on 64 bit OS only.
	  
----BONUS FEATURE----
-Tab can be used to CHANGE FONT.
	  
----THANKS----



----LATEST----
1.Since the database that this project is connected to is no more,
  executables won't work as expected.
2.You can either make it from source using your own mysql database on host or
  can create mysql db on https://www.db4free.net and use that.
3.This project is made using codeblocks so you can use the same to open, view and
  make changes to it.
