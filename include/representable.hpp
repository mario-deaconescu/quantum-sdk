#pragma once

#include <string>

namespace QPP {

    class Representable {
    public:
        [[nodiscard]] virtual std::string getRepresentation() const = 0;

        friend std::ostream &operator<<(std::ostream &os, const Representable &representable) {
            os << representable.getRepresentation();
            return os;
        }

        virtual ~Representable() = default;
    };

}