/*==============================================================================
** Copyright (C) 2026-2029 WingSummer
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

#include "scriptobjects.h"

using namespace WingHex;

ASScriptDictionary::~ASScriptDictionary() {
    if (_data) {
        _deleter(_data);
        _data = nullptr;
    }
}

void *ASScriptDictionary::takeReturn() { return std::exchange(_data, nullptr); }

void ASScriptDictionary::setFromWingAngelApi(void *data) {
    _data = static_cast<CScriptDictionary *>(data);
}

bool ASScriptDictionary::isValid() const { return _data; }

void ASScriptDictionary::release(void *data) {
    if (data) {
        _deleter(data);
    }
}

ASScriptDictionary::ASScriptDictionary(CScriptDictionary *data) : _data(data) {}

ASScriptArray::~ASScriptArray() {
    if (_data) {
        _deleter(_data);
        _data = nullptr;
    }
}

void *ASScriptArray::takeReturn() { return std::exchange(_data, nullptr); }

void ASScriptArray::setFromWingAngelApi(void *data) {
    _data = static_cast<CScriptArray *>(data);
}

bool ASScriptArray::isValid() const { return _data; }

void ASScriptArray::release(void *data) {
    if (data) {
        _deleter(data);
    }
}

ASScriptArray::ASScriptArray(CScriptArray *data) : _data(data) {}

ASScriptGrid::~ASScriptGrid() {
    if (_data) {
        _deleter(_data);
        _data = nullptr;
    }
}

void *ASScriptGrid::takeReturn() { return std::exchange(_data, nullptr); }

bool ASScriptGrid::isValid() const { return _data; }

void ASScriptGrid::release(void *data) {
    if (data) {
        _deleter(data);
    }
}

ASScriptGrid::ASScriptGrid(CScriptGrid *data) : _data(data) {}

ASScriptAny::~ASScriptAny() {
    if (_data) {
        _deleter(_data);
        _data = nullptr;
    }
}

void *ASScriptAny::takeReturn() { return std::exchange(_data, nullptr); }

bool ASScriptAny::isValid() const { return _data; }

void ASScriptAny::release(void *data) {
    if (data) {
        _deleter(data);
    }
}

ASScriptAny::ASScriptAny(CScriptAny *data) : _data(data) {}
