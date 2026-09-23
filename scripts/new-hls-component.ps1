[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Name,
    [string]$Part = 'xc7z007sclg400-1',
    [string]$ClockNs = '10',
    [string]$ProjectRoot = (Split-Path $PSScriptRoot -Parent)
)

$ErrorActionPreference = 'Stop'
try {
    # Use the name as both a folder name and a C++ top-function identifier.
    if ($Name -cnotmatch '^[a-zA-Z][a-zA-Z0-9_]*$' -or $Name.Contains('__')) {
        throw 'Use a name starting with a letter, followed by letters, digits, or single underscores.'
    }
    $reserved = ('alignas alignof and and_eq asm atomic_cancel atomic_commit atomic_noexcept auto bitand bitor bool break case catch char char8_t char16_t char32_t class compl concept const consteval constexpr constinit const_cast continue co_await co_return co_yield decltype default delete do double dynamic_cast else enum explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private protected public reflexpr register reinterpret_cast requires return short signed sizeof static static_assert static_cast struct switch synchronized template this thread_local throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq main' -split ' ')
    if ($Name -cin $reserved -or $Name -match '^(CON|PRN|AUX|NUL|COM[0-9]|LPT[0-9])$') {
        throw "Reserved component name: $Name"
    }
    if ($Part -notmatch '^[a-zA-Z0-9][a-zA-Z0-9_-]*$') {
        throw 'Enter an FPGA part identifier, for example xc7z007sclg400-1.'
    }
    if ($ClockNs -notmatch '^\d+(\.\d+)?$' -or [double]::Parse($ClockNs, [Globalization.CultureInfo]::InvariantCulture) -le 0) {
        throw 'Clock period must be a positive number of nanoseconds, for example 10 or 6.67.'
    }
    $root = (Resolve-Path -LiteralPath $ProjectRoot).Path
    $componentDir = Join-Path $root "hls\$Name"
    if (Test-Path -LiteralPath $componentDir) {
        throw "Component directory already exists; nothing overwritten: $componentDir"
    }
    $tasksPath = Join-Path $root '.vscode\tasks.json'
    $tasksText = [IO.File]::ReadAllText($tasksPath)
    $tasks = $tasksText | ConvertFrom-Json
    $selector = @($tasks.inputs | Where-Object { $_.id -eq 'hlsConfig' })
    if ($selector.Count -ne 1 -or $selector[0].type -ne 'pickString') {
        throw 'Expected one hlsConfig pickString input in .vscode/tasks.json.'
    }
    $configRelative = "hls/$Name/hls_config.cfg"
    foreach ($option in $selector[0].options) {
        if ($option -eq $configRelative -or $option.value -eq $configRelative) {
            throw "Component is already registered: $configRelative"
        }
    }
    $selector[0].options = @($selector[0].options) + @([pscustomobject]@{label = $Name; value = $configRelative})
    $updatedTasks = $tasks | ConvertTo-Json -Depth 30
    $utf8 = New-Object System.Text.UTF8Encoding($false)
    $header = @"
#pragma once
#include <ap_int.h>

// Starter design: 32-bit unsigned addition (wraps on overflow).
void $Name(ap_uint<32> a, ap_uint<32> b, ap_uint<32>& result);
"@
    $source = @"
#include "$Name.h"

void $Name(ap_uint<32> a, ap_uint<32> b, ap_uint<32>& result) {
    result = a + b;
}
"@
    $testbench = @"
#include "$Name.h"
#include <iostream>

int main() {
    const unsigned inputs[][2] = {{0u, 0u}, {3u, 4u}, {0xffffffffu, 1u}};
    const unsigned expected[] = {0u, 7u, 0u};
    for (unsigned i = 0; i < 3; ++i) {
        ap_uint<32> result;
        $Name(inputs[i][0], inputs[i][1], result);
        if (result != expected[i]) {
            std::cerr << "FAIL: case " << i << ", got " << result << std::endl;
            return 1;
        }
    }
    std::cout << "PASS" << std::endl;
    return 0;
}
"@
    $config = @"
part=$Part

[hls]
syn.top=$Name
syn.file=$Name.cpp
syn.file=$Name.h
tb.file=${Name}_test.cpp
clock=${ClockNs}ns
package.output.format=ip_catalog
package.output.syn=false
"@
    # Check for concurrent edits before creating files or registering the component.
    if ([IO.File]::ReadAllText($tasksPath) -cne $tasksText) {
        throw 'tasks.json changed during creation; retry the task.'
    }
    $null = New-Item -ItemType Directory -Path $componentDir
    [IO.File]::WriteAllText((Join-Path $componentDir "$Name.h"), $header + "`n", $utf8)
    [IO.File]::WriteAllText((Join-Path $componentDir "$Name.cpp"), $source + "`n", $utf8)
    [IO.File]::WriteAllText((Join-Path $componentDir "${Name}_test.cpp"), $testbench + "`n", $utf8)
    [IO.File]::WriteAllText((Join-Path $componentDir 'hls_config.cfg'), $config + "`n", $utf8)
    [IO.File]::WriteAllText($tasksPath, $updatedTasks + "`n", $utf8)
    Write-Host "Created: $componentDir"
    Write-Host 'Added to HLS: run selected stage. Press Ctrl+Shift+B and choose the new component.'
    Write-Host 'The starter implements 32-bit addition. Edit its ports, logic, and testbench for your design.'
    Write-Host 'The device identifier will be checked by Vitis when you build.'
}
catch {
    Write-Error $_ -ErrorAction Continue
    exit 1
}
