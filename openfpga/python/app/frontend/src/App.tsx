import { useEffect } from "preact/hooks";
import { chatInput } from "./components/Chat";
import { Console } from "./components/Console";
import { Settings } from "./components/Settings";
import { Sidebar, SidebarResize } from "./components/Sidebar";
import { TopBar } from "./components/TopBar";
import { Welcome } from "./components/Welcome";
import { Workspace } from "./components/Workspace";
import { activeTab, closeTab, consoleOpen, loaded, loadAll, nextTab, settingsOpen, sidebarOpen, stop, welcoming, CHAT } from "./store";

/** The keys: Ctrl+K the chat, Ctrl+. stop, Ctrl+` the console, Ctrl+B the sidebar,
 * Ctrl+J and Ctrl+Shift+J the next and the previous tab, Ctrl+W close a result tab, Esc close */
export function handleKey(e: KeyboardEvent): boolean {
  if (e.key === "Escape" && settingsOpen.value) {
    settingsOpen.value = false;
    return true;
  }
  if (!e.ctrlKey) return false;
  switch (e.key.toLowerCase()) {
    case "k":
      activeTab.value = CHAT;
      setTimeout(() => chatInput.current?.focus());
      return true;
    case ".":
      void stop();
      return true;
    case "`":
      consoleOpen.value = !consoleOpen.value;
      return true;
    case "b":
      sidebarOpen.value = !sidebarOpen.value;
      return true;
    case "j":
      nextTab(e.shiftKey ? -1 : 1);
      return true;
    case "w":
      if (activeTab.value !== CHAT) {
        closeTab(activeTab.value);
        return true;
      }
      return false;
  }
  return false;
}

export function App() {
  useEffect(() => {
    void loadAll();
    const onKey = (e: KeyboardEvent) => {
      if (handleKey(e)) e.preventDefault();
    };
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, []);
  return (
    <div class="app">
      <TopBar />
      {!loaded.value ? (
        <div class="body" />
      ) : welcoming() ? (
        <Welcome />
      ) : (
        <>
          <div class="body">
            <Sidebar />
            <SidebarResize />
            <Workspace />
          </div>
          <Console />
        </>
      )}
      <Settings />
    </div>
  );
}
