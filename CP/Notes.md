1. To delete last element from map: `m.erase(prev(m.end())`. `m.erase(m.rbegin())` does not work.
2. map `[]` will insert an element if not found. `at()` throws `out_of_range` excpetion if not found.