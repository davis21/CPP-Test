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
vector<int> extractIntegersFromString(const string& str);

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
int part1(const vector<string>& lines);

// Solves part 2 of the puzzle, given the input lines.
// Part 2 unlocks after part 1 is solved and usually builds on the same input.
int part2(const vector<string>& lines);

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

int part1(const vector<string>& lines)
{
    // Implement the logic for part 1 here
    int result = 0;

    // Dial starts at 50. Can rotate left or right, depending on the letter at the start of the line.
    // If it's L, rotate left by the number of degrees specified. If it's R, rotate right by the number of degrees specified.
    // The dial is circular, so if it goes past 0 or 100, it wraps around to the other side.
    // The actual password is the number of times the dial is left pointing at 0 after any rotation in the sequence.
    int dialPosition = 50;
    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        char direction = line[0];
        int degrees = stoi(line.substr(1));

        if (direction == 'L')
        {
            degrees = (degrees + 100) % 100;
            dialPosition -= degrees;
            if (dialPosition < 0)
            {
                dialPosition += 100;
            }
        }
        else if (direction == 'R')
        {
            degrees %= 100;
            dialPosition += degrees;
            if (dialPosition >= 100)
            {
                dialPosition -= 100;
            }
        }
        
        if (dialPosition == 0)
        {
            result++;
        }
    }

    return result;
}

int part2(const vector<string>& lines)
{
    // Implement the logic for part 2 here
    int result = 0;

    // Dial starts at 50. Can rotate left or right, depending on the letter at the start of the line.
    // If it's L, rotate left by the number of degrees specified. If it's R, rotate right by the number of degrees specified.
    // The dial is circular, so if it goes past 0 or 100, it wraps around to the other side.
    // Now the result is every time the dial passes through 0, not just when it lands on 0. So if the dial is at 1 and rotates left by 2, it passes through 0 and lands on 99, so that counts as a pass through 0.
    // Be careful: if the dial were pointing at 50, a single rotation like R1000 would cause the dial to point at 0 ten times before returning back to 50!
    // This needs to be counted as 10 passes through 0, not just 1.
    int dialPosition = 50;
    
    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        char direction = line[0];
        int degrees = stoi(line.substr(1));

        if (direction == 'L')
        {
            for (int j = 0; j < degrees; j++)
            {
                dialPosition--;
                if (dialPosition < 0)
                {
                    dialPosition += 100;
                }
                if (dialPosition == 0)
                {
                    result++;
                }
            }
        }
        else if (direction == 'R')
        {
            for (int j = 0; j < degrees; j++)
            {
                dialPosition++;
                if (dialPosition >= 100)
                {
                    dialPosition -= 100;
                }
                if (dialPosition == 0)
                {
                    result++;
                }
            }
        }
    }

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

vector<int> extractIntegersFromString(const string& str)
{
    vector<int> integers;
    int current = 0;           // the number currently being built, digit by digit
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
