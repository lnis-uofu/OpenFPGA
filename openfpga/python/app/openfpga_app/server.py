"""The HTTP side: the API of the page and the files of the front end"""

import json
from pathlib import Path
from typing import Any, Dict, Optional

from starlette.applications import Starlette
from starlette.requests import Request
from starlette.responses import FileResponse, HTMLResponse, JSONResponse, Response, StreamingResponse
from starlette.routing import Mount, Route
from starlette.staticfiles import StaticFiles

from . import events, uploads, viewer
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


def _refused(refused: "uploads.UploadRefused") -> Dict[str, Any]:
    return {
        "ok": False,
        "diagnostics": [
            {
                "severity": "error",
                "kind": refused.kind,
                "message": refused.message,
                "hint": refused.hint or None,
            }
        ],
    }


def create_app(config: AppConfig, backend: Backend, static: Optional[Path] = None) -> Starlette:
    static = Path(static) if static is not None else STATIC

    async def index(request: Request) -> Response:
        page = static / "index.html"
        if not page.is_file():
            return Response(_NO_BUILD, media_type="text/html", status_code=503)
        return FileResponse(page, headers={"Cache-Control": "no-store"})

    async def session(request: Request) -> Response:
        return JSONResponse(await backend.view())

    async def app_config(request: Request) -> Response:
        return JSONResponse(
            {
                "schema_version": 1,
                "console": config.console,
                "backend": backend.kind,
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

    async def new_session(request: Request) -> Response:
        name = (await body(request)).get("name")
        if name is not None and not isinstance(name, str):
            return JSONResponse({"error": "name is text"}, 400)
        package = (await body(request)).get("device_package")
        if package is not None and not isinstance(package, str):
            return JSONResponse({"error": "device_package is text"}, 400)
        return JSONResponse(await backend.new_session(name, package))

    async def apply_package(request: Request) -> Response:
        package = (await body(request)).get("device_package")
        if not isinstance(package, str) or not package:
            return JSONResponse({"error": "device_package is the name of a package"}, 400)
        return JSONResponse(await backend.apply_device_package(package))

    async def file(request: Request) -> Response:
        answer = await backend.read_file(request.query_params.get("name", ""))
        if answer.get("ok"):
            return JSONResponse(answer)
        kind = (answer.get("diagnostics") or [{}])[0].get("kind")
        status = {"file_not_found": 404, "path_refused": 403, "demo": 404, "ambiguous_file": 409}.get(kind, 400)
        return JSONResponse(answer, status)

    async def report(request: Request) -> Response:
        """The HTML report of a stage, as a page of its own. It is the server's own page, with every
        value escaped; the policy also forbids scripts and any load from elsewhere"""
        text = await backend.read_report(request.query_params.get("stage", ""))
        if text is None:
            return Response("No report of that stage in this session", status_code=404, media_type="text/plain")
        return HTMLResponse(
            text,
            headers={
                "Content-Security-Policy": "default-src 'none'; style-src 'unsafe-inline'; base-uri 'none'; form-action 'none'",
                "X-Content-Type-Options": "nosniff",
                "Cache-Control": "no-store",
            },
        )

    async def save_file(request: Request) -> Response:
        name = request.query_params.get("name", "")
        data = bytearray()
        async for chunk in request.stream():
            data += chunk
            if len(data) > viewer.MAX_SHOWN_BYTES + 1024:
                return JSONResponse(_refused(uploads.UploadRefused("too_large", "Text too large")), 413)
        try:
            text = bytes(data).decode("utf-8")
        except UnicodeDecodeError:
            return JSONResponse({"error": "the text is not UTF-8"}, 400)
        answer = await backend.write_file(name, text)
        return _file_answer(answer)

    async def copy_file(request: Request) -> Response:
        return _file_answer(await backend.copy_file(request.query_params.get("name", "")))

    def _file_answer(answer: Dict[str, Any]) -> Response:
        if answer.get("ok"):
            return JSONResponse(answer)
        kind = (answer.get("diagnostics") or [{}])[0].get("kind")
        status = {"file_not_found": 404, "path_refused": 403, "demo": 404, "no_session": 404, "unknown_session": 404, "ambiguous_file": 409}.get(kind, 400)
        return JSONResponse(answer, status)

    async def sessions(request: Request) -> Response:
        return JSONResponse(await backend.list_sessions())

    async def open_session(request: Request) -> Response:
        session = (await body(request)).get("session")
        if not isinstance(session, str) or not session or len(session) > 64:
            return JSONResponse({"error": "session is the id of a session"}, 400)
        return _file_answer(await backend.open_session(session))

    async def device_packages(request: Request) -> Response:
        return JSONResponse(await backend.device_packages())

    async def close_session(request: Request) -> Response:
        return JSONResponse(await backend.close_session())

    async def restore_session(request: Request) -> Response:
        checkpoint = (await body(request)).get("checkpoint")
        if checkpoint is not None and not isinstance(checkpoint, str):
            return JSONResponse({"error": "checkpoint is text"}, 400)
        return JSONResponse(await backend.restore_session(checkpoint))

    async def upload(request: Request) -> Response:
        name = request.query_params.get("name", "")
        overwrite = request.query_params.get("overwrite") == "1"
        limit = uploads.MAX_FILE_BYTES
        try:
            uploads.check_name(name)
        except uploads.UploadRefused as refused:
            return JSONResponse(_refused(refused), 400)
        declared = request.headers.get("content-length")
        if declared is not None and declared.isdigit() and int(declared) > limit:
            return JSONResponse(_refused(uploads.UploadRefused("too_large", "File too large")), 413)
        data = bytearray()
        async for chunk in request.stream():
            data += chunk
            if len(data) > limit:  # a stream that does not say its size
                return JSONResponse(
                    _refused(uploads.UploadRefused("too_large", "File too large")), 413
                )
        answer = await backend.upload(name, bytes(data), overwrite)
        if answer.get("ok"):
            return JSONResponse(answer)
        kind = (answer.get("diagnostics") or [{}])[0].get("kind")
        status = {"file_exists": 409, "too_large": 413, "session_full": 413, "demo": 404}.get(
            kind, 400
        )
        return JSONResponse(answer, status)

    async def remove_upload(request: Request) -> Response:
        answer = await backend.remove_upload(request.query_params.get("name", ""))
        if answer.get("ok"):
            return JSONResponse(answer)
        kind = (answer.get("diagnostics") or [{}])[0].get("kind")
        status = {"file_not_found": 404, "no_session": 404, "demo": 404}.get(kind, 400)
        return JSONResponse(answer, status)

    routes = [
        Route("/", index),
        Route("/api/session", session),
        Route("/api/config", app_config),
        Route("/api/chat", chat, methods=["POST"]),
        Route("/api/cancel", cancel, methods=["POST"]),
        Route("/api/console", console, methods=["POST"]),
        Route("/api/device-packages", device_packages),
        Route("/api/file", file),
        Route("/api/report", report),
        Route("/api/file", save_file, methods=["PUT"]),
        Route("/api/file/copy", copy_file, methods=["POST"]),
        Route("/api/sessions", sessions),
        Route("/api/session/open", open_session, methods=["POST"]),
        Route("/api/session/new", new_session, methods=["POST"]),
        Route("/api/session/device", apply_package, methods=["POST"]),
        Route("/api/session/close", close_session, methods=["POST"]),
        Route("/api/session/restore", restore_session, methods=["POST"]),
        Route("/api/upload", upload, methods=["POST"]),
        Route("/api/upload", remove_upload, methods=["DELETE"]),
    ]
    if (static / "assets").is_dir():
        routes.append(Mount("/assets", StaticFiles(directory=static / "assets")))
    app = Starlette(routes=routes)
    app.add_middleware(SecurityMiddleware, token=config.token, allowed_hosts=config.allowed_hosts())
    return app
