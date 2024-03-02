#include "pgex.h"
#include <regex>
#include "linked_list.h"
#include "ponystring.h"

namespace ponyregex {

    List<ponystring> findAllMatches(const ponystring& text, const ponystring& pattern) {
        List<ponystring> matches;
        std::regex regexPattern(pattern.str());
        std::smatch match;

        auto it = text.cbegin();
        auto end = text.cend();

        while (std::regex_search(it, end, match, regexPattern)) {
            matches.append(ponystring(match.str()));
            it = match.suffix().first;
        }

        return matches;
    }

    ponystring replaceAllMatches(const ponystring& text, const ponystring& pattern, const ponystring& replacement) {
        std::regex regexPattern(pattern.str());
        return ponystring(std::regex_replace(text.str(), regexPattern, replacement.str()));
    }

}