[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('csim', 'synth', 'cosim', 'package', 'all')]
    [string]$Action,
    [Parameter(Mandatory = $true)]
    [string]$Config,
    [string]$VitisRoot = 'C:\AMDDesignTools\2026.1\Vitis',
    [switch]$DryRun,
    [switch]$OpenReport
)

$ErrorActionPreference = 'Stop'
try {
    $repoRoot = Split-Path $PSScriptRoot -Parent
    if (-not [IO.Path]::IsPathRooted($Config)) {
        $Config = Join-Path $repoRoot $Config
    }
    $configPath = (Resolve-Path -LiteralPath $Config).Path
    $componentDir = Split-Path $configPath -Parent
    # Run beside the config so its relative source/test-data paths retain their meaning.
    # A separate work directory avoids the IDE's existing output directories.
    $outputDir = Join-Path $componentDir 'build\vscode-hls'
    $compiler = Join-Path $VitisRoot 'bin\v++.bat'
    $runner = Join-Path $VitisRoot 'bin\vitis-run.bat'
    foreach ($tool in @($compiler, $runner)) {
        if (-not (Test-Path -LiteralPath $tool -PathType Leaf)) {
            throw "Tool not found: $tool. Set -VitisRoot to your Vitis installation."
        }
    }
    $stages = switch ($Action) {
        'csim' { @('csim') }
        'synth' { @('synth') }
        'cosim' { @('synth', 'cosim') }
        'package' { @('synth', 'package') }
        'all' { @('csim', 'synth', 'cosim', 'package') }
    }
    Write-Host "Config: $configPath"
    Write-Host "Outputs: $outputDir"
    Push-Location -LiteralPath $componentDir
    try {
        foreach ($stage in $stages) {
            $executable = $runner
            $toolArgs = @('--mode', 'hls', "--$stage")
            if ($stage -eq 'synth') {
                $executable = $compiler
                $toolArgs = @('--compile', '--mode', 'hls')
            }
            $toolArgs += @('--config', $configPath, '--work_dir', $outputDir)
            Write-Host "`nHLS stage: $stage"
            Write-Host ($executable + ' ' + ($toolArgs -join ' '))
            if (-not $DryRun) {
                # AMD's launchers initialize their own tool environment.
                & $executable @toolArgs
                if ($LASTEXITCODE -ne 0) {
                    throw "HLS $stage failed (exit code $LASTEXITCODE)."
                }
                if ($stage -eq 'synth' -and $OpenReport) {
                    $reportPath = Join-Path $outputDir 'hls\syn\report\csynth.rpt'
                    # Viewing a report must not turn a successful build into a failure.
                    try {
                        if (-not (Test-Path -LiteralPath $reportPath -PathType Leaf)) {
                            throw "Synthesis summary not found: $reportPath"
                        }
                        $codeCommand = Get-Command code.cmd -ErrorAction Stop
                        & $codeCommand.Source --reuse-window $reportPath
                        if ($LASTEXITCODE -ne 0) {
                            throw "VS Code returned exit code $LASTEXITCODE."
                        }
                    }
                    catch {
                        Write-Warning "Could not open synthesis summary: $_"
                        Write-Host "Report: $reportPath"
                    }
                }
            }
        }
    }
    finally { Pop-Location }
}
catch {
    Write-Error $_ -ErrorAction Continue
    exit 1
}
