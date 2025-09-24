from fastapi import FastAPI, Request

app = FastAPI()


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
    return {"ip": get_client_ip(request)}
