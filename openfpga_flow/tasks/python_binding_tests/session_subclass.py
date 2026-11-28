"""A subclass of openfpga.Session for test_session_extensions.py: it adds a method that a worker
may call, and keeps something of its own in its checkpoints. Imported by the tests and by their
workers (OPENFPGA_SESSION_CLASS=session_subclass:Recording)"""

import json
import os

import openfpga


class Recording(openfpga.Session):
    """Records the hooks of restore() that ran, in order, and keeps a note in its checkpoints"""

    REMOTE_METHODS = frozenset(("twice", "hooks"))

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.note = None
        self._hooks = []

    def twice(self, number):
        return 2 * number

    def hooks(self):
        return list(self._hooks)

    def not_remote(self):
        return "only in the process of the session"

    def _checkpoint_extra(self, directory):
        with open(os.path.join(directory, "note.json"), "w") as handle:
            json.dump(self.note, handle)
        return dict(super()._checkpoint_extra(directory), recording={"file": "note.json"})

    def _restore_inputs(self, manifest, directory):
        self._hooks.append(("inputs", self.state().inputs.vpr_arch is None))
        super()._restore_inputs(manifest, directory)

    def _restore_extra(self, extra, directory):
        state = self.state()
        # After the architectures, before the circuit
        self._hooks.append(
            ("extra", state.inputs.openfpga_arch is not None, state.inputs.circuit is None)
        )
        with open(os.path.join(directory, extra["recording"]["file"])) as handle:
            self.note = json.load(handle)
