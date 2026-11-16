import { useEffect, useRef, useState } from "preact/hooks";
import { Message, banner, busy, config, messages, sendMessage, stop } from "../store";
import { Markdown } from "../markdown";
import { CallGroup } from "./ToolCard";

const EXAMPLES = [
  "Run the flow on routing_test with a channel width of 20 and tell me the Fmax.",
  "Which options of the router can I change?",
  "Where is the critical path, and how long is it?",
];

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
        message.role === "user" ? (
          <p class="text user-text" key={`t${index}`}>
            {part.text}
          </p>
        ) : (
          <Markdown key={`t${index}`} text={part.text} />
        ),
      );
    }
  });
  flush();
  const [copied, setCopied] = useState(false);
  const written = message.parts.flatMap((p) => (p.kind === "text" ? [p.text] : [])).join("\n\n");
  return (
    <div class={`message message-${message.role}`}>
      <div class="bubble">{blocks}</div>
      {message.role !== "user" && written && (
        <div class="message-actions">
          <button
            type="button"
            class="copy-message"
            aria-label="Copy message"
            title="Copy"
            onClick={() => {
              void navigator.clipboard?.writeText(written);
              setCopied(true);
              setTimeout(() => setCopied(false), 1200);
            }}
          >
            {copied ? "✓ Copied" : "⧉ Copy"}
          </button>
        </div>
      )}
    </div>
  );
}

export const chatInput = { current: null as HTMLTextAreaElement | null };

/** The chat: the messages with their tool cards, and the box to write in */
export function Chat() {
  const [text, setText] = useState("");
  const end = useRef<HTMLDivElement>(null);
  const list = useRef<HTMLDivElement>(null);
  const stuck = useRef(true); // follow the stream only while the reader is at the bottom
  const [away, setAway] = useState(false);
  useEffect(() => {
    if (stuck.current) end.current?.scrollIntoView?.({ block: "end" });
  });
  const onScroll = () => {
    const el = list.current;
    if (!el) return;
    const near = el.scrollHeight - el.scrollTop - el.clientHeight < 40;
    stuck.current = near;
    setAway(!near);
  };
  const latest = () => {
    stuck.current = true;
    setAway(false);
    end.current?.scrollIntoView?.({ block: "end" });
  };
  const grow = (el: HTMLTextAreaElement) => {
    el.style.height = "auto";
    el.style.height = `${Math.min(el.scrollHeight, 200)}px`;
  };
  const send = () => {
    const message = text;
    setText("");
    stuck.current = true; // what the person sends is where the answer will be
    void sendMessage(message);
  };
  const noModel = config.value?.backend === "session" && !config.value.model.endpoint;
  const b = banner.value;
  return (
    <section class="chat" aria-label="Chat">
      <div class="messages" role="log" aria-live="polite" ref={list} onScroll={onScroll}>
        <div class="thread">
        {messages.value.length === 0 &&
          (noModel ? (
            <p class="muted empty">
              No model is connected. A line that starts with <code>/</code> is a tool call: try <code>/help</code>, then{" "}
              <code>/session_open</code>.
            </p>
          ) : (
            <div class="empty">
              <p class="muted">Ask for a flow to be run, or what a result means. For example:</p>
              <div class="examples">
                {EXAMPLES.map((example) => (
                  <button
                    type="button"
                    class="example"
                    key={example}
                    onClick={() => {
                      setText(example);
                      chatInput.current?.focus();
                    }}
                  >
                    {example}
                  </button>
                ))}
              </div>
            </div>
          ))}
        {messages.value.map((m) => (
          <MessageView key={m.id} message={m} />
        ))}
        {busy.value && (
          <p class="muted working" role="status">
            <span class="pulse" aria-hidden="true" /> working…
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
      </div>
      {away && (
        <button type="button" class="jump-latest" onClick={latest}>
          ↓ Latest
        </button>
      )}
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
          placeholder="Message the assistant…"
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
    </section>
  );
}
