#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "../include/lexer.hpp"
#include "../include/token.hpp"

using namespace std;

static const string TEST_DIR = "data/test_lexer";
static const string EXPECTED_DIR = "data/test_lexer/expected";
static const string LOG_PATH = "logs/test-lexer.log";
static int g_failures = 0;
static bool g_verbose = false;
static ofstream g_log;

struct CategoryExpectation {
    string label;
    string filePath;
};

static void fail(const string& message) {
    cerr << "[FAIL] " << message << endl;
    if (g_log.is_open()) {
        g_log << "[FAIL] " << message << "\n";
    }
    g_failures++;
}

static void info(const string& message) {
    cout << message << endl;
    if (g_log.is_open()) {
        g_log << message << "\n";
    }
}

static void logText(const string& text) {
    if (g_log.is_open()) {
        g_log << text;
    }
}

static vector<Token> tokenizeFile(const string& path) {
    Lexer lexer(path);
    return lexer.tokenize();
}

static string renderTokenBlock(const string& path, const vector<Token>& tokens) {
    string out = "=== TOKENS FOR " + path + " ===\n";
    for (const Token& tk : tokens) {
        out += tk.toString();
        out += "\n";
    }
    return out;
}

static string expectedPathFromInputFile(const string& filePath) {
    size_t slashPos = filePath.find_last_of('/');
    string fileName = (slashPos == string::npos) ? filePath : filePath.substr(slashPos + 1);

    size_t extPos = fileName.rfind(".md");
    if (extPos == string::npos) {
        return EXPECTED_DIR + "/" + fileName + ".expected";
    }

    return EXPECTED_DIR + "/" + fileName.substr(0, extPos) + ".expected";
}

static string readExpectedBlockFromFile(const string& expectedPath) {
    ifstream in(expectedPath);
    if (!in.is_open()) {
        fail("Cannot open expected file: " + expectedPath);
        return "";
    }

    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    return content;
}

static void printFirstDiffContext(const string& label,
                                  const string& expected,
                                  const string& actual) {
    size_t i = 0;
    while (i < expected.size() && i < actual.size() && expected[i] == actual[i]) {
        i++;
    }

    if (i == expected.size() && i == actual.size()) {
        return;
    }

    size_t start = (i > 40) ? i - 40 : 0;
    size_t expectedLen = (expected.size() - start > 120) ? 120 : expected.size() - start;
    size_t actualLen = (actual.size() - start > 120) ? 120 : actual.size() - start;

    string diffA = "[DIFF] " + label + " mismatch at byte " + to_string(i);
    string diffB = "[DIFF] Expected snippet: \n" + expected.substr(start, expectedLen);
    string diffC = "[DIFF] Actual snippet: \n" + actual.substr(start, actualLen);

    if (g_verbose) {
        cerr << diffA << endl;
        cerr << diffB << endl;
        cerr << diffC << endl;
    }

    if (g_log.is_open()) {
        g_log << diffA << "\n";
        g_log << diffB << "\n";
        g_log << diffC << "\n";
    }
}

static void validateCategory(const CategoryExpectation& expected) {
    vector<Token> tokens = tokenizeFile(expected.filePath);
    string actualBlock = renderTokenBlock(expected.filePath, tokens);
    string expectedPath = expectedPathFromInputFile(expected.filePath);
    string expectedBlock = readExpectedBlockFromFile(expectedPath);

    if (expectedBlock.empty()) {
        return;
    }

    logText("--- ACTUAL (" + expected.label + ") ---\n");
    logText(actualBlock);

    if (g_verbose) {
        cout << "--- ACTUAL (" << expected.label << ") ---\n";
        cout << actualBlock;
    }

    if (actualBlock != expectedBlock) {
        printFirstDiffContext(expected.label, expectedBlock, actualBlock);
        fail(expected.label + ": token stream differs from expected file output");
        return;
    }

    info("[OK] " + expected.label + " passed");
}

static void testHeadings() {
    CategoryExpectation expected = {
        "Headings",
        TEST_DIR + "/01_headings.md"
    };
    validateCategory(expected);
}

static void testParagraphsAndBreaks() {
    CategoryExpectation expected = {
        "Paragraphs and breaks",
        TEST_DIR + "/02_paragraphs_and_breaks.md"
    };
    validateCategory(expected);
}

static void testEmphasis() {
    CategoryExpectation expected = {
        "Emphasis",
        TEST_DIR + "/03_emphasis.md"
    };
    validateCategory(expected);
}

static void testBlockquotes() {
    CategoryExpectation expected = {
        "Blockquotes",
        TEST_DIR + "/04_blockquotes.md"
    };
    validateCategory(expected);
}

static void testLists() {
    CategoryExpectation expected = {
        "Lists",
        TEST_DIR + "/05_lists.md"
    };
    validateCategory(expected);
}

static void testCode() {
    CategoryExpectation expected = {
        "Code",
        TEST_DIR + "/06_code.md"
    };
    validateCategory(expected);
}

static void testLinksAndImages() {
    CategoryExpectation expected = {
        "Links and images",
        TEST_DIR + "/07_links_and_images.md"
    };
    validateCategory(expected);
}

static void testEscapesSymbolsAndRules() {
    CategoryExpectation expected = {
        "Escapes symbols and rules",
        TEST_DIR + "/08_escapes_symbols_and_rules.md"
    };
    validateCategory(expected);
}

static void testTables() {
    CategoryExpectation expected = {
        "Tables",
        TEST_DIR + "/09_tables.md"
    };
    validateCategory(expected);
}

static void testHtml() {
    CategoryExpectation expected = {
        "HTML",
        TEST_DIR + "/10_html.md"
    };
    validateCategory(expected);
}

static void testAutolinksFootnotesDefinition() {
    CategoryExpectation expected = {
        "Autolinks footnotes definition",
        TEST_DIR + "/11_autolinks_footnotes_definition.md"
    };
    validateCategory(expected);
}

static void testMixedStress() {
    CategoryExpectation expected = {
        "Mixed stress",
        TEST_DIR + "/12_mixed_stress.md"
    };
    validateCategory(expected);
}

int main(int argc, char* argv[]) {
    system("mkdir -p logs");
    g_log.open(LOG_PATH, ios::out | ios::trunc);
    if (!g_log.is_open()) {
        cerr << "Cannot open log file: " << LOG_PATH << endl;
        return 1;
    }

    if (argc > 2) {
        cerr << "Usage: " << argv[0] << " [-v]" << endl;
        g_log << "Usage: " << argv[0] << " [-v]\n";
        return 1;
    }

    if (argc == 2) {
        string flag = argv[1];
        if (flag == "-v") {
            g_verbose = true;
        } else {
            cerr << "Unknown flag: " << flag << endl;
            cerr << "Usage: " << argv[0] << " [-v]" << endl;
            g_log << "Unknown flag: " << flag << "\n";
            g_log << "Usage: " << argv[0] << " [-v]\n";
            return 1;
        }
    }

    info("Running lexer tests...");

    testHeadings();
    testParagraphsAndBreaks();
    testEmphasis();
    testBlockquotes();
    testLists();
    testCode();
    testLinksAndImages();
    testEscapesSymbolsAndRules();
    testTables();
    testHtml();
    testAutolinksFootnotesDefinition();
    testMixedStress();

    if (g_failures > 0) {
        cerr << "[FAIL] " << g_failures << " category test(s) failed" << endl;
        g_log << "[FAIL] " << g_failures << " category test(s) failed\n";
        return 1;
    }

    info("[OK] All category lexer tests passed");
    return 0;
}
