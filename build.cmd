@echo off

cl /I".\src" /D"__MSACCESS__" .\test\main.c .\src\dbx.c 
