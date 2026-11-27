// The menu of a right click on the page: the page's own, in place of the browser's. It has no
// commands yet. The browser's menu stays in a field of text (to copy and paste), and the editor
// keeps its own menu.
import { signal } from "@preact/signals";
import { useEffect, useRef } from "preact/hooks";

/** Where the menu is open, in px of the window, or null */
export const contextMenu = signal<{ x: number; y: number } | null>(null);

/** Whether a right click on this target keeps the browser's menu: a field of text */
export function keepsBrowserMenu(target: EventTarget | null): boolean {
  const element = target as HTMLElement | null;
  if (!element || typeof element.closest !== "function") return false;
  return !!element.closest(
    "input, textarea, [contenteditable]:not([contenteditable='false'])",
  );
}

/** A right click: the menu of the page, unless the click is in a field of text or something (the
 * editor) has its own menu */
export function onContextMenu(e: MouseEvent) {
  if (e.defaultPrevented || keepsBrowserMenu(e.target)) return;
  e.preventDefault();
  contextMenu.value = { x: e.clientX, y: e.clientY };
}

export function ContextMenu() {
  const menu = contextMenu.value;
  const ref = useRef<HTMLDivElement>(null);
  useEffect(() => {
    document.addEventListener("contextmenu", onContextMenu);
    return () => document.removeEventListener("contextmenu", onContextMenu);
  }, []);
  useEffect(() => {
    if (!menu) return;
    const close = () => (contextMenu.value = null);
    const onDown = (e: MouseEvent) => {
      if (!ref.current?.contains(e.target as Node)) close();
    };
    const onKey = (e: KeyboardEvent) => {
      if (e.key === "Escape") close();
    };
    document.addEventListener("mousedown", onDown, true);
    document.addEventListener("keydown", onKey, true);
    window.addEventListener("blur", close);
    window.addEventListener("resize", close);
    document.addEventListener("scroll", close, true);
    return () => {
      document.removeEventListener("mousedown", onDown, true);
      document.removeEventListener("keydown", onKey, true);
      window.removeEventListener("blur", close);
      window.removeEventListener("resize", close);
      document.removeEventListener("scroll", close, true);
    };
  }, [menu]);
  if (!menu) return null;
  // Inside the window: the menu is about 190 by 40 px
  const x = Math.max(0, Math.min(menu.x, window.innerWidth - 200));
  const y = Math.max(0, Math.min(menu.y, window.innerHeight - 50));
  return (
    <div
      ref={ref}
      class="context-menu"
      role="menu"
      aria-label="Menu"
      style={{ left: `${x}px`, top: `${y}px` }}
    >
      <div class="context-menu-item" role="menuitem" aria-disabled="true">
        Under development
      </div>
    </div>
  );
}
