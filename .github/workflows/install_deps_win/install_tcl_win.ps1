# Install dependencies for source code build on powershell

## Download and extract the tcl8.6 package
param(
    [string]$DownloadUrl = "https://github.com/teclabat/tcltk-binaries/releases/download/version-8.6.18.14/tcltk86-8.6.18.14.Win10.nightly.20260214.tgz",
    [switch]$AddToPath = $true,
    [switch]$AddToGithubEnv = $true
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
# There is subdir created when extracting the package. Hardcode here but it is dirty. Make it flexible when we can control the hierarchy when extracting
$TclHomeDir = Join-Path $InstallDir "tcltk86-8.6.18.14.Win10.nightly.20260214"

# EnvScope - Define where the path should be added (Session, User, or Machine)
$EnvScope = "User"  # Change this to "User" or "Session" based on desired scope

# === Optionally add bin to PATH ===
if ($AddToPath) {
    $binPath = Join-Path $TclHomeDir "bin"
    $currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")

    if ($currentPath -notlike "*$binPath*") {
        Write-Host "Adding $binPath to system PATH..."
        [System.Environment]::SetEnvironmentVariable(
            "Path",
            "$binPath;$currentPath",
            $EnvScope
        )
        Write-Host "PATH updated. Restart your shell to apply."
    }
    # Most binary packages have: InstallDir/lib
    $tclDirPath = $TclHomeDir
    
    if (!(Test-Path $tclDirPath)) {
        Write-Warning "TCL_DIR directory not found at $tclDirPath"
    } else {
        Write-Host "Setting TCL_DIR to $tclDirPath"
        [System.Environment]::SetEnvironmentVariable(
            "TCL_DIR",
            $tclDirPath,
            $EnvScope
        )
        $TCL_DIR = [System.Environment]::GetEnvironmentVariable("TCL_DIR", $EnvScope)
        Write-Host "TCL_DIR=${TCL_DIR} set in $EnvScope scope."
        if ($AddToGithubEnv) {
            Write-Host "User chooses to add TCL_DIR=${TCL_DIR} to github env"
            "TCL_DIR=${TCL_DIR}" >> $env:GITHUB_OUTPUT
        }
    }
}

Write-Host "Installation complete!"
Write-Host "Tcl directory:" (Get-ChildItem -Path $TclHomeDir)
Write-Host "Tclsh path:" (Join-Path $TclHomeDir "bin\tclsh.exe")

