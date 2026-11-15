"""The HTTP side: the API of the page and the files of the front end"""

import json
from pathlib import Path
from typing import Any, Dict, Optional

from starlette.applications import Starlette
from starlette.requests import Request
from starlette.responses import FileResponse, JSONResponse, Response, StreamingResponse
from starlette.routing import Mount, Route
from starlette.staticfiles import StaticFiles

from . import events
from .backend import Backend
from .config import AppConfig
from .security import SecurityMiddleware

STATIC = Path(__file__).with_name("static")
# The most that the page may send as a message or a command
_MAX_TEXT = 8000
_NO_BUILD = """<!doctype html><meta charset="utf-8"><title>OpenFPGA</title>
<body style="font-family:sans-serif;max-width:36em;margin:4em auto">
<h1>OpenFPGA</h1><p>The front end is not built. In
<code>openfpga/python/app/frontend</code> run <code>npm install</code> and
<code>npm run build</code>.</p></body>"""


def create_app(config: AppConfig, backend: Backend, static: Optional[Path] = None) -> Starlette:
    static = Path(static) if static is not None else STATIC

    async def index(request: Request) -> Response:
        page = static / "index.html"
        if not page.is_file():
            return Response(_NO_BUILD, media_type="text/html", status_code=503)
        return FileResponse(page, headers={"Cache-Control": "no-store"})

    async def session(request: Request) -> Response:
        return JSONResponse(backend.view())

    async def app_config(request: Request) -> Response:
        return JSONResponse(
            {
                "schema_version": 1,
                "console": config.console,
                "model": {"endpoint": config.model_endpoint, "name": config.model_name},
            }
        )

    async def body(request: Request) -> Dict[str, Any]:
        try:
            data = await request.json()
        except ValueError:
            data = None
        return data if isinstance(data, dict) else {}

    async def chat(request: Request) -> Response:
        message = (await body(request)).get("message")
        if not isinstance(message, str) or not message.strip() or len(message) > _MAX_TEXT:
            return JSONResponse({"error": "message is text of 1 to %d characters" % _MAX_TEXT}, 400)

        async def stream():
            try:
                async for item in backend.chat(message):
                    yield events.format_sse(item)
            except Exception as failure:  # noqa: BLE001 - the page gets the failure as an event
                yield events.format_sse(events.error(str(failure) or type(failure).__name__))

        return StreamingResponse(
            stream(),
            media_type="text/event-stream",
            headers={"Cache-Control": "no-store", "X-Accel-Buffering": "no"},
        )

    async def cancel(request: Request) -> Response:
        backend.cancel()
        return JSONResponse({"ok": True})

    async def console(request: Request) -> Response:
        if not config.console:
            return JSONResponse({"error": "The console is off: start with --console"}, 404)
        command = (await body(request)).get("command")
        if not isinstance(command, str) or len(command) > _MAX_TEXT or "\n" in command:
            return JSONResponse({"error": "command is one line of text"}, 400)
        return JSONResponse(await backend.console(command))

    routes = [
        Route("/", index),
        Route("/api/session", session),
        Route("/api/config", app_config),
        Route("/api/chat", chat, methods=["POST"]),
        Route("/api/cancel", cancel, methods=["POST"]),
        Route("/api/console", console, methods=["POST"]),
    ]
    if (static / "assets").is_dir():
        routes.append(Mount("/assets", StaticFiles(directory=static / "assets")))
    app = Starlette(routes=routes)
    app.add_middleware(SecurityMiddleware, token=config.token, allowed_hosts=config.allowed_hosts())
    return app
