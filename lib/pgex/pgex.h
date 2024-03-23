#ifndef PONYREGEX_H
#define PONYREGEX_H

#include <string>
#include "linked_list.h"
#include "ponystring.h"

namespace ponyregex {

    // Функция для поиска всех совпадений в строке по регулярному выражению
    List<ponystring> findAllMatches(const ponystring& text, const ponystring& pattern);

    // Функция для замены всех совпадений в строке по регулярному выражению
    ponystring replaceAllMatches(const ponystring& text, const ponystring& pattern, const ponystring& replacement);

}

#endif // PONYREGEX_H
