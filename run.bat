@echo off
cargo build
start /b "" "C:\Program Files (x86)\Steam\steamapps\common\Battleborn\Binaries\Win64\Battleborn.exe"
timeout /t 5 /nobreak
.\Injector.exe --process-name Battleborn.exe --inject target/x86_64-pc-windows-msvc/debug/ReBorn.dll