import { busy, devicePackages, makeSessionFromPackage } from "../store";

/** The packages that a session can be created from: it reads their architectures and waits for the circuit */
export function PackagePicker() {
  const found = devicePackages.value;
  return (
    <div class="packages" role="region" aria-label="Device packages">
      {found === null ? (
        <div class="muted">Looking…</div>
      ) : (
        <>
          {found.packages.length === 0 && (
            <div class="muted">No device packages. Start the app with --device-packages FOLDER.</div>
          )}
          {found.packages.map((p) => (
            <div key={p.name} class="package">
              <div class="package-head">
                <strong>{p.name}</strong>
                <span class="chip">{p.layout}</span>
                {p.lut_size != null && <span class="chip">K{p.lut_size}</span>}
              </div>
              <div class="muted package-text">{p.description}</div>
              {p.tags.length > 0 && <div class="muted package-text">{p.tags.join(" · ")}</div>}
              <button disabled={busy.value} onClick={() => void makeSessionFromPackage(p.name)}>
                Create session
              </button>
            </div>
          ))}
          {found.problems.map((p) => (
            <div key={p.name} class="package package-bad" title={p.message}>
              <strong>{p.name}</strong> <span class="muted">cannot be used: {p.message.replace(/^The device package '[^']*' cannot be used: /, "")}</span>
            </div>
          ))}
        </>
      )}
    </div>
  );
}
