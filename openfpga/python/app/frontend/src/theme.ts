import { signal } from "@preact/signals";

/** The theme of the page: follow the system, or light, or dark. It is kept in this browser */
export type Theme = "auto" | "light" | "dark";
export const THEMES: Theme[] = ["auto", "light", "dark"];
const KEY = "openfpga-theme";

export const theme = signal<Theme>("auto");

function stored(): Theme {
  try {
    const value = window.localStorage.getItem(KEY);
    return THEMES.includes(value as Theme) ? (value as Theme) : "auto";
  } catch {
    return "auto"; // storage is off (a private window): the page still works
  }
}

/** Show a theme and keep it. "auto" leaves it to the system: the page has no mark then */
export function setTheme(next: Theme) {
  theme.value = next;
  const root = document.documentElement;
  if (next === "auto") root.removeAttribute("data-theme");
  else root.setAttribute("data-theme", next);
  try {
    if (next === "auto") window.localStorage.removeItem(KEY);
    else window.localStorage.setItem(KEY, next);
  } catch {
    /* not kept; it holds until the page is closed */
  }
}

export function loadTheme() {
  setTheme(stored());
}

export function nextTheme() {
  setTheme(THEMES[(THEMES.indexOf(theme.value) + 1) % THEMES.length]);
}
