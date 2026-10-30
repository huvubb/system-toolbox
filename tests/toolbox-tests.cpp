#ifdef NDEBUG
#undef NDEBUG
#endif
#include <cassert>
#include <conio.h>

// Exercise menu logic without waiting for a real keyboard or running maintenance.
int TestGetch() { return 0; }
#define _getch TestGetch
#define main ToolboxMain
#include "../system-toolbox.cpp"
#undef main
#undef _getch

std::string ReadFile(const std::filesystem::path &path)
{
    std::ifstream in(path, std::ios::binary);
    assert(in.is_open());
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void WriteFile(const std::filesystem::path &path, const std::string &content)
{
    std::ofstream out(path, std::ios::binary);
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    out.flush();
    assert(out.good());
}

std::vector<BYTE> EncryptNcmKeyForTest(std::vector<BYTE> plain)
{
    const BYTE coreKey[] = "hzHRAmso5kInbaxW";
    struct KeyBlob
    {
        BLOBHEADER header;
        DWORD length;
        BYTE key[sizeof(coreKey) - 1];
    } keyBlob = {{PLAINTEXTKEYBLOB, CUR_BLOB_VERSION, 0, CALG_AES_128}, sizeof(coreKey) - 1, {}};
    memcpy(keyBlob.key, coreKey, sizeof(keyBlob.key));
    HCRYPTPROV provider = 0;
    HCRYPTKEY key = 0;
    assert(CryptAcquireContextW(&provider, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT));
    assert(CryptImportKey(provider, reinterpret_cast<const BYTE *>(&keyBlob), sizeof(keyBlob), 0, 0, &key));
    DWORD mode = CRYPT_MODE_ECB;
    assert(CryptSetKeyParam(key, KP_MODE, reinterpret_cast<BYTE *>(&mode), 0));
    DWORD length = static_cast<DWORD>(plain.size());
    plain.resize(plain.size() + 16);
    assert(CryptEncrypt(key, 0, TRUE, 0, plain.data(), &length, static_cast<DWORD>(plain.size())));
    plain.resize(length);
    CryptDestroyKey(key);
    CryptReleaseContext(provider, 0);
    return plain;
}

void WriteNcmFixture(const std::filesystem::path &path, const std::string &audio)
{
    std::vector<BYTE> key{'n', 'e', 't', 'e', 'a', 's', 'e', 'c', 'l', 'o', 'u', 'd', 'm', 'u', 's', 'i', 'c',
                          'f', 'i', 'x', 't', 'u', 'r', 'e'};
    std::vector<BYTE> encryptedKey = EncryptNcmKeyForTest(key);
    for (BYTE &byte : encryptedKey)
        byte ^= 0x64;
    std::vector<BYTE> box(256);
    for (size_t i = 0; i < box.size(); ++i)
        box[i] = static_cast<BYTE>(i);
    BYTE j = 0;
    for (size_t i = 0; i < box.size(); ++i)
    {
        j = static_cast<BYTE>(j + box[i] + key[17 + i % (key.size() - 17)]);
        std::swap(box[i], box[j]);
    }
    std::vector<BYTE> encryptedAudio(audio.begin(), audio.end());
    for (size_t i = 0; i < encryptedAudio.size(); ++i)
    {
        const BYTE index = static_cast<BYTE>(i + 1);
        encryptedAudio[i] ^= box[static_cast<BYTE>(box[index] + box[static_cast<BYTE>(box[index] + index)])];
    }
    std::ofstream out(path, std::ios::binary);
    const char magic[] = "CTENFDAM";
    out.write(magic, 8);
    out.write("\0\0", 2);
    auto writeUint32 = [&out](uint32_t value) { out.write(reinterpret_cast<const char *>(&value), sizeof(value)); };
    writeUint32(static_cast<uint32_t>(encryptedKey.size()));
    out.write(reinterpret_cast<const char *>(encryptedKey.data()), static_cast<std::streamsize>(encryptedKey.size()));
    writeUint32(0);
    out.write("\0\0\0\0\0", 5);
    writeUint32(8);
    writeUint32(5);
    out.write("cover\0\0\0", 8);
    out.write(reinterpret_cast<const char *>(encryptedAudio.data()), static_cast<std::streamsize>(encryptedAudio.size()));
    assert(out.good());
}

int main()
{
    DWORD downloadSize = GetEnvironmentVariableW(L"TOOLBOX_TEST_DOWNLOAD_URL", nullptr, 0);
    if (downloadSize)
    {
        std::vector<wchar_t> url(downloadSize), output(32768);
        GetEnvironmentVariableW(L"TOOLBOX_TEST_DOWNLOAD_URL", url.data(), downloadSize);
        DWORD size = GetEnvironmentVariableW(L"TOOLBOX_TEST_DOWNLOAD_OUTPUT", output.data(), static_cast<DWORD>(output.size()));
        assert(size && size < output.size());
        return DownloadWithFastdl(PathToUtf8(std::filesystem::path(url.data())),
                                  PathToUtf8(std::filesystem::path(output.data()))) ? 0 : 1;
    }
    DWORD musicInputSize = GetEnvironmentVariableW(L"TOOLBOX_TEST_MUSIC_INPUT", nullptr, 0);
    if (musicInputSize)
    {
        std::vector<wchar_t> musicInput(musicInputSize), musicOutput(32768);
        GetEnvironmentVariableW(L"TOOLBOX_TEST_MUSIC_INPUT", musicInput.data(), musicInputSize);
        DWORD outputSize = GetEnvironmentVariableW(L"TOOLBOX_TEST_MUSIC_OUTPUT", musicOutput.data(),
                                                   static_cast<DWORD>(musicOutput.size()));
        assert(outputSize && outputSize < musicOutput.size());
        return RunMusicConverter(PathToUtf8(std::filesystem::path(musicInput.data())),
                                 PathToUtf8(std::filesystem::path(musicOutput.data())), "", "") ? 0 : 1;
    }
    std::wstring path = L"C:\\Tools;C:\\Node";
    assert(PathToUtf8(std::filesystem::u8path("中文 O'Brien & path")) == "中文 O'Brien & path");
    assert(RunRepairOperation({}) == -1);
    assert(RunRepairOperation({L"powershell.exe", L"-NoProfile", L"-NonInteractive", L"-Command", L"exit 0"}) == 0);
    assert(RunRepairOperation({L"powershell.exe", L"-NoProfile", L"-NonInteractive", L"-Command", L"exit 7"}) == 7);
    assert(!FindExecutableOnPath(L"cmd.exe").empty());
    assert(FindExecutableOnPath(L"toolbox-this-file-does-not-exist-9a4e.exe").empty());
    {
        std::vector<wchar_t> signedFile(32768);
        DWORD length = GetEnvironmentVariableW(L"TOOLBOX_TEST_PSEXEC_FILE", signedFile.data(), static_cast<DWORD>(signedFile.size()));
        if (length && length < signedFile.size()) assert(IsTrustedMicrosoftExecutable(std::filesystem::path(signedFile.data())));
    }
    assert(DescribeCommandResult("cmd /d /c exit 0", 0).find("成功") != std::string::npos);
    for (const std::string command : {"winget install --id Example.App", "msiexec /i package.msi", "dism /online /restorehealth"})
        for (const int code : {3010, 1641})
            assert(DescribeCommandResult(command, code).find("重启") != std::string::npos);
    for (const int code : {7, 3010, 1641})
    {
        const auto message = DescribeCommandResult("cmd /d /c exit 7", code);
        assert(message.find("失败") != std::string::npos);
        assert(message.find(std::to_string(code)) != std::string::npos);
    }
    assert(DescribeTaskState("waiting", false).find("等待") != std::string::npos);
    assert(DescribeTaskState("running", false).find("运行") != std::string::npos);
    assert(DescribeTaskState("done", false).find("完成") != std::string::npos);
    assert(DescribeTaskState("failed", false).find("失败") != std::string::npos);
    assert(DescribeTaskState("waiting", true).find("暂停") != std::string::npos);
    assert(DescribeTaskState("running", true).find("暂停") != std::string::npos);
    {
        auto& queue = GetTaskQueue();
        const bool savedEnabled = queue.enabled;
        queue.enabled = false;
        std::ostringstream feedback;
        auto *savedOutput = std::cout.rdbuf(feedback.rdbuf());
        const bool succeeded = RunCmd("cmd /d /c exit 0", false);
        const bool failed = RunCmd("cmd /d /c exit 7", false);
        std::cout.rdbuf(savedOutput);
        queue.enabled = savedEnabled;
        assert(succeeded);
        assert(!failed);
        assert(feedback.str().find("成功") != std::string::npos);
        assert(feedback.str().find("失败") != std::string::npos);
        assert(feedback.str().find("返回码:") == std::string::npos);
    }
    {
        auto& queue = GetTaskQueue();
        const bool savedEnabled = queue.enabled;
        size_t before;
        { std::lock_guard<std::mutex> lock(queue.mutex); before = queue.tasks.size(); }
        queue.enabled = true;
        std::ostringstream feedback;
        auto *savedOutput = std::cout.rdbuf(feedback.rdbuf());
        bool succeeded = RunCmd("cmd /d /c exit 0", false, true);
        bool failed = RunCmd("cmd /d /c exit 7", false, true);
        std::cout.rdbuf(savedOutput);
        queue.enabled = savedEnabled;
        assert(succeeded && !failed);
        assert(feedback.str().find("失败") != std::string::npos);
        { std::lock_guard<std::mutex> lock(queue.mutex); assert(queue.tasks.size() == before); }
    }
    std::vector<wchar_t> selected;
    assert(ParseDriveList(" d, E:,d ", selected) && selected == std::vector<wchar_t>({L'D', L'E'}));
    for (const std::string input : {"D&whoami", "D,E,", "D,,E", "1:", "D:folder", ""})
        assert(!ParseDriveList(input, selected));
    const std::vector<std::wstring> bootEntries{L"third-party startup task", L"autocheck autochk /k:CE *",
                                               L"autocheck autochk /p \\??\\D:", L"autocheck autochk /p \\??\\E:"};
    std::vector<std::wstring> changed, restoredBoot;
    assert(TransformBootExecute(bootEntries, {L'D'}, true, changed));
    assert(changed == std::vector<std::wstring>({L"third-party startup task", L"autocheck autochk /k:CED *",
                                               L"autocheck autochk /p \\??\\E:"}));
    assert(TransformBootExecute(changed, {L'D'}, false, restoredBoot));
    assert(restoredBoot == std::vector<std::wstring>({L"third-party startup task", L"autocheck autochk /k:CE *",
                                                    L"autocheck autochk /p \\??\\E:"}));
    assert(!TransformBootExecute({L"third-party startup task"}, {L'D'}, true, changed));
    assert(!TransformBootExecute(bootEntries, {L'1'}, true, changed));
    assert(!TransformBootExecute({}, {L'D'}, true, changed));
    assert(TransformBootExecute({}, {L'D'}, true, changed, {}, true));
    assert(changed == std::vector<std::wstring>({L"autocheck autochk /k:D *"}));
    assert(TransformBootExecute({L"autocheck autochk *", L"autocheck autochk /p \\??\\Volume{2eca078d-5cbc-43d3-aff8-7e8511f60d0e}"},
                               {L'D'}, true, changed, {L"\\\\?\\Volume{2eca078d-5cbc-43d3-aff8-7e8511f60d0e}\\"}));
    assert(changed == std::vector<std::wstring>({L"autocheck autochk /k:D *"}));
    assert(IsVolumeGuidPath(L"\\\\?\\Volume{2eca078d-5cbc-43d3-aff8-7e8511f60d0e}\\"));
    assert(!IsVolumeGuidPath(L"C:\\"));
    assert(NormalizeMountPoint("x:") == L"X:\\");
    assert(NormalizeMountPoint("relative-folder").empty());
    const std::string manifestHash(64, 'a');
    std::istringstream validManifest(manifestHash + "  system-toolbox.exe\r\n");
    assert(ReadToolboxManifestHash(validManifest) == std::string(64, 'A'));
    for (const auto& manifest : std::vector<std::string>{manifestHash + "  fastdl.exe\n", "bad  system-toolbox.exe\n",
                                manifestHash + " system-toolbox.exe\n" + manifestHash + " system-toolbox.exe\n"})
    {
        std::istringstream inputManifest(manifest);
        assert(ReadToolboxManifestHash(inputManifest).empty());
    }
    {
        TaskQueue queue;
        { std::lock_guard<std::mutex> lock(queue.mutex); queue.parallel = 0; }
        int first = queue.Add(L"cmd.exe /d /c exit 0");
        assert(queue.Control(first, 1));
        assert(queue.Control(first, 3, 2));
        assert(!queue.Control(first, 3, 3));
        assert(queue.Control(first, 2));
        { std::lock_guard<std::mutex> lock(queue.mutex); queue.parallel = 1; }
        bool completed = false;
        for (int attempt = 0; attempt < 100; ++attempt)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::lock_guard<std::mutex> lock(queue.mutex);
            if (queue.tasks[0]->state == "done") { completed = true; break; }
            assert(queue.tasks[0]->state != "failed");
        }
        assert(completed);
        int slow = queue.Add(L"cmd.exe /d /c ping -n 4 127.0.0.1 >nul");
        bool started = false;
        for (int attempt = 0; attempt < 100; ++attempt)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::lock_guard<std::mutex> lock(queue.mutex);
            if (queue.tasks[slow-1]->state == "running") { started = true; break; }
        }
        assert(started);
        assert(queue.Control(slow, 1));
        assert(queue.Control(slow, 3, 0));
        int other = queue.Add(L"cmd.exe /d /c exit 0");
        bool otherCompleted = false;
        for (int attempt = 0; attempt < 100; ++attempt)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::lock_guard<std::mutex> lock(queue.mutex);
            if (queue.tasks[other-1]->state == "done") { otherCompleted = true; break; }
        }
        assert(otherCompleted);
        assert(queue.Control(slow, 2));
    }
    AddPathEntries(path, L"c:\\tools;C:\\Python;;C:\\Node");
    assert(path == L"C:\\Tools;C:\\Node;C:\\Python");
    assert(QuoteProcessArgument(L"C:\\音乐 文件\\") == L"\"C:\\音乐 文件\\\\\"");
    assert(QuoteProcessArgument(L"a\"b") == L"\"a\\\"b\"");

    std::istringstream input("\n42\n -7 \n12abc\n99999999999999999999999\nabc\n\n");
    auto *oldInput = std::cin.rdbuf(input.rdbuf());
    assert(GetIntInput("", 23) == 23);
    assert(GetIntInput("") == 42);
    assert(GetIntInput("") == -7);
    assert(GetIntInput("") == -1);
    assert(GetIntInput("") == -1);
    assert(GetIntInput("") == -1);
    assert(GetIntInput("") == -1);
    std::cin.rdbuf(oldInput);
    std::cin.clear();

    auto root = std::filesystem::current_path() / ("testdata-" + std::to_string(GetCurrentProcessId()));
    assert(std::filesystem::create_directory(root));
    auto invalidSignature = root / L"not-signed.exe";
    WriteFile(invalidSignature, "not a signed executable");
    assert(!IsTrustedMicrosoftExecutable(invalidSignature));
    auto diagnosticsScript = root / L"diagnostics.ps1";
    WriteFile(diagnosticsScript, std::string("\xEF\xBB\xBF") + GetDiagnosticsReportScript());
    assert(RunPowerShellFile(R"PS(param([string]$Source)
$ErrorActionPreference='Stop'
$tokens=$null; $errors=$null
$null=[Management.Automation.Language.Parser]::ParseFile($Source,[ref]$tokens,[ref]$errors)
if ($errors.Count) { throw ($errors | Out-String) }
)PS", {diagnosticsScript.wstring()}));
    auto rejectedReport = root / L"invalid-diagnostic";
    assert(!RunPowerShellFile(GetDiagnosticsReportScript(), {L"Unknown", rejectedReport.wstring()}));
    assert(!std::filesystem::exists(rejectedReport));
    auto applicationsReport = root / std::filesystem::u8path("中文 O'Brien & reports");
    assert(RunPowerShellFile(GetDiagnosticsReportScript(), {L"Apps", applicationsReport.wstring()}));
    assert(std::filesystem::is_directory(applicationsReport));
    assert(RunPowerShellFile(R"PS(param([string]$Root)
$ErrorActionPreference='Stop'
$reports=@(Get-ChildItem -LiteralPath $Root -Recurse -File -Filter *.csv)
if ($reports.Count -ne 1) { throw 'Expected one installed-applications CSV report' }
$rows=@(Import-Csv -LiteralPath $reports[0].FullName)
if (-not $rows.Count) { throw 'Installed-applications report unexpectedly empty' }
$columns=@($rows[0].PSObject.Properties.Name)
if ('DisplayName' -notin $columns -or 'DisplayVersion' -notin $columns) {
    throw 'Missing installed-application names or versions'
}
)PS", {applicationsReport.wstring()}));
    auto sleepReport = root / L"sleep-report";
    assert(RunPowerShellFile(GetDiagnosticsReportScript(), {L"Sleep", sleepReport.wstring()}));
    assert(RunPowerShellFile(R"PS(param([string]$Root)
$ErrorActionPreference='Stop'
$reports=@(Get-ChildItem -LiteralPath $Root -Recurse -File -Filter sleep.txt)
if ($reports.Count -ne 1) { throw 'Expected one sleep diagnostics report' }
$text=Get-Content -LiteralPath $reports[0].FullName -Raw
foreach ($argument in @('/a','/lastwake','/waketimers','/requests')) {
    if (-not $text.Contains("=== powercfg $argument ===")) {
        throw ('Missing power configuration query: '+$argument)
    }
}
if ([regex]::Matches($text,'(?m)^Exit code: \d+\r?$').Count -ne 4) {
    throw 'Expected an exit status for every power configuration query'
}
)PS", {sleepReport.wstring()}));
    auto source = root / std::filesystem::u8path("中文 O'Brien & input.bin");
    auto mediaScript = root / L"iso-boot-media.ps1";
    WriteFile(mediaScript, std::string("\xEF\xBB\xBF") + GetIsoBootMediaScript());
    assert(RunPowerShellFile(R"PS(param([string]$Source)
$ErrorActionPreference='Stop'
$tokens=$null; $errors=$null
$null=[Management.Automation.Language.Parser]::ParseFile($Source,[ref]$tokens,[ref]$errors)
if ($errors.Count) { throw ($errors | Out-String) }
. $Source -FunctionsOnly
$disk=[pscustomobject]@{Number=7;UniqueId='test-disk';Size=16GB;IsBoot=$false;IsSystem=$false;
 IsOffline=$false;IsReadOnly=$false;IsClustered=$false;BusType='USB';HealthStatus='Healthy'}
Assert-MediaDisk $disk @(0)
function Must-Reject($Candidate,$Protected) {
    $rejected=$false
    try { Assert-MediaDisk $Candidate $Protected } catch { $rejected=$true }
    if (-not $rejected) { throw 'Unsafe target accepted' }
}
Must-Reject $null @()
Must-Reject $disk @(7)
foreach($flag in @('IsBoot','IsSystem','IsOffline','IsReadOnly','IsClustered')) {
    $disk.$flag=$true; Must-Reject $disk @(); $disk.$flag=$false
}
foreach($bus in @('Spaces','Virtual','File Backed Virtual')) {
    $disk.BusType=$bus; Must-Reject $disk @()
}
$disk.BusType='USB'
$disk.UniqueId=''; Must-Reject $disk @(); $disk.UniqueId='test-disk'
$disk.Size=1GB; Must-Reject $disk @(); $disk.Size=16GB
$disk.Size=3TB; Must-Reject $disk @(); $disk.Size=16GB
$disk.HealthStatus='Warning'; Must-Reject $disk @()
Write-Host 'ISO media script parses; protected and ambiguous disks rejected without disk writes.'
)PS", {mediaScript.wstring()}));
    WriteFile(source, "abc");
    auto untrustedDlc = root / "untrusted.exe";
    auto installedDlc = root / "disktool.exe";
    WriteFile(untrustedDlc, "not a disktool executable");
    WriteFile(installedDlc, "preserved DLC");
    assert(!InstallDisktoolFile(untrustedDlc, installedDlc));
    assert(ReadFile(installedDlc) == "preserved DLC");
    assert(GetFileSHA256(PathToUtf8(source)) == "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD");
    WriteFile(source, "");
    assert(GetFileSHA256(PathToUtf8(source)) == "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855");
    WriteFile(source, std::string(1000000, 'a'));
    assert(GetFileSHA256(PathToUtf8(source)) == "CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0");
    assert(GetFileSHA256(PathToUtf8(root / "missing")).empty());
    assert(GetLocalExeHash().size() == 64);
    assert(GetBuildIdentity() == "SHA256 " + GetLocalExeHash().substr(0, 12));
    auto archive = root / "中文 O'Brien & release.zip";
    auto archivedExe = root / "system-toolbox.exe";
    WriteFile(archivedExe, "abc");
    assert(RunPowerShellFile("param([string]$Source,[string]$Zip)\n$ErrorActionPreference='Stop'\n"
                            "Compress-Archive -LiteralPath $Source -DestinationPath $Zip -Force\n",
                            {archivedExe.wstring(), archive.wstring()}));
    assert(GetRemoteExeHash(PathToUtf8(archive)) == GetFileSHA256(PathToUtf8(archivedExe)));
    auto installDirectory = root / "install";
    assert(std::filesystem::create_directory(installDirectory));
    auto installedExe = installDirectory / "system-toolbox.exe";
    WriteFile(installedExe, "preserved executable");
    assert(!RunPowerShellFile(GetUpdateInstallScript(), {L"2147483647", archive.wstring(),
                                                       installedExe.wstring(), std::wstring(64, L'0')}));
    assert(ReadFile(installedExe) == "preserved executable");
    assert(std::filesystem::exists(root / "update-error.txt"));

    auto ncmSource = root / "fixture.ncm";
    auto ncmOutput = root / "fixture.audio";
    WriteNcmFixture(ncmSource, "ID3fixture-audio");
    std::string ncmError;
    assert(DecodeNcmAudio(ncmSource, ncmOutput, ncmError));
    assert(ReadFile(ncmOutput) == "ID3fixture-audio");
    WriteFile(ncmSource, "not-ncm");
    assert(!DecodeNcmAudio(ncmSource, ncmOutput, ncmError));

    assert(StringToBase(std::string("\0\xFF", 2), 2, " ") == "00000000 11111111");
    assert(StringToBase("AB", 8, " ") == "101 102");
    assert(StringToBase("AB", 16, " ") == "41 42");
    assert(StringToBase("A", 32, " ") == "21");
    assert(StringToBase("A", 36, " ") == "1T");
    assert(StringToBase("A", 1, " ").empty());
    unsigned long long number = 0;
    assert(TryParseUnsigned("FFFFFFFFFFFFFFFF", 16, number));
    assert(number == (std::numeric_limits<unsigned long long>::max)());
    assert(!TryParseUnsigned("18446744073709551616", 10, number));
    assert(!TryParseUnsigned("123junk", 10, number));
    assert(!TryParseUnsigned("-1", 10, number));
    assert(!TryParseUnsigned("10", 1, number));
    assert(!TryParseUnsigned("10", 37, number));

    auto encoded = root / "encoded.txt";
    auto restored = root / "restored.bin";
    std::string bytes;
    for (int i = 0; i < 256; ++i)
        bytes += static_cast<char>(i);
    WriteFile(source, bytes);
    for (int base : {2, 8, 10, 16, 32, 36, 64})
    {
        assert(ConvertFileBase(PathToUtf8(source), PathToUtf8(encoded), base, false));
        assert(ConvertFileBase(PathToUtf8(encoded), PathToUtf8(restored), base, true));
        assert(ReadFile(restored) == bytes);
    }
    assert(!ConvertFileBase(PathToUtf8(source), PathToUtf8(source), 16, false));
    assert(ReadFile(source) == bytes);
    WriteFile(restored, "preserved");
    for (const std::string invalid : {"256", "-1", "12abc", "2进制:"})
    {
        WriteFile(encoded, invalid);
        assert(!ConvertFileBase(PathToUtf8(encoded), PathToUtf8(restored), 10, true));
        assert(ReadFile(restored) == "preserved");
    }
    WriteFile(encoded, "@@@@");
    assert(!ConvertFileBase(PathToUtf8(encoded), PathToUtf8(restored), 64, true));
    assert(ReadFile(restored) == "preserved");
    WriteFile(source, "abc");
    assert(ConvertFileBase(PathToUtf8(source), PathToUtf8(encoded), 64, false));
    assert(ReadFile(encoded) == "YWJj");
    WriteFile(source, "");
    for (int base : {2, 16, 64})
    {
        assert(ConvertFileBase(PathToUtf8(source), PathToUtf8(encoded), base, false));
        assert(ConvertFileBase(PathToUtf8(encoded), PathToUtf8(restored), base, true));
        assert(ReadFile(restored).empty());
    }

    std::istringstream shortFile(std::string("A\0\xFF", 3));
    std::ostringstream dump;
    assert(DumpHex(shortFile, dump, 256) == 3);
    assert(dump.str().find("41 00 FF") != std::string::npos);
    std::istringstream limited("abcdefghijklmnopqrst");
    dump.str("");
    assert(DumpHex(limited, dump, 17) == 17);
    assert(limited.peek() == 'r');
    assert(dump.str().find("00000010  71") != std::string::npos);
    assert(DumpHex(limited, dump, 0) == 0);

    auto versionPath = std::filesystem::u8path(GetLastVersionFile());
    bool hadVersion = std::filesystem::exists(versionPath);
    std::string savedVersion = hadVersion ? ReadFile(versionPath) : "";
    WriteFile(versionPath, std::string(64, '0'));
    assert(CheckJustUpdated());
    g_bOnlineMode = false;
    ShowUpdateContent();
    assert(!CheckJustUpdated());
    assert(GetLastVersion() == GetLocalExeHash());
    WriteFile(versionPath, "v1.9.0");
    assert(!CheckJustUpdated());
    assert(GetLastVersion() == GetLocalExeHash());

    for (const std::string choices : {"abc\n0\n", "0\n", "36\n0\n0\n", "37\n0\n0\n", "38\n0\n0\n", ""})
    {
        std::istringstream menuInput(choices);
        std::ostringstream output;
        oldInput = std::cin.rdbuf(menuInput.rdbuf());
        auto *oldOutput = std::cout.rdbuf(output.rdbuf());
        assert(ToolboxMain() == 0);
        assert(output.str().find("29. 进制转换工具") != std::string::npos);
        assert(output.str().find("36. 系统诊断与报告") != std::string::npos);
        if (choices == "36\n0\n0\n")
            assert(output.str().find("5. 软件清单") != std::string::npos);
        if (choices == "37\n0\n0\n")
            assert(output.str().find("DirectX 与 DLL 修复") != std::string::npos);
        if (choices == "38\n0\n0\n")
            assert(output.str().find("安装 PsExec64 到当前目录并设置 PATH") != std::string::npos);
        std::cout.rdbuf(oldOutput);
        std::cin.rdbuf(oldInput);
        std::cin.clear();
    }
    if (hadVersion)
        WriteFile(versionPath, savedVersion);
    else
        std::filesystem::remove(versionPath);
    std::filesystem::remove_all(root);
    std::cout << "All regression checks passed.\n";
}
