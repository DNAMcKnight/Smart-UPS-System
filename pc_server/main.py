from fastapi import FastAPI, Request
import win32api
import win32con
import ctypes
import time
import subprocess
import asyncio

app = FastAPI()

allowed_hosts = ["127.0.0.1", "192.168.0.66"]


def get_client_ip(request: Request) -> str:
    # Prefer X-Forwarded-For
    xff = request.headers.get("x-forwarded-for")
    if xff:
        return xff.split(",")[0].strip()

    # Then X-Real-IP
    xri = request.headers.get("x-real-ip")
    if xri:
        return xri

    # Fallback to client host, only if available
    if request.client:
        return request.client.host

    return "unknown"


@app.get("/ip")
async def ip(request: Request):
    """retuns IP or forbidden if host not allowed"""
    if request.headers.get("host") not in allowed_hosts:
        return {"ip": "forbidden"}
    return {"ip": get_client_ip(request)}


async def initiate_shutdown():
    timeout = 15
    user32 = ctypes.windll.user32
    result = user32.MessageBoxTimeoutW(
        0,  # hWnd = no owner
        f"Shutdown has been initiated press CANCEL to stop this operation. The system will auto shutdown in 15 seconds or as soon as you press OK",  # text
        "Auto Shutdown",  # title
        win32con.MB_OKCANCEL
        | win32con.MB_TOPMOST
        | win32con.MB_ICONWARNING,  # buttons + always on top
        0,
        timeout * 1000,  # timeout in milliseconds
    )

    # Detect which button was clicked
    if result == win32con.IDOK:
        subprocess.run(["shutdown", "/s", "/f", "/t", "0"])
    elif result == win32con.IDCANCEL:
        pass
    else:
        subprocess.run(["shutdown", "/s", "/f", "/t", "0"])


@app.get("/shutdown")
async def shutdown(request: Request):
    """checks the IP and opens a popup before initiating a shutdown"""
    print(request.client.host)
    if request.client.host in allowed_hosts:
        asyncio.create_task(initiate_shutdown())
        return 120
    return {"ip": "forbidden"}
    # return {"ip": get_client_ip(request)}
