//
// Created by nietup on 12.07.17.
//

#ifndef DEGREE_UTILITIES_H
#define DEGREE_UTILITIES_H

class Utilities {
public:
    std::pair<uint, uint> unpair(uint i) {
        int y = (int) floor(0.5 * (sqrt(8.0 * i + 1.0) + 1.0));
        int x = (int) (0.5 * (y * y + y - 2 * i) - 1);
        return std::pair<uint, uint>{x, y};
    };

    template<class T>
    bool myContains(std::weak_ptr<T> object,
                    const std::vector<std::weak_ptr<T>> &v) {

        for (auto &a : v) {
            if (object.lock().get() == a.lock().get()) {
                return true;
            }
        }

        return false;
    }
};
#endif //DEGREE_UTILITIES_H
