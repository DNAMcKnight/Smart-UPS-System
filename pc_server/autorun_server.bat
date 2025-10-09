@echo off
cd /d E:\development\ESP8266\Smart-UPS-System\pc_server
REM Activate the virtual environment and run the server using its Python
call venv\Scripts\activate.bat
python -m uvicorn main:app --host 0.0.0.0 --port 8000 --reload
pause