import { fireEvent, render, screen } from "@testing-library/preact";
import { beforeEach, describe, expect, it } from "vitest";
import { Settings } from "./components/Settings";
import { TopBar } from "./components/TopBar";
import { settingsOpen } from "./store";
import { loadTheme, setTheme, theme } from "./theme";

const root = () => document.documentElement;

beforeEach(() => {
  window.localStorage.clear();
  setTheme("auto");
  settingsOpen.value = false;
});

describe("the theme", () => {
  it("leaves the page to the system when it is auto, and marks it when it is chosen", () => {
    expect(root().hasAttribute("data-theme")).toBe(false);
    setTheme("dark");
    expect(root().getAttribute("data-theme")).toBe("dark");
    setTheme("light");
    expect(root().getAttribute("data-theme")).toBe("light");
    setTheme("auto");
    expect(root().hasAttribute("data-theme")).toBe(false);
  });

  it("is kept in the browser and read again when the page opens", () => {
    setTheme("dark");
    expect(window.localStorage.getItem("openfpga-theme")).toBe("dark");
    setTheme("auto");
    setTheme("light");
    theme.value = "auto";
    loadTheme();
    expect(theme.value).toBe("light");
    expect(root().getAttribute("data-theme")).toBe("light");
    setTheme("auto");
    expect(window.localStorage.getItem("openfpga-theme")).toBeNull();
  });

  it("ignores a stored value that is not a theme", () => {
    window.localStorage.setItem("openfpga-theme", "purple");
    loadTheme();
    expect(theme.value).toBe("auto");
  });

  it("goes round auto, light, dark from the top bar", () => {
    render(<TopBar />);
    const button = () => screen.getByRole("button", { name: /^Theme:/ });
    expect(button().getAttribute("aria-label")).toBe("Theme: auto");
    fireEvent.click(button());
    expect(button().getAttribute("aria-label")).toBe("Theme: light");
    fireEvent.click(button());
    expect(button().getAttribute("aria-label")).toBe("Theme: dark");
    expect(root().getAttribute("data-theme")).toBe("dark");
    fireEvent.click(button());
    expect(button().getAttribute("aria-label")).toBe("Theme: auto");
  });

  it("is chosen by name in the settings, and says which one is on", () => {
    settingsOpen.value = true;
    render(<Settings />);
    fireEvent.click(screen.getByText("Dark"));
    expect(screen.getByText("Dark").getAttribute("aria-pressed")).toBe("true");
    expect(screen.getByText("Light").getAttribute("aria-pressed")).toBe("false");
    expect(root().getAttribute("data-theme")).toBe("dark");
  });
});
