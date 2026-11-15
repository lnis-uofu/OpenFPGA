"""The local web app: a chat, the state of a session and a console, in the browser.

See README.md next to this package. It serves on the local machine only. This first version has a
demo backend (a recorded conversation) so that the page can be built and tested; the agent loop
and the connection to the tools come later.
"""

from .config import AppConfig
from .events import EVENT_TYPES, SCHEMA_VERSION

__all__ = ["AppConfig", "EVENT_TYPES", "SCHEMA_VERSION"]
