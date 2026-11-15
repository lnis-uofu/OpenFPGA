import { defineConfig } from "vitest/config";
import preact from "@preact/preset-vite";

// The bundle goes into the Python package, which serves it: no Node is needed by a user
export default defineConfig({
  plugins: [preact()],
  build: {
    outDir: "../openfpga_app/static",
    emptyOutDir: true,
    sourcemap: false,
  },
  server: {
    // The development server asks the app (started with --demo) for the API
    proxy: { "/api": "http://127.0.0.1:8770" },
  },
  test: {
    environment: "jsdom",
    globals: true,
    setupFiles: ["src/test-setup.ts"],
  },
});
