import { config, settingsOpen } from "../store";
import { THEMES, setTheme, theme } from "../theme";

/** Settings: the model (read from the config, not edited here) and the console. The servers of MCP
 * are not connected in this version */
export function Settings() {
  if (!settingsOpen.value) return null;
  const c = config.value;
  return (
    <div class="overlay" role="dialog" aria-modal="true" aria-label="Settings">
      <div class="dialog">
        <div class="dialog-head">
          <h1>Settings</h1>
          <button onClick={() => (settingsOpen.value = false)}>← Back</button>
        </div>
        <h2>Theme</h2>
        <div class="theme-choice" role="group" aria-label="Theme">
          {THEMES.map((name) => (
            <button type="button" key={name} aria-pressed={theme.value === name} onClick={() => setTheme(name)}>
              {name === "auto" ? "Auto (the system)" : name === "light" ? "Light" : "Dark"}
            </button>
          ))}
        </div>
        <h2>Model</h2>
        <p class="muted">Read from the configuration of the app; edit it there.</p>
        <p>
          Endpoint: <code>{c?.model.endpoint ?? "(none: recorded demo)"}</code>
        </p>
        <p>
          Name: <code>{c?.model.name ?? "demo"}</code>
        </p>
        <h2>Console</h2>
        <p>
          {c?.console ? "On" : "Off"}. It runs any command of the OpenFPGA shell in the session: it is your own shell with the
          workspace as its directory, not a sandbox. Turn it on by starting the app with <code>--console</code>.
        </p>
        <h2>MCP servers</h2>
        <p class="muted">Not connected in this version.</p>
      </div>
    </div>
  );
}
