import { config, settingsOpen, sidebarOpen, usage, view } from "../store";
import { nextTheme, theme } from "../theme";

const THEME_SYMBOL = { auto: "◐", light: "☀", dark: "☾" } as const;

const SYMBOL = { none: "○", running: "░", reached: "✔", failed: "⚠" } as const;

/** The top bar: only the latest state of the session, the model and the settings */
export function TopBar() {
  const latest = view.value.latest;
  const model = config.value?.model;
  const session = view.value.session;
  return (
    <header class="topbar">
      <button class="icon only-narrow" aria-label="Show or hide the sidebar" onClick={() => (sidebarOpen.value = !sidebarOpen.value)}>
        ≡
      </button>
      <strong class="brand">OpenFPGA</strong>
      <span class={`latest latest-${latest.status}`} role="status" aria-live="polite">
        <span aria-hidden="true">{SYMBOL[latest.status]}</span> {latest.text}
      </span>
      <span class="spacer" />
      {usage.value && (
        <span
          class={`context ${usage.value.used > usage.value.limit * 0.8 ? "context-full" : ""}`}
          title={`Step ${usage.value.step}: the context of the model is ${usage.value.used} of ${usage.value.limit} tokens`}
        >
          ctx {usage.value.used.toLocaleString()}/{usage.value.limit.toLocaleString()}
        </span>
      )}
      {session && (
        <span
          class={`worker worker-${session.worker}`}
          title={session.worker === "running" ? `The worker of session ${session.id} is running` : session.ended ?? "The worker has ended"}
        >
          <span class={`status-dot ${session.worker === "running" ? "on" : "off"}`} aria-hidden="true" />{" "}
          {session.worker === "running" ? "worker running" : session.worker === "none" ? "closed" : "⚠ worker ended"}
        </span>
      )}
      <span class="model" title={model?.endpoint ?? "recorded demo"}>
        <span class={model?.endpoint ? "dot ok" : "dot off"} aria-hidden="true">●</span> {model?.name ?? "demo"}
      </span>
      <button
        class="icon"
        aria-label={`Theme: ${theme.value}`}
        title={`Theme: ${theme.value} (click for the next: auto, light, dark)`}
        onClick={nextTheme}
      >
        {THEME_SYMBOL[theme.value]}
      </button>
      <button class="icon" aria-label="Settings" onClick={() => (settingsOpen.value = true)}>
        ⚙
      </button>
    </header>
  );
}
