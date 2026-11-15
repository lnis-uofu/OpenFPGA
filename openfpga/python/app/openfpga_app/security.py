"""The checks of every request: a page that any site can open is a way into the machine

Host (against DNS rebinding) and Origin (against pages of other sites) are checked, there is no
CORS, and a token is needed: in a cookie that the browser keeps (SameSite=Strict), or in the
header Authorization for a program. The token comes in the address that the server prints, once:
it is taken out of the address at the first load.
"""

import secrets
from typing import Callable, Iterable
from urllib.parse import parse_qsl, urlencode

from starlette.requests import Request
from starlette.responses import HTMLResponse, RedirectResponse, Response
from starlette.types import ASGIApp, Receive, Scope, Send

COOKIE = "openfpga_app_token"
UNAUTHORIZED_PAGE = """<!doctype html><meta charset="utf-8"><title>OpenFPGA</title>
<body style="font-family:sans-serif;max-width:32em;margin:4em auto">
<h1>OpenFPGA</h1>
<p>Open the address that the server printed in the terminal. It has a token.</p></body>"""


def _same(a: str, b: str) -> bool:
    return secrets.compare_digest(a.encode(), b.encode())


class SecurityMiddleware:
    """Refuses the request that has a foreign Host or Origin, or no token"""

    def __init__(self, app: ASGIApp, token: str, allowed_hosts: Iterable[str]):
        self.app = app
        self.token = token
        self.hosts = set(allowed_hosts)

    async def __call__(self, scope: Scope, receive: Receive, send: Send):
        if scope["type"] not in ("http", "websocket"):
            return await self.app(scope, receive, send)
        request = Request(scope, receive)
        refusal = self._check(request)
        if refusal is not None:
            if scope["type"] == "websocket":
                return await send({"type": "websocket.close", "code": 1008})
            return await refusal(scope, receive, send)
        return await self.app(scope, receive, send)

    def _check(self, request: Request):
        host = request.headers.get("host", "")
        if host not in self.hosts:
            return Response("Bad Host", status_code=421)
        origin = request.headers.get("origin")
        if origin is not None and origin not in {"http://" + h for h in self.hosts}:
            return Response("Bad Origin", status_code=403)
        if self._has_token(request):
            return None
        given = dict(parse_qsl(request.url.query)).get("token")
        if given is not None and _same(given, self.token) and request.scope.get("method") == "GET":
            # The first load: keep the token in a cookie and take it out of the address
            rest = urlencode([(k, v) for k, v in parse_qsl(request.url.query) if k != "token"])
            target = request.url.path + ("?" + rest if rest else "")
            response = RedirectResponse(target, status_code=303)
            response.set_cookie(COOKIE, self.token, httponly=True, samesite="strict", path="/")
            return response
        if request.url.path.startswith("/api/"):
            return Response("Token needed", status_code=401)
        return HTMLResponse(UNAUTHORIZED_PAGE, status_code=401)

    def _has_token(self, request: Request) -> bool:
        cookie = request.cookies.get(COOKIE)
        if cookie is not None and _same(cookie, self.token):
            return True
        header = request.headers.get("authorization", "")
        return header.startswith("Bearer ") and _same(header[7:], self.token)
