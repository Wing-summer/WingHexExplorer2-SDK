#include "iwingangel.h"

using namespace WingHex;

IWingAngel::IWingAngel() {}

WingHex::IWingAngel::asRetCodes
IWingAngel::registerEnums(const char *type,
                          const QHash<const char *, int> value) {
    auto r = registerEnum(type);
    if (r != asRetCodes::asSUCCESS) {
        return r;
    }
    for (auto &&v : value.asKeyValueRange()) {
        auto r = registerEnumValue(type, v.first, v.second);
        if (r != asRetCodes::asSUCCESS) {
            return r;
        }
    }
    return asRetCodes::asSUCCESS;
}
