import { useEffect, useRef, useState } from "preact/hooks";
import {
  banner,
  busy,
  config,
  makeSession,
  messages,
  packagesOpen,
  sendMessage,
  sessionsOpen,
  stop,
  togglePackages,
  toggleSessions,
  view,
  welcomeChat,
} from "../store";
import { EXAMPLES, MessageView, chatInput } from "./Chat";
import { PackagePicker } from "./PackagePicker";
import { SessionPicker } from "./SessionPicker";

/** The first screen, while there is no session and nothing has been said: a prompt in the middle and
 * the two ways to start a session at the bottom. It gives way to the full interface as soon as a
 * session exists or a message is sent */
export function Welcome() {
  const [text, setText] = useState("");
  const noModel = !config.value?.model.endpoint;
  const b = banner.value;
  const end = useRef<HTMLDivElement>(null);
  const talking = welcomeChat.value;
  useEffect(() => {
    end.current?.scrollIntoView?.({ block: "end" });
  });
  const send = () => {
    const message = text;
    setText("");
    welcomeChat.value = true; // the answer is shown here, below, until a session is loaded
    void sendMessage(message);
  };
  const grow = (el: HTMLTextAreaElement) => {
    el.style.height = "auto";
    el.style.height = `${Math.min(el.scrollHeight, 200)}px`;
  };
  return (
    <main class="welcome" aria-label="Start">
      <div class="welcome-centre">
        <h1>OpenFPGA</h1>
        <p class="muted">
          {noModel ? (
            <>
              No model is connected. A line that starts with <code>/</code> is a tool call: try <code>/help</code>.
            </>
          ) : (
            "Ask for a flow to be run, or start a session of your own below."
          )}
        </p>
        <form
          class="welcome-prompt"
          onSubmit={(e) => {
            e.preventDefault();
            send();
          }}
        >
          <textarea
            ref={(el) => {
              chatInput.current = el;
            }}
            value={text}
            rows={3}
            autofocus
            placeholder={noModel ? "/help" : "Ask for a flow to be run, or what a result means…"}
            aria-label="Message"
            onInput={(e) => {
              const el = e.target as HTMLTextAreaElement;
              setText(el.value);
              grow(el);
            }}
            onKeyDown={(e) => {
              if (e.key === "Enter" && !e.shiftKey) {
                e.preventDefault();
                send();
              }
            }}
          />
          {busy.value ? (
            <button type="button" onClick={() => void stop()}>
              ■ Stop
            </button>
          ) : (
            <button type="submit" disabled={!text.trim()}>
              Send
            </button>
          )}
        </form>
        {talking && (
          <section class="welcome-log" aria-label="Answer">
            <div class="welcome-log-head">
              <span class="muted" role="status">
                {view.value.latest.text}
                {busy.value ? " · working…" : ""}
              </span>
              <span class="spacer" />
              <button type="button" onClick={() => (welcomeChat.value = false)}>
                Open the full interface ›
              </button>
            </div>
            <div class="welcome-log-body" role="log" aria-live="polite">
              {messages.value.map((m) => (
                <MessageView key={m.id} message={m} />
              ))}
              <div ref={end} />
            </div>
          </section>
        )}
        {!noModel && !talking && (
          <div class="examples">
            {EXAMPLES.map((example) => (
              <button type="button" class="example" key={example} onClick={() => setText(example)}>
                {example}
              </button>
            ))}
          </div>
        )}
        {b && (
          <div class="banner" role="alert">
            ⚠ {b.message}
            {b.hint && <div>{b.hint}</div>}
          </div>
        )}
      </div>
      {packagesOpen.value && (
        <div class="welcome-packages">
          <PackagePicker />
        </div>
      )}
      {sessionsOpen.value && (
        <div class="welcome-packages">
          <SessionPicker />
        </div>
      )}
      <div class="welcome-buttons">
        <button class="primary" disabled={busy.value} onClick={() => void makeSession()}>
          Create session
        </button>
        <button disabled={busy.value} aria-expanded={packagesOpen.value} onClick={() => void togglePackages()}>
          Create session from device package…
        </button>
        <button disabled={busy.value} aria-expanded={sessionsOpen.value} onClick={() => void toggleSessions()}>
          Open session…
        </button>
      </div>
    </main>
  );
}
