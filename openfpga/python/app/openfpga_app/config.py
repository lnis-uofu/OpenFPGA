"""What the app is started with"""

import dataclasses
import secrets
from typing import Optional

LOOPBACK = ("127.0.0.1", "localhost", "::1")


@dataclasses.dataclass(frozen=True)
class AppConfig:
    host: str = "127.0.0.1"
    port: int = 8770
    token: str = dataclasses.field(default_factory=lambda: secrets.token_urlsafe(32))
    # The console of the shell is off unless asked for: it runs any command
    console: bool = False
    # An address that is not the local machine is refused unless this says it is meant
    allow_public_bind: bool = False
    # The endpoint of the model, shown in the settings (read only)
    model_endpoint: Optional[str] = None
    model_name: Optional[str] = None

    def __post_init__(self):
        if self.host not in LOOPBACK and not self.allow_public_bind:
            raise ValueError(
                "The app listens on the local machine only. %r is another address: "
                "--allow-public-bind says that it is meant (there are no accounts, only a token)"
                % self.host
            )

    def allowed_hosts(self):
        """The values of the Host header that are the app: the names of the local machine with the
        port (and the one that the app was started with)"""
        names = {"127.0.0.1", "localhost", "[::1]", self.host}
        return {"%s:%d" % (name, self.port) for name in names}
