param([string]$KitDirectory = '', [switch]$DownloadOnly)
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'
$repoRoot = Split-Path $PSScriptRoot -Parent
Set-Location -LiteralPath $repoRoot
Add-Type -AssemblyName System.Net.Http
Add-Type -AssemblyName System.IO.Compression.FileSystem
function Get-RuntimeSha256([string]$Path) {
    $stream = [IO.File]::OpenRead($Path)
    $hasher = [Security.Cryptography.SHA256]::Create()
    try { return [BitConverter]::ToString($hasher.ComputeHash($stream)).Replace('-','').ToLowerInvariant() }
    finally { $hasher.Dispose(); $stream.Dispose() }
}
$manifest = Get-Content -LiteralPath (Join-Path $repoRoot 'runtime-manifest.json') -Raw | ConvertFrom-Json
$runtimeRoot = Join-Path $repoRoot 'runtime'
$cacheRoot = Join-Path $repoRoot '.cache'
New-Item -ItemType Directory -Force -Path $runtimeRoot,$cacheRoot | Out-Null
$marker = Join-Path $runtimeRoot '.baseline-installed'
if (!(Test-Path -LiteralPath $marker)) {
    if (!$KitDirectory) {
        $credentialLines = "protocol=https`nhost=github.com`n`n" | git credential fill
        if ($LASTEXITCODE -ne 0) { throw 'Sign in to GitHub through Git Credential Manager, then retry.' }
        $tokenLine = $credentialLines | Where-Object { $_ -like 'password=*' } | Select-Object -First 1
        if (!$tokenLine) { throw 'GitHub credential unavailable.' }
        $token = $tokenLine.Substring(9)
        $headers = @{Authorization="Bearer $token";Accept='application/vnd.github+json';'User-Agent'='LAQIA-Setup'}
        $release = Invoke-RestMethod -Uri "https://api.github.com/repos/$($manifest.repository)/releases/tags/$($manifest.tag)" -Headers $headers
    }
    foreach ($asset in $manifest.assets) {
        $zipPath = Join-Path $cacheRoot $asset.name
        if ($KitDirectory) { $zipPath = Join-Path $KitDirectory $asset.name }
        if (!(Test-Path -LiteralPath $zipPath) -or (Get-RuntimeSha256 $zipPath) -ne $asset.sha256) {
            if ($KitDirectory) { throw "Local kit missing or invalid: $($asset.name)" }
            $remoteAsset = $release.assets | Where-Object { $_.name -eq $asset.name } | Select-Object -First 1
            if (!$remoteAsset) { throw "Release asset missing: $($asset.name)" }
            Write-Host "Downloading $($asset.name)..."
            $handler = New-Object System.Net.Http.HttpClientHandler
            $handler.AllowAutoRedirect = $false
            $http = New-Object System.Net.Http.HttpClient($handler)
            $http.Timeout = [TimeSpan]::FromMinutes(90)
            $url = $remoteAsset.url
            try {
                for ($redirect = 0; $redirect -lt 6; $redirect++) {
                    $request = New-Object System.Net.Http.HttpRequestMessage([System.Net.Http.HttpMethod]::Get,$url)
                    $request.Headers.Add('User-Agent','LAQIA-Setup')
                    if (([Uri]$url).Host -eq 'api.github.com') {
                        $request.Headers.Add('Authorization',"Bearer $token")
                        $request.Headers.Add('Accept','application/octet-stream')
                    }
                    $response = $http.SendAsync($request,[System.Net.Http.HttpCompletionOption]::ResponseHeadersRead).GetAwaiter().GetResult()
                    if ([int]$response.StatusCode -ge 300 -and [int]$response.StatusCode -lt 400) {
                        $url = $response.Headers.Location.AbsoluteUri
                        if (![Uri]::IsWellFormedUriString($url,[UriKind]::Absolute) -or ([Uri]$url).Scheme -ne 'https') { throw 'Unsafe download redirect.' }
                        $response.Dispose(); $request.Dispose(); continue
                    }
                    $response.EnsureSuccessStatusCode() | Out-Null
                    $stream = $response.Content.ReadAsStreamAsync().GetAwaiter().GetResult()
                    $file = [IO.File]::Create($zipPath + '.partial')
                    try { $stream.CopyTo($file) } finally { $file.Dispose();$stream.Dispose();$response.Dispose();$request.Dispose() }
                    Move-Item -LiteralPath ($zipPath+'.partial') -Destination $zipPath -Force
                    break
                }
            } finally { $http.Dispose();$handler.Dispose() }
        }
        if ((Get-RuntimeSha256 $zipPath) -ne $asset.sha256) { throw "Checksum mismatch: $($asset.name)" }
        Write-Host "Extracting $($asset.name)..."
        $zip = [IO.Compression.ZipFile]::OpenRead($zipPath)
        try {
            foreach ($entry in $zip.Entries) {
                $relative = $entry.FullName.Replace('/','\')
                $allowed = $relative -match '^(client|python|pylibs|qemu|server|legacy-objects|node)\\' -or $relative -in @('debian-base.qcow2','libmysqlclient15.deb')
                $dest = [IO.Path]::GetFullPath((Join-Path $runtimeRoot $relative))
                if (!$allowed -or !$dest.StartsWith($runtimeRoot+'\',[StringComparison]::OrdinalIgnoreCase)) { throw "Unsafe archive entry: $relative" }
                if (!$entry.Name) { continue }
                [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($dest)) | Out-Null
                [IO.Compression.ZipFileExtensions]::ExtractToFile($entry,$dest,$true)
            }
        } finally { $zip.Dispose() }
    }
    [IO.File]::WriteAllText($marker,$manifest.tag)
    $token = $null; $credentialLines = $null; $headers = $null
}
if ($DownloadOnly) { Write-Host 'Runtime downloaded and verified.'; return }
& (Join-Path $runtimeRoot 'python\python.exe') -X utf8 (Join-Path $PSScriptRoot 'setup.py')
if ($LASTEXITCODE -ne 0) { throw 'Setup did not complete. Rerun Initialize.cmd; existing character data will be kept.' }
Write-Host 'Ready. Run Play.cmd. Use Update-Workspace.cmd after publishing changes from another PC.'
