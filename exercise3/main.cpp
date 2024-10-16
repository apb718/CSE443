// Copyright Alec Byrd 2024
/**
 * A simple program to dedupe a list of users, IP-addresses, and ports
 * to retain either the latest two or just the 3rd (cummulatively)
 * entry for each user.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Synonyms to make the code a bit more readable.

/** The list of input lines read from a given data file.  This list is
 *  maintained in-memory to facilitate de-duping and returning the
 *  values.
 */
using DataList = std::vector<std::string>;

/** A temporary unordered map that is used to ease looking-up repeated
 * user ID's in the data file. The key to this map is the user ID and
 * value is the index position in the DataList vector. For example,
 * if DataList has the following 4 lines in it: 
 * {"bob 1.1.1 20", "may 2.2.2 30", "bob 3.3.3. 40", "doe 4.4.4. 50"}
 * then, OccursList would be {{"bob", {0, 2}}, {"may", {2}}, {"doe", {3}}.
 * This way if we encounter another line for "bob" (that is the 3rd entry
 * for user bob), we can clear out lines at index positions 0 and 2 to
 * de-dupe the data.
 */
using OccursList = std::unordered_map<std::string, std::vector<size_t>>;


/**
 * Prints out an unordered map for debugging
*/
void printUMap(OccursList& ol) {
    for (const auto& pair : ol) {
        std::cout << pair.first << ": ";
        for (const auto& item : pair.second) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}


/**
 * Method that returns a de-duped list of data from the given input
 * stream.  The input stream is assumed to have data in the format:
 * userID IP-address port.  This method dedupes based on userID
 * only. If the same userID occurs for the 3rd time, then this method
 * clears out the previous two entries.
 *
 * \param[in] is The input stream from where the data is to be read.
 *
 * \return This method returns a vector that contains the de-deuped
 * information, with duplicates replaced by an empty string.
 */
DataList dedupe(std::istream& is) {
    // The vector that contains de-duped list of lines (read from the
    // input stream is) to be returned back to the caller
    DataList data;
    std::string tmp;
    while (std::getline(is, tmp)) {
        data.push_back(tmp);
    }
    


    // The following unordered map tracks number of occurrences for
    // each user. See documentaiton on the OccursList (above) for
    // additional details.
    OccursList occurs;

    for (size_t i = 0; i < data.size(); i++) {
        std::stringstream ss(data[i]);
        std::string id, ip, port;
        ss >> id >> ip >> port;
        
        occurs[id].push_back(i);
        if (occurs[id].size() > 2) {
            data[occurs[id].at(0)] = "";
            data[occurs[id].at(1)] = "";
            occurs[id].erase(occurs[id].begin());
            occurs[id].erase(occurs[id].begin());
        }
    }

    // Keep processing triplets of data from the input stream.

    // Return the de-duped list of entries
    return data;
}


//-------------------------------------------------------------------
//  DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//-------------------------------------------------------------------


/**
 * The main method that just acts as a test for the dedupe method in
 * this program.
 *
 * \param[in] argc The number of command-line arguments.  This program
 * requires at least 1.
 *
 * \param[in] argv The actual command-line argument. The first
 * command-line argument is assumed to be the data file to be
 * processed.
 */
int main(int argc, char *argv[]) {
    // Check to ensure we have a data file specified.
    if (argc != 2) {
        std::cout << "Specify data file as command-line argument.\n";
        return 1;
    }
    // Open the data file and ensure it is readable.
    std::ifstream is(argv[1]);
    if (!is.good()) {
        std::cout << "Error reading data from " << argv[1] << std::endl;
        return 2;
    }
    // Now have the dedupe method do the core processing
    const auto result = dedupe(is);
    // Finally, print the deduped list
    for (size_t i = 0; (i < result.size()); i++) {
        std::cout << i << ": " << result[i] << std::endl;
    }
}
