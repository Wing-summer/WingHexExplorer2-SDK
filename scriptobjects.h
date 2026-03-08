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

#ifndef SCRIPTOBJECTS_H
#define SCRIPTOBJECTS_H

#include "wingplugin_global.h"

class CScriptAny;
class CScriptArray;
class CScriptDictionary;
class CScriptGrid;
class PluginSystem;
class ScriptArray;

namespace WingHex {

typedef void (*ScriptObjDeleter)(void *data);

class WINGPLUGIN_EXPORT ASScriptDictionary {
    friend class ::PluginSystem;

public:
    ~ASScriptDictionary();

    // Sets a key/value pair
    virtual bool set(const QString &key, void *value,
                     std::string_view type) = 0;
    virtual bool set(const QString &key, const quint64 &value) = 0;
    virtual bool set(const QString &key, const qint64 &value) = 0;
    virtual bool set(const QString &key, const double &value) = 0;

    // Gets the stored value. Returns false if the value isn't compatible with
    // the informed typeId
    virtual bool get(const QString &key, void *value,
                     std::string_view type) const = 0;
    virtual bool get(const QString &key, quint64 &value) const = 0;
    virtual bool get(const QString &key, qint64 &value) const = 0;
    virtual bool get(const QString &key, double &value) const = 0;

    // Returns the type id of the stored value, or negative if it doesn't exist
    virtual QString getType(const QString &key) const = 0;

    // Returns true if the key is set
    virtual bool exists(const QString &key) const = 0;

    // Returns true if there are no key/value pairs in the dictionary
    virtual bool isEmpty() const = 0;

    // Returns the number of key/value pairs in the dictionary
    virtual uint size() const = 0;

    // Deletes the key
    virtual bool remove(const QString &key) = 0;

    // Deletes all keys
    virtual void clear() = 0;

public:
    void *takeReturn();
    void setFromWingAngelApi(void *data);
    bool isValid() const;

    static void release(void *data);

private:
    Q_DISABLE_COPY_MOVE(ASScriptDictionary)
    inline static ScriptObjDeleter _deleter;

protected:
    ASScriptDictionary(CScriptDictionary *data);
    CScriptDictionary *_data;
};

class WINGPLUGIN_EXPORT ASScriptArray {
    friend class ::PluginSystem;
    friend class ::ScriptArray;

public:
    ~ASScriptArray();

    // Type information
    virtual QString elementType() const = 0;

    // Get the current size
    virtual uint size() const = 0;

    // Returns true if the array is empty
    virtual bool isEmpty() const = 0;

    // Pre-allocates memory for elements
    virtual void reserve(uint maxElements) = 0;

    // Resize the array
    virtual void resize(uint numElements) = 0;

    // Get a pointer to an element. Returns 0 if out of bounds
    virtual void *at(uint index) = 0;
    virtual const void *at(uint index) const = 0;

    // Set value of an element.
    // The value arg should be a pointer to the value that will be copied to the
    // element. Remember, if the array holds handles the value parameter should
    // be the address of the handle. The refCount of the object will also be
    // incremented
    virtual void setValue(uint index, void *value) = 0;

    // Array manipulation
    virtual void insert(uint index, void *value) = 0;
    virtual void insert(uint index, const ASScriptArray &arr) = 0;
    virtual void append(void *value) = 0;
    virtual void removeAt(uint index) = 0;
    virtual void removeLast() = 0;
    virtual void removeRange(uint start, uint count) = 0;
    virtual void sortAsc() = 0;
    virtual void sortDesc() = 0;
    virtual void sortAsc(uint startAt, uint count) = 0;
    virtual void sortDesc(uint startAt, uint count) = 0;
    virtual void sort(uint startAt, uint count, bool asc) = 0;
    virtual void reverse() = 0;
    virtual int find(const void *value) const = 0;
    virtual int find(uint startAt, const void *value) const = 0;
    virtual int findByRef(const void *ref) const = 0;
    virtual int findByRef(uint startAt, const void *ref) const = 0;

    // Return the address of internal buffer for direct manipulation of elements
    virtual void *buffer() = 0;

public:
    void *takeReturn();
    void setFromWingAngelApi(void *data);
    bool isValid() const;

    static void release(void *data);

private:
    Q_DISABLE_COPY_MOVE(ASScriptArray)
    inline static ScriptObjDeleter _deleter;

protected:
    ASScriptArray(CScriptArray *data);
    CScriptArray *_data;
};

class WINGPLUGIN_EXPORT ASScriptGrid {
    friend class ::PluginSystem;

public:
    ~ASScriptGrid();

    virtual QString elementType() const = 0;

    // Size
    virtual uint width() const = 0;
    virtual uint height() const = 0;
    virtual void resize(uint width, uint height) = 0;

    // Get a pointer to an element. Returns 0 if out of bounds
    virtual void *at(uint x, uint y) = 0;
    virtual const void *at(uint x, uint y) const = 0;

    // Set value of an element
    // Remember, if the grid holds handles the value parameter should be the
    // address of the handle. The refCount of the object will also be
    // incremented
    virtual void setValue(uint x, uint y, void *value) = 0;

public:
    void *takeReturn();
    bool isValid() const;

    static void release(void *data);

private:
    Q_DISABLE_COPY_MOVE(ASScriptGrid)
    inline static ScriptObjDeleter _deleter;

protected:
    ASScriptGrid(CScriptGrid *data);
    CScriptGrid *_data;
};

using ASScript2DArray = ASScriptGrid;

class WINGPLUGIN_EXPORT ASScriptAny {
    friend class ::PluginSystem;

public:
    ~ASScriptAny();

    // Store the value, either as variable type, integer number, or real number
    virtual void store(void *ref, std::string_view type) = 0;
    virtual void store(const quint64 &value) = 0;
    virtual void store(const qint64 &value) = 0;
    virtual void store(const double &value) = 0;

    // Retrieve the stored value, either as variable type, integer number, or
    // real number
    virtual bool retrieve(void *ref, std::string_view type) const = 0;
    virtual bool retrieve(quint64 &value) const = 0;
    virtual bool retrieve(qint64 &value) const = 0;
    virtual bool retrieve(double &value) const = 0;

public:
    void *takeReturn();
    bool isValid() const;

    static void release(void *data);

private:
    Q_DISABLE_COPY_MOVE(ASScriptAny)
    inline static ScriptObjDeleter _deleter;

protected:
    ASScriptAny(CScriptAny *data);
    CScriptAny *_data;
};

} // namespace WingHex

#endif // SCRIPTOBJECTS_H
