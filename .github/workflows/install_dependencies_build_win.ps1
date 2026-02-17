# Install dependencies for source code build on powershell

# Download and extract the swigwin package
(New-Object System.Net.WebClient).DownloadFile("https://prdownloads.sourceforge.net", "swigwin-4.4.1.zip")
Expand-Archive -Path swigwin-4.4.1.zip -DestinationPath .
# Add the directory containing the swig.exe to the PATH
$env:Path += ";$PWD\swigwin-4.4.1"
# Make the updated PATH available to subsequent steps in the job
echo "::add-path::$PWD\swigwin-4.4.1"
