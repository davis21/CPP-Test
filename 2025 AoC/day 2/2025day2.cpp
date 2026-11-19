// =============================================================================
// Advent of Code starter template
//
// How to use:
//   1. Save your puzzle input as "input.txt" in the directory you run the
//      program from, or pass a different file as the first command-line
//      argument (e.g. "starter.exe example.txt" to test with the sample input).
//   2. Write your solutions in part1() and part2().
//   3. Call them from main() and print the results.
//
// Helpers provided:
//   - readFileToStringVector    : load the puzzle input as a list of lines
//   - splitString               : break a line into pieces on a delimiter
//   - extractIntegersFromString : pull every number out of a line
//   - inbounds                  : check whether a (row, col) is inside a grid
//   - findChar                  : locate the first occurrence of a char in a grid
//   - countNeighbors            : count adjacent cells matching a char
// =============================================================================

// --- Standard library headers commonly needed for AoC puzzles ---
#include <iostream>       // cout / cerr for printing answers and errors
#include <fstream>        // ifstream for reading the input file
#include <string>         // string
#include <vector>         // vector: default container for parsed input and grids
#include <map>            // ordered map: keys kept sorted
#include <unordered_map>  // hash map: fast lookups for counting, memoization, etc.
#include <set>            // ordered set: sorted unique values
#include <unordered_set>  // hash set: fast "have I seen this?" checks (visited states)
#include <queue>          // queue for BFS, priority_queue for Dijkstra
#include <algorithm>      // sort, reverse, find, count, min/max, etc.
#include <numeric>        // accumulate (sums), gcd, lcm (common in cycle puzzles)
#include <sstream>        // stringstream for tokenizing strings

using namespace std;

// AoC answers often overflow a 32-bit int, so use 64-bit integers for anything
// that might get large (sums, products, counts).
using ll = long long;

// Direction offsets for moving around a grid. Index d gives the step
// (DR[d], DC[d]) in rows and columns:
//   0-3: up, down, left, right (orthogonal neighbors)
//   4-7: up-left, up-right, down-left, down-right (diagonal neighbors)
// Loop d from 0 to 4 for orthogonal moves only, or 0 to 8 to include diagonals.
const int DR[8] = {-1, 1,  0, 0, -1, -1,  1, 1};   // row change
const int DC[8] = { 0, 0, -1, 1, -1,  1, -1, 1};   // column change

// -----------------------------------------------------------------------------
// Forward declarations: these let main() and the part functions sit at the top
// of the file while the helper implementations live at the bottom.
// -----------------------------------------------------------------------------

// Reads the file at `filename` and returns its contents as a vector of strings,
// one element per line (newline characters are stripped, blank lines are kept).
// Prints an error and exits the program if the file cannot be opened.
vector<string> readFileToStringVector(const string& filename);

// Splits `str` into tokens separated by `delimiter`.
// Empty tokens (from consecutive, leading, or trailing delimiters) are skipped,
// e.g. splitString("a,,b,", ',') returns {"a", "b"}.
vector<string> splitString(const string& str, char delimiter);

// Scans `str` and returns every integer found in it, in order, as long longs.
// Any non-digit character acts as a separator. A '-' is treated as a minus sign
// only when it directly precedes a digit and isn't already inside a number,
// e.g. "x=-12, y=7" returns {-12, 7} and "3-5" returns {3, 5}.
// Handy for AoC lines like "Sensor at x=2, y=-18" where only the numbers matter.
vector<ll> extractIntegersFromString(const string& str);

// Returns true if (r, c) is a valid position in `grid`, i.e. the row exists
// and the column is within that row's length. Use it before indexing a
// neighbor so you don't read off the edge of the grid.
bool inbounds(int r, int c, const vector<string>& grid);

// Searches `grid` row by row, left to right, and returns the {row, col} of the
// first cell equal to `target`, or {-1, -1} if it isn't found.
// Useful for finding a start position such as 'S'.
pair<int, int> findChar(const vector<string>& grid, char target);

// Counts how many cells next to (r, c) in `grid` equal `target`.
// numDirs = 4 checks only up/down/left/right; numDirs = 8 also checks diagonals.
// Neighbors outside the grid are ignored.
int countNeighbors(const vector<string>& grid, int r, int c, char target, int numDirs = 4);

// Solves part 1 of the puzzle, given the input lines.
// Returns the answer as a string so it works for numeric and text answers alike.
ll part1(const vector<string>& lines);

// Solves part 2 of the puzzle, given the input lines.
// Part 2 unlocks after part 1 is solved and usually builds on the same input.
ll part2(const vector<string>& lines);

// Entry point. Reads the input file named by the first command-line argument
// (or "input.txt" if none is given) into a vector of lines, which you then
// pass to part1() and part2().
int main(int argc, char* argv[])
{
    // Use the filename from the command line if one was given, so you can
    // switch between the example input and your real input without recompiling.
    string filename = (argc > 1) ? argv[1] : "input.txt";

    // Load the whole puzzle input, one string per line.
    vector<string> lines = readFileToStringVector(filename);

    // Put code here to call part1 and part2 with the lines read from the file
    cout << "Part 1: " << part1(lines) << "\n";
    cout << "Part 2: " << part2(lines) << "\n";

    return 0;
}

ll part1(const vector<string>& lines)
{
    // Implement the logic for part 1 here
    ll result = 0;

    // Given a list of ID ranges, we need to find the number of invalid IDs.
    // The ranges given are separated by commas on a single line in the input.
    // Each range themselves is given as a pair of numbers separated by a dash, e.g. 123-456.
    // An invalid ID is one that has a repeated pattern, i.e. 55 (5 twice), 6464 (64 repeated), 123123 (123 repeated), etc.
    // We must find all of the invalid IDs in the given ranges and then return the SUM of all of the invalid IDs.
    
    // Step 1: Extract the ranges from the input line.
    vector<string> ranges = splitString(lines[0], ',');
    for (auto r : ranges)
    {
        // extract the start and end of the range
        vector<ll> currentRange = extractIntegersFromString(r);
        if (currentRange.size() != 2)
        {
            cerr << "Error: Invalid range format: " << r << "\n";
            exit(1);
        }

        // Start = currentRange[0], End = currentRange[1]
        ll start = currentRange[0];
        ll end = currentRange[1];

        // Loop through the range and check each ID for invalidity
        for (ll currentId = start; currentId <= end; ++currentId)
        {
            // Convert the ID to a string to check for repeated patterns
            string idStr = to_string(currentId);

            // If the length of the ID is odd, it cannot have a repeated pattern, so we skip it
            if (idStr.length() % 2 != 0)
            {
                continue;
            }

            // Split the ID string into two halves and check if they are equal
            string firstHalf = idStr.substr(0, idStr.length() / 2);
            string secondHalf = idStr.substr(idStr.length() / 2);
            if (firstHalf == secondHalf)
            {
                // This ID is invalid, so we add it to the result
                result += currentId;
            }
        }
    }

    return result;
}

ll part2(const vector<string>& lines)
{
    // Running total of every invalid ID found across all ranges. This is a
    // long long because summing many large IDs can easily overflow an int.
    ll result = 0;

    // PROBLEM RECAP
    // Part 2 uses the same ranges as part 1 but a broader definition of "invalid".
    // An ID is invalid if it consists entirely of some block of digits repeated
    // AT LEAST twice (part 1 only allowed exactly twice). For example:
    //   12341234   -> "1234" x 2
    //   123123123  -> "123"  x 3
    //   1212121212 -> "12"   x 5
    //   1111111    -> "1"    x 7
    // The answer is the SUM of all invalid IDs in all of the ranges.

    // -------------------------------------------------------------------------
    // STEP 1: Split the input into individual ranges.
    // The whole input is one line like "11-22,95-115,998-1012", so splitting
    // lines[0] on ',' gives one string per range: {"11-22", "95-115", ...}.
    // -------------------------------------------------------------------------
    vector<string> ranges = splitString(lines[0], ',');
    for (auto r : ranges)
    {
        // ---------------------------------------------------------------------
        // STEP 2: Parse the range's start and end.
        // extractIntegersFromString("95-115") returns {95, 115}. The '-' is
        // treated as a separator, not a minus sign, because it comes right after
        // a digit. Anything other than exactly two numbers means the input
        // isn't in the expected format, so stop with an error.
        // ---------------------------------------------------------------------
        vector<ll> currentRange = extractIntegersFromString(r);
        if (currentRange.size() != 2)
        {
            cerr << "Error: Invalid range format: " << r << "\n";
            exit(1);
        }

        ll start = currentRange[0];
        ll end = currentRange[1];

        // ---------------------------------------------------------------------
        // STEP 3: Test every ID in the range, inclusive of both ends.
        // ---------------------------------------------------------------------
        for (ll currentId = start; currentId <= end; ++currentId)
        {
            // Working with the ID as a string makes it easy to grab chunks
            // of digits with substr() and compare them.
            string idStr = to_string(currentId);

            // Flag that is set once we find ANY block length that makes this
            // ID a repetition. One match is enough to call it invalid.
            bool isInvalid = false;

            // -----------------------------------------------------------------
            // STEP 4: Try every possible length for the repeating block.
            //
            // `len` is the candidate block length. It only goes up to half the
            // ID's length because the block has to appear at least twice: a
            // block longer than half the string couldn't fit twice.
            //
            // Note: single-digit IDs have length/2 == 0, so this loop doesn't
            // run at all and they are correctly treated as valid.
            // -----------------------------------------------------------------
            for (int len = 1; len <= idStr.length() / 2; ++len)
            {
                // The block must tile the string exactly with nothing left
                // over, so `len` must divide the ID's length evenly.
                // e.g. for a 6-digit ID, only lengths 1, 2, and 3 are possible;
                // a 4-digit block would leave 2 digits left over.
                if (idStr.length() % len == 0)
                {
                    // ---------------------------------------------------------
                    // STEP 5: Check whether the first `len` digits repeat
                    // across the entire ID.
                    //
                    // Take the first block as the pattern, then walk through
                    // the rest of the string one block at a time (i = len,
                    // 2*len, 3*len, ...) and compare each block to it.
                    // ---------------------------------------------------------
                    string pattern = idStr.substr(0, len);

                    // Assume the pattern repeats until a block proves otherwise.
                    bool isRepeated = true;
                    for (int i = len; i < idStr.length(); i += len)
                    {
                        // As soon as one block differs, this `len` can't
                        // work, so stop comparing and try the next length.
                        if (idStr.substr(i, len) != pattern)
                        {
                            isRepeated = false;
                            break;
                        }
                    }

                    // Every block matched the pattern, so the ID is just the
                    // pattern repeated (length / len) times. No need to check
                    // longer block lengths; one match is enough.
                    if (isRepeated)
                    {
                        isInvalid = true;
                        break;
                    }
                }
            }

            // -----------------------------------------------------------------
            // STEP 6: Add invalid IDs to the running total.
            //
            // Worked example, currentId = 123123123 (length 9):
            //   len = 1: 9 % 1 == 0, pattern "1"; block "2" differs -> no
            //   len = 2: 9 % 2 != 0 -> skipped
            //   len = 3: 9 % 3 == 0, pattern "123"; blocks "123", "123" match
            //            -> isInvalid = true, so 123123123 is added
            // -----------------------------------------------------------------
            if (isInvalid)
            {
                result += currentId;
            }
        }
    }

    // The sum of every invalid ID across all ranges is the puzzle answer.
    return result;
}

// =============================================================================
// Helper implementations
// =============================================================================

vector<string> readFileToStringVector(const string& filename)
{
    vector<string> lines;
    ifstream inputFile(filename);

    // Fail loudly if the file is missing. A common cause is running the program
    // from a different working directory than the one containing input.txt.
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }

    // Read one line at a time until end of file. Blank lines are kept, since
    // many puzzles use them to separate sections of the input.
    string line;
    while (getline(inputFile, line))
    {
        lines.push_back(line);
    }

    return lines;
}

vector<string> splitString(const string& str, char delimiter)
{
    vector<string> tokens;
    string token;
    stringstream tokenStream(str);

    // getline with a custom delimiter reads up to the next `delimiter` each call.
    while (getline(tokenStream, token, delimiter))
    {
        // Skip empty pieces so repeated delimiters (e.g. multiple spaces between
        // numbers) don't produce blank tokens.
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

vector<ll> extractIntegersFromString(const string& str)
{
    vector<ll> integers;
    ll current = 0;           // the number currently being built, digit by digit
    bool inNumber = false;    // true while in the middle of reading a number
    bool isNegative = false;  // true if the current number started with '-'

    for (int i = 0; i < str.size(); ++i)
    {
        char c = str[i];

        // A '-' starts a negative number only if a digit follows it and we aren't
        // already reading a number (so "3-5" is read as 3 and 5, not 3 and -5).
        if (c == '-' && !inNumber && (i + 1 < str.size() && isdigit(str[i + 1])))
        {
            isNegative = true;
            inNumber = true;
        }
        // A digit: append it to the number being built.
        else if (isdigit(c))
        {
            current = current * 10 + (c - '0');
            inNumber = true;
        }
        // Any other character ends the current number, if there is one.
        else
        {
            if (inNumber)
            {
                if (isNegative)
                {
                    current = -current;
                }
                integers.push_back(current);

                // Reset state for the next number.
                current = 0;
                isNegative = false;
                inNumber = false;
            }
        }
    }

    // If the string ended in the middle of a number, save that last number too.
    if (inNumber)
    {
        if (isNegative)
        {
            current = -current;
        }
        integers.push_back(current);
    }

    return integers;
}

bool inbounds(int r, int c, const vector<string>& grid)
{
    // Check the row first: grid[r] is only safe to access once r is known valid.
    return r >= 0 && r < (int)grid.size() && c >= 0 && c < (int)grid[r].size();
}

pair<int, int> findChar(const vector<string>& grid, char target)
{
    // Scan every cell, returning as soon as a match is found.
    for (int r = 0; r < (int)grid.size(); ++r)
    {
        for (int c = 0; c < (int)grid[r].size(); ++c)
        {
            if (grid[r][c] == target)
            {
                return {r, c};
            }
        }
    }

    // Not found anywhere in the grid.
    return {-1, -1};
}

int countNeighbors(const vector<string>& grid, int r, int c, char target, int numDirs)
{
    int count = 0;

    // Step in each of the first numDirs directions from DR/DC.
    for (int d = 0; d < numDirs; ++d)
    {
        int nr = r + DR[d];
        int nc = c + DC[d];

        // Skip neighbors that fall outside the grid.
        if (!inbounds(nr, nc, grid)) continue;
        if (grid[nr][nc] == target) count++;
    }
    return count;
}
