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

int main()
{
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
    WriteFile(versionPath, "old-version");
    assert(CheckJustUpdated());
    g_bOnlineMode = false;
    ShowUpdateContent();
    assert(!CheckJustUpdated());
    assert(GetLastVersion() == TOOLBOX_VERSION);

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
