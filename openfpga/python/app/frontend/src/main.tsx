import { render } from "preact";
import { App } from "./App";
import { loadTheme } from "./theme";
import "./style.css";

loadTheme();

render(<App />, document.getElementById("app")!);
