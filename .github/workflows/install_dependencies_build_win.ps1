# Install dependencies for source code build on powershell

## Download and extract the tcl8.6 package
param(
    [string]$DownloadUrl = "https://github.com/teclabat/tcltk-binaries/releases/download/version-8.6.18.14/tcltk86-8.6.18.14.Win10.nightly.20260214.tgz",
    [switch]$AddToPath = $true
)

$ErrorActionPreference = "Stop"

# Temporary download path
$CurrDir = (Get-Location).Path
$InstallDir = Join-Path $CurrDir "third_party_tcltk"
$TempArchive = Join-Path $env:TEMP "tcltk.tgz"
$ExtractDir  = Join-Path $env:TEMP "tcltk_extract"

# Clean up old temp data
Remove-Item -Path $TempArchive -Force -ErrorAction SilentlyContinue
Remove-Item -Path $ExtractDir -Recurse -Force -ErrorAction SilentlyContinue

# === Download ===
Write-Host "Downloading Tcl/Tk TGZ from:" $DownloadUrl
Invoke-WebRequest -Uri $DownloadUrl -OutFile $TempArchive

# === Extract TGZ ===
Write-Host "Extracting archive..."
New-Item -ItemType Directory -Path $ExtractDir -Force | Out-Null

# Try using built-in tar (available in Windows 10+ / PowerShell 7+)
$tarFound = Get-Command tar.exe -ErrorAction SilentlyContinue

if ($tarFound) {
    Write-Host "Using built-in tar to extract..."
    & tar.exe -xzf $TempArchive -C $ExtractDir
} else {
    throw "tar.exe not found. cannot extract .tgz"
}

# === Install (copy files) ===
Write-Host "Installing to: $InstallDir"
if (!(Test-Path $InstallDir)) {
    New-Item -ItemType Directory -Path $InstallDir | Out-Null
}

# Copy extracted files to target
Copy-Item -Path (Join-Path $ExtractDir "*") -Destination $InstallDir -Recurse -Force
$TclHomeDir = Join-Path $InstallDir "tcltk86-8.6.18.14.Win10.nightly.20260214"

# === Optionally add bin to PATH ===
if ($AddToPath) {
    $binPath = Join-Path $TclHomeDir "bin"
    $currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")

    if ($currentPath -notlike "*$binPath*") {
        Write-Host "Adding $binPath to system PATH..."
        [Environment]::SetEnvironmentVariable(
            "Path",
            "$currentPath;$binPath",
            "Machine"
        )
        Write-Host "PATH updated. Restart your shell to apply."
    }
    # Most binary packages have: InstallDir/lib
    $tclLibPath = Join-Path $TclHomeDir "lib"
    
    if (!(Test-Path $tclLibPath)) {
        Write-Warning "TCL_LIBRARY directory not found at $tclLibPath"
    } else {
        Write-Host "Setting TCL_LIBRARY to $tclLibPath"
        [Environment]::SetEnvironmentVariable(
            "TCL_LIBRARY",
            $tclLibPath,
            $EnvScope
        )
        Write-Host "TCL_LIBRARY set in $EnvScope scope."
    }
    # Most binary packages have: InstallDir/include
    $tclInclPath = Join-Path $TclHomeDir "include"
    
    if (!(Test-Path $tclInclPath)) {
        Write-Warning "TCL_INCLUDE_PATH directory not found at $tclInclPath"
    } else {
        Write-Host "Setting TCL_INCLUDE_PATH to $tclInclPath"
        [Environment]::SetEnvironmentVariable(
            "TCL_INCLUDE_PATH",
            $tclInclPath,
            $EnvScope
        )
        Write-Host "TCL_INCLUDE_PATH set in $EnvScope scope."
    }
}

Write-Host "Installation complete!"
Write-Host "Tcl directory:" (Get-ChildItem -Path $TclHomeDir)
Write-Host "Tclsh path:" (Join-Path $TclHomeDir "bin\tclsh.exe")

