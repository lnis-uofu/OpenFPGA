// Monaco, for the files that a person opens. It is loaded the first time that a file is opened
// (import() from the viewer), so the page does not carry it before that. Only what a read-only
// viewer needs: the editor, search, folding, brackets, go to line; the languages of the files of a
// flow (XML for the architectures and the packing; SystemVerilog; Tcl for the SDC; BLIF for the circuits and ODC for the constraints of a design, defined in blif.ts and odc.ts). There is
// no worker of a language: the editor worker is enough.
import * as monaco from "monaco-editor/editor/editor.api.js";
import "monaco-editor/editor/browser/coreCommands.js";
import "monaco-editor/editor/browser/widget/codeEditor/codeEditorWidget.js";
import "monaco-editor/editor/contrib/bracketMatching/browser/bracketMatching.js";
import "monaco-editor/editor/contrib/clipboard/browser/clipboard.js";
import "monaco-editor/editor/contrib/find/browser/findController.js";
import "monaco-editor/editor/contrib/folding/browser/folding.js";
import "monaco-editor/editor/contrib/multicursor/browser/multicursor.js";
import "monaco-editor/editor/contrib/wordHighlighter/browser/wordHighlighter.js";
import "monaco-editor/editor/contrib/contextmenu/browser/contextmenu.js";
import "monaco-editor/editor/contrib/linesOperations/browser/linesOperations.js";
import "monaco-editor/editor/standalone/browser/quickAccess/standaloneGotoLineQuickAccess.js";
import "../node_modules/monaco-editor/esm/vs/base/browser/ui/codicons/codicon/codicon.css";
import "monaco-editor/languages/definitions/xml/register.js";
import "monaco-editor/languages/definitions/systemverilog/register.js";
import "monaco-editor/languages/definitions/tcl/register.js";
import { BLIF_ID, blifConfiguration, blifTokens } from "./blif";
import { ODC_ID, odcConfiguration, odcTokens } from "./odc";
import EditorWorker from "monaco-editor/editor/editor.worker.js?worker";

monaco.languages.register({ id: BLIF_ID, extensions: [".blif", ".eblif"], aliases: ["BLIF", "blif"] });
monaco.languages.setLanguageConfiguration(BLIF_ID, blifConfiguration);
monaco.languages.setMonarchTokensProvider(BLIF_ID, blifTokens as never);
monaco.languages.register({ id: ODC_ID, extensions: [".odc"], aliases: ["ODC", "odc"] });
monaco.languages.setLanguageConfiguration(ODC_ID, odcConfiguration);
monaco.languages.setMonarchTokensProvider(ODC_ID, odcTokens as never);

(self as unknown as { MonacoEnvironment: unknown }).MonacoEnvironment = {
  getWorker: () => new EditorWorker(),
};

export { monaco };
