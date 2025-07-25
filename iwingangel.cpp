/*==============================================================================
** Copyright (C) 2024-2027 WingSummer
**
** You can redistribute this file and/or modify it under the terms of the
** BSD 3-Clause.
**
** THIS FILE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
** =============================================================================
*/

#include "iwingangel.h"

using namespace WingHex;

IWingAngel::IWingAngel() {}

WingHex::asRetCodes IWingAngel::registerEnums(const QString &type,
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

IWingGeneric::IWingGeneric() {}
