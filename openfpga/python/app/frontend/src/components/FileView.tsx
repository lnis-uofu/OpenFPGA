import { useEffect, useRef, useState } from "preact/hooks";
import { languageOf } from "../language";
import { loadFile, openFiles } from "../store";
import { theme } from "../theme";

function size(bytes: number): string {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)} kB`;
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

const isDark = () =>
  theme.value === "dark" || (theme.value === "auto" && !!window.matchMedia?.("(prefers-color-scheme: dark)").matches);

/** Monaco, read only. It is loaded the first time (the page does not carry it before); if it
 * cannot be loaded the text is shown as it is */
function Editor(props: { name: string; text: string }) {
  const host = useRef<HTMLDivElement>(null);
  const editor = useRef<{ getValue(): string; setValue(v: string): void; dispose(): void } | null>(null);
  const latest = useRef(props.text);
  const [plain, setPlain] = useState(false);
  latest.current = props.text;
  const dark = isDark();

  useEffect(() => {
    let gone = false;
    import("../monaco")
      .then(({ monaco }) => {
        if (gone || !host.current) return;
        monaco.editor.setTheme(isDark() ? "vs-dark" : "vs");
        editor.current = monaco.editor.create(host.current, {
          value: latest.current,
          language: languageOf(props.name),
          readOnly: true,
          automaticLayout: true,
          minimap: { enabled: false },
          scrollBeyondLastLine: false,
          fontSize: 13,
          fontFamily: 'ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace',
          renderWhitespace: "none",
          wordWrap: "off",
          ariaLabel: props.name,
        });
      })
      .catch(() => setPlain(true));
    return () => {
      gone = true;
      editor.current?.dispose();
      editor.current = null;
    };
  }, [props.name]);

  useEffect(() => {
    if (editor.current && editor.current.getValue() !== props.text) editor.current.setValue(props.text);
  }, [props.text]);

  useEffect(() => {
    void import("../monaco").then(({ monaco }) => monaco.editor.setTheme(dark ? "vs-dark" : "vs")).catch(() => undefined);
  }, [dark]);

  if (plain) return <pre class="file-plain">{props.text}</pre>;
  return <div class="editor-host" ref={host} />;
}

/** A file of the session in a tab: the text in Monaco, with its size and a way to read it again */
export function FileView(props: { name: string }) {
  const state = openFiles.value[props.name];
  const base = props.name.split("/").pop() || props.name;
  return (
    <section class="file-view" aria-label={`File ${base}`}>
      <div class="result-head">
        <strong title={props.name}>{base}</strong>
        {state?.status === "ready" && (
          <span class="muted">
            {size(state.bytes)}
            {state.truncated ? " · the first megabyte is shown" : ""}
          </span>
        )}
        {state?.status === "loading" && <span class="muted">reading…</span>}
        <span class="spacer" />
        <button onClick={() => void loadFile(props.name)}>Reload</button>
        <button onClick={() => navigator.clipboard?.writeText(state?.text ?? "")} disabled={!state?.text}>
          Copy text
        </button>
      </div>
      {state?.status === "failed" ? (
        <p class="file-error" role="alert">
          {state.message}
        </p>
      ) : state && (state.text || state.status === "ready") ? (
        <Editor name={props.name} text={state.text} />
      ) : (
        <p class="muted pad">Reading…</p>
      )}
    </section>
  );
}
