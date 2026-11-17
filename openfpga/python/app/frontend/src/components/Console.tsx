import { useEffect, useRef, useState } from "preact/hooks";
import {
  CONSOLE_HEIGHT_DEFAULT,
  config,
  consoleBusy,
  consoleHeight,
  consoleLines,
  consoleOpen,
  consoleRun,
  setConsoleHeight,
  view,
} from "../store";

/** The console of the OpenFPGA shell: a drawer under the page. Off unless the app was started with
 * --console, because it runs any command */
export function Console() {
  const [text, setText] = useState("");
  const [history, setHistory] = useState<string[]>([]);
  const [at, setAt] = useState<number | null>(null);
  const end = useRef<HTMLDivElement>(null);
  useEffect(() => {
    end.current?.scrollIntoView?.({ block: "end" });
  });
  const on = config.value?.console ?? false;
  const toggle = () => (consoleOpen.value = !consoleOpen.value);
  if (!consoleOpen.value) {
    return (
      <footer class="console collapsed">
        <button class="console-toggle" aria-expanded="false" onClick={toggle}>
          ▸ Console{on ? "" : " (off: start with --console)"}
        </button>
      </footer>
    );
  }
  const run = () => {
    const command = text.trim();
    if (!command) return;
    setHistory([...history, command]);
    setAt(null);
    setText("");
    void consoleRun(command);
  };
  const recall = (step: -1 | 1) => {
    if (!history.length) return;
    const next = at === null ? (step < 0 ? history.length - 1 : null) : at + step;
    if (next === null || next >= history.length) {
      setAt(null);
      setText("");
    } else if (next >= 0) {
      setAt(next);
      setText(history[next]);
    }
  };
  const session = view.value.session;
  const drag = (e: PointerEvent) => {
    // Dragging the top edge up makes the console higher
    const handle = e.currentTarget as HTMLElement;
    const startY = e.clientY;
    const start = consoleHeight.value;
    handle.setPointerCapture?.(e.pointerId);
    const move = (m: PointerEvent) => setConsoleHeight(start + (startY - m.clientY));
    const stopDrag = () => {
      handle.removeEventListener("pointermove", move);
      handle.removeEventListener("pointerup", stopDrag);
      handle.removeEventListener("pointercancel", stopDrag);
    };
    handle.addEventListener("pointermove", move);
    handle.addEventListener("pointerup", stopDrag);
    handle.addEventListener("pointercancel", stopDrag);
    e.preventDefault();
  };
  return (
    <footer class="console" style={{ "--console-h": `${consoleHeight.value}px` }}>
      <div
        class="console-resize"
        role="separator"
        aria-orientation="horizontal"
        aria-label="Resize the console"
        aria-valuenow={consoleHeight.value}
        tabIndex={0}
        title="Drag to resize (arrow keys too; double click for the usual size)"
        onPointerDown={drag}
        onDblClick={() => setConsoleHeight(CONSOLE_HEIGHT_DEFAULT)}
        onKeyDown={(e) => {
          if (e.key === "ArrowUp") (e.preventDefault(), setConsoleHeight(consoleHeight.value + 24));
          else if (e.key === "ArrowDown") (e.preventDefault(), setConsoleHeight(consoleHeight.value - 24));
        }}
      />
      <div class="console-head">
        <button class="console-toggle" aria-expanded="true" onClick={toggle}>
          ▾ Console
        </button>
        {session && <span class="muted">session {session.id}</span>}
        <span class="spacer" />
        <button onClick={() => (consoleLines.value = [])}>Clear</button>
      </div>
      {on ? (
        <>
          <div class="console-lines" role="log">
            {consoleLines.value.map((l, i) => (
              <div key={i} class={`line line-${l.kind}`}>
                {l.kind === "input" ? "openfpga> " : l.kind === "model" ? "model › " : ""}
                {l.text}
              </div>
            ))}
            {consoleBusy.value && <div class="line muted">busy…</div>}
            <div ref={end} />
          </div>
          <label class="console-input">
            <span>openfpga&gt; </span>
            <input
              value={text}
              aria-label="Command"
              disabled={consoleBusy.value}
              onInput={(e) => setText((e.target as HTMLInputElement).value)}
              onKeyDown={(e) => {
                if (e.key === "Enter") run();
                else if (e.key === "ArrowUp") (e.preventDefault(), recall(-1));
                else if (e.key === "ArrowDown") (e.preventDefault(), recall(1));
              }}
            />
          </label>
        </>
      ) : (
        <p class="muted pad">The console is off. It runs any command of the OpenFPGA shell: start the app with --console.</p>
      )}
    </footer>
  );
}
