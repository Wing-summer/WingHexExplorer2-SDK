#include "iwingangel.h"

using namespace WingHex;

IWingAngel::IWingAngel() {}

WingHex::IWingAngel::asRetCodes
IWingAngel::registerEnums(const QString &type,
                          const QHash<QString, int> value) {
    auto t = type.toUtf8();
    auto r = registerEnum(t);
    if (r != asRetCodes::asSUCCESS) {
        return r;
    }
    for (auto &&v : value.asKeyValueRange()) {
        auto r = registerEnumValue(t, v.first.toUtf8(), v.second);
        if (r != asRetCodes::asSUCCESS) {
            return r;
        }
    }
    return asRetCodes::asSUCCESS;
}
