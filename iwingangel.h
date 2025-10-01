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

#ifndef IWINGANGEL_H
#define IWINGANGEL_H

#include "WingPlugin/wingcore.h"
#include "WingPlugin/wingplugin_global.h"

#include <QHash>
#include <type_traits>

namespace WingHex {

// some classes and helper copied from angelscript.h

class asWingUnknownClass;
typedef void (asWingUnknownClass::*asWINGMETHOD_t)();
typedef void (*asWINGFUNCTION_t)();

// A helper class
class asIWingGeneric;
typedef void (*asWINGGENFUNC_t)(asIWingGeneric *);

struct asFuncPtr {
    asFuncPtr(uchar f = 0) {
        for (size_t n = 0; n < sizeof(ptr.dummy); n++)
            ptr.dummy[n] = 0;
        flag = f;
    }

    void CopyMethodPtr(const void *mthdPtr, size_t size) {
        for (size_t n = 0; n < size; n++)
            ptr.dummy[n] = reinterpret_cast<const char *>(mthdPtr)[n];
    }

    union {
        // The largest known method point is 20 bytes (MSVC 64bit),
        // but with 8byte alignment this becomes 24 bytes. So we need
        // to be able to store at least that much.
        char dummy[25];
        struct {
            asWINGMETHOD_t mthd;
            char dummy[25 - sizeof(asWINGMETHOD_t)];
        } m;
        struct {
            asWINGFUNCTION_t func;
            char dummy[25 - sizeof(asWINGFUNCTION_t)];
        } f;
    } ptr;

    uchar flag; // 1 = generic, 2 = global func, 3 = method
};

//-----------------------------------------------------------------
// Function pointers

// Template function to capture all global functions,
// except the ones using the generic calling convention
template <class T>
inline asFuncPtr asWingFunctionPtr(T func) {
    // Mark this as a global function
    asFuncPtr p(2);

#ifdef AS_64BIT_PTR
    // The size_t cast is to avoid a compiler warning with asFUNCTION(0)
    // on 64bit, as 0 is interpreted as a 32bit int value
    p.ptr.f.func = reinterpret_cast<asWINGFUNCTION_t>(size_t(func));
#else
    // MSVC6 doesn't like the size_t cast above so I
    // solved this with a separate code for 32bit.
    p.ptr.f.func = reinterpret_cast<asWINGFUNCTION_t>(func);
#endif

    return p;
}

// Specialization for functions using the generic calling convention
template <>
inline asFuncPtr asWingFunctionPtr<asWINGGENFUNC_t>(asWINGGENFUNC_t func) {
    // Mark this as a generic function
    asFuncPtr p(1);
    p.ptr.f.func = reinterpret_cast<asWINGFUNCTION_t>(func);
    return p;
}

// Method pointers

// Declare a dummy class so that we can determine the size of a simple
// method pointer
class asWingSimpleDummy {};
typedef void (asWingSimpleDummy::*WINGSIMPLEMETHOD_t)();
static constexpr int AS_SINGLE_PTR_SIZE = sizeof(WINGSIMPLEMETHOD_t);

// Define template
template <int N>
struct asWingMethodPtr {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // This version of the function should never be executed, nor
        // compiled, as it would mean that the size of the method pointer
        // cannot be determined.

        int ERROR_UnsupportedMethodPtr[N - 100];

        asFuncPtr p(0);
        return p;
    }
};

// Template specialization
template <>
struct asWingMethodPtr<AS_SINGLE_PTR_SIZE> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, AS_SINGLE_PTR_SIZE);
        return p;
    }
};

#if defined(_MSC_VER) && !defined(__MWERKS__)
// MSVC and Intel uses different sizes for different class method pointers
template <>
struct asWingMethodPtr<AS_SINGLE_PTR_SIZE + 1 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, AS_SINGLE_PTR_SIZE + sizeof(int));
        return p;
    }
};

template <>
struct asWingMethodPtr<AS_SINGLE_PTR_SIZE + 2 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // On 32bit platforms with is where a class with virtual inheritance
        // falls. On 64bit platforms we can also fall here if 8byte data
        // alignments is used.

        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, AS_SINGLE_PTR_SIZE + 2 * sizeof(int));

        // Microsoft has a terrible optimization on class methods with
        // virtual inheritance. They are hardcoding an important offset,
        // which is not coming in the method pointer.

#if defined(_MSC_VER) && !defined(AS_64BIT_PTR)
        // Method pointers for virtual inheritance is not supported,
        // as it requires the location of the vbase table, which is
        // only available to the C++ compiler, but not in the method
        // pointer.

        // You can get around this by forward declaring the class and
        // storing the sizeof its method pointer in a constant. Example:

        // class ClassWithVirtualInheritance;
        // const int ClassWithVirtualInheritance_workaround = sizeof(void
        // ClassWithVirtualInheritance::*());

        // This will force the compiler to use the unknown type
        // for the class, which falls under the next case

        // Copy the virtual table index to the 4th dword so that AngelScript
        // can properly detect and deny the use of methods with virtual
        // inheritance.
        *(reinterpret_cast<quint32 *>(&p) + 3) =
            *(reinterpret_cast<quint32 *>(&p) + 2);
#endif

        return p;
    }
};

template <>
struct asWingMethodPtr<AS_SINGLE_PTR_SIZE + 3 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, AS_SINGLE_PTR_SIZE + 3 * sizeof(int));
        return p;
    }
};

template <>
struct asWingMethodPtr<AS_SINGLE_PTR_SIZE + 4 * sizeof(int)> {
    template <class M>
    static asFuncPtr Convert(M Mthd) {
        // On 64bit platforms with 8byte data alignment
        // the unknown class method pointers will come here.

        // Mark this as a class method
        asFuncPtr p(3);
        p.CopyMethodPtr(&Mthd, AS_SINGLE_PTR_SIZE + 4 * sizeof(int));
        return p;
    }
};

#endif

#define AS_WING_METHOD_AMBIGUITY_CAST(t) static_cast<t>

#define asWINGMETHOD(c, m)                                                     \
    WingHex::asWingMethodPtr<sizeof(void (c::*)())>::Convert(                  \
        (void (c::*)())(&c::m))
#define asWINGMETHODPR(c, m, p, r)                                             \
    WingHex::asWingMethodPtr<sizeof(void (c::*)())>::Convert(                  \
        AS_WING_METHOD_AMBIGUITY_CAST(r(c::*) p)(&c::m))

#define asWINGFUNCTION(f) WingHex::asWingFunctionPtr(f)
#if (defined(_MSC_VER) && _MSC_VER <= 1200) ||                                 \
    (defined(__BORLANDC__) && __BORLANDC__ < 0x590)
// MSVC 6 has a bug that prevents it from properly compiling using the correct
// asFUNCTIONPR with operator > so we need to use ordinary C style cast instead
// of static_cast. The drawback is that the compiler can't check that the cast
// is really valid. BCC v5.8 (C++Builder 2006) and earlier have a similar bug
// which forces us to fall back to a C-style cast.
#define asWINGFUNCTIONPR(f, p, r)                                              \
    WingHex::asWingFunctionPtr((void (*)())((r(*) p)(f)))
#else
#define asWINGFUNCTIONPR(f, p, r)                                              \
    WingHex::asWingFunctionPtr(                                                \
        reinterpret_cast<void (*)()>(static_cast<r(*) p>(f)))
#endif

/**
 * @brief The IWingGeneric class
 * @note helper class for generic call params
 */
class WINGPLUGIN_EXPORT IWingGeneric {
public:
    IWingGeneric();
    ~IWingGeneric() = default;

public:
    // Arguments
    virtual int argCount() const = 0;
    virtual int argTypeId(uint arg, quint32 *flags = nullptr) const = 0;
    virtual QString argTypeName(uint arg, quint32 *flags = nullptr) = 0;
    virtual uchar argByte(uint arg) = 0;
    virtual quint16 argWord(uint arg) = 0;
    virtual quint32 argDWord(uint arg) = 0;
    virtual quint64 argQWord(uint arg) = 0;
    virtual float argFloat(uint arg) = 0;
    virtual double argDouble(uint arg) = 0;
    virtual void *argAddress(uint arg) = 0;
    virtual void *argObject(uint arg) = 0;
    virtual void *addressOfArg(uint arg) = 0;

    // Return value
    virtual int returnTypeId(quint32 *flags = 0) const = 0;
    virtual asRetCodes setReturnByte(uchar val) = 0;
    virtual asRetCodes setReturnWord(quint16 val) = 0;
    virtual asRetCodes setReturnDWord(quint32 val) = 0;
    virtual asRetCodes setReturnQWord(quint64 val) = 0;
    virtual asRetCodes setReturnFloat(float val) = 0;
    virtual asRetCodes setReturnDouble(double val) = 0;
    virtual asRetCodes setReturnAddress(void *addr) = 0;
    virtual asRetCodes setReturnObject(void *obj) = 0;
    virtual void *addressOfReturnLocation() = 0;
};

/**
 * @brief The IWingAngel class
 * @note we can register the script types by this class
 */
class WINGPLUGIN_EXPORT IWingAngel {
public:
    IWingAngel();

public:
    using ScriptFn = std::function<QVariant(const QVariantList &)>;

public:
    // copy from angelscript.h

    // Object type flags
    enum asEObjTypeFlags : quint64 {
        asOBJ_REF = (1 << 0),
        asOBJ_VALUE = (1 << 1),
        asOBJ_GC = (1 << 2),
        asOBJ_POD = (1 << 3),
        asOBJ_NOHANDLE = (1 << 4),
        asOBJ_SCOPED = (1 << 5),
        asOBJ_TEMPLATE = (1 << 6),
        asOBJ_ASHANDLE = (1 << 7),
        asOBJ_APP_CLASS = (1 << 8),
        asOBJ_APP_CLASS_CONSTRUCTOR = (1 << 9),
        asOBJ_APP_CLASS_DESTRUCTOR = (1 << 10),
        asOBJ_APP_CLASS_ASSIGNMENT = (1 << 11),
        asOBJ_APP_CLASS_COPY_CONSTRUCTOR = (1 << 12),
        asOBJ_APP_CLASS_C = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR),
        asOBJ_APP_CLASS_CD = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
                              asOBJ_APP_CLASS_DESTRUCTOR),
        asOBJ_APP_CLASS_CA = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
                              asOBJ_APP_CLASS_ASSIGNMENT),
        asOBJ_APP_CLASS_CK = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
                              asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_CDA =
            (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
             asOBJ_APP_CLASS_DESTRUCTOR + asOBJ_APP_CLASS_ASSIGNMENT),
        asOBJ_APP_CLASS_CDK =
            (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
             asOBJ_APP_CLASS_DESTRUCTOR + asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_CAK =
            (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
             asOBJ_APP_CLASS_ASSIGNMENT + asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_CDAK =
            (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR +
             asOBJ_APP_CLASS_DESTRUCTOR + asOBJ_APP_CLASS_ASSIGNMENT +
             asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_D = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_DESTRUCTOR),
        asOBJ_APP_CLASS_DA = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_DESTRUCTOR +
                              asOBJ_APP_CLASS_ASSIGNMENT),
        asOBJ_APP_CLASS_DK = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_DESTRUCTOR +
                              asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_DAK =
            (asOBJ_APP_CLASS + asOBJ_APP_CLASS_DESTRUCTOR +
             asOBJ_APP_CLASS_ASSIGNMENT + asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_A = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_ASSIGNMENT),
        asOBJ_APP_CLASS_AK = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_ASSIGNMENT +
                              asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_K =
            (asOBJ_APP_CLASS + asOBJ_APP_CLASS_COPY_CONSTRUCTOR),
        asOBJ_APP_CLASS_MORE_CONSTRUCTORS = (quint64(1) << 31),
        asOBJ_APP_PRIMITIVE = (1 << 13),
        asOBJ_APP_FLOAT = (1 << 14),
        asOBJ_APP_ARRAY = (1 << 15),
        asOBJ_APP_CLASS_ALLINTS = (1 << 16),
        asOBJ_APP_CLASS_ALLFLOATS = (1 << 17),
        asOBJ_NOCOUNT = (1 << 18),
        asOBJ_APP_CLASS_ALIGN8 = (1 << 19),
        asOBJ_IMPLICIT_HANDLE = (1 << 20),
        asOBJ_APP_CLASS_UNION = (quint64(1) << 32),
        asOBJ_MASK_VALID_FLAGS = 0x1801FFFFFul,
        // Internal flags
        asOBJ_SCRIPT_OBJECT = (1 << 21),
        asOBJ_SHARED = (1 << 22),
        asOBJ_NOINHERIT = (1 << 23),
        asOBJ_FUNCDEF = (1 << 24),
        asOBJ_LIST_PATTERN = (1 << 25),
        asOBJ_ENUM = (1 << 26),
        asOBJ_TEMPLATE_SUBTYPE = (1 << 27),
        asOBJ_TYPEDEF = (1 << 28),
        asOBJ_ABSTRACT = (1 << 29),
        asOBJ_APP_ALIGN16 = (1 << 30)
    };

    enum asBehaviours {
        // Value object memory management
        asBEHAVE_CONSTRUCT,
        asBEHAVE_LIST_CONSTRUCT,
        asBEHAVE_DESTRUCT,

        // Reference object memory management
        asBEHAVE_FACTORY,
        asBEHAVE_LIST_FACTORY,
        asBEHAVE_ADDREF,
        asBEHAVE_RELEASE,
        asBEHAVE_GET_WEAKREF_FLAG,

        // Object operators
        asBEHAVE_TEMPLATE_CALLBACK,

        // Garbage collection behaviours
        asBEHAVE_FIRST_GC,
        asBEHAVE_GETREFCOUNT = asBEHAVE_FIRST_GC,
        asBEHAVE_SETGCFLAG,
        asBEHAVE_GETGCFLAG,
        asBEHAVE_ENUMREFS,
        asBEHAVE_RELEASEREFS,
        asBEHAVE_LAST_GC = asBEHAVE_RELEASEREFS,

        asBEHAVE_MAX
    };

    // Calling conventions
    enum class asCallConvTypes {
        asCALL_CDECL = 0,
        asCALL_STDCALL = 1,
        asCALL_THISCALL_ASGLOBAL = 2,
        asCALL_THISCALL = 3,
        asCALL_CDECL_OBJLAST = 4,
        asCALL_CDECL_OBJFIRST = 5,
        asCALL_GENERIC = 6,
        asCALL_THISCALL_OBJLAST = 7,
        asCALL_THISCALL_OBJFIRST = 8
    };

public:
    virtual asRetCodes registerGlobalFunction(
        uint retMetaType, const ScriptFn &fn, const QString &fnName,
        const QVector<QPair<uint /*metaType*/, QString /*paramName*/>> &params =
            {}) = 0;

    // A hacking way to register script function (Generic_Call)
    // This registering way is not safe. There is no
    // other checking except function's signature.
    // You should handle your all the types and pay yourself.
    virtual asRetCodes registerGlobalFunction(const QString &decl,
                                              const UNSAFE_SCFNPTR &fn) = 0;

    virtual void registerScriptMarco(const QString &marco) = 0;

    virtual void registerScriptMarcos(const QStringList &marcos) = 0;

    // ============= low level functions begin =============
public:
    virtual asRetCodes setDefaultNamespace(const char *nameSpace) = 0;
    virtual asRetCodes restoreDefaultNamespace() = 0;

public:
    virtual asRetCodes registerEnum(const char *type) = 0;
    virtual asRetCodes registerEnumValue(const char *type, const char *name,
                                         int value) = 0;

public:
    virtual asRetCodes registerFuncdef(const char *decl) = 0;

public:
    virtual asRetCodes registerTypedef(const char *type, const char *decl) = 0;

public:
    virtual asRetCodes registerGlobalProperty(const char *declaration,
                                              void *pointer) = 0;

public:
    virtual asRetCodes registerObjectType(const char *obj, int byteSize,
                                          quint64 flags) = 0;

    virtual asRetCodes
    registerObjectProperty(const char *obj, const char *declaration,
                           int byteOffset, int compositeOffset = 0,
                           bool isCompositeIndirect = false) = 0;
    virtual asRetCodes
    registerObjectMethod(const char *obj, const char *declaration,
                         const asFuncPtr &funcPointer, asCallConvTypes callConv,
                         void *auxiliary = 0, int compositeOffset = 0,
                         bool isCompositeIndirect = false) = 0;

    virtual asRetCodes registerObjectBehaviour(
        const char *obj, asBehaviours behaviour, const char *declaration,
        const asFuncPtr &funcPointer, asCallConvTypes callConv,
        void *auxiliary = 0, int compositeOffset = 0,
        bool isCompositeIndirect = false) = 0;

public:
    struct EvalResult {
        QString value;   // for display
        bool expandable; // indicate whether is expandable
    };

    struct ExpandResult {
        QString type;
        void *buffer;
    };

    using EvaluateResult =
        std::variant<std::monostate, EvalResult, QVector<ExpandResult>,
                     QHash<QString, ExpandResult>>;

    enum class EvalMode { Eval, Expand };

    using Evaluator =
        std::function<EvaluateResult(void *result, EvalMode mode)>;

    virtual asRetCodes registerObjectEvaluator(const char *obj,
                                               const Evaluator &ev) = 0;

public:
    virtual asRetCodes registerInterface(const char *name) = 0;

    virtual asRetCodes registerInterfaceMethod(const char *intf,
                                               const char *declaration) = 0;

public:
    // very low-level to register global function
    virtual asRetCodes registerGlobalFunction(const char *declaration,
                                              const asFuncPtr &funcPointer,
                                              asCallConvTypes callConv,
                                              void *auxiliary = 0) = 0;

public:
    asRetCodes registerEnums(const QString &type,
                             const QHash<QString, int> value);
};

template <typename T>
quint32 asGetTypeTraits() {
#if defined(_MSC_VER) || defined(_LIBCPP_TYPE_TRAITS) || (__GNUC__ >= 5) ||    \
    (defined(__clang__) && !defined(CLANG_PRE_STANDARD))
    // MSVC, XCode/Clang, and gnuc 5+
    // C++11 compliant code
    bool hasConstructor = std::is_default_constructible<T>::value &&
                          !std::is_trivially_default_constructible<T>::value;
    bool hasDestructor = std::is_destructible<T>::value &&
                         !std::is_trivially_destructible<T>::value;
    bool hasAssignmentOperator = std::is_copy_assignable<T>::value &&
                                 !std::is_trivially_copy_assignable<T>::value;
    bool hasCopyConstructor = std::is_copy_constructible<T>::value &&
                              !std::is_trivially_copy_constructible<T>::value;
#elif (defined(__GNUC__) &&                                                    \
       (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8))) ||            \
    (defined(__clang__) && defined(CLANG_PRE_STANDARD))
    // gnuc 4.8 is using a mix of C++11 standard and pre-standard templates
    bool hasConstructor = std::is_default_constructible<T>::value &&
                          !std::has_trivial_default_constructor<T>::value;
    bool hasDestructor = std::is_destructible<T>::value &&
                         !std::is_trivially_destructible<T>::value;
    bool hasAssignmentOperator = std::is_copy_assignable<T>::value &&
                                 !std::has_trivial_copy_assign<T>::value;
    bool hasCopyConstructor = std::is_copy_constructible<T>::value &&
                              !std::has_trivial_copy_constructor<T>::value;
#else
    // All other compilers and versions are assumed to use non C++11
    // compliant code until proven otherwise Not fully C++11 compliant. The
    // has_trivial checks were used while the standard was still being
    // elaborated, but were then removed in favor of the above is_trivially
    // checks
    // http://stackoverflow.com/questions/12702103/writing-code-that-works-when-has-trivial-destructor-is-defined-instead-of-is
    // https://github.com/mozart/mozart2/issues/51
    bool hasConstructor = std::is_default_constructible<T>::value &&
                          !std::has_trivial_default_constructor<T>::value;
    bool hasDestructor = std::is_destructible<T>::value &&
                         !std::has_trivial_destructor<T>::value;
    bool hasAssignmentOperator = std::is_copy_assignable<T>::value &&
                                 !std::has_trivial_copy_assign<T>::value;
    bool hasCopyConstructor = std::is_copy_constructible<T>::value &&
                              !std::has_trivial_copy_constructor<T>::value;
#endif
    bool isFloat = std::is_floating_point<T>::value;
    bool isPrimitive = std::is_integral<T>::value ||
                       std::is_pointer<T>::value || std::is_enum<T>::value;
    bool isClass = std::is_class<T>::value;
    bool isArray = std::is_array<T>::value;

    if (isFloat)
        return IWingAngel::asOBJ_APP_FLOAT;
    if (isPrimitive)
        return IWingAngel::asOBJ_APP_PRIMITIVE;

    if (isClass) {
        quint64 flags = IWingAngel::asOBJ_APP_CLASS;
        if (hasConstructor)
            flags |= IWingAngel::asOBJ_APP_CLASS_CONSTRUCTOR;
        if (hasDestructor)
            flags |= IWingAngel::asOBJ_APP_CLASS_DESTRUCTOR;
        if (hasAssignmentOperator)
            flags |= IWingAngel::asOBJ_APP_CLASS_ASSIGNMENT;
        if (hasCopyConstructor)
            flags |= IWingAngel::asOBJ_APP_CLASS_COPY_CONSTRUCTOR;
        return flags;
    }

    if (isArray)
        return IWingAngel::asOBJ_APP_ARRAY;

    // Unknown type traits
    return 0;
}

template <typename T>
inline void registerMetaEnum(IWingAngel *o) {
    Q_ASSERT(o);
    auto e = QMetaEnum::fromType<T>();
    auto len = e.keyCount();
    auto ename = e.enumName();
    auto r = o->registerEnum(ename);
    Q_ASSERT(r != asRetCodes::asSUCCESS);
    Q_UNUSED(r);
    for (int i = 0; i < len; ++i) {
        auto key = e.key(i);
        auto value = e.value(i);
        r = o->registerEnumValue(ename, key, value);
        Q_ASSERT(r != asRetCodes::asSUCCESS);
        Q_UNUSED(r);
    }
}

template <typename T>
inline T resolveUnsafeParamAs(void *param) {
    return *reinterpret_cast<T *>(param);
}

template <typename T, typename = std::enable_if<!std::is_pointer_v<T>>>
inline T resolveUnsafeParamAsIn(void *param) {
    return **reinterpret_cast<T **>(param);
}

template <typename T, typename = std::enable_if<!std::is_pointer_v<T>>>
inline T *resolveUnsafeParamAsOut(void *param) {
    return *reinterpret_cast<T **>(param);
}

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::IWingGeneric)

#endif // IWINGANGEL_H
