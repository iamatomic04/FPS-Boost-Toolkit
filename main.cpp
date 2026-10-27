// FPS Boost Toolkit - Windows console tool: high-performance power plans + temp cleanup.
// Build: see README.md (needs C++17).

#define _CRT_SECURE_NO_WARNINGS  // MSVC: allow std::getenv
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

#ifdef _WIN32
#define PIPE_OPEN  _popen
#define PIPE_CLOSE _pclose
#else
#define PIPE_OPEN  popen
#define PIPE_CLOSE pclose
#endif

// Full path so cmd.exe never picks up a fake powercfg.exe from the current folder.
const std::string POWERCFG = "%SystemRoot%\\System32\\powercfg.exe";

// Built-in power plan GUIDs.
const std::string GUID_BALANCED = "381b4222-f694-41f0-9685-ff5bb260df2e";
const std::string GUID_HIGH = "8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c";
const std::string GUID_ULTIMATE = "e9a42b02-d5df-448d-aa00-03f14749eb61";

// Names of the copies this tool creates when a built-in plan is missing.
// We pick the name ourselves (Windows' own plan names are localized), so we can find the copy later.
const std::string NAME_HIGH = "FPS Boost - High Performance";
const std::string NAME_ULTIMATE = "FPS Boost - Ultimate Performance";

// ---------- helpers ----------

// Runs a command silently; true if it exited with code 0.
bool runQuiet(const std::string& cmd) {
    return system((cmd + " >nul 2>&1").c_str()) == 0;
}

// Runs a command and returns everything it printed.
std::string capture(const std::string& cmd) {
    std::string out;
    FILE* pipe = PIPE_OPEN(cmd.c_str(), "r");
    if (!pipe) return out;
    char buf[256];
    while (fgets(buf, sizeof buf, pipe)) out += buf;
    PIPE_CLOSE(pipe);
    return out;
}

// First GUID in the text, or "" (powercfg's wording is localized, the GUID format is not).
std::string findGuid(const std::string& text) {
    static const std::regex re(
        "[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    std::smatch m;
    return std::regex_search(text, m, re) ? m.str() : std::string();
}

// GUID of the plan with exactly this name, or "".
std::string findPlanByName(const std::string& name) {
    std::istringstream lines(capture(POWERCFG + " /list"));
    std::string line;
    while (std::getline(lines, line))
        if (line.find("(" + name + ")") != std::string::npos) return findGuid(line);
    return "";
}

// ---------- power plans ----------

// Activates a built-in plan. If this PC doesn't have it, creates (or reuses) our own copy of it.
// duplicatescheme always gives the copy a NEW guid, so we read it from the command's output.
bool activatePlan(const std::string& builtinGuid, const std::string& ourName) {
    if (runQuiet(POWERCFG + " /setactive " + builtinGuid)) return true;

    std::string guid = findPlanByName(ourName);
    if (guid.empty()) {
        guid = findGuid(capture(POWERCFG + " /duplicatescheme " + builtinGuid));
        if (guid.empty()) return false;
        runQuiet(POWERCFG + " /changename " + guid + " \"" + ourName + "\"");
    }
    return runQuiet(POWERCFG + " /setactive " + guid);
}

void setPlan(const char* label, const std::string& builtinGuid, const std::string& ourName) {
    std::cout << "[+] Switching to " << label << "..." << std::endl;
    if (activatePlan(builtinGuid, ourName))
        std::cout << "[OK] Power plan set to " << label << "." << std::endl;
    else
        std::cout << "[-] Failed. This PC may not support the " << label
        << " plan (try running as Administrator)." << std::endl;
}

void restoreBalanced() {
    std::cout << "[+] Restoring Balanced..." << std::endl;
    if (runQuiet(POWERCFG + " /setactive " + GUID_BALANCED))
        std::cout << "[OK] Power plan set to Balanced." << std::endl;
    else
        std::cout << "[-] Failed to switch to Balanced." << std::endl;
}

void showPlan() {
    if (system((POWERCFG + " /getactivescheme").c_str()) != 0)
        std::cout << "[-] Could not read the current power plan." << std::endl;
}

// ---------- temp cleanup ----------

// Deletes everything inside dir. Items that are in use or protected are skipped, never forced.
void cleanFolder(const char* label, const fs::path& dir) {
    std::cout << "[+] Cleaning " << label << "..." << std::endl;
    std::error_code ec;
    // has_relative_path() is false for a drive root such as C:\ - never clean that.
    if (!dir.has_relative_path() || !fs::is_directory(dir, ec)) {
        std::cout << "[-] Not a valid folder; nothing done." << std::endl;
        return;
    }
    int removed = 0, skipped = 0;
    for (fs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code rec;
        fs::remove_all(it->path(), rec);
        if (rec) ++skipped; else ++removed;
    }
    std::cout << "[OK] Removed " << removed << " item(s), skipped " << skipped
        << " (in use or no permission)." << std::endl;
}

void cleanUserTemp() {
    // GetTempPath (used by temp_directory_path) falls back to the user profile folder when
    // TMP and TEMP are both unset - refuse in that case instead of cleaning the profile.
    if (!std::getenv("TMP") && !std::getenv("TEMP")) {
        std::cout << "[-] TEMP/TMP is not set; refusing to clean." << std::endl;
        return;
    }
    std::error_code ec;
    fs::path dir = fs::temp_directory_path(ec);  // Unicode-safe, unlike getenv on non-ASCII user names
    if (ec) {
        std::cout << "[-] Could not locate the Temp folder." << std::endl;
        return;
    }
    cleanFolder("user Temp folder", dir);
}

void cleanWindowsTemp() {
    const char* root = std::getenv("SystemRoot");
    if (!root || !*root) {
        std::cout << "[-] SystemRoot is not set; refusing to clean." << std::endl;
        return;
    }
    std::cout << "    (Most items here need Administrator rights; the rest are skipped.)" << std::endl;
    cleanFolder("Windows Temp folder", fs::path(root) / "Temp");
}

// ---------- menu ----------

void displayMenu() {
    std::cout << "========================================" << std::endl;
    std::cout << "           FPS BOOST TOOLKIT            " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << " 1. Show current power plan" << std::endl;
    std::cout << " 2. High Performance power plan" << std::endl;
    std::cout << " 3. Ultimate Performance power plan" << std::endl;
    std::cout << " 4. Restore Balanced power plan" << std::endl;
    std::cout << " 5. Clean user Temp folder" << std::endl;
    std::cout << " 6. Clean Windows Temp folder (needs admin)" << std::endl;
    std::cout << " 0. Exit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Select an option: ";
}

// Returns the number typed, -1 for non-numeric input, 0 (exit) if stdin is closed.
int readChoice() {
    int choice;
    if (!(std::cin >> choice)) {
        if (std::cin.eof()) return 0;
        std::cin.clear();
        choice = -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

int main() {
    for (;;) {
        displayMenu();
        switch (readChoice()) {
        case 1: showPlan(); break;
        case 2: setPlan("High Performance", GUID_HIGH, NAME_HIGH); break;
        case 3: setPlan("Ultimate Performance", GUID_ULTIMATE, NAME_ULTIMATE); break;
        case 4: restoreBalanced(); break;
        case 5: cleanUserTemp(); break;
        case 6: cleanWindowsTemp(); break;
        case 0:
            std::cout << "Exiting FPS Boost Toolkit. Goodbye!" << std::endl;
            return 0;
        default:
            std::cout << "[-] Invalid option!" << std::endl;
            break;
        }
        std::cout << std::endl;
    }
}