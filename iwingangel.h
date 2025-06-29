#ifndef IWINGANGEL_H
#define IWINGANGEL_H

#include "WingPlugin/wingcore.h"
#include "WingPlugin/wingplugin_global.h"

namespace WingHex {

// copy from angelscript.h
enum class asRetCodes {
    asSUCCESS = 0,
    asERROR = -1,
    asCONTEXT_ACTIVE = -2,
    asCONTEXT_NOT_FINISHED = -3,
    asCONTEXT_NOT_PREPARED = -4,
    asINVALID_ARG = -5,
    asNO_FUNCTION = -6,
    asNOT_SUPPORTED = -7,
    asINVALID_NAME = -8,
    asNAME_TAKEN = -9,
    asINVALID_DECLARATION = -10,
    asINVALID_OBJECT = -11,
    asINVALID_TYPE = -12,
    asALREADY_REGISTERED = -13,
    asMULTIPLE_FUNCTIONS = -14,
    asNO_MODULE = -15,
    asNO_GLOBAL_VAR = -16,
    asINVALID_CONFIGURATION = -17,
    asINVALID_INTERFACE = -18,
    asCANT_BIND_ALL_FUNCTIONS = -19,
    asLOWER_ARRAY_DIMENSION_NOT_REGISTERED = -20,
    asWRONG_CONFIG_GROUP = -21,
    asCONFIG_GROUP_IS_IN_USE = -22,
    asILLEGAL_BEHAVIOUR_FOR_TYPE = -23,
    asWRONG_CALLING_CONV = -24,
    asBUILD_IN_PROGRESS = -25,
    asINIT_GLOBAL_VARS_FAILED = -26,
    asOUT_OF_MEMORY = -27,
    asMODULE_IS_IN_USE = -28
};

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

} // namespace WingHex

Q_DECLARE_METATYPE(WingHex::IWingGeneric)

#endif // IWINGANGEL_H
