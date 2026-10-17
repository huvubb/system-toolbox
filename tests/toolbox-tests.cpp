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
        return DownloadWithFastdl(std::filesystem::path(url.data()).u8string(),
                                  std::filesystem::path(output.data()).u8string()) ? 0 : 1;
    }
    DWORD musicInputSize = GetEnvironmentVariableW(L"TOOLBOX_TEST_MUSIC_INPUT", nullptr, 0);
    if (musicInputSize)
    {
        std::vector<wchar_t> musicInput(musicInputSize), musicOutput(32768);
        GetEnvironmentVariableW(L"TOOLBOX_TEST_MUSIC_INPUT", musicInput.data(), musicInputSize);
        DWORD outputSize = GetEnvironmentVariableW(L"TOOLBOX_TEST_MUSIC_OUTPUT", musicOutput.data(),
                                                   static_cast<DWORD>(musicOutput.size()));
        assert(outputSize && outputSize < musicOutput.size());
        return RunMusicConverter(std::filesystem::path(musicInput.data()).u8string(),
                                 std::filesystem::path(musicOutput.data()).u8string(), "", "") ? 0 : 1;
    }
    std::wstring path = L"C:\\Tools;C:\\Node";
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
    auto source = root / std::filesystem::u8path("中文 O'Brien & input.bin");
    WriteFile(source, "abc");
    assert(GetFileSHA256(source.u8string()) == "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD");
    WriteFile(source, "");
    assert(GetFileSHA256(source.u8string()) == "E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855");
    WriteFile(source, std::string(1000000, 'a'));
    assert(GetFileSHA256(source.u8string()) == "CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0");
    assert(GetFileSHA256((root / "missing").u8string()).empty());
    assert(GetLocalExeHash().size() == 64);
    assert(GetBuildIdentity() == "SHA256 " + GetLocalExeHash().substr(0, 12));
    auto archive = root / "中文 O'Brien & release.zip";
    auto archivedExe = root / "system-toolbox.exe";
    WriteFile(archivedExe, "abc");
    assert(RunPowerShellFile("param([string]$Source,[string]$Zip)\n$ErrorActionPreference='Stop'\n"
                            "Compress-Archive -LiteralPath $Source -DestinationPath $Zip -Force\n",
                            {archivedExe.wstring(), archive.wstring()}));
    assert(GetRemoteExeHash(archive.u8string()) == GetFileSHA256(archivedExe.u8string()));
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
        assert(ConvertFileBase(source.u8string(), encoded.u8string(), base, false));
        assert(ConvertFileBase(encoded.u8string(), restored.u8string(), base, true));
        assert(ReadFile(restored) == bytes);
    }
    assert(!ConvertFileBase(source.u8string(), source.u8string(), 16, false));
    assert(ReadFile(source) == bytes);
    WriteFile(restored, "preserved");
    for (const std::string invalid : {"256", "-1", "12abc", "2进制:"})
    {
        WriteFile(encoded, invalid);
        assert(!ConvertFileBase(encoded.u8string(), restored.u8string(), 10, true));
        assert(ReadFile(restored) == "preserved");
    }
    WriteFile(encoded, "@@@@");
    assert(!ConvertFileBase(encoded.u8string(), restored.u8string(), 64, true));
    assert(ReadFile(restored) == "preserved");
    WriteFile(source, "abc");
    assert(ConvertFileBase(source.u8string(), encoded.u8string(), 64, false));
    assert(ReadFile(encoded) == "YWJj");
    WriteFile(source, "");
    for (int base : {2, 16, 64})
    {
        assert(ConvertFileBase(source.u8string(), encoded.u8string(), base, false));
        assert(ConvertFileBase(encoded.u8string(), restored.u8string(), base, true));
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

    for (const std::string choices : {"abc\n0\n", "0\n", ""})
    {
        std::istringstream menuInput(choices);
        std::ostringstream output;
        oldInput = std::cin.rdbuf(menuInput.rdbuf());
        auto *oldOutput = std::cout.rdbuf(output.rdbuf());
        assert(ToolboxMain() == 0);
        assert(output.str().find("29. 进制转换工具") != std::string::npos);
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
