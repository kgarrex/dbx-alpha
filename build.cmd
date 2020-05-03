@echo off

cl /I".\src" /D"__ODBC__" .\test\main.c .\src\dbx.c 
