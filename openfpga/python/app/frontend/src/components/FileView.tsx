import { useEffect, useRef, useState } from "preact/hooks";
import { languageOf } from "../language";
import { config, editCopy, loadFile, openFiles, saveFile } from "../store";
import { theme } from "../theme";

function size(bytes: number): string {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)} kB`;
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

const isDark = () =>
  theme.value === "dark" || (theme.value === "auto" && !!window.matchMedia?.("(prefers-color-scheme: dark)").matches);

/** Monaco. It is loaded the first time (the page does not carry it before); if it cannot be loaded
 * the text is shown as it is. A file that may be edited is edited here: onChange gets the text
 * after each change, onSave is called by Ctrl+S (Cmd+S) */
function Editor(props: { name: string; text: string; editable: boolean; onChange: (text: string) => void; onSave: () => void }) {
  const host = useRef<HTMLDivElement>(null);
  const editor = useRef<{ getValue(): string; setValue(v: string): void; dispose(): void } | null>(null);
  const latest = useRef(props.text);
  const save = useRef(props.onSave);
  const change = useRef(props.onChange);
  const [plain, setPlain] = useState(false);
  latest.current = props.text;
  save.current = props.onSave;
  change.current = props.onChange;
  const dark = isDark();

  useEffect(() => {
    let gone = false;
    import("../monaco")
      .then(({ monaco }) => {
        if (gone || !host.current) return;
        monaco.editor.setTheme(isDark() ? "vs-dark" : "vs");
        const created = monaco.editor.create(host.current, {
          value: latest.current,
          language: languageOf(props.name),
          readOnly: !props.editable,
          automaticLayout: true,
          minimap: { enabled: false },
          scrollBeyondLastLine: false,
          fontSize: 13,
          fontFamily: 'ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace',
          renderWhitespace: "none",
          wordWrap: "off",
          ariaLabel: props.name,
        });
        editor.current = created;
        if (props.editable) {
          created.onDidChangeModelContent(() => change.current(created.getValue()));
          created.addCommand(monaco.KeyMod.CtrlCmd | monaco.KeyCode.KeyS, () => save.current());
        }
      })
      .catch(() => setPlain(true));
    return () => {
      gone = true;
      editor.current?.dispose();
      editor.current = null;
    };
  }, [props.name, props.editable]);

  useEffect(() => {
    if (editor.current && editor.current.getValue() !== props.text) editor.current.setValue(props.text);
  }, [props.text]);

  useEffect(() => {
    void import("../monaco").then(({ monaco }) => monaco.editor.setTheme(dark ? "vs-dark" : "vs")).catch(() => undefined);
  }, [dark]);

  if (plain) return <pre class="file-plain">{props.text}</pre>;
  return <div class="editor-host" ref={host} />;
}

/** A file of the session in a tab: the text in Monaco, with its size and a way to read it again. A
 * file that the person gave or that the model wrote is edited and saved here; any other file of a
 * kind that a flow reads can be copied into the session to be edited there */
export function FileView(props: { name: string }) {
  const state = openFiles.value[props.name];
  const base = props.name.split("/").pop() || props.name;
  const real = config.value?.backend === "session";
  const editable = !!state?.editable && real;
  const [draft, setDraft] = useState<string | null>(null);
  const [note, setNote] = useState<{ kind: "ok" | "warn" | "error"; text: string } | null>(null);
  const [saving, setSaving] = useState(false);
  const dirty = editable && draft !== null && draft !== state?.text;
  useEffect(() => setDraft(null), [state?.text]); // what was read or saved is the text now

  const save = async () => {
    if (!dirty || draft === null || saving) return;
    setSaving(true);
    const result = await saveFile(props.name, draft);
    setSaving(false);
    setNote(
      !result.ok
        ? { kind: "error", text: result.message }
        : result.inUse
          ? { kind: "warn", text: "Saved. The session had read this file: read it again for the change to count." }
          : { kind: "ok", text: "Saved." },
    );
  };
  const reload = () => {
    if (dirty && !window.confirm("Throw away the changes that are not saved?")) return;
    setNote(null);
    void loadFile(props.name);
  };
  return (
    <section class="file-view" aria-label={`File ${base}`}>
      <div class="result-head">
        <strong title={props.name}>
          {base}
          {dirty ? " ●" : ""}
        </strong>
        {state?.status === "ready" && (
          <span class="muted">
            {size(state.bytes)}
            {state.truncated ? " · the first megabyte is shown" : ""}
            {editable ? "" : " · read only"}
          </span>
        )}
        {state?.status === "loading" && <span class="muted">reading…</span>}
        {note && (
          <span class={`file-note file-note-${note.kind}`} role="status">
            {note.text}
          </span>
        )}
        <span class="spacer" />
        {editable && (
          <button class="save-file" disabled={!dirty || saving} onClick={() => void save()} title="Save (Ctrl+S)">
            Save
          </button>
        )}
        {real && state?.copyable && !state.truncated && (
          <button onClick={() => void editCopy(props.name)} title="Copy this file into the session and edit the copy">
            Edit a copy
          </button>
        )}
        <button onClick={reload}>Reload</button>
        <button onClick={() => navigator.clipboard?.writeText(draft ?? state?.text ?? "")} disabled={!state?.text}>
          Copy text
        </button>
      </div>
      {state?.status === "failed" ? (
        <p class="file-error" role="alert">
          {state.message}
        </p>
      ) : state && (state.text || state.status === "ready") ? (
        <Editor name={props.name} text={state.text} editable={editable} onChange={setDraft} onSave={() => void save()} />
      ) : (
        <p class="muted pad">Reading…</p>
      )}
    </section>
  );
}
