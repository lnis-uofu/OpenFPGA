import { useEffect, useRef, useState } from "preact/hooks";
import { Message, banner, busy, config, messages, sendMessage, stop } from "../store";
import { CallGroup } from "./ToolCard";

/** The parts of a message in order: text, then the calls in a row as one group */
function MessageView(props: { message: Message }) {
  const { message } = props;
  const blocks: preact.ComponentChildren[] = [];
  let run: string[] = [];
  const flush = () => {
    if (run.length) blocks.push(<CallGroup key={`g${blocks.length}`} callIds={run} />);
    run = [];
  };
  message.parts.forEach((part, index) => {
    if (part.kind === "call") run.push(part.callId);
    else {
      flush();
      blocks.push(
        <p class="text" key={`t${index}`}>
          {part.text}
        </p>,
      );
    }
  });
  flush();
  return (
    <div class={`message message-${message.role}`}>
      <div class="who">{message.role === "user" ? "you" : "assistant"}</div>
      <div class="bubble">{blocks}</div>
    </div>
  );
}

export const chatInput = { current: null as HTMLTextAreaElement | null };

/** The chat: the messages with their tool cards, and the box to write in */
export function Chat() {
  const [text, setText] = useState("");
  const end = useRef<HTMLDivElement>(null);
  useEffect(() => {
    end.current?.scrollIntoView?.({ block: "end" });
  });
  const send = () => {
    const message = text;
    setText("");
    void sendMessage(message);
  };
  const b = banner.value;
  return (
    <section class="chat" aria-label="Chat">
      <div class="messages" role="log" aria-live="polite">
        {messages.value.length === 0 &&
          (config.value?.backend === "session" ? (
            <p class="muted empty">
              No model is connected. A line that starts with <code>/</code> is a tool call: try <code>/help</code>, then{" "}
              <code>/session_open</code>.
            </p>
          ) : (
            <p class="muted empty">Ask for a flow to be run, or what the last result means.</p>
          ))}
        {messages.value.map((m) => (
          <MessageView key={m.id} message={m} />
        ))}
        {busy.value && (
          <p class="muted working" role="status">
            ░ working…
          </p>
        )}
        {b && (
          <div class="banner" role="alert">
            ⚠ {b.message}
            {b.hint && <div>{b.hint}</div>}
          </div>
        )}
        <div ref={end} />
      </div>
      <form
        class="send"
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
          rows={2}
          placeholder="Message…"
          aria-label="Message"
          onInput={(e) => setText((e.target as HTMLTextAreaElement).value)}
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
    </section>
  );
}
