/** The language of Monaco for a file name: what is not one of these is plain text */
export function languageOf(name: string): string {
  const ext = name.includes(".") ? name.slice(name.lastIndexOf(".") + 1).toLowerCase() : "";
  if (["xml", "net", "pb_type"].includes(ext)) return "xml"; // the packing (.net) is XML
  if (["v", "vh", "sv", "svh"].includes(ext)) return "systemverilog";
  if (["blif", "eblif"].includes(ext)) return "blif"; // the circuits
  if (["sdc", "tcl", "xdc"].includes(ext)) return "tcl";
  return "plaintext";
}
